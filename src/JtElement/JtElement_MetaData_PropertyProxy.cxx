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

#include <JtElement_MetaData_PropertyProxy.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtElement_MetaData_PropertyProxy, JtData_Object)

IMPLEMENT_OBJECT_CLASS(JtElement_MetaData_PropertyProxy, "Property Proxy Meta Data Object",
                       "ce357247-38fb-11d1-a5-06-00-60-97-bd-c6-e1")

//=======================================================================
//function : Read
//purpose  : Read this entity from a JT file (spec §8.1.4, Figure 108)
//           Logical Element Header Compressed | U8 Version |
//           while (MbString Key not empty) { U8 Type, Value }
//=======================================================================
Standard_Boolean JtElement_MetaData_PropertyProxy::Read (JtData_Reader& theReader)
{
  if (!JtData_Object::Read (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  myProperties.clear();

  for (;;)
  {
    Property aProp;
    aProp.Type       = 0;
    aProp.IntValue   = 0;
    aProp.FloatValue = 0.0f;
    aProp.Year = aProp.Month = aProp.Day = 0;
    aProp.Hour = aProp.Minute = aProp.Second = 0;

    if (!theReader.ReadMbString (aProp.Key))
      return Standard_False;

    if (aProp.Key.Length() == 0)
      break; // empty key signals end of list

    if (!theReader.ReadU8 (aProp.Type))
      return Standard_False;

    switch (aProp.Type)
    {
      case 0: // Unknown / no value
        break;
      case 1: // MbString
        if (!theReader.ReadMbString (aProp.StringValue))
          return Standard_False;
        break;
      case 2: // I32
        if (!theReader.ReadI32 (aProp.IntValue))
          return Standard_False;
        break;
      case 3: // F32
        if (!theReader.ReadF32 (aProp.FloatValue))
          return Standard_False;
        break;
      case 4: // Date (I16 x6: Year Month Day Hour Minute Second)
        if (!theReader.ReadI16 (aProp.Year)
         || !theReader.ReadI16 (aProp.Month)
         || !theReader.ReadI16 (aProp.Day)
         || !theReader.ReadI16 (aProp.Hour)
         || !theReader.ReadI16 (aProp.Minute)
         || !theReader.ReadI16 (aProp.Second))
          return Standard_False;
        break;
      default:
        return Standard_False; // unknown type, cannot continue parsing
    }

    myProperties.push_back (aProp);
  }

  return Standard_True;
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtElement_MetaData_PropertyProxy::Dump (Standard_OStream& theStream) const
{
  theStream << "JtElement_MetaData_PropertyProxy ";
  return JtData_Object::Dump (theStream);
}
