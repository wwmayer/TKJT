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

#include <JtAttribute_DrawStyle.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtAttribute_DrawStyle, JtAttribute_Base)

IMPLEMENT_OBJECT_CLASS(JtAttribute_DrawStyle, "Draw Style Attribute",
                       "10dd1014-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtAttribute_DrawStyle::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  if (!JtAttribute_Base::Read (theReader))
    return Standard_False;

  return theReader.ReadU8 (myDataFlags);
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Draw Style Attribute Element (spec §6.1.2.4, Figure 54)
//           Base Attribute Data | I8 Version | U8 Data Flags
//=======================================================================
Standard_Boolean JtAttribute_DrawStyle::ReadV10 (JtData_Reader& theReader)
{
  if (!JtAttribute_Base::ReadV10 (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  return theReader.ReadU8 (myDataFlags);
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtAttribute_DrawStyle::Dump (Standard_OStream& theStream) const
{
  theStream << "JtAttribute_DrawStyle (flags=0x" << std::hex << (int)myDataFlags
            << std::dec << ") ";
  return JtAttribute_Base::Dump (theStream);
}
