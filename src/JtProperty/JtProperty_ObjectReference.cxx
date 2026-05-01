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

#include <JtProperty_ObjectReference.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtProperty_ObjectReference, JtProperty_Base)

IMPLEMENT_OBJECT_CLASS(JtProperty_ObjectReference, "JT Object Reference Property Atom",
                       "10dd1004-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtProperty_ObjectReference::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  if (!JtProperty_Base::Read (theReader))
    return Standard_False;

  return theReader.ReadI32 (myObjectID);
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ JT Object Reference Property Atom Element (spec Figure 74)
//           Base Property Atom Data | U8 Version | I32 Object ID
//=======================================================================
Standard_Boolean JtProperty_ObjectReference::ReadV10 (JtData_Reader& theReader)
{
  if (!JtProperty_Base::ReadV10 (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  return theReader.ReadI32 (myObjectID);
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtProperty_ObjectReference::Dump (Standard_OStream& theStream) const
{
  theStream << "JtProperty_ObjectReference (id=" << myObjectID << ") ";
  return JtProperty_Base::Dump (theStream);
}
