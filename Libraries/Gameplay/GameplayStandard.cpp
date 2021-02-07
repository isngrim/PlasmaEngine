// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

#include "IndexedHalfEdgeMesh.hpp"
#include "QuickHull3DBindings.hpp"

namespace Plasma
{

// Enums
LightningDefineEnum(OrientationBases);
LightningDefineEnum(PathFinderStatus);

LightningDefineRange(IndexedHalfEdgeMeshVertexArray::RangeType);
LightningDefineRange(IndexedHalfEdgeMeshEdgeArray::RangeType);
LightningDefineRange(IndexedHalfEdgeFaceEdgeIndexArray::RangeType);
LightningDefineRange(IndexedHalfEdgeMeshFaceArray::RangeType);

LightningDefineStaticLibrary(GameplayLibrary)
{
  builder.CreatableInScriptDefault = false;

  // Enums
  LightningInitializeEnum(OrientationBases);
  LightningInitializeEnum(PathFinderStatus);

  // Ranges
  LightningInitializeRangeAs(IndexedHalfEdgeMeshVertexArray::RangeType, "IndexedHalfEdgeMeshVertexArrayRange");
  LightningInitializeRangeAs(IndexedHalfEdgeMeshEdgeArray::RangeType, "IndexedHalfEdgeMeshEdgeArrayRange");
  LightningInitializeRangeAs(IndexedHalfEdgeFaceEdgeIndexArray::RangeType, "IndexedHalfEdgeFaceEdgeIndexArrayRange");
  LightningInitializeRangeAs(IndexedHalfEdgeMeshFaceArray::RangeType, "IndexedHalfEdgeMeshFaceArrayRange");


  LightningInitializeType(TileMapSource);
  LightningInitializeType(Reactive);
  LightningInitializeType(ReactiveSpace);
  LightningInitializeType(Orientation);
  LightningInitializeType(TileMap);
  LightningInitializeType(RandomContext);
  LightningInitializeType(DefaultGameSetup);
  LightningInitializeType(PathFinderBaseEvent);
  LightningInitializeTypeAs(PathFinderEvent<Vec3>, "PathFinderEvent");
  LightningInitializeTypeAs(PathFinderEvent<IntVec3>, "PathFinderGridEvent");
  LightningInitializeType(PathFinder);
  LightningInitializeType(PathFinderRequest);
  LightningInitializeType(PathFinderGrid);
  LightningInitializeType(PathFinderMesh);

  LightningInitializeType(UnitTestSystem);
  LightningInitializeType(UnitTestEvent);
  LightningInitializeType(UnitTestEndFrameEvent);
  LightningInitializeType(UnitTestBaseMouseEvent);
  LightningInitializeType(UnitTestMouseEvent);
  LightningInitializeType(UnitTestMouseDropEvent);
  LightningInitializeType(UnitTestKeyboardEvent);
  LightningInitializeType(UnitTestKeyboardTextEvent);
  LightningInitializeType(UnitTestWindowEvent);

  LightningInitializeType(IndexedHalfEdgeMeshVertexArray);
  LightningInitializeType(IndexedHalfEdgeMeshEdgeArray);
  LightningInitializeType(IndexedHalfEdgeFaceEdgeIndexArray);
  LightningInitializeType(IndexedHalfEdgeMeshFaceArray);
  LightningInitializeType(IndexedHalfEdge);
  LightningInitializeType(IndexedHalfEdgeFace);
  LightningInitializeType(IndexedHalfEdgeMesh);
  LightningInitializeTypeAs(QuickHull3DInterface, "QuickHull3D");

  LightningInitializeTypeAs(PlasmaStatic, "Plasma");

  EngineLibraryExtensions::AddNativeExtensions(builder);
}

void GameplayLibrary::Initialize()
{
  BuildStaticLibrary();
  MetaDatabase::GetInstance()->AddNativeLibrary(GetLibrary());

  InitializeResourceManager(TileMapSourceManager);
}

void GameplayLibrary::Shutdown()
{
  GetLibrary()->ClearComponents();
}

} // namespace Plasma
