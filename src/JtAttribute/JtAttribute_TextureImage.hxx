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

#ifndef _JtAttribute_TextureImage_HeaderFile
#define _JtAttribute_TextureImage_HeaderFile

#include <JtAttribute_Base.hxx>
#include <TCollection_ExtendedString.hxx>
#include <vector>

//! Texture Image Attribute Element defines a texture image and its mapping
//! environment (spec §6.1.2.3).
class JtAttribute_TextureImage : public JtAttribute_Base
{
public:
  //! Image Format Description (spec §6.1.2.3.5).
  struct ImageFormatDesc
  {
    Jt_U32 PixelFormat;
    Jt_U32 PixelDataType;
    Jt_I16 Dimensionality;
    Jt_I16 RowAlignment;
    Jt_I16 Width;
    Jt_I16 Height;
    Jt_I16 Depth;
    Jt_I16 NbBorderTexels;
    Jt_U32 SharedImageFlag;
    Jt_I16 MipmapsCount;
  };

  //! Inline texture image (one per mipmap level) — raw byte blob.
  struct MipmapImage
  {
    std::vector<uint8_t> Texels;
  };

  //! One inline texture image (Image Format Description + mipmap chain).
  struct InlineImage
  {
    ImageFormatDesc Format;
    std::vector<MipmapImage> Mipmaps;
  };

public:
  //! Read this entity from a JT file.
  Standard_EXPORT Standard_Boolean Read (JtData_Reader& theReader) Standard_OVERRIDE;

  //! Dump this entity.
  Standard_EXPORT Standard_Integer Dump (Standard_OStream& theStream) const Standard_OVERRIDE;

  //! Access inline images (valid when inline storage flag == 1).
  const std::vector<InlineImage>&               InlineImages()   const { return myInlineImages; }
  //! Access external storage names (valid when inline storage flag == 0).
  const std::vector<TCollection_ExtendedString>& ExternalNames() const { return myExternalNames; }

  DEFINE_STANDARD_RTTIEXT(JtAttribute_TextureImage,JtAttribute_Base)
  DEFINE_OBJECT_CLASS (JtAttribute_TextureImage)

private:
  std::vector<InlineImage>               myInlineImages;
  std::vector<TCollection_ExtendedString> myExternalNames;
};

DEFINE_STANDARD_HANDLE(JtAttribute_TextureImage, JtAttribute_Base)

#endif // _JtAttribute_TextureImage_HeaderFile
