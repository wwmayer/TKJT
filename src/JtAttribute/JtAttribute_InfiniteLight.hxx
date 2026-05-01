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

#ifndef _JtAttribute_InfiniteLight_HeaderFile
#define _JtAttribute_InfiniteLight_HeaderFile

#include <JtAttribute_Base.hxx>

//! Infinite Light Attribute Element (spec §6.1.2.6, Figure 56).
//! GUID: 10dd1028-2ac8-11d1-9b-6b-00-80-c7-bb-59-97
//! Directional light; Base Light Data + Direction vector.
class JtAttribute_InfiniteLight : public JtAttribute_Base
{
public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  Standard_EXPORT virtual Standard_Boolean ReadV10 (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Return ambient RGBA (4 floats).
  const Jt_F32* AmbientColor()  const { return myAmbientColor;  }
  //! Return diffuse RGBA (4 floats).
  const Jt_F32* DiffuseColor()  const { return myDiffuseColor;  }
  //! Return specular RGBA (4 floats).
  const Jt_F32* SpecularColor() const { return mySpecularColor; }
  //! Return brightness.
  Jt_F32 Brightness()           const { return myBrightness;    }
  //! Return light direction (3 floats: X, Y, Z).
  const Jt_F32* Direction()     const { return myDirection;     }

  DEFINE_STANDARD_RTTIEXT(JtAttribute_InfiniteLight,JtAttribute_Base)
  DEFINE_OBJECT_CLASS (JtAttribute_InfiniteLight)

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
  // Infinite Light specific
  Jt_F32 myDirection[3];
};

DEFINE_STANDARD_HANDLE(JtAttribute_InfiniteLight, JtAttribute_Base)

#endif // _JtAttribute_InfiniteLight_HeaderFile
