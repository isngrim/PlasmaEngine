#pragma once

namespace Plasma
{
	inline Array<const char*> GetRequiredExtensions()
	{
		Array<const char*> extensions;
#if defined(PlasmaDebug)
		extensions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
#if defined(PlasmaPlatformWindows)
		extensions.PushBack(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
		return extensions;
	}


	inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
	                                             const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	                                             const VkAllocationCallbacks* pAllocator,
	                                             VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	inline void DestroyDebugUtilsMessengerEXT(VkInstance instance,
	                                          VkDebugUtilsMessengerEXT debugMessenger,
	                                          const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}
}
