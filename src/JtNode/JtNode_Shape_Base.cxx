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

#include <JtNode_Shape_Base.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtNode_Shape_Base, JtNode_Base)

IMPLEMENT_OBJECT_CLASS(JtNode_Shape_Base, "Base Shape Object",
                       "10dd1059-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtNode_Shape_Base::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // Legacy path: JT 8.x / 9.x
  // JT 9.5 has a reserved BBoxF32 before the untransformed bbox; absent in v8.
  if (!JtNode_Base::Read (theReader))
    return Standard_False;

  if (theReader.Model()->MajorVersion() >= 9)
  {
    Jt_I16 aVersion;
    if (!theReader.ReadI16 (aVersion))
      return Standard_False;
  }

  return theReader.ReadUniformStruct<Jt_F32> (myReservedBnd)
      && theReader.ReadUniformStruct<Jt_F32> (myUntransBnd)
      && theReader.ReadF32   (myArea)
      && theReader.ReadArray (myVertexRange)
      && theReader.ReadArray (myNodeRange)
      && theReader.ReadArray (myPolyRange)
      && theReader.ReadI32   (mySize)
      && theReader.ReadF32   (myCompression);
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Base Shape Data (spec §6.1.1.10.1, Figure 36)
//           Base Node Data | U8 Version | BBoxF32 UntransformedBBox | F32 Area |
//           Vertex Count Range | Node Count Range | Polygon Count Range |
//           U32 Size | F32 Compression Level
//           Note: reserved BBoxF32 present in JT 9.5 is removed in JT 10.
//=======================================================================
Standard_Boolean JtNode_Shape_Base::ReadV10 (JtData_Reader& theReader)
{
  if (!JtNode_Base::ReadV10 (theReader))
    return Standard_False;

  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  return theReader.ReadUniformStruct<Jt_F32> (myUntransBnd)
      && theReader.ReadF32   (myArea)
      && theReader.ReadArray (myVertexRange)
      && theReader.ReadArray (myNodeRange)
      && theReader.ReadArray (myPolyRange)
      && theReader.ReadI32   (mySize)
      && theReader.ReadF32   (myCompression);
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtNode_Shape_Base::Dump (Standard_OStream& theStream) const
{
  theStream << "JtNode_Shape_Base ";
  return JtNode_Base::Dump (theStream);
}
