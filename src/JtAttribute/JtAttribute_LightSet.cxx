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

#include <JtAttribute_LightSet.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtAttribute_LightSet, JtAttribute_Base)

IMPLEMENT_OBJECT_CLASS(JtAttribute_LightSet, "Light Set Attribute",
                       "10dd1096-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtAttribute_LightSet::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // No legacy (pre-10) definition exists for this element
  return Standard_False;
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Light Set Attribute Element (spec Figure 55)
//           Base Attribute Data | I8 Version | I32 LightCount | I32 LightObjectID[n]
//=======================================================================
Standard_Boolean JtAttribute_LightSet::ReadV10 (JtData_Reader& theReader)
{
  if (!JtAttribute_Base::ReadV10 (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  Jt_I32 aLightCount;
  if (!theReader.ReadI32 (aLightCount))
    return Standard_False;

  myLightObjectIDs.Clear();
  for (Jt_I32 i = 0; i < aLightCount; ++i)
  {
    Jt_I32 aLightObjID;
    if (!theReader.ReadI32 (aLightObjID))
      return Standard_False;
    myLightObjectIDs.Append (aLightObjID);
  }
  return Standard_True;
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtAttribute_LightSet::Dump (Standard_OStream& theStream) const
{
  return JtAttribute_Base::Dump (theStream);
}
