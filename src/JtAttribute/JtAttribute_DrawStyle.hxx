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

#ifndef _JtAttribute_DrawStyle_HeaderFile
#define _JtAttribute_DrawStyle_HeaderFile

#include <JtAttribute_Base.hxx>

//! Draw Style Attribute Element (spec §6.1.2.4, Figure 54).
//! Defines rendering state (lighting, culling, shading flags).
//! GUID: 10dd1014-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
class JtAttribute_DrawStyle : public JtAttribute_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Return the data flags byte (see Table 35 in spec for bit meanings).
  Jt_U8 DataFlags() const { return myDataFlags; }

  DEFINE_STANDARD_RTTIEXT(JtAttribute_DrawStyle,JtAttribute_Base)
  DEFINE_OBJECT_CLASS (JtAttribute_DrawStyle)

protected:
  Jt_U8 myDataFlags;
};

DEFINE_STANDARD_HANDLE(JtAttribute_DrawStyle, JtAttribute_Base)

#endif // _JtAttribute_DrawStyle_HeaderFile
