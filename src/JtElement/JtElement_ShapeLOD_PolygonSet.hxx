// JT format reading and visualization tools
// Copyright (C) 2013-2015 OPEN CASCADE SAS
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

#ifndef _JtElement_ShapeLOD_PolygonSet_HeaderFile
#define _JtElement_ShapeLOD_PolygonSet_HeaderFile

#include <JtElement_ShapeLOD_Vertex.hxx>

//! A Polygon Set LOD Element holds the geometric shape definition data
//! for a single LOD of a collection of independent and unconnected polygons.
class JtElement_ShapeLOD_PolygonSet : public JtElement_ShapeLOD_Vertex
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read    (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Read this entity from a JT 10+ file.
  Standard_EXPORT Standard_Boolean ReadV10 (JtData_Reader& theReader);

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  DEFINE_STANDARD_RTTIEXT(JtElement_ShapeLOD_PolygonSet,JtElement_ShapeLOD_Vertex)
  DEFINE_OBJECT_CLASS (JtElement_ShapeLOD_PolygonSet)
};

DEFINE_STANDARD_HANDLE(JtElement_ShapeLOD_PolygonSet, JtElement_ShapeLOD_Vertex)

#endif // _JtElement_ShapeLOD_PolygonSet_HeaderFile
