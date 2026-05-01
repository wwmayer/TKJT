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

#ifndef _JtProperty_Integer_HeaderFile
#define _JtProperty_Integer_HeaderFile

#include <JtProperty_Base.hxx>

//! Integer Property Atom Element (spec §6.2.1.4, Figure 72).
//! GUID: 10dd102b-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
class JtProperty_Integer : public JtProperty_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Get the integer value.
  Jt_I32 Value() const { return myValue; }

  DEFINE_STANDARD_RTTIEXT(JtProperty_Integer,JtProperty_Base)
  DEFINE_OBJECT_CLASS (JtProperty_Integer)

protected:
  Jt_I32 myValue;
};

DEFINE_STANDARD_HANDLE(JtProperty_Integer, JtProperty_Base)

#endif // _JtProperty_Integer_HeaderFile
