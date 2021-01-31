#pragma once

namespace Plasma
{

    inline bool CheckExtensionSupprt(const char* checkExtension, Array<VkExtensionProperties> availableLayers)
    {
        for(int i = 0; i < availableLayers.Size(); i++)
        {
            if (strcmp(availableLayers[i].extensionName, checkExtension) == 0)
            {
                return true;
            }
        }
        return false;
    }

    inline Array<const char*> GetValidationLayers()
    {
        Array<const char*> layers;
        layers.PushBack("VK_LAYER_KHRONOS_validation");        
        return layers;
    };
    
    inline bool CheckValidationLayerSupport()
    {
        u32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        Array<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.Data());

        Array<const char*> layers = GetValidationLayers();
        for(int i = 0; i < layers.Size(); i++)
        {
            bool layerFound = false;

            for(int i = 0; i < availableLayers.Size(); i++)
            {
                if(strcmp(layers[i], availableLayers[i].layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound)
                return false;
        }

        return true;
    }

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                      void* pUserData)
    {
        if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            Error(pCallbackData->pMessage);
        }
        else
        {
            Warn(pCallbackData->pMessage);
        }

        return VK_FALSE;
    }
    
    inline void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
    }
}
