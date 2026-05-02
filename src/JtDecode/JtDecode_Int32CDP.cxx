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

#include <JtDecode_Int32CDP.hxx>

#include <JtDecode_BitReader.hxx>
#include <JtDecode_ProbContext.hxx>

#include <JtDecode_Int32CDP_Null.hxx>
#include <JtDecode_Int32CDP_Bitlength.hxx>
#include <JtDecode_Int32CDP_Bitlength2.hxx>
#include <JtDecode_Int32CDP_BitrateV10.hxx>
#include <JtDecode_Int32CDP_Huffman.hxx>
#include <JtDecode_Int32CDP_Arithmetic.hxx>
#include <JtDecode_Int32CDP_Chopper.hxx>

#include <JtData_Message.hxx>

JtDecode_Int32CDP::EncodedDataHandle JtDecode_Int32CDP::loadCDP1 (JtData_Reader& theReader)
{
  // Read codec type
  Standard_Integer aCodec;
  if (!theReader.ReadU8 (aCodec))
    return EncodedDataHandle();

  // Null codec, just read uncoded values
  if (aCodec == 0)
  {
    Jt_VecU32 aCodeText;
    if (!theReader.ReadVec (aCodeText))
      return EncodedDataHandle();

    return EncodedDataHandle (new JtDecode_Int32CDP_Null (aCodeText.Move()));
  }

  // Bits based codec
  JtData_SingleHandle<JtDecode_Int32CDP_Bits> aData;
  switch (aCodec)
  {
  case 1:
    aData = new JtDecode_Int32CDP_Bitlength  (theReader);
    break;
  case 2:
    aData = new JtDecode_Int32CDP_Huffman    (theReader);
    break;
  case 3:
    aData = new JtDecode_Int32CDP_Arithmetic (theReader);
    break;
  default:
    TRACE ("Unknown Codec (" + aCodec + ")!");
    return EncodedDataHandle();
  }

  // Huffman or arithmetic codec: read probability contexts and load OOB data
  Standard_Integer aProbContextTableCount = 0;
  if (aCodec >= 2)
  {
    // read probability contexts
    if (!theReader.ReadU8 (aProbContextTableCount))
      return EncodedDataHandle();

    JtData_Vector<JtDecode_ProbContextI32> aProbContexts (aProbContextTableCount);
    {
      JtDecode_BitReader::Raw aContextReader (theReader);

      if (!aProbContexts[0].ReadFirst (aContextReader))
        return EncodedDataHandle();

      for (Standard_Integer i = 1; i < aProbContextTableCount; i++)
        if (!aProbContexts[i].ReadNext (aContextReader, aProbContexts[0]))
          return EncodedDataHandle();
    }
    aData->SetProbContexts (aProbContexts.Move());

    // load out-of-band data
    Jt_I32 anOutOfBandValueCount;
    theReader.ReadI32 (anOutOfBandValueCount);
    if (anOutOfBandValueCount > 0)
    {
      JtDecode_Int32CDP anOOBData;
      if (!anOOBData.Load1 (theReader))
        return EncodedDataHandle();
      aData->SetOOBData (anOOBData);
    }
  }

  // Read CodeText length and number of output values
  Jt_I32 aCodeTextLength;
  Jt_I32 aValueCount;
  if (!theReader.ReadI32 (aCodeTextLength)
   || !theReader.ReadI32 (aValueCount))
  {
    return EncodedDataHandle();
  }
  aData->SetOutValCount (aValueCount);

  // Read symbol count if present
  if (aProbContextTableCount > 1)
  {
    Jt_I32 aSymbolCount;
    if (!theReader.ReadI32 (aSymbolCount))
      return EncodedDataHandle();
  }

  // Read CodeText data
  if (!aData->LoadVec())
    return EncodedDataHandle();

  return EncodedDataHandle (aData);
}

JtDecode_Int32CDP::EncodedDataHandle JtDecode_Int32CDP::loadCDP2 (JtData_Reader& theReader)
{
  // Read number of output values
  Jt_I32 aValueCount;
  if (!theReader.ReadI32 (aValueCount))
  { return EncodedDataHandle(); }

  if (aValueCount == 0)
    return EncodedDataHandle (new JtDecode_Int32CDP_Null (Jt_VecU32().Move()));

  // Read codec type
  Standard_Integer aCodec;
  if (!theReader.ReadU8 (aCodec))
  { return EncodedDataHandle(); }

  // Chopper pseudo-codec
  if (aCodec == 4)
  {
    Jt_U8 aChop;
    if (!theReader.ReadU8 (aChop))
      return EncodedDataHandle();

    if (aChop == 0)
      return loadCDP2 (theReader);

    Jt_I32 aBias;
    Jt_U8  aSpan;
    if (!theReader.ReadI32 (aBias)
     || !theReader.ReadU8  (aSpan))
      return EncodedDataHandle();

    // load chopped MSB and LSB data
    JtDecode_Int32CDP aMSBData;
    JtDecode_Int32CDP aLSBData;
    if (!aMSBData.Load2 (theReader)
     || !aLSBData.Load2 (theReader))
      return EncodedDataHandle();

    if (aMSBData.GetOutValCount() != aValueCount
     || aLSBData.GetOutValCount() != aValueCount)
    {
      ALARM ("Error: Chopped data size mismatch");
      return EncodedDataHandle();
    }

    return EncodedDataHandle (
      new JtDecode_Int32CDP_Chopper (aMSBData, aLSBData, aChop, aSpan, aBias));
  }

  // Null, bitlength or arithmetic codec: read CodeText length
  Jt_I32 aCodeTextLength;
  if (!theReader.ReadI32 (aCodeTextLength) || aCodeTextLength <= 0)
  { return EncodedDataHandle(); }

  // Null codec
  if (aCodec == 0)
  {
    Jt_VecU32 aCodeText;
    if (!theReader.ReadVec (aCodeText, (aCodeTextLength + 31) / 32))
      return EncodedDataHandle();

    return EncodedDataHandle (new JtDecode_Int32CDP_Null (aCodeText.Move()));
  }

  // Bits based codec
  JtData_SingleHandle<JtDecode_Int32CDP_Bits> aData;
  switch (aCodec)
  {
  case 1: // Bitlength codec
    aData = new JtDecode_Int32CDP_Bitlength2 (theReader);
    break;

  case 3: // Arithmetic codec
    aData = new JtDecode_Int32CDP_Arithmetic (theReader);
    break;

  default: // Unknown codec
    TRACE ("Unknown Codec (" + aCodec + ")!");
    return EncodedDataHandle();
  }

  // Bitlength or arithmetic codec: set expected output values count
  aData->SetOutValCount (aValueCount);

  // Bitlength or arithmetic codec: read CodeText data
  if (!aData->Load ((aCodeTextLength + 31) / 32))
  { return EncodedDataHandle(); }

  // Arithmetic codec: read probability context and load OOB data
  if (aCodec == 3)
  {
    // read probability context
    JtData_Vector<JtDecode_ProbContextI32> aProbContexts (1);
    {
      JtDecode_BitReader::Raw aContextReader (theReader);
      if (!aProbContexts[0].ReadMk2 (aContextReader))
      { return EncodedDataHandle(); }
    }
    aData->SetProbContexts (aProbContexts.Move());

    // load out-of-band data
    JtDecode_Int32CDP anOOBData;
    if (!anOOBData.Load2 (theReader))
    { return EncodedDataHandle(); }

    aData->SetOOBData (anOOBData);
  }

  return EncodedDataHandle (aData);
}

// =======================================================================
// function : loadCDP_V10
// purpose  : JT 10+ Int32 Compressed Data Packet (Fig 132 of JT v10 spec).
//            Written from spec only. Does NOT call loadCDP1/loadCDP2 or any
//            V8/V9 reading helper.
//
// Format (per Fig 132):
//   I32  Value Count
//   [if Value Count > 0]
//     U8  CODEC Type:
//       0 = Null      → I32 CodeText Length  +  VecU32 CodeText
//       1 = Bitlength → I32 CodeText Length  +  VecU32 CodeText
//       3 = Arithmetic→ I32 CodeText Length  +  VecU32 CodeText
//                        + Int32 Probability Context (JT10 format)
//                        + Int32CDP OOB Data (recursive, since LOD segs are
//                          not externally compressed — Table 6 of spec)
//       4 = Chopper   → U8 Chop Bits
//                       [Chop Bits > 0]:  I32 Value Bias + U8 Value Span Bits
//                                         + Int32CDP MSB + Int32CDP LSB
//                       [Chop Bits == 0]: recursive (another Int32CDP follows)
//       5 = Move-to-Front → not yet implemented
// =======================================================================
JtDecode_Int32CDP::EncodedDataHandle JtDecode_Int32CDP::loadCDP_V10 (JtData_Reader& theReader)
{
  // I32: Value Count
  // Spec ref: Fig 132 — Int32CDP collection (JT v10 Rev C, p.155)
  Jt_I32 aValueCount;
  if (!theReader.ReadI32 (aValueCount))
  { return EncodedDataHandle(); }

  if (aValueCount == 0)
    return EncodedDataHandle (new JtDecode_Int32CDP_Null (Jt_VecU32().Move()));

  // U8: CODEC Type
  Standard_Integer aCodec;
  if (!theReader.ReadU8 (aCodec))
  { return EncodedDataHandle(); }

  // ---- CODEC 4: Chopper ----
  if (aCodec == 4)
  {
    Jt_U8 aChopBits;
    if (!theReader.ReadU8 (aChopBits))
      return EncodedDataHandle();

    if (aChopBits == 0)
      return loadCDP_V10 (theReader); // Chop Bits == 0 means another CDP follows

    Jt_I32 aValueBias;
    Jt_U8  aValueSpanBits;
    if (!theReader.ReadI32 (aValueBias) || !theReader.ReadU8 (aValueSpanBits))
    { return EncodedDataHandle(); }

    JtDecode_Int32CDP aMSBData, aLSBData;
    if (!aMSBData.LoadV10 (theReader) || !aLSBData.LoadV10 (theReader))
      return EncodedDataHandle();

    if (aMSBData.GetOutValCount() != aValueCount || aLSBData.GetOutValCount() != aValueCount)
    {
      ALARM ("Error: V10 Chopped data size mismatch");
      return EncodedDataHandle();
    }

    return EncodedDataHandle (
      new JtDecode_Int32CDP_Chopper (aMSBData, aLSBData, aChopBits, aValueSpanBits, aValueBias));
  }

  // ---- CODEC 0, 1, 3: read I32 CodeText Length + VecU32 CodeText ----
  Jt_I32 aCodeTextLength;
  if (!theReader.ReadI32 (aCodeTextLength))
  { return EncodedDataHandle(); }

  // CODEC 0: Null — CodeText holds the raw values directly
  if (aCodec == 0)
  {
    Jt_VecU32 aCodeText;
    Standard_Integer aWords = (aCodeTextLength > 0) ? (aCodeTextLength + 31) / 32 : 0;
    if (aWords > 0 && !theReader.ReadVec (aCodeText, aWords))
      return EncodedDataHandle();
    return EncodedDataHandle (new JtDecode_Int32CDP_Null (aCodeText.Move()));
  }

  // CODEC 1 (Bitlength) or CODEC 3 (Arithmetic)
  JtData_SingleHandle<JtDecode_Int32CDP_Bits> aData;
  if (aCodec == 1)
    // V10 Bitlength uses nibbler-encoded min/max (fixed) or mean (variable).
    // Spec ref: Annex B.2, BitLengthCodec2::decode (JT v10 Rev C, p.228-229).
    aData = new JtDecode_Int32CDP_BitrateV10 (theReader);
  else if (aCodec == 3)
    aData = new JtDecode_Int32CDP_Arithmetic (theReader);
  else
  {
    TRACE ("V10 CDP: unknown CODEC type (" + aCodec + ")");
    return EncodedDataHandle();
  }

  aData->SetOutValCount (aValueCount);

  // Load CodeText (may be 0 bits when all values are OOB)
  if (aCodeTextLength > 0)
  {
    if (!aData->Load ((aCodeTextLength + 31) / 32))
    { return EncodedDataHandle(); }
  }

  // CODEC 3 (Arithmetic): read JT10 probability context + optional OOB recursive CDP.
  // Spec ref: Fig 132, Int32 Compressed Data Packet (JT v10 Rev C, p.155-158):
  //   → Int32 Probability Context (Fig 133)
  //   → THEN: if segment NOT externally compressed: nested Int32CDP for OOB values
  //            if segment IS externally compressed:  I32 OOB Count + VecI32 OOB Values
  // The nested OOB CDP is only present in the file when the probability context
  // contains at least one escape entry (i.e. some values were out-of-band).
  // If no escape entry exists, all values were encoded inline and no OOB CDP follows.
  if (aCodec == 3)
  {
    JtData_Vector<JtDecode_ProbContextI32> aProbContexts (1);
    {
      JtDecode_BitReader::Raw aContextReader (theReader);
      if (!aProbContexts[0].ReadV10 (aContextReader))
      { return EncodedDataHandle(); }
    }

    // Check whether any entry is an escape (symbol == -2).
    // OOB CDP is written only when escape entries exist (Fig 132 / spec p.156).
    Standard_Boolean aHasEscape = Standard_False;
    for (Standard_Size i = 0; i < aProbContexts[0].Size(); i++)
      if (aProbContexts[0][i].symbol == -2) { aHasEscape = Standard_True; break; }

    aData->SetProbContexts (aProbContexts.Move());

    if (aHasEscape)
    {
      // OOB data: LOD segments (type 7-16) are NOT externally compressed (Table 6),
      // so OOB is a nested Int32CDP (not raw VecI32).
      JtDecode_Int32CDP anOOBData;
      if (!anOOBData.LoadV10 (theReader))
      { return EncodedDataHandle(); }

      aData->SetOOBData (anOOBData);
    }
    else
    {
    }
  }

  return EncodedDataHandle (aData);
}
