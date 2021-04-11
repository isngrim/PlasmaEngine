#pragma once

namespace Plasma
{
	inline void InitalizeVulkan(VulkanRuntimeData& runtimeData)
	{
		CreateInstance(runtimeData);
		PlasmaPrintFilter(Filter::RendererFilter, "Vulkan: Instance Created");
		SetupDebugMessenger(runtimeData);
		PlasmaPrintFilter(Filter::RendererFilter, "Vulkan: Debug Manager Created");
		CreateSurface(runtimeData);
	    PlasmaPrintFilter(Filter::RendererFilter, "Vulkan: Surface Created");
		// Physical Device
		// Logical Device
		// Sync
	}
}
