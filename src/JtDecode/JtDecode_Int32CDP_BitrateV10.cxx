// JT format reading and visualization tools
// Copyright (C) 2014-2015 OPEN CASCADE SAS
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2 of the License, or any later
// version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// Copy of the GNU General Public License is in LICENSE.txt and
// on <http://www.gnu.org/licenses/>.

#include <JtDecode_Int32CDP_BitrateV10.hxx>
#include <cstdint>

// -----------------------------------------------------------------------
// nibblerGetU32
//
// Reads an unsigned value encoded as 4-bit nibbles with a 1-bit continuation
// flag after each nibble.  Each iteration: read 4 bits, shift into result,
// read 1 continuation bit.  Stop when continuation == 0.
//
// Spec ref: Annex B.2, nibblerGet(UInt32) (JT v10 Rev C, p.222):
//   do {
//     GetUnsignedBits(uTmp, cNibbleWidth);   // cNibbleWidth = 4
//     uTmp <<= cNibbles * cNibbleWidth;
//     oiVal |= uTmp;
//     GetUnsignedBits(bMoreBits, 1);
//     cNibbles++;
//   } while (bMoreBits);
// -----------------------------------------------------------------------
uint32_t JtDecode_Int32CDP_BitrateV10::nibblerGetU32()
{
  uint32_t aVal     = 0;
  uint32_t cNibbles = 0;
  uint32_t bMore;
  do
  {
    uint32_t uTmp = ReadU32 (4);
    uTmp <<= cNibbles * 4u;
    aVal |= uTmp;
    bMore = ReadU32 (1);
    cNibbles++;
  }
  while (bMore);
  return aVal;
}

// -----------------------------------------------------------------------
// nibblerGetI32
//
// Same as nibblerGetU32 but sign-extends the result using the total bit width
// consumed (cNibbles * 4).
//
// Spec ref: Annex B.2, nibblerGet(Int32) (JT v10 Rev C, p.222-223):
//   // Sign-extend the resulting bits
//   UInt32 sw = cNibbles * cNibbleWidth;
//   if (sw < 32) { oiVal <<= 32 - sw; oiVal >>= 32 - sw; }
// -----------------------------------------------------------------------
int32_t JtDecode_Int32CDP_BitrateV10::nibblerGetI32()
{
  int32_t  aVal     = 0;
  uint32_t cNibbles = 0;
  uint32_t bMore;
  do
  {
    uint32_t uTmp = ReadU32 (4);
    uTmp <<= cNibbles * 4u;
    aVal |= static_cast<int32_t>(uTmp);
    bMore = ReadU32 (1);
    cNibbles++;
  }
  while (bMore);

  // Sign-extend
  unsigned sw = cNibbles * 4u;
  if (sw < 32u)
  {
    aVal <<= (32u - sw);
    aVal >>= (32u - sw);
  }
  return aVal;
}

// -----------------------------------------------------------------------
// bitsizeU
//
// Number of bits required to represent an unsigned integer (32 - nlz(x)).
//
// Spec ref: Annex B.2, bitsize(UInt32 x) = 32 - nlz(x) (JT v10 Rev C, p.229).
// -----------------------------------------------------------------------
unsigned JtDecode_Int32CDP_BitrateV10::bitsizeU (uint32_t x)
{
  if (x == 0) return 0;
  unsigned n = 0;
  while ((x >> n)) n++;
  return n;
}

// -----------------------------------------------------------------------
// decode
//
// Main decode function implementing BitLengthCodec2::decode from the spec.
//
// Spec ref: Annex B.2, BitLengthCodec2::decode (JT v10 Rev C, p.228-229):
//
//   Read tag bit (0 = fixed-width, 1 = variable-width).
//
//   Fixed-width path (tag == 0):
//     nibblerGet(iMinSymbol)
//     nibblerGet(iMaxSymbol)
//     cNumCurBits = bitsize(iMaxSymbol - iMinSymbol)
//     for each value: GetUnsignedBits(iSymbol, cNumCurBits); output = iSymbol + iMinSymbol
//
//   Variable-width path (tag == 1):
//     nibblerGet(iMean)
//     cBlkValBits = 4  (delta field-width bits per block header)
//     cBlkLenBits = 4  (run-length bits per block header)
//     cCurFieldWidth = 0
//     loop ii = 0 .. nValues:
//       do { GetSignedBits(delta, 4); cCurFieldWidth += delta; }
//       while (delta == cMaxFieldDecr || delta == cMaxFieldIncr)
//       GetUnsignedBits(cRunLen, 4)
//       for each in run: GetSignedBits(v, cCurFieldWidth); output = v + iMean
//       ii += cRunLen
// -----------------------------------------------------------------------
void JtDecode_Int32CDP_BitrateV10::decode (int32_t* theResultPtr, int32_t* theResultEnd)
{
  const int32_t nValues = static_cast<int32_t>(theResultEnd - theResultPtr);

  // Read the mode tag bit: 0 = fixed-width, 1 = variable-width.
  // Spec ref: Annex B.2, GetUnsignedBits(iTmp, 1) (JT v10 Rev C, p.228).
  uint32_t aTag = ReadU32 (1);

  if (aTag == 0)
  {
    // ---- Fixed-width path ----
    // Spec ref: Annex B.2, fixed-width branch (JT v10 Rev C, p.228).
    int32_t  iMinSymbol = nibblerGetI32();
    int32_t  iMaxSymbol = nibblerGetI32();
    unsigned cNumCurBits = bitsizeU (static_cast<uint32_t>(iMaxSymbol - iMinSymbol));

    for (int32_t i = 0; i < nValues; i++)
    {
      uint32_t uSymbol = (cNumCurBits > 0) ? ReadU32 (cNumCurBits) : 0;
      theResultPtr[i] = static_cast<int32_t>(uSymbol) + iMinSymbol;
    }
  }
  else
  {
    // ---- Variable-width path ----
    // Spec ref: Annex B.2, variable-width branch (JT v10 Rev C, p.528-529).
    int32_t iMean = nibblerGetI32();

    // Fixed block-encoding constants (spec: cBlkValBits=4, cBlkLenBits=4).
    // Spec ref: Annex B.2, encode() comment "A block is: 3 bits... 4 bits of
    //   block length" — the decoder uses cBlkValBits=4, cBlkLenBits=4
    //   (JT v10 Rev C, p.224).
    const unsigned cBlkValBits = 4;
    const unsigned cBlkLenBits = 4;
    const int32_t  cMaxFieldDecr = -(1 << (cBlkValBits - 1));      // -8
    const int32_t  cMaxFieldIncr =  (1 << (cBlkValBits - 1)) - 1;  // +7

    int32_t  cCurFieldWidth = 0;
    int32_t* pOut = theResultPtr;
    int32_t  ii   = 0;

    while (ii < nValues)
    {
      // Adjust current field width via one or more delta steps.
      // Loop while delta hits the max increment/decrement sentinel values.
      // Spec ref: Annex B.2, do { GetSignedBits(cDeltaFieldWidth, cBlkValBits);
      //   cCurFieldWidth += cDeltaFieldWidth; } while ... (JT v10 Rev C, p.529).
      int32_t cDelta;
      do
      {
        cDelta = ReadI32 (cBlkValBits);
        cCurFieldWidth += cDelta;
      }
      while (cDelta == cMaxFieldDecr || cDelta == cMaxFieldIncr);

      // Read run length (4 bits, unsigned).
      // Spec ref: Annex B.2, GetUnsignedBits(cRunLen, cBlkLenBits) (JT v10 Rev C, p.529).
      uint32_t cRunLen = ReadU32 (cBlkLenBits);

      // Read run values: each is a signed cCurFieldWidth-bit integer offset from iMean.
      // Spec ref: Annex B.2, GetSignedBits(iTmp, cCurFieldWidth) (JT v10 Rev C, p.529).
      for (uint32_t k = 0; k < cRunLen; k++)
      {
        int32_t iTmp = (cCurFieldWidth > 0) ? ReadI32 (cCurFieldWidth) : 0;
        *pOut++ = iTmp + iMean;
      }

      ii += static_cast<int32_t>(cRunLen);
    }
  }
}
