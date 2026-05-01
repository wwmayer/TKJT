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

#ifndef _JtElement_MetaData_PropertyProxy_HeaderFile
#define _JtElement_MetaData_PropertyProxy_HeaderFile

#include <JtData_Object.hxx>
#include <TCollection_ExtendedString.hxx>
#include <vector>
#include <utility>

//! Property Proxy Meta Data Element serves as a proxy for all meta-data
//! properties associated with a particular Meta Data Node Element. Properties
//! are name/value pairs where the value type is one of String, I32, F32, or Date.
class JtElement_MetaData_PropertyProxy : public JtData_Object
{
public:
  //! Property value union storage.
  struct Property
  {
    TCollection_ExtendedString Key;
    Jt_U8                      Type; //!< 1=String 2=I32 3=F32 4=Date
    TCollection_ExtendedString StringValue;
    Jt_I32                     IntValue;
    Jt_F32                     FloatValue;
    Jt_I16                     Year, Month, Day, Hour, Minute, Second;
  };

  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Access the parsed properties.
  const std::vector<Property>& Properties() const { return myProperties; }

  DEFINE_STANDARD_RTTIEXT(JtElement_MetaData_PropertyProxy,JtData_Object)
  DEFINE_OBJECT_CLASS (JtElement_MetaData_PropertyProxy)

private:
  std::vector<Property> myProperties;
};

DEFINE_STANDARD_HANDLE(JtElement_MetaData_PropertyProxy, JtData_Object)

#endif // _JtElement_MetaData_PropertyProxy_HeaderFile
