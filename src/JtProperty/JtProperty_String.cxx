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

#include <JtProperty_String.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtProperty_String, JtProperty_Base)

IMPLEMENT_OBJECT_CLASS(JtProperty_String, "String Object",
                       "10dd106e-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtProperty_String::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // Legacy path: JT 8.x / 9.x
  if (!JtProperty_Base::Read (theReader))
    return Standard_False;

  if (theReader.Model()->MajorVersion() > 8)
  {
    Jt_I16 aVersion;
    if (!theReader.ReadI16 (aVersion))
      return Standard_False;
  }

  return theReader.ReadMbString (myValue);
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ String Property Atom Element (spec Figure 71)
//           Base Property Atom Data | U8 Version | MbString Value
//=======================================================================
Standard_Boolean JtProperty_String::ReadV10 (JtData_Reader& theReader)
{
  if (!JtProperty_Base::ReadV10 (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  return theReader.ReadMbString (myValue);
}



//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtProperty_String::Dump (Standard_OStream& theStream) const
{
//  theStream << "JtProperty_String { " << myValue << " } ";
  return JtProperty_Base::Dump (theStream);
}
