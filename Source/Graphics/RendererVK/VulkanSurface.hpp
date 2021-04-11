#pragma once
#if defined(PlasmaPlatformWindows)
#include "vulkan/vulkan_win32.h"
#endif
namespace Plasma
{
	inline void CreateSurface(VulkanRuntimeData& runtimeData)
	{
#if defined(PlasmaPlatformWindows)
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = (HWND)runtimeData.mWindowHandle;
    createInfo.hinstance = GetModuleHandle(nullptr);

    auto CreateWin32SurfaceKHR =
        (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(runtimeData.mInstance, "vkCreateWin32SurfaceKHR");

    if (!CreateWin32SurfaceKHR)
    {
      Error("Vulkan: No Vulkan Surface");
    }
    if(CreateWin32SurfaceKHR(runtimeData.mInstance, &createInfo, nullptr, &runtimeData.mSurface) != VK_SUCCESS)
    {
      Error("Vulkan: Surface failed to create");
    }
#else
	if (!SDL_Vulkan_CreateSurface(window, instance, &surface))
	{
		throw sdl2::SDLError("Error creating a vulkan surface");
	}
#endif
	}
}
