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

#include <JtAttribute_TextureImage.hxx>
#include <JtData_Reader.hxx>

IMPLEMENT_STANDARD_RTTIEXT(JtAttribute_TextureImage, JtAttribute_Base)

IMPLEMENT_OBJECT_CLASS(JtAttribute_TextureImage, "Texture Image Attribute Object",
                       "10dd1073-2ac8-11d1-9b-6b-00-80-c7-bb-59-97")

// Helper: read Image Format Description and mipmap texel blobs.
static Standard_Boolean readInlineImage (JtData_Reader& theReader,
                                         JtAttribute_TextureImage::InlineImage& theImg)
{
  // Image Format Description (spec §6.1.2.3.5)
  JtAttribute_TextureImage::ImageFormatDesc& aFmt = theImg.Format;
  if (!theReader.ReadU32 (aFmt.PixelFormat)
   || !theReader.ReadU32 (aFmt.PixelDataType)
   || !theReader.ReadI16 (aFmt.Dimensionality)
   || !theReader.ReadI16 (aFmt.RowAlignment)
   || !theReader.ReadI16 (aFmt.Width)
   || !theReader.ReadI16 (aFmt.Height)
   || !theReader.ReadI16 (aFmt.Depth)
   || !theReader.ReadI16 (aFmt.NbBorderTexels)
   || !theReader.ReadU32 (aFmt.SharedImageFlag)
   || !theReader.ReadI16 (aFmt.MipmapsCount))
    return Standard_False;

  // Total Image Data Size (total bytes of all texel data, excluding byte-count fields)
  Jt_I32 aTotalSize;
  if (!theReader.ReadI32 (aTotalSize))
    return Standard_False;

  const Jt_I16 aNbMipmaps = aFmt.MipmapsCount;
  theImg.Mipmaps.resize (aNbMipmaps);

  for (Jt_I16 i = 0; i < aNbMipmaps; ++i)
  {
    Jt_I32 aByteCount;
    if (!theReader.ReadI32 (aByteCount))
      return Standard_False;

    if (aByteCount < 0)
      return Standard_False;

    theImg.Mipmaps[i].Texels.resize (static_cast<std::size_t> (aByteCount));
    if (aByteCount > 0
     && !theReader.ReadArray (theImg.Mipmaps[i].Texels.data(),
                              static_cast<std::size_t> (aByteCount)))
      return Standard_False;
  }

  return Standard_True;
}

//=======================================================================
//function : Read
//purpose  : Read this entity from a JT file (spec §6.1.2.3, Figure 48/49)
//           Base Attribute Data | U8 Version Number | Texture Vers-1 Data
//=======================================================================
Standard_Boolean JtAttribute_TextureImage::Read (JtData_Reader& theReader)
{
  if (!JtAttribute_Base::ReadV10 (theReader))
    return Standard_False;

  // Element-level version number (only version 1 currently defined)
  Jt_U8 aVersion;
  if (!theReader.ReadU8 (aVersion))
    return Standard_False;

  if (aVersion != 1)
    return Standard_True; // unknown version, skip gracefully

  // ---- Texture Vers-1 Data (spec §6.1.2.3.1, Figure 49) ----

  Jt_I32 aTextureType;
  if (!theReader.ReadI32 (aTextureType))
    return Standard_False;

  // Texture Environment (spec §6.1.2.3.2, Figure 50)
  // 8 × I32 scalar fields
  Jt_I32 aEnvInts[8];
  if (!theReader.ReadArray (aEnvInts))
    return Standard_False;

  // RGBA Blend Colour (4 × F32)
  Jt_F32 aBlendColour[4];
  if (!theReader.ReadArray (aBlendColour))
    return Standard_False;

  // RGBA Border Colour (4 × F32)
  Jt_F32 aBorderColour[4];
  if (!theReader.ReadArray (aBorderColour))
    return Standard_False;

  // Mx4F32 Texture Transform (4×4 = 16 × F32)
  Jt_F32 aTexTransform[16];
  if (!theReader.ReadArray (aTexTransform))
    return Standard_False;

  // Texture Coord Generation Parameters (spec §6.1.2.3.3, Figure 51)
  // 4 × I32 Tex Coord Gen Mode
  Jt_I32 aTexCoordGenMode[4];
  if (!theReader.ReadArray (aTexCoordGenMode))
    return Standard_False;

  // 4 × PlaneF32 (each plane = 4 × F32, total 16 × F32)
  Jt_F32 aRefPlanes[16];
  if (!theReader.ReadArray (aRefPlanes))
    return Standard_False;

  // Texture Channel
  Jt_I32 aTextureChannel;
  if (!theReader.ReadI32 (aTextureChannel))
    return Standard_False;

  // Tex Coord Channel
  Jt_I32 aTexCoordChannel;
  if (!theReader.ReadI32 (aTexCoordChannel))
    return Standard_False;

  // U32 Empty Field
  Jt_U32 anEmptyField;
  if (!theReader.ReadU32 (anEmptyField))
    return Standard_False;

  // Inline Image Storage Flag
  Jt_U8 anInlineFlag;
  if (!theReader.ReadU8 (anInlineFlag))
    return Standard_False;

  // Image Count
  Jt_I32 anImageCount;
  if (!theReader.ReadI32 (anImageCount))
    return Standard_False;

  if (anImageCount < 0)
    return Standard_False;

  if (anInlineFlag == 1)
  {
    // Inline Texture Image Data (spec §6.1.2.3.4)
    myInlineImages.resize (static_cast<std::size_t> (anImageCount));
    for (Jt_I32 i = 0; i < anImageCount; ++i)
      if (!readInlineImage (theReader, myInlineImages[i]))
        return Standard_False;
  }
  else
  {
    // External Storage Names (Image Count × MbString)
    myExternalNames.resize (static_cast<std::size_t> (anImageCount));
    for (Jt_I32 i = 0; i < anImageCount; ++i)
      if (!theReader.ReadMbString (myExternalNames[i]))
        return Standard_False;
  }

  return Standard_True;
}

//=======================================================================
//function : Dump
//purpose  :
//=======================================================================
Standard_Integer JtAttribute_TextureImage::Dump (Standard_OStream& theStream) const
{
  theStream << "JtAttribute_TextureImage ";
  return JtAttribute_Base::Dump (theStream);
}
