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

#ifndef _JtProperty_Date_HeaderFile
#define _JtProperty_Date_HeaderFile

#include <JtProperty_Base.hxx>
#include <JtData_Types.hxx>

//! Date Property Atom Element (spec §6.2.1.6, Figure 75).
//! GUID: ce357246-38fb-11d1-a5-06-00-60-97-bd-c6-e1
class JtProperty_Date : public JtProperty_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  Jt_I16 Year()   const { return myYear; }
  Jt_I16 Month()  const { return myMonth; }
  Jt_I16 Day()    const { return myDay; }
  Jt_I16 Hour()   const { return myHour; }
  Jt_I16 Minute() const { return myMinute; }
  Jt_I16 Second() const { return mySecond; }

  DEFINE_STANDARD_RTTIEXT(JtProperty_Date,JtProperty_Base)
  DEFINE_OBJECT_CLASS (JtProperty_Date)

protected:
  Jt_I16 myYear;
  Jt_I16 myMonth;
  Jt_I16 myDay;
  Jt_I16 myHour;
  Jt_I16 myMinute;
  Jt_I16 mySecond;
};

DEFINE_STANDARD_HANDLE(JtProperty_Date, JtProperty_Base)

#endif // _JtProperty_Date_HeaderFile
