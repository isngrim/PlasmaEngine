#pragma once

#include "Common/CommonStandard.hpp"
#include "Meta/MetaStandard.hpp"

#if defined(PlasmaPlatformWindows)
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "VulkanHeaders/vulkan.h"
#include "VulkanHeaders/vulkan_core.h"
#include "VulkanHeaders/vk_sdk_platform.h"

#include "VulkanValidationLayer.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanInitialization.hpp"
#include "VulkanRenderer.hpp"