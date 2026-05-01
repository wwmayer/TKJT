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

#ifndef _JtAttribute_LightSet_HeaderFile
#define _JtAttribute_LightSet_HeaderFile

#include <JtAttribute_Base.hxx>
#include <NCollection_Vector.hxx>

//! Light Set Attribute Element (spec §6.1.2.5, Figure 55).
//! GUID: 10dd1096-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
//! Holds an unordered list of Light Object IDs.
class JtAttribute_LightSet : public JtAttribute_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Return the list of light object IDs.
  const NCollection_Vector<Jt_I32>& LightObjectIDs() const { return myLightObjectIDs; }

  DEFINE_STANDARD_RTTIEXT(JtAttribute_LightSet,JtAttribute_Base)
  DEFINE_OBJECT_CLASS (JtAttribute_LightSet)

protected:
  NCollection_Vector<Jt_I32> myLightObjectIDs;
};

DEFINE_STANDARD_HANDLE(JtAttribute_LightSet, JtAttribute_Base)

#endif // _JtAttribute_LightSet_HeaderFile
