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

#include <JtProperty_Date.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtProperty_Date, JtProperty_Base)

IMPLEMENT_OBJECT_CLASS(JtProperty_Date, "Date Property Atom",
                       "ce357246-38fb-11d1-a5-06-00-60-97-bd-c6-e1")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtProperty_Date::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  if (!JtProperty_Base::Read (theReader))
    return Standard_False;

  return theReader.ReadI16 (myYear)
      && theReader.ReadI16 (myMonth)
      && theReader.ReadI16 (myDay)
      && theReader.ReadI16 (myHour)
      && theReader.ReadI16 (myMinute)
      && theReader.ReadI16 (mySecond);
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Date Property Atom Element (spec Figure 75)
//           Base Property Atom Data | U8 Version |
//           I16 Year | I16 Month | I16 Day | I16 Hour | I16 Minute | I16 Second
//=======================================================================
Standard_Boolean JtProperty_Date::ReadV10 (JtData_Reader& theReader)
{
  if (!JtProperty_Base::ReadV10 (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  return theReader.ReadI16 (myYear)
      && theReader.ReadI16 (myMonth)
      && theReader.ReadI16 (myDay)
      && theReader.ReadI16 (myHour)
      && theReader.ReadI16 (myMinute)
      && theReader.ReadI16 (mySecond);
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtProperty_Date::Dump (Standard_OStream& theStream) const
{
  theStream << "JtProperty_Date ("
    << myYear << "-" << myMonth << "-" << myDay << " "
    << myHour << ":" << myMinute << ":" << mySecond << ") ";
  return JtProperty_Base::Dump (theStream);
}
