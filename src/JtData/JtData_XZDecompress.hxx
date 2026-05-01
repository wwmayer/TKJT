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

#ifndef _JtData_XZDecompress_HeaderFile
#define _JtData_XZDecompress_HeaderFile

#include <JtData_Reader.hxx>
#include <vector>

//! Class performing decompression of XZ (LZMA2) compressed segment data.
class JtData_XZDecompress : public JtData_Reader
{
public:
  //! Constructor: reads theLength bytes of XZ-compressed data from theReader
  //! and decompresses the whole block into an internal buffer.
  Standard_EXPORT JtData_XZDecompress (JtData_Reader& theReader,
                                       const Standard_Size theLength);

  //! Read raw bytes from the decompressed buffer.
  Standard_EXPORT Standard_Boolean ReadBytes (void*         theBuffer,
                                              Standard_Size theLength) Standard_OVERRIDE;

  //! Skip some bytes in the decompressed buffer.
  Standard_EXPORT Standard_Boolean SkipBytes (Standard_Size theLength) Standard_OVERRIDE;

  //! Get current reading position within the decompressed data.
  Standard_EXPORT Standard_Size    GetPosition() const Standard_OVERRIDE;

private:
  std::vector<uint8_t> myData;   //!< fully decompressed content
  Standard_Size        myPos;    //!< current read cursor
};

#endif // _JtData_XZDecompress_HeaderFile
