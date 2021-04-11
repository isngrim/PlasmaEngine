#pragma once

namespace Plasma
{
	const Array<const char*> validationLayer = {"VK_LAYER_KHRONOS_validation"};

	inline bool CheckValidationLayerSupport()
	{
		u32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		Array<VkLayerProperties> avalibleLayer(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, avalibleLayer.Data());

		for (const char* layerNames : validationLayer)
		{
			bool layerFound = false;

			for (const VkLayerProperties& layerProperties : avalibleLayer)
			{
				if (strcmp(layerNames, layerProperties.layerName) == 0)
				{
					layerFound = true;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
	                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	                                                    void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		{
			Error("Vulkan Validation Error: %s", pCallbackData->pMessage);
			break;
		}
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		{
			Warn("Vulkan Validation Warning: %s", pCallbackData->pMessage);
		}
		defaut:
			{
				PlasmaPrintFilter(Filter::RendererFilter, "Vulkan Validation Log: %s", pCallbackData->pMessage);
				break;
			}
		}

		return VK_FALSE;
	}

	inline void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
  {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
  }
	

	inline void SetupDebugMessenger(VulkanRuntimeData& runtimeData)
	{
#if defined(PlasmaDebug)
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(runtimeData.mInstance, &createInfo, nullptr, &runtimeData.mDebugMessenger) != VK_SUCCESS)
      Error("Vulkan: Failed to setup debug manager");
	}
#endif
} // namespace Plasma
