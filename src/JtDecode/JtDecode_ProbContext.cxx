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

#include <JtDecode_ProbContext.hxx>

Standard_Boolean JtDecode_ProbContextI32::ReadFirst (JtDecode_BitReader::Raw& theReader)
{
  return read (theReader, 32, 6, 6, 6, 6, 32);
}

Standard_Boolean JtDecode_ProbContextI32::ReadNext (JtDecode_BitReader::Raw& theReader,
                                                    const JtDecode_ProbContextI32& theFirstContext)
{
  if (!read (theReader, 32, 6, 6, 0, 6, 0))
    return Standard_False;

  NCollection_DataMap<Standard_Integer, int32_t> aValuesMap;
  Standard_Size i;
  for (i = 0; i < theFirstContext.Size(); i++)
    aValuesMap.Bind (theFirstContext[i].symbol, theFirstContext[i].associatedValue);
  for (i = 0; i < Size(); i++)
    aValuesMap.Find (myEntries[i].symbol, myEntries[i].associatedValue);

  return Standard_True;
}

Standard_Boolean JtDecode_ProbContextI32::ReadMk2 (JtDecode_BitReader::Raw& theReader)
{
  return read (theReader, 16, 6, 6, 6, 0, 32);
}

Standard_Boolean JtDecode_ProbContextI32::read (JtDecode_BitReader::Raw& theReader,
                                                const unsigned theEntryCountWidth,
                                                const unsigned theSymBitsNumWidth,
                                                const unsigned theOccBitsNumWidth,
                                                const unsigned theValBitsNumWidth,
                                                const unsigned theNxtBitsNumWidth,
                                                const unsigned theMinValueWidth)
{
  if (!theReader.Load (theEntryCountWidth + theSymBitsNumWidth + theOccBitsNumWidth
                     + theValBitsNumWidth + theNxtBitsNumWidth + theMinValueWidth))
    return Standard_False;

  unsigned      anEntryCount = theReader.ReadU32Or0 (theEntryCountWidth);
  unsigned      aSymBits     = theReader.ReadU32Or0 (theSymBitsNumWidth);
  unsigned      aOccBits     = theReader.ReadU32Or0 (theOccBitsNumWidth);
  unsigned      aValBits     = theReader.ReadU32Or0 (theValBitsNumWidth);
  unsigned      aNxtBits     = theReader.ReadU32Or0 (theNxtBitsNumWidth);
  int32_t       aMinValue    = theReader.ReadI32Or0 (theMinValueWidth);

  if (!theReader.Load (anEntryCount * (aSymBits + aOccBits + aValBits + aNxtBits)))
    return Standard_False;

  myEntries.Allocate (anEntryCount);
  myTotalCount = 0;

  for (Standard_Size i = 0; i < anEntryCount; i++)
  {
    Entry& anEntry = myEntries[i];

    anEntry.symbol          = theReader.ReadU32Or0 (aSymBits) - 2;
    anEntry.occCount        = theReader.ReadU32Or0 (aOccBits);
    anEntry.associatedValue = theReader.ReadU32Or0 (aValBits) + aMinValue;
    anEntry.nextContext     = theReader.ReadU32Or0 (aNxtBits);

    myTotalCount += anEntry.occCount;
  }

  return Standard_True;
}

// =======================================================================
// JtDecode_ProbContextI32::ReadV10
// =======================================================================
// Reads a JT 10+ probability context table from a bit-packed Raw reader.
// Written from spec only — does NOT call read() or reuse any V8/V9 logic.
//
// Header (bit-packed, Fig 133 / Fig 134 of JT v10 spec):
//   U32{16}  Entry Count
//   U32{6}   OccurrenceBitsCount (bits per occurrence count field)
//   U32{7}   ValueBitsCount      (bits per value field, unsigned)
//   I32{32}  Min Value
//
// Per entry (Entry Count times):
//   U32{1}         IsEscape  — 1 = escape / OOB placeholder
//   U32{occBits}   OccurrenceCount
//   I32{valBits}   Value  (stored unsigned; actual = value + MinValue)
//
// JT10 has no explicit symbol or nextContext field.
// We synthesise: symbol = -2 for escape, sequential index otherwise.
//               nextContext = 0 always (single probability context).
//
// Spec ref: Fig 132 (Int32 CDP, JT v10 Rev C, p.155-158);
//           Fig 133/134 (ProbabilityContext Table, p.158-161).
Standard_Boolean JtDecode_ProbContextI32::ReadV10 (JtDecode_BitReader::Raw& theReader)
{
  // Load header bits: 16 + 6 + 7 + 32 = 61 bits.
  if (!theReader.Load (16 + 6 + 7 + 32))
    return Standard_False;

  unsigned anEntryCount = theReader.ReadU32Or0 (16);
  unsigned aOccBits     = theReader.ReadU32Or0 (6);
  unsigned aValBits     = theReader.ReadU32Or0 (7);
  int32_t  aMinValue    = theReader.ReadI32Or0 (32);

  if (anEntryCount == 0)
  {
    myEntries.Allocate (0);
    myTotalCount = 0;
    return Standard_True;
  }

  // Load entry bits: each entry is (1 + occBits + valBits) bits.
  if (!theReader.Load (anEntryCount * (1 + aOccBits + aValBits)))
    return Standard_False;

  myEntries.Allocate (anEntryCount);
  myTotalCount = 0;

  for (Standard_Size i = 0; i < anEntryCount; i++)
  {
    Entry& e = myEntries[i];

    unsigned isEscape   = theReader.ReadU32Or0 (1);
    e.occCount          = theReader.ReadU32Or0 (aOccBits);
    e.associatedValue   = theReader.ReadI32Or0 (aValBits) + aMinValue;
    // Synthesise fields used by the existing arithmetic decoder:
    e.symbol            = isEscape ? -2 : static_cast<int32_t> (i);
    e.nextContext       = 0;

    myTotalCount += e.occCount;
  }

  return Standard_True;
}
