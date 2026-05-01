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

#ifndef _JtNode_Switch_HeaderFile
#define _JtNode_Switch_HeaderFile

#include <JtNode_Group.hxx>

//! Switch Node Element (spec §6.1.1.9, Figure 34).
//! Like a Group Node but with a single "selected child" index that
//! indicates which child (or none) a traverser should process.
//! GUID: 10dd10f3-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
class JtNode_Switch : public JtNode_Group
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Return the selected child index (-1 means no child selected).
  Jt_I32 SelectedChild() const { return mySelectedChild; }

  DEFINE_STANDARD_RTTIEXT(JtNode_Switch,JtNode_Group)
  DEFINE_OBJECT_CLASS (JtNode_Switch)

protected:
  Jt_I32 mySelectedChild;
};

DEFINE_STANDARD_HANDLE(JtNode_Switch, JtNode_Group)

#endif // _JtNode_Switch_HeaderFile
