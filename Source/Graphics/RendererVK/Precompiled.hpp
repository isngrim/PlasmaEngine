#pragma once

#include "CommonStandard.hpp"
#include "SupportStandard.hpp"

#if defined(PlasmaPlatformWindows)
#define VK_USE_PLATFORM_WIN32_KHR
#else
#error TODO: None windows platforms
#endif

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

#if defined(PlasmaPlatformWindows)
#include <vulkan/vulkan_win32.h>
#endif

#include "volk.h"

#include "RendererVKStandard.hpp"

#include "VulkanStructures.hpp"
#include "VulkanExtensions.hpp"
#include "VulkanValidation.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanInitialization.hpp"

#include "VulkanRenderer.hpp"