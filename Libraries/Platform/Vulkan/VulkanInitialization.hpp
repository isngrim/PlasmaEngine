#pragma once
#include "VulkanValidationLayer.hpp"

#if defined (PlasmaRelease)
    const bool gEnableValidationLayers = false;
#else
    const bool gEnableValidationLayers = true;
#endif

namespace Plasma
{
    inline Status CreateInstance(RendererInitializeData initializeData, VulkanRuntimeData& vulkanRuntimeData)
    {
        if(gEnableValidationLayers && !CheckValidationLayerSupport())
        {
            return Status(StatusState::Failure, "Debug Layers requested but not supported");
        }

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
            return Status(StatusState::Failure, "Unable to create instance");
        }
        
        return Status(StatusState::Success, "Instance Created");
    }

    inline Status CreateSurface(RendererInitializeData initializeData,VulkanRuntimeData& vulkanRuntimeData)
    {
#if defined(PlasmaPlatformWindows)
        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = (HWND)initializeData.mWindow;
        createInfo.hinstance = GetModuleHandle(nullptr);

        auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(vulkanRuntimeData.mInstance, "vkCreateWin32SurfaceKHR");

        if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(vulkanRuntimeData.mInstance, &createInfo, nullptr, &vulkanRuntimeData.mSurface) != VK_SUCCESS)
            return Status(StatusState::Failure, "Failed to create Surface");

        return Status(StatusState::Success, "Surface Created");
#else        
#error WICKEDENGINE VULKAN DEVICE ERROR: PLATFORM NOT SUPPORTED
#endif
    }
    
    inline Status InitializeVulkan(RendererInitializeData initializeData, VulkanRuntimeData& vulkanRuntimeData)
    {
        Status initStatus;
        initStatus = CreateInstance(initializeData, vulkanRuntimeData);
        if(initStatus.Failed()) return initStatus;
        initStatus = CreateSurface(initializeData, vulkanRuntimeData);
        if(initStatus.Failed()) return initStatus;

        
    }
}
