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

#include <JtData_XZDecompress.hxx>

#include <lzma.h>
#include <string.h>

//=======================================================================
//function : JtData_XZDecompress
//purpose  : Constructor — reads and decompresses the XZ-compressed segment
//=======================================================================

JtData_XZDecompress::JtData_XZDecompress (JtData_Reader& theReader,
                                          const Standard_Size theLength)
  : JtData_Reader (theReader.Model())
  , myPos (0)
{
  if (theLength == 0)
    return;

  // Load the entire compressed block from the upstream reader
  const void* aRawPtr = theReader.Load (theLength);
  if (!aRawPtr)
    return;

  // Decompress using liblzma (XZ format auto-detection)
  const uint8_t* aIn  = static_cast<const uint8_t*>(aRawPtr);
  const uint64_t aMemLimit = UINT64_MAX;

  // Use a grow-as-needed output buffer
  myData.resize (theLength * 8);   // initial guess: 8× compressed size

  lzma_stream aStream = LZMA_STREAM_INIT;
  lzma_ret    aRet    = lzma_stream_decoder (&aStream, aMemLimit, LZMA_CONCATENATED);
  if (aRet != LZMA_OK)
  {
    theReader.Unload (aRawPtr);
    return;
  }

  aStream.next_in  = aIn;
  aStream.avail_in = theLength;

  Standard_Size aOut = 0;

  for (;;)
  {
    aStream.next_out  = myData.data() + aOut;
    aStream.avail_out = myData.size() - aOut;

    aRet = lzma_code (&aStream, LZMA_FINISH);

    aOut = myData.size() - aStream.avail_out;

    if (aRet == LZMA_STREAM_END)
      break;

    if (aRet != LZMA_OK)
    {
      aOut = 0;   // signal failure by leaving empty buffer
      break;
    }

    if (aStream.avail_out == 0)
    {
      // output buffer exhausted — grow it
      myData.resize (myData.size() * 2);
    }
  }

  lzma_end (&aStream);
  theReader.Unload (aRawPtr);

  myData.resize (aOut);
}

//=======================================================================
//function : ReadBytes
//purpose  : Serve bytes from the decompressed buffer
//=======================================================================

Standard_Boolean JtData_XZDecompress::ReadBytes (void*         theBuffer,
                                                 Standard_Size theLength)
{
  if (myPos + theLength > myData.size())
    return Standard_False;

  memcpy (theBuffer, myData.data() + myPos, theLength);
  myPos += theLength;
  return Standard_True;
}

//=======================================================================
//function : SkipBytes
//purpose  : Advance the read cursor without copying data
//=======================================================================

Standard_Boolean JtData_XZDecompress::SkipBytes (Standard_Size theLength)
{
  if (myPos + theLength > myData.size())
    return Standard_False;

  myPos += theLength;
  return Standard_True;
}

//=======================================================================
//function : GetPosition
//purpose  : Current position within the decompressed data
//=======================================================================

Standard_Size JtData_XZDecompress::GetPosition() const
{
  return myPos;
}
