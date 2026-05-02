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

#ifndef _JtDecode_VertexData_Deering_HeaderFile
#define _JtDecode_VertexData_Deering_HeaderFile

#include <JtDecode_VertexData.hxx>

//! Decoder for Deering normals decoding.
class JtDecode_VertexData_Deering : public JtDecode_VertexData
{
public:
  //! Constructor (V8/V9): 4 CDPs — sextant, octant, theta, psi as separate streams.
  Standard_EXPORT JtDecode_VertexData_Deering (
    const Jt_U8 theNbBits,
    JtDecode_Unpack& theUnpacker = JtDecode_Unpack_Null);

  //! Constructor (V10): 1 CDP containing packed [sextant:3][octant:3][theta:n][psi:n] per normal.
  //! Spec ref: Fig 139, Compressed Vertex Normal Array (JT v10 Rev C, p.165-166) and
  //!           Annex B.4.2 DeeringNormalCodec::unpackCode (p.237-238).
  Standard_EXPORT JtDecode_VertexData_Deering (
    const Jt_U8 theNbBits,
    const Standard_Boolean theIsV10);

protected:
  //! Get expected count of output components.
  Standard_EXPORT Standard_Integer getOutCompCount (Standard_Size thePackageCount) Standard_OVERRIDE;

  //! Decoding method.
  Standard_EXPORT void decode (Decoded::Ref theResults) Standard_OVERRIDE;

  Jt_U8            myNbBits;
  Standard_Boolean myIsV10;
};

#endif
