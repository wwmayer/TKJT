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

#ifndef _JtAttribute_PointLight_HeaderFile
#define _JtAttribute_PointLight_HeaderFile

#include <JtAttribute_Base.hxx>

//! Point Light Attribute Element (spec §6.1.2.7, Figure 58).
//! Emits light from a position, optionally as a spot with a cone angle.
//! Uses Base Light Data (not Base Attribute Data) — fields are read directly.
//! GUID: 10dd1045-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
class JtAttribute_PointLight : public JtAttribute_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  DEFINE_STANDARD_RTTIEXT(JtAttribute_PointLight,JtAttribute_Base)
  DEFINE_OBJECT_CLASS (JtAttribute_PointLight)

protected:
  // Base Light Data fields
  Jt_F32 myAmbientColor[4];
  Jt_F32 myDiffuseColor[4];
  Jt_F32 mySpecularColor[4];
  Jt_F32 myBrightness;
  Jt_I32 myCoordSystem;
  Jt_U8  myShadowCasterFlag;
  Jt_F32 myShadowOpacity;
  Jt_F32 myNonShadowAlphaFactor;
  Jt_F32 myShadowAlphaFactor;

  // Point Light specific fields
  Jt_F32 myPosition[4];         // HCoordF32 (homogeneous: x,y,z,w)
  Jt_F32 myConstantAttenuation;
  Jt_F32 myLinearAttenuation;
  Jt_F32 myQuadraticAttenuation;
  Jt_F32 mySpreadAngle;
  Jt_F32 mySpotDirection[3];    // DirF32
  Jt_I32 mySpotIntensity;
};

DEFINE_STANDARD_HANDLE(JtAttribute_PointLight, JtAttribute_Base)

#endif // _JtAttribute_PointLight_HeaderFile
