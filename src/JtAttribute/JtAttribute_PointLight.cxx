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

#include <JtAttribute_PointLight.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtAttribute_PointLight, JtAttribute_Base)

IMPLEMENT_OBJECT_CLASS(JtAttribute_PointLight, "Point Light Attribute",
                       "10dd1045-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

//=======================================================================
//function : Read
//purpose  : Read this entity from a translate file
//=======================================================================
Standard_Boolean JtAttribute_PointLight::Read (JtData_Reader& theReader)
{
  if (theReader.Model()->MajorVersion() >= 10)
    return ReadV10 (theReader);

  // No legacy (pre-10) definition in spec; skip gracefully
  return Standard_False;
}

//=======================================================================
//function : ReadV10
//purpose  : Read JT 10+ Point Light Attribute Element (spec §6.1.2.7, Figure 58)
//           Logical Element Header | Base Light Data | I8 Version |
//           HCoordF32 Position | Attenuation Coefficients |
//           F32 Spread Angle | DirF32 Spot Direction | I32 Spot Intensity
//
//           Point Light uses Base Light Data (NOT Base Attribute Data).
//           JtData_Object::Read is a no-op, so we read all fields directly.
//
//           Base Light Data (Figure 57):
//             U8 Version | RGBA AmbientColour | RGBA DiffuseColour | RGBA SpecularColour |
//             F32 Brightness | I32 CoordSystem | U8 ShadowCasterFlag |
//             F32 ShadowOpacity | F32 NonShadowAlphaFactor | F32 ShadowAlphaFactor
//
//           Attenuation Coefficients (Figure 60):
//             F32 Constant | F32 Linear | F32 Quadratic
//=======================================================================
Standard_Boolean JtAttribute_PointLight::ReadV10 (JtData_Reader& theReader)
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

  // --- Point Light version (Figure 58) ---
  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  // HCoordF32 Position: 4 x F32
  if (!theReader.ReadArray (myPosition))
    return Standard_False;

  // Attenuation Coefficients (Figure 60)
  if (!theReader.ReadF32 (myConstantAttenuation)
   || !theReader.ReadF32 (myLinearAttenuation)
   || !theReader.ReadF32 (myQuadraticAttenuation))
    return Standard_False;

  // Spread Angle, Spot Direction, Spot Intensity
  return theReader.ReadF32 (mySpreadAngle)
      && theReader.ReadArray (mySpotDirection)
      && theReader.ReadI32 (mySpotIntensity);
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtAttribute_PointLight::Dump (Standard_OStream& theStream) const
{
  theStream << "JtAttribute_PointLight (pos=["
    << myPosition[0] << "," << myPosition[1] << "," << myPosition[2] << "] "
    << "spread=" << mySpreadAngle << ") ";
  return JtAttribute_Base::Dump (theStream);
}
