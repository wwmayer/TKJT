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

#ifndef _JtDecode_Int32CDP_BitrateV10_HeaderFile
#define _JtDecode_Int32CDP_BitrateV10_HeaderFile

#include <JtDecode_Int32CDP_Bits.hxx>

//! Bitlength CODEC decoder for JT 10.x (BitLengthCodec2).
//!
//! Implements the decode path described in Annex B.2 (JT v10 Rev C, p.228-229):
//!   - tag bit 0 → fixed-width: nibblerGet(min), nibblerGet(max), then fixed-width fields
//!   - tag bit 1 → variable-width: nibblerGet(mean), then 4-bit delta field-width + 4-bit
//!     run-length blocks with signed field-width data values.
//!
//! This is a completely separate implementation from JtDecode_Int32CDP_Bitlength2, which
//! decodes the V8/V9 format (different mean/width encoding).
class JtDecode_Int32CDP_BitrateV10 : public JtDecode_Int32CDP_Bits
{
public:
  //! Initialize by reader (bits are loaded externally via JtDecode_Int32CDP_Bits::Load).
  JtDecode_Int32CDP_BitrateV10 (JtData_Reader& theReader)
  : JtDecode_Int32CDP_Bits (theReader) {}

protected:
  //! Decode all values according to Annex B.2, BitLengthCodec2::decode.
  Standard_EXPORT void decode (int32_t* theResultPtr, int32_t* theResultEnd) Standard_OVERRIDE;

private:
  //! nibblerGet for unsigned value: reads 4-bit nibbles + 1 continuation bit each.
  //! Spec ref: Annex B.2, nibblerGet(UInt32) (JT v10 Rev C, p.222).
  uint32_t nibblerGetU32();

  //! nibblerGet for signed value: same as unsigned, then sign-extends.
  //! Spec ref: Annex B.2, nibblerGet(Int32) (JT v10 Rev C, p.222-223).
  int32_t  nibblerGetI32();

  //! bitsize(unsigned x): number of bits to represent x (32 - leading zeros).
  //! Spec ref: Annex B.2 (JT v10 Rev C, p.229).
  static unsigned bitsizeU (uint32_t x);
};

#endif
