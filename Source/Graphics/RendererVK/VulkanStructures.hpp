#pragma once

namespace Plasma
{	
	struct VulkanRuntimeData
	{
        IntVec2 mClientSize;
	    OsHandle mWindowHandle = nullptr;

		VkInstance mInstance;
        VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
		VkSurfaceKHR mSurface;
        VkDevice mDevice;
	};
}