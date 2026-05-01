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

#include <JtAttribute_InfiniteLight.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtAttribute_InfiniteLight, JtAttribute_Base)

IMPLEMENT_OBJECT_CLASS(JtAttribute_InfiniteLight, "Infinite Light Attribute",
                       "10dd1028-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtAttribute_InfiniteLight::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // No legacy (pre-10) definition in spec; skip gracefully
  return Standard_False;
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Infinite Light Attribute Element (spec Figure 56)
//           Logical Element Header | Base Light Data | I8 Version | DirF3 Direction
//
//           Infinite Light uses Base Light Data (NOT Base Attribute Data).
//           JtData_Object::Read is a no-op, so we read all fields directly.
//
//           Base Light Data (Figure 57):
//             I8 Version | RGBA AmbientColour | RGBA DiffuseColour | RGBA SpecularColour |
//             F32 Brightness | I32 CoordSystem | U8 ShadowCasterFlag |
//             F32 ShadowOpacity | F32 NonShadowAlphaFactor | F32 ShadowAlphaFactor
//=======================================================================
Standard_Boolean JtAttribute_InfiniteLight::ReadV10 (JtData_Reader& theReader)
{
  // --- Base Light Data (Figure 57) ---
  Jt_U8 aBaseLightVersion;
  if (!theReader.ReadU8 (aBaseLightVersion))
    return Standard_False;

  if (!theReader.ReadArray (myAmbientColor)
   || !theReader.ReadArray (myDiffuseColor)
   || !theReader.ReadArray (mySpecularColor)
   || !theReader.ReadF32 (myBrightness)
   || !theReader.ReadI32 (myCoordSystem)
   || !theReader.ReadU8  (myShadowCasterFlag)
   || !theReader.ReadF32 (myShadowOpacity)
   || !theReader.ReadF32 (myNonShadowAlphaFactor)
   || !theReader.ReadF32 (myShadowAlphaFactor))
    return Standard_False;

  // --- Infinite Light version (Figure 56) ---
  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  // DirF3: 3 x F32
  return theReader.ReadArray (myDirection);
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtAttribute_InfiniteLight::Dump (Standard_OStream& theStream) const
{
  return JtAttribute_Base::Dump (theStream);
}
