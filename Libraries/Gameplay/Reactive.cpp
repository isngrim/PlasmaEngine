// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Plasma
{

void Reactive::Initialize(CogInitializer& initializer)
{
  HasOrAdd<ReactiveSpace>(GetSpace());
}

void Reactive::Serialize(Serializer& stream)
{
  SerializeName(mActive);
}

LightningDefineType(Reactive, builder, type)
{
  PlasmaBindComponent();
  PlasmaBindSetup(SetupMode::CallSetDefaults);
  PlasmaBindDocumented();

  LightningBindFieldProperty(mActive);

}

void Reactive::SetDefaults()
{
  mActive = true;
}

Reactive::Reactive()
{
}

Reactive::~Reactive()
{
}

LightningDefineType(ReactiveSpace, builder, type)
{
  PlasmaBindComponent();
  PlasmaBindSetup(SetupMode::DefaultSerialization);
  PlasmaBindDocumented();

  PlasmaBindDependency(Space);

  LightningBindGetter(Over);
  LightningBindFieldProperty(mRaycaster);
}

void ReactiveSpace::Serialize(Serializer& stream)
{
  bool success = Serialization::Policy<Raycaster>::Serialize(stream, "Raycaster", mRaycaster);
  if (success == false)
  {
    mRaycaster.AddProvider(new PhysicsRaycastProvider());
  }
}

Cog* ReactiveSpace::GetOver()
{
  return mOver;
}

} // namespace Plasma
