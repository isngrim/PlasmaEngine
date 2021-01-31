#pragma once

namespace Plasma
{
struct VulkanRuntimeData
    {
        VkInstance mInstance = VK_NULL_HANDLE;
        VkSurfaceKHR mSurface = VK_NULL_HANDLE;
        VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
        VkDevice mDevice = VK_NULL_HANDLE;

        int mGraphicsFamily = -1;
        int mPresentFamily = -1;
        int mCopyFamily = -1;
        int mComputeFamily = -1;

        VkQueue mGraphicsQueue = VK_NULL_HANDLE;
        VkQueue mPresentQueue = VK_NULL_HANDLE;
        VkQueue mComputeQueue = VK_NULL_HANDLE;
        VkQueue mCopyQueue = VK_NULL_HANDLE;
        
        size_t mShaderIdentifierSize = 0;
        uint32_t mVariableRateShadingTileSize = 0;

        VkSurfaceCapabilitiesKHR mSwapchainCapabilities;
        Array<VkSurfaceFormatKHR> mSwapchainFormats;
        Array<VkPresentModeKHR> mSwapchainPresentModes;
    };

    struct VulkanFeatureData
    {
        VkPhysicalDeviceFeatures2 mFeatures2 = {};
        VkPhysicalDeviceVulkan11Features mFeatures1_1 = {};
        VkPhysicalDeviceVulkan12Features mFeatures1_2 = {};
        VkPhysicalDeviceAccelerationStructureFeaturesKHR mAccelerationStructureFeatures = {};
        VkPhysicalDeviceRayTracingPipelineFeaturesKHR mRaytracingFeatures = {};
        VkPhysicalDeviceRayQueryFeaturesKHR mRaytracingQueryFeatures = {};
        VkPhysicalDeviceFragmentShadingRateFeaturesKHR mFragmentShadingRateFeatures = {};
        VkPhysicalDeviceMeshShaderFeaturesNV mMeshShaderFeatures = {};
    };

    struct VulkanPropertyData
    {
        VkPhysicalDeviceProperties2 mProperties2 = {};
        VkPhysicalDeviceVulkan11Properties mProperties1_1 = {};
        VkPhysicalDeviceVulkan12Properties mProperties1_2 = {};
        VkPhysicalDeviceAccelerationStructurePropertiesKHR mAccelerationStructureProperties = {};
        VkPhysicalDeviceRayTracingPipelinePropertiesKHR mRaytracingProperties = {};
        VkPhysicalDeviceFragmentShadingRatePropertiesKHR mFragmentShadingRateProperties = {};
        VkPhysicalDeviceMeshShaderPropertiesNV mMeshShaderProperties = {};
    };
}
