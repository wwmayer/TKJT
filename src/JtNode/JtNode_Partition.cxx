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

#include <JtNode_Partition.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtNode_Partition, JtNode_Group)

IMPLEMENT_OBJECT_CLASS(JtNode_Partition, "Partition Object",
                       "10dd103e-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtNode_Partition::Read (JtData_Reader& theReader)
{
  myModel = theReader.Model();

  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // Legacy path: JT 8.x / 9.x
  if (!JtNode_Group::Read (theReader))
    return Standard_False;

  Jt_I32 aFlags;
  if (!theReader.ReadI32 (aFlags))
    return Standard_False;

  if (!theReader.ReadMbString (myFileName))
    return Standard_False;

  const Standard_Boolean hasUntransformedBndBox = ((aFlags & 1) != 0);

  Jt_BBoxF32 aBBox;
  if (!theReader.ReadUniformStruct<Jt_F32> (aBBox))
    return Standard_False;

  Jt_F32 anArea;
  Jt_I32 aVrtxRange[2];
  Jt_I32 aNodeRange[2];
  Jt_I32 aPolyRange[2];
  if (!theReader.ReadF32   (anArea)
   || !theReader.ReadArray (aVrtxRange)
   || !theReader.ReadArray (aNodeRange)
   || !theReader.ReadArray (aPolyRange))
    return Standard_False;

  if (hasUntransformedBndBox)
    if (!theReader.ReadUniformStruct<Jt_F32> (aBBox))
      return Standard_False;

  return Standard_True;
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Partition Node Data (spec §6.1.1.3, Figure 22/23)
//           After Group Node Data: U8 Version | I32 Flags | MbString FileName |
//           BBoxF32 TransformedBBox | F32 Area | I32[2] VertexRange |
//           I32[2] NodeRange | I32[2] PolyRange | (optional) BBoxF32 UntransBBox
//=======================================================================
Standard_Boolean JtNode_Partition::ReadV10 (JtData_Reader& theReader)
{
  if (!JtNode_Group::ReadV10 (theReader))
    return Standard_False;

  Jt_U8  aVersion;
  Jt_I32 aFlags;
  if (!theReader.ReadU8  (aVersion)
   || !theReader.ReadI32 (aFlags))
    return Standard_False;

  if (!theReader.ReadMbString (myFileName))
    return Standard_False;

  const Standard_Boolean hasUntransformedBndBox = ((aFlags & 1) != 0);

  Jt_BBoxF32 aBBox;
  Jt_F32     anArea;
  Jt_I32     aVrtxRange[2];
  Jt_I32     aNodeRange[2];
  Jt_I32     aPolyRange[2];
  if (!theReader.ReadUniformStruct<Jt_F32> (aBBox)
   || !theReader.ReadF32   (anArea)
   || !theReader.ReadArray (aVrtxRange)
   || !theReader.ReadArray (aNodeRange)
   || !theReader.ReadArray (aPolyRange))
    return Standard_False;

  if (hasUntransformedBndBox)
  {
    // Some JT 10 writers set this flag on root partitions but omit the bbox data.
    // Guard against reading past the end of the element.
    const Standard_Size anElemEnd = theReader.Model()->CurrentElementEnd();
    if (anElemEnd == 0 || theReader.GetPosition() + sizeof(Jt_BBoxF32) <= anElemEnd)
      if (!theReader.ReadUniformStruct<Jt_F32> (aBBox))
        return Standard_False;
  }

  return Standard_True;
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtNode_Partition::Dump (Standard_OStream& theStream) const
{
  theStream << "JtNode_Partition ";
  return JtNode_Group::Dump (theStream);
}

//=======================================================================
//function : Load
//purpose  : Load the referenced JT file and bind its LSG to this node
//=======================================================================
Standard_Boolean JtNode_Partition::Load()
{
  Handle(JtData_Model)     aModel    = new JtData_Model (myFileName, myModel);
  Handle(JtNode_Partition) aRootNode = aModel->Init();
  if (aRootNode.IsNull())
    return Standard_False;

  myChildren = aRootNode->Children();

  return Standard_True;
}

//=======================================================================
//function : Unload
//purpose  : Unbind children from this node and unload the referenced JT file
//=======================================================================
void JtNode_Partition::Unload()
{
  myChildren.Free();
}
