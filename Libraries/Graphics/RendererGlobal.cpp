// MIT Licensed (see LICENSE.md).

#include "Precompiled.hpp"

namespace Plasma
{

namespace PL
{
Renderer* gRenderer;
}

LightningDefineExternalBaseType(GraphicsDriverSupport, TypeCopyMode::ReferenceType, builder, type)
{
  type->HandleManager = LightningManagerId(PointerManager);
  
  LightningBindFieldGetter(mTesselation);
  LightningBindFieldGetter(mUAVLoadFormatCommon);
  LightningBindFieldGetter(mRaytracing);
  LightningBindFieldGetter(mRaytracingInline);
  LightningBindFieldGetter(mMeshShaders);
  LightningBindFieldGetter(mVariableRateShading);
  LightningBindFieldGetter(mVariableRateShadingTier2);
  LightningBindFieldGetter(mBindlessDescriptors);
  LightningBindFieldGetter(mUAVFormatR11G11B10F);
}

} // namespace Plasma
