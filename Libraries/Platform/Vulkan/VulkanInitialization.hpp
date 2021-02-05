#pragma once
#include "VulkanValidationLayer.hpp"

#if defined (PlasmaRelease)
    const bool gEnableValidationLayers = false;
#else
    const bool gEnableValidationLayers = true;
#endif

namespace Plasma
{
    inline void InitializeVulkan(RendererInitializeData initializeData, VulkanRuntimeData& vulkanRuntimeData, VulkanFeatureData& vulkanFeatureData, VulkanPropertyData& vulkanPropertyData, GraphicsDriverSupport& mDriverSupport)
    {
		if(gEnableValidationLayers && !CheckValidationLayerSupport())
		{
		    ErrorIf(true, "Debug Layers Not Supported");
		}

    	VkResult res = volkInitialize();
    	Assert(res == VK_SUCCESS);

        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = initializeData.mTitle.c_str();
        applicationInfo.applicationVersion = VK_MAKE_VERSION(0,0,1); //TODO: give real version info
        applicationInfo.pEngineName = "Plasma";
        applicationInfo.engineVersion = VK_MAKE_VERSION(GetMajorVersion(), GetMinorVersion(), GetRevisionNumber());
        applicationInfo.apiVersion = VK_API_VERSION_1_2;

        u32 extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        Array<VkExtensionProperties> availableInstanceExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableInstanceExtensions.Data());

        Array<const char*> extensionNames;
        
        if (CheckExtensionSupprt(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, availableInstanceExtensions))
        {
            // This is needed for not only debug layer, but also debug markers, object naming, etc:
            extensionNames.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
		
        extensionNames.PushBack(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(PlasmaPlatformWindows)
        extensionNames.PushBack(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
        extensionNames.PushBack(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif


        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &applicationInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.Size());
        createInfo.ppEnabledExtensionNames = extensionNames.Data();
        createInfo.enabledLayerCount = 0;
        if (gEnableValidationLayers)
        {
            Array<const char*> validationLayers = GetValidationLayers();
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.Size());
            createInfo.ppEnabledLayerNames = validationLayers.Data();
        }
        
        VkResult result = vkCreateInstance(&createInfo, nullptr, &vulkanRuntimeData.mInstance);
        if(result != VK_SUCCESS)
        {
        	Error("Unable to create instance");
        }
       
        
#if defined(PlasmaPlatformWindows)
        VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.hwnd = (HWND)initializeData.mWindow;
        surfaceInfo.hinstance = GetModuleHandle(nullptr);

        auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(vulkanRuntimeData.mInstance, "vkCreateWin32SurfaceKHR");

        if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(vulkanRuntimeData.mInstance, &surfaceInfo, nullptr, &vulkanRuntimeData.mSurface) != VK_SUCCESS)
            Error("Failed to create Surface");
    	
#else        
        #error ERROR: PLATFORM NOT SUPPORTED
#endif

        {
			uint32_t deviceCount = 0;
			res = vkEnumeratePhysicalDevices(vulkanRuntimeData.mInstance, &deviceCount, nullptr);
			Assert(res == VK_SUCCESS);

			if (deviceCount == 0)
			{
				DoNotifyWarning("Vulkan Not Supported", "Failed to find a GPU with Vulkan support!");
			}

			Array<VkPhysicalDevice> devices(deviceCount);
			res = vkEnumeratePhysicalDevices(vulkanRuntimeData.mInstance, &deviceCount, devices.Data());
			Assert(res == VK_SUCCESS);

			Array<const char*> required_deviceExtensions;
          	required_deviceExtensions.PushBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			required_deviceExtensions.PushBack(VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME);
			Array<const char*> enabled_deviceExtensions;

			for(int i = 0; i < devices.Size(); i++)
			{
				bool suitable = true;

				uint32_t extensionCount;
				VkResult res = vkEnumerateDeviceExtensionProperties(devices[i], nullptr, &extensionCount, nullptr);
				Assert(res == VK_SUCCESS);
				Array<VkExtensionProperties> available_deviceExtensions(extensionCount);
				res = vkEnumerateDeviceExtensionProperties(devices[i], nullptr, &extensionCount, available_deviceExtensions.Data());
				Assert(res == VK_SUCCESS);

				for(int j = 0; j < required_deviceExtensions.Size(); j++)
				{
					if (!CheckExtensionSupprt(required_deviceExtensions[j], available_deviceExtensions))
					{
						suitable = false;
					}
				}
				if (!suitable)
					continue;

				// Swapchain query:
				res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[i], vulkanRuntimeData.mSurface, &vulkanRuntimeData.mSwapchainCapabilities);
				Assert(res == VK_SUCCESS);

				uint32_t formatCount;
				res = vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], vulkanRuntimeData.mSurface, &formatCount, nullptr);
				Assert(res == VK_SUCCESS);

				if (formatCount != 0)
				{
					vulkanRuntimeData.mSwapchainFormats.Resize(formatCount);
					res = vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], vulkanRuntimeData.mSurface, &formatCount, vulkanRuntimeData.mSwapchainFormats.Data());
					Assert(res == VK_SUCCESS);
				}

				uint32_t presentModeCount;
				res = vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], vulkanRuntimeData.mSurface, &presentModeCount, nullptr);
				Assert(res == VK_SUCCESS);

				if (presentModeCount != 0) {
					vulkanRuntimeData.mSwapchainPresentModes.Resize(presentModeCount);
					res = vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], vulkanRuntimeData.mSurface, &presentModeCount, vulkanRuntimeData.mSwapchainPresentModes.Data());
					Assert(res == VK_SUCCESS);
				}

				suitable = !vulkanRuntimeData.mSwapchainFormats.Empty() && !vulkanRuntimeData.mSwapchainPresentModes.Empty();

				if (suitable) 
				{
					vulkanFeatureData.mFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
					vulkanFeatureData.mFeatures1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
					vulkanFeatureData.mFeatures1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
					vulkanFeatureData.mFeatures2.pNext = &vulkanFeatureData.mFeatures1_1;
					vulkanFeatureData.mFeatures1_1.pNext = &vulkanFeatureData.mFeatures1_2;
					void** features_chain = &vulkanFeatureData.mFeatures1_2.pNext;
					vulkanFeatureData.mAccelerationStructureFeatures = {};
					vulkanFeatureData.mRaytracingFeatures = {};
					vulkanFeatureData.mRaytracingQueryFeatures = {};
					vulkanFeatureData.mFragmentShadingRateFeatures = {};
					vulkanFeatureData.mMeshShaderFeatures = {};

					vulkanPropertyData.mProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
					vulkanPropertyData.mProperties1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
					vulkanPropertyData.mProperties1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
					vulkanPropertyData.mProperties2.pNext = &vulkanPropertyData.mProperties1_1;
					vulkanPropertyData.mProperties1_1.pNext = &vulkanPropertyData.mProperties1_2;
					void** properties_chain = &vulkanPropertyData.mProperties1_2.pNext;
					vulkanPropertyData.mAccelerationStructureProperties = {};
					vulkanPropertyData.mRaytracingProperties = {};
					vulkanPropertyData.mFragmentShadingRateProperties = {};
					vulkanPropertyData.mMeshShaderProperties = {};

					enabled_deviceExtensions = required_deviceExtensions;

					if (CheckExtensionSupprt(VK_KHR_SPIRV_1_4_EXTENSION_NAME, available_deviceExtensions))
					{
						enabled_deviceExtensions.PushBack(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
					}

					if (CheckExtensionSupprt(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME, available_deviceExtensions))
					{
						enabled_deviceExtensions.PushBack(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
						vulkanFeatureData.mAccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
						*features_chain = &vulkanFeatureData.mAccelerationStructureFeatures;
						features_chain = &vulkanFeatureData.mAccelerationStructureFeatures.pNext;
						vulkanPropertyData.mAccelerationStructureProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR;
						*properties_chain = &vulkanPropertyData.mAccelerationStructureProperties;
						properties_chain = &vulkanPropertyData.mAccelerationStructureProperties.pNext;

						if (CheckExtensionSupprt(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, available_deviceExtensions))
						{
							enabled_deviceExtensions.PushBack(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
							enabled_deviceExtensions.PushBack(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME);
							vulkanFeatureData.mRaytracingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
							*features_chain = &vulkanFeatureData.mRaytracingFeatures;
							features_chain = &vulkanFeatureData.mRaytracingFeatures.pNext;
							vulkanPropertyData.mRaytracingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
							*properties_chain = &vulkanPropertyData.mRaytracingProperties;
							properties_chain = &vulkanPropertyData.mRaytracingProperties.pNext;
						}

						if (CheckExtensionSupprt(VK_KHR_RAY_QUERY_EXTENSION_NAME, available_deviceExtensions))
						{
							enabled_deviceExtensions.PushBack(VK_KHR_RAY_QUERY_EXTENSION_NAME);
							enabled_deviceExtensions.PushBack(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
							vulkanFeatureData.mRaytracingQueryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
							*features_chain = &vulkanFeatureData.mRaytracingQueryFeatures;
							features_chain = &vulkanFeatureData.mRaytracingQueryFeatures.pNext;
						}
					}

					if (!gEnableValidationLayers && CheckExtensionSupprt(VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME, available_deviceExtensions))
					{
						// Note: VRS will crash vulkan validation layers: https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/2473
						enabled_deviceExtensions.PushBack(VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME);
						vulkanFeatureData.mFragmentShadingRateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
						*features_chain = &vulkanFeatureData.mFragmentShadingRateFeatures;
						features_chain = &vulkanFeatureData.mFragmentShadingRateFeatures.pNext;
						vulkanPropertyData.mFragmentShadingRateProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR;
						*properties_chain = &vulkanPropertyData.mFragmentShadingRateProperties;
						properties_chain = &vulkanPropertyData.mFragmentShadingRateProperties.pNext;
					}

					if (CheckExtensionSupprt(VK_NV_MESH_SHADER_EXTENSION_NAME, available_deviceExtensions))
					{
						enabled_deviceExtensions.PushBack(VK_NV_MESH_SHADER_EXTENSION_NAME);
						vulkanFeatureData.mMeshShaderFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV;
						*features_chain = &vulkanFeatureData.mMeshShaderFeatures;
						features_chain = &vulkanFeatureData.mMeshShaderFeatures.pNext;
						vulkanPropertyData.mMeshShaderProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV;
						*properties_chain = &vulkanPropertyData.mMeshShaderProperties;
						properties_chain = &vulkanPropertyData.mMeshShaderProperties.pNext;
					}

					vkGetPhysicalDeviceProperties2(devices[i], &vulkanPropertyData.mProperties2);

					bool discrete = vulkanPropertyData.mProperties2.properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
					if (discrete || vulkanRuntimeData.mPhysicalDevice == VK_NULL_HANDLE)
					{
						vulkanRuntimeData.mPhysicalDevice = devices[i];
						if (discrete)
						{
							break; // if this is discrete GPU, look no further (prioritize discrete GPU)
						}
					}
				}
			}

			if (vulkanRuntimeData.mPhysicalDevice == VK_NULL_HANDLE)
			{
				DoNotifyError("Vulkan Physical Device Error", "Failed to find a suitable GPU");
			}

			Assert(properties2.properties.limits.timestampComputeAndGraphics == VK_TRUE);

			vkGetPhysicalDeviceFeatures2(vulkanRuntimeData.mPhysicalDevice, &vulkanFeatureData.mFeatures2);

			Assert(vulkanFeatureData.mFeatures2.features.imageCubeArray == VK_TRUE);
			Assert(vulkanFeatureData.mFeatures2.features.independentBlend == VK_TRUE);
			Assert(vulkanFeatureData.mFeatures2.features.geometryShader == VK_TRUE);
			Assert(vulkanFeatureData.mFeatures2.features.samplerAnisotropy == VK_TRUE);
			Assert(vulkanFeatureData.mFeatures2.features.shaderClipDistance == VK_TRUE);
			Assert(vulkanFeatureData.mFeatures2.features.textureCompressionBC == VK_TRUE);
			Assert(vulkanFeatureData.mFeatures2.features.occlusionQueryPrecise == VK_TRUE);
			if (vulkanFeatureData.mFeatures2.features.tessellationShader == VK_TRUE)
			{
				mDriverSupport.mTesselation = true;
			}
			if (vulkanFeatureData.mFeatures2.features.shaderStorageImageExtendedFormats == VK_TRUE)
			{
				mDriverSupport.mUAVLoadFormatCommon = true;
			}

			if (vulkanFeatureData.mRaytracingFeatures.rayTracingPipeline == VK_TRUE)
			{
				Assert(vulkanFeatureData.mAccelerationStructureFeatures.accelerationStructure == VK_TRUE);
				Assert(vulkanFeatureData.mFeatures1_2.bufferDeviceAddress == VK_TRUE);
				mDriverSupport.mRaytracing = true;
				vulkanRuntimeData.mShaderIdentifierSize = vulkanPropertyData.mRaytracingProperties.shaderGroupHandleSize;
			}
			if (vulkanFeatureData.mRaytracingQueryFeatures.rayQuery == VK_TRUE)
			{
				Assert(vulkanFeatureData.mAccelerationStructureFeatures.accelerationStructure == VK_TRUE);
				Assert(vulkanFeatureData.mFeatures1_2.bufferDeviceAddress == VK_TRUE);
				mDriverSupport.mRaytracingInline = true;
			}
			if (vulkanFeatureData.mMeshShaderFeatures.meshShader == VK_TRUE && vulkanFeatureData.mMeshShaderFeatures.taskShader == VK_TRUE)
			{
				mDriverSupport.mMeshShaders = true;
			}
			if (vulkanFeatureData.mFragmentShadingRateFeatures.pipelineFragmentShadingRate == VK_TRUE)
			{
				mDriverSupport.mVariableRateShading = true;

			}
			if (vulkanFeatureData.mFragmentShadingRateFeatures.attachmentFragmentShadingRate == VK_TRUE)
			{
				mDriverSupport.mVariableRateShadingTier2 = true;
				vulkanRuntimeData.mVariableRateShadingTileSize = std::min(vulkanPropertyData.mFragmentShadingRateProperties.maxFragmentShadingRateAttachmentTexelSize.width, vulkanPropertyData.mFragmentShadingRateProperties.maxFragmentShadingRateAttachmentTexelSize.height);
			}

			assert(vulkanFeatureData.mFeatures1_2.hostQueryReset == VK_TRUE);

			if (vulkanFeatureData.mFeatures1_2.descriptorIndexing)
			{
				mDriverSupport.mBindlessDescriptors = true;
			}
			
			VkFormatProperties formatProperties = {};
			vkGetPhysicalDeviceFormatProperties(vulkanRuntimeData.mPhysicalDevice, VK_FORMAT_B10G11R11_UFLOAT_PACK32, &formatProperties);
			if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)
			{
				mDriverSupport.mUAVFormatR11G11B10F = true;
			}

			// Find queue families:
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(vulkanRuntimeData.mPhysicalDevice, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(vulkanRuntimeData.mPhysicalDevice, &queueFamilyCount, queueFamilies.data());

			// Query base queue families:
			int familyIndex = 0;
			for (const auto& queueFamily : queueFamilies)
			{
				VkBool32 presentSupport = false;
				VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(vulkanRuntimeData.mPhysicalDevice, (uint32_t)familyIndex, vulkanRuntimeData.mSurface, &presentSupport);
				Assert(res == VK_SUCCESS);

				if (vulkanRuntimeData.mPresentFamily < 0 && queueFamily.queueCount > 0 && presentSupport) {
					vulkanRuntimeData.mPresentFamily = familyIndex;
				}

				if (vulkanRuntimeData.mGraphicsFamily < 0 && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					vulkanRuntimeData.mGraphicsFamily = familyIndex;
				}

				if (vulkanRuntimeData.mCopyFamily < 0 && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
					vulkanRuntimeData.mCopyFamily = familyIndex;
				}

				if (vulkanRuntimeData.mComputeFamily < 0 && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
					vulkanRuntimeData.mComputeFamily = familyIndex;
				}

				familyIndex++;
			}

			// Now try to find dedicated compute and transfer queues:
			familyIndex = 0;
			for (const auto& queueFamily : queueFamilies)
			{
				if (queueFamily.queueCount > 0 &&
					queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT &&
					!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
					!(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
					) {
					vulkanRuntimeData.mCopyFamily = familyIndex;
				}

				if (queueFamily.queueCount > 0 &&
					queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT &&
					!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					) {
					vulkanRuntimeData.mComputeFamily = familyIndex;
				}

				familyIndex++;
			}

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<int> uniqueQueueFamilies = { vulkanRuntimeData.mGraphicsFamily, vulkanRuntimeData.mPresentFamily, vulkanRuntimeData.mCopyFamily, vulkanRuntimeData.mComputeFamily };

			float queuePriority = 1.0f;
			for (int queueFamily : uniqueQueueFamilies)
			{
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkDeviceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = nullptr;
			createInfo.pNext = &vulkanFeatureData.mFeatures2;

			createInfo.enabledExtensionCount = static_cast<uint32_t>(enabled_deviceExtensions.Size());
			createInfo.ppEnabledExtensionNames = enabled_deviceExtensions.Data();

			if (gEnableValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(GetValidationLayers().Size());
				createInfo.ppEnabledLayerNames = GetValidationLayers().Data();
			}
			else
			{
				createInfo.enabledLayerCount = 0;
			}

			res = vkCreateDevice(vulkanRuntimeData.mPhysicalDevice, &createInfo, nullptr, &vulkanRuntimeData.mDevice);
			Assert(res == VK_SUCCESS);

			volkLoadDevice(vulkanRuntimeData.mDevice);
			
			vkGetDeviceQueue(vulkanRuntimeData.mDevice, vulkanRuntimeData.mGraphicsFamily, 0, &vulkanRuntimeData.mGraphicsQueue);
			vkGetDeviceQueue(vulkanRuntimeData.mDevice, vulkanRuntimeData.mPresentFamily, 0, &vulkanRuntimeData.mPresentQueue);
			vkGetDeviceQueue(vulkanRuntimeData.mDevice, vulkanRuntimeData.mCopyFamily, 0, &vulkanRuntimeData.mCopyQueue);
			vkGetDeviceQueue(vulkanRuntimeData.mDevice, vulkanRuntimeData.mComputeFamily, 0, &vulkanRuntimeData.mComputeQueue);
		}
    }
}
