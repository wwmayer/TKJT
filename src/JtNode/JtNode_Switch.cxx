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

#include <JtNode_Switch.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtNode_Switch, JtNode_Group)

IMPLEMENT_OBJECT_CLASS(JtNode_Switch, "Switch Node",
                       "10dd10f3-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtNode_Switch::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // Legacy path: JT 8.x / 9.x reads as a group then selected child
  if (!JtNode_Group::Read (theReader))
    return Standard_False;

  Jt_I32 aSelected;
  if (!theReader.ReadI32 (aSelected))
    return Standard_False;

  mySelectedChild = aSelected;
  return Standard_True;
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Switch Node Element (spec §6.1.1.9, Figure 34)
//           Group Node Data | U8 Version | U32 Selected Child
//           (Group Node Data contains the children list via JtNode_Group::ReadV10)
//=======================================================================
Standard_Boolean JtNode_Switch::ReadV10 (JtData_Reader& theReader)
{
  if (!JtNode_Group::ReadV10 (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  Jt_U32 aSelected;
  if (!theReader.ReadU32 (aSelected))
    return Standard_False;

  // U32 max value (0xFFFFFFFF) used to indicate "no child selected" (-1 semantics)
  mySelectedChild = (aSelected == 0xFFFFFFFFu) ? -1 : static_cast<Jt_I32>(aSelected);
  return Standard_True;
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtNode_Switch::Dump (Standard_OStream& theStream) const
{
  theStream << "JtNode_Switch (selected=" << mySelectedChild << ") ";
  return JtNode_Group::Dump (theStream);
}
