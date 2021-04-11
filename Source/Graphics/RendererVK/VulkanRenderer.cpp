#include "Precompiled.hpp"

#define VOLK_IMPLEMENTATION
#include "volk.h"

namespace Plasma
{
Renderer* CreateRendererOpenVK(RendererInitData initData, String& error)
{
  return new VulkanRenderer(initData, error);
}

VulkanRenderer::VulkanRenderer(RendererInitData initData, String& error)
{
  Initialize(initData, error);
}

VulkanRenderer::~VulkanRenderer()
{
  Shutdown();
}

void VulkanRenderer::Initialize(RendererInitData initData, String& error)
{
	VulkanRuntimeData runtimeData;
	runtimeData.mClientSize = initData.mClientSize;
	runtimeData.mWindowHandle = initData.mWindowHandle;
	InitalizeVulkan(runtimeData);
    //Sets Runtime Data
    // Init
    // Resource
    // SwapChain
    // possible first render
}

void VulkanRenderer::Shutdown()
{
    
}

void VulkanRenderer::BuildOrthographicTransform(Mat4Ref matrix, float size, float aspect, float nearPlane, float farPlane)
{
    
}

void VulkanRenderer::BuildPerspectiveTransform(Mat4Ref matrix, float fov, float aspect, float nearPlane, float farPlane)
{
    
}

MaterialRenderData* VulkanRenderer::CreateMaterialRenderData()
{
  return nullptr;
}

MeshRenderData* VulkanRenderer::CreateMeshRenderData()
{
  return nullptr;
}

TextureRenderData* VulkanRenderer::CreateTextureRenderData()
{
  return nullptr;
}

void VulkanRenderer::AddMaterial(AddMaterialInfo* info)
{
    
}

void VulkanRenderer::AddMesh(AddMeshInfo* info)
{
    
}

void VulkanRenderer::AddTexture(AddTextureInfo* info)
{
    
}

void VulkanRenderer::RemoveMaterial(MaterialRenderData* data)
{
    
}

void VulkanRenderer::RemoveMesh(MeshRenderData* data)
{
    
}

void VulkanRenderer::RemoveTexture(TextureRenderData* data)
{
    
}

bool VulkanRenderer::GetLazyShaderCompilation()
{
  return true;
}

void VulkanRenderer::SetLazyShaderCompilation(bool isLazy)
{
    
}

void VulkanRenderer::AddShaders(Array<ShaderEntry>& entries, uint forceCompileBatchCount)
{
    
}

void VulkanRenderer::RemoveShaders(Array<ShaderEntry>& entries)
{
    
}

void VulkanRenderer::SetVSync(bool vsync)
{
    
}

void VulkanRenderer::GetTextureData(GetTextureDataInfo* info)
{
    
}

void VulkanRenderer::DoRenderTasks(RenderTasks* renderTasks, RenderQueues* renderQueues)
{
    
}
}