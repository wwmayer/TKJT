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

#include <JtAttribute_Material.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtAttribute_Material, JtAttribute_Base)

IMPLEMENT_OBJECT_CLASS(JtAttribute_Material, "Material Object",
                       "10dd1030-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

namespace
{
  //! Auxiliary function
  static Standard_Boolean readColor (JtData_Reader&   theReader,
                                     Jt_F32         (&theVector) [4],
                                     Standard_Boolean theIsSingleF32 = Standard_False)
  {
    if (!theIsSingleF32)
      return theReader.ReadArray (theVector);

    Jt_F32 aVal;
    if (!theReader.ReadF32 (aVal))
      return Standard_False;

    theVector[0] = aVal;
    theVector[1] = aVal;
    theVector[2] = aVal;
    theVector[3] = 1.0;

    return Standard_True;
  }
}

//=======================================================================
//function : JtAttribute_Material
//purpose  :
//=======================================================================
JtAttribute_Material::JtAttribute_Material()
  : myVersion (0) {}

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtAttribute_Material::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // Legacy path: JT 8.x / 9.x
  if (!JtAttribute_Base::Read (theReader))
    return Standard_False;

  if (theReader.Model()->MajorVersion() > 8
  && !theReader.ReadI16 (myVersion))
    return Standard_False;

  if (!theReader.ReadU16 (myDataFlags)
   || !readColor (theReader, myAmbientColor , (myDataFlags & 3) == 3)
   || !readColor (theReader, myDiffuseColor)
   || !readColor (theReader, mySpecularColor, (myDataFlags & 5) == 5)
   || !readColor (theReader, myEmissionColor, (myDataFlags & 9) == 9)
   || !theReader.ReadF32 (myShininess))
    return Standard_False;

  if (myVersion == 2
  && !theReader.ReadF32 (myReflectivity))
    return Standard_False;

  return Standard_True;
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Material Attribute Element (spec §6.1.2.2, Figure 47)
//           Base Attribute Data | I8 Version | U16 Data Flags |
//           RGBA Ambient | RGBA Diffuse+Alpha | RGBA Specular | RGBA Emission |
//           F32 Shininess | F32 Reflectivity | F32 Bumpiness
//           Note: all RGBA always full 4xF32 in JT 10 (no compressed single-F32 shorthand).
//=======================================================================
Standard_Boolean JtAttribute_Material::ReadV10 (JtData_Reader& theReader)
{
  if (!JtAttribute_Base::ReadV10 (theReader))
    return Standard_False;

  Jt_U8  aVersion;
  Jt_F32 aBumpiness;
  if (!theReader.ReadU8  (aVersion)
   || !theReader.ReadU16 (myDataFlags)
   || !readColor (theReader, myAmbientColor)
   || !readColor (theReader, myDiffuseColor)
   || !readColor (theReader, mySpecularColor)
   || !readColor (theReader, myEmissionColor)
   || !theReader.ReadF32 (myShininess)
   || !theReader.ReadF32 (myReflectivity)
   || !theReader.ReadF32 (aBumpiness))
    return Standard_False;

  myVersion = aVersion;
  return Standard_True;
}



//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtAttribute_Material::Dump (Standard_OStream& theStream) const
{
  theStream << "JtAttribute_Material ";
  return JtAttribute_Base::Dump (theStream);
}
