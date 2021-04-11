#pragma once

namespace Plasma
{
	inline void CreateInstance(VulkanRuntimeData& runtimeData)
	{
#if defined(PlasmaDebug)
		ErrorIf(!CheckValidationLayerSupport, "Validation Layers Not supported");
#endif

		VkResult res = volkInitialize();
		ErrorIf(res != VK_SUCCESS, "Volk Init Failed");

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = GetApplicationName().c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "PlasmaEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(GetMajorVersion(), GetMinorVersion(), GetPatchVersion());
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo{};
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

#if defined(PlasmaDebug)
		createInfo.enabledLayerCount = static_cast<u32>(validationLayer.Size());
		createInfo.ppEnabledLayerNames = validationLayer.Data();
		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
#else
  createInfo.enabledLayerCount = 0;
  createInfo.pNext = nullptr;
#endif

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.Size());
		createInfo.ppEnabledExtensionNames = extensions.Data();
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &runtimeData.mInstance);
		ErrorIf(result != VK_SUCCESS, "Unable to create vulkan instance");

		volkLoadInstanceOnly(runtimeData.mInstance);
	}
}
