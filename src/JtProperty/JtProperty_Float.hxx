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

#ifndef _JtProperty_Float_HeaderFile
#define _JtProperty_Float_HeaderFile

#include <JtProperty_Base.hxx>

//! Floating Point Property Atom Element (spec §6.2.1.3, Figure 73).
//! GUID: 10dd1019-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
class JtProperty_Float : public JtProperty_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Get the float value.
  Jt_F32 Value() const { return myValue; }

  DEFINE_STANDARD_RTTIEXT(JtProperty_Float,JtProperty_Base)
  DEFINE_OBJECT_CLASS (JtProperty_Float)

protected:
  Jt_F32 myValue;
};

DEFINE_STANDARD_HANDLE(JtProperty_Float, JtProperty_Base)

#endif // _JtProperty_Float_HeaderFile
