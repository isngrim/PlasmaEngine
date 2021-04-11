#pragma once

#if defined(PlasmaPlatformWindows)
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

namespace Plasma
{
class PlasmaNoImportExport RendererVKLibrary
{
public:
  static void Initialize();
  static void Shutdown();
};
} // namespace Plasma