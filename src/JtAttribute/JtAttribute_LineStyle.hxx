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

#ifndef _JtAttribute_LineStyle_HeaderFile
#define _JtAttribute_LineStyle_HeaderFile

#include <JtAttribute_Base.hxx>

//! Linestyle Attribute Element (spec §6.1.2.8, Figure 61).
//! Defines rendering properties for polylines (line type, antialiasing, width).
//! GUID: 10dd10c4-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
class JtAttribute_LineStyle : public JtAttribute_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Return data flags (bits 0-3: line type, bit 4: antialiasing flag).
  Jt_U8  DataFlags() const { return myDataFlags; }
  //! Return line width in pixels (>0).
  Jt_F32 LineWidth() const { return myLineWidth; }

  DEFINE_STANDARD_RTTIEXT(JtAttribute_LineStyle,JtAttribute_Base)
  DEFINE_OBJECT_CLASS (JtAttribute_LineStyle)

protected:
  Jt_U8  myDataFlags;
  Jt_F32 myLineWidth;
};

DEFINE_STANDARD_HANDLE(JtAttribute_LineStyle, JtAttribute_Base)

#endif // _JtAttribute_LineStyle_HeaderFile
