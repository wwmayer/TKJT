// JT format reading and visualization tools
// Copyright (C) 2014-2015 OPEN CASCADE SAS
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

#include <JtDecode_VertexData_Deering.hxx>

//#define NO_JT_MULTITHREADING
#include <JtData_Parallel.hxx>
#include <cmath>

namespace
{
  static const Standard_Integer TABLE_BITS = 13;

  struct SinCos
  {
    Standard_Real Sin, Cos;
    void Set (Standard_Real aValue)
    {
      Sin = sin (aValue);
      Cos = cos (aValue);
    }
  };

  static Standard_Real Phi[16]= {0.904837, 0.818731, 0.740818,	0.670320,	0.606531,	0.548812, 0.449329, 0.367879,
                                 0.22313,  0.135335, 0.090484,	0.163746,	0.222245,	0.268128,	0.303265,	0.329287};
  class LookupTable
  {
    static const size_t BITS_SIZE = 16;
    static const size_t TABLE_SIZE = 1 << TABLE_BITS;
  public:
    SinCos Theta[TABLE_SIZE + BITS_SIZE];
    SinCos Psi  [TABLE_SIZE + BITS_SIZE];
    LookupTable()
    {
      Standard_Real aMaxPsi     = 0.615479709;
      Standard_Real aTableSizeF = (Standard_Real) TABLE_SIZE;
      size_t i = 0;
      for ( ; i <= TABLE_SIZE; ++i)
      {
        Theta[i].Set (std::asin (std::tan (aMaxPsi * (TABLE_SIZE - i) / aTableSizeF)));
        Psi  [i].Set (aMaxPsi * (i / aTableSizeF));
      }
      size_t j = 0;
      for (i=TABLE_SIZE+1 ; i < TABLE_SIZE + BITS_SIZE; i++,j++)
      {
        Theta[i].Set (std::asin (std::tan (Phi[j])));
        Psi  [i].Set (Phi[j]);
      }
    }
  };

  static const LookupTable LOOKUP_TABLE;
}

JtDecode_VertexData_Deering::JtDecode_VertexData_Deering (const Jt_U8      theNbBits,
                                                          JtDecode_Unpack& theUnpacker)
  : JtDecode_VertexData (4, theUnpacker)
  , myNbBits (theNbBits)
  , myIsV10 (Standard_False) {}

// V10 constructor: 1 CDP, packed code = [sextant:3][octant:3][theta:n][psi:n]
// Spec ref: Fig 139 (JT v10 Rev C, p.165-166); Annex B.4.2 unpackCode (p.237-238)
JtDecode_VertexData_Deering::JtDecode_VertexData_Deering (const Jt_U8            theNbBits,
                                                          const Standard_Boolean theIsV10)
  : JtDecode_VertexData (1, JtDecode_Unpack_Null)
  , myNbBits (theNbBits)
  , myIsV10 (Standard_True)
{
  (void)theIsV10; // always true; parameter is only for overload disambiguation
}

Standard_Integer JtDecode_VertexData_Deering::getOutCompCount (Standard_Size)
{
  return 3;
}

void JtDecode_VertexData_Deering::decode (Decoded::Ref theResults)
{
  if (myIsV10)
  {
    // V10: single CDP with packed codes, one per normal.
    // Spec ref: Fig 139 (JT v10 Rev C, p.165-166); Annex B.4.2 unpackCode (p.237-238):
    //   code layout = [sextant:3][octant:3][theta:numBits][psi:numBits]
    Jt_VecI32 aCodes (decodePackage (0));
    Standard_Integer anOffset = TABLE_BITS - myNbBits;
    Jt_U32 aMask = myNbBits ? ((1u << myNbBits) - 1u) : 0u;
    for (Decoded::SizeType i = 0; i < theResults.Count(); ++i)
    {
      Jt_U32 aCode    = static_cast<Jt_U32> (aCodes[i]);
      Jt_U32 aSextant = (aCode >> (2 * myNbBits + 3)) & 7u;
      Jt_U32 anOctant = (aCode >> (2 * myNbBits))     & 7u;
      Jt_U32 aTheta   = (aCode >>      myNbBits)       & aMask;
      Jt_U32 aPsi     =  aCode                         & aMask;

      const SinCos& aThetaPar = LOOKUP_TABLE.Theta[(aTheta + (aSextant & 1)) << anOffset];
      const SinCos& aPsiPar   = LOOKUP_TABLE.Psi  [ aPsi                     << anOffset];

      Jt_F32 x = static_cast <Jt_F32> (aPsiPar.Cos * aThetaPar.Cos);
      Jt_F32 y = static_cast <Jt_F32> (aPsiPar.Sin);
      Jt_F32 z = static_cast <Jt_F32> (aPsiPar.Cos * aThetaPar.Sin);

      Jt_DirF32 aResult;
      switch (aSextant)
      {
      default:
      case 0: aResult.X = x;  aResult.Y = y;  aResult.Z = z;  break; // No op
      case 1: aResult.X = z;  aResult.Y = y;  aResult.Z = x;  break; // Mirror about x=z
      case 2: aResult.X = y;  aResult.Y = z;  aResult.Z = x;  break; // Rotate CW
      case 3: aResult.X = y;  aResult.Y = x;  aResult.Z = z;  break; // Mirror about x=y
      case 4: aResult.X = z;  aResult.Y = x;  aResult.Z = y;  break; // Rotate CCW
      case 5: aResult.X = x;  aResult.Y = z;  aResult.Z = y;  break; // Mirror about y=z
      }

      if ((anOctant & 4) == 0) aResult.X = -aResult.X;
      if ((anOctant & 2) == 0) aResult.Y = -aResult.Y;
      if ((anOctant & 1) == 0) aResult.Z = -aResult.Z;

      theResults[i] = aResult;
    }
    return;
  }

  Jt_VecU32 aSextantCodes, anOctantCodes, aThetaCodes, aPsiCodes;

  JtData_Parallel::TaskGroup aDecodeTasks;
  aDecodeTasks.Run (getDecodingFunctor (0, aSextantCodes));
  aDecodeTasks.Run (getDecodingFunctor (1, anOctantCodes));
  aDecodeTasks.Run (getDecodingFunctor (2, aThetaCodes));
  aDecodeTasks.Run (getDecodingFunctor (3, aPsiCodes));
  aDecodeTasks.Wait();

  Standard_Integer anOffset = TABLE_BITS - myNbBits;
  for (Decoded::SizeType i = 0; i < theResults.Count(); ++i)
  {
    Jt_U32 aSextant = aSextantCodes[i];

    const SinCos& aThetaPar = LOOKUP_TABLE.Theta[(aThetaCodes[i] + (aSextant & 1)) << anOffset];
    const SinCos& aPsiPar   = LOOKUP_TABLE.Psi  [ aPsiCodes  [i]                   << anOffset];

    Jt_F32 x = static_cast <Jt_F32> (aPsiPar.Cos * aThetaPar.Cos);
    Jt_F32 y = static_cast <Jt_F32> (aPsiPar.Sin);
    Jt_F32 z = static_cast <Jt_F32> (aPsiPar.Cos * aThetaPar.Sin);

    // Change coordinates based on the sextant
    Jt_DirF32 aResult;
    switch (aSextant)
    {
    default:
    case 0: // No op
      aResult.X = x;
      aResult.Y = y;
      aResult.Z = z;
      break;

    case 1: // Mirror about x=z plane
      aResult.X = z;
      aResult.Y = y;
      aResult.Z = x;
      break;

    case 2: // Rotate CW
      aResult.X = y;
      aResult.Y = z;
      aResult.Z = x;
      break;

    case 3: // Mirror about x=y plane
      aResult.X = y;
      aResult.Y = x;
      aResult.Z = z;
      break;

    case 4: // Rotate CCW
      aResult.X = z;
      aResult.Y = x;
      aResult.Z = y;
      break;

    case 5: // Mirror about y=z plane
      aResult.X = x;
      aResult.Y = z;
      aResult.Z = y;
      break;
    }

    // Change some more based on the octant
    Jt_U32 anOctant = anOctantCodes[i];

    // if first bit is 0, negate x component
    if ((anOctant & 4) == 0)
      aResult.X = -aResult.X;

    // if second bit is 0, negate y component
    if ((anOctant & 2) == 0)
      aResult.Y = -aResult.Y;

    // if third bit is 0, negate z component
    if ((anOctant & 1) == 0)
      aResult.Z = -aResult.Z;

    theResults[i] = aResult;
  }
}
