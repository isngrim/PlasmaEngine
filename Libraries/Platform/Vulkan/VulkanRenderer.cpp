#include "Precompiled.hpp"

namespace Plasma
{
    VulkanRendeer::VulkanRendeer(RendererInitializeData initializeData, String& error)
    {
        
    }

    VulkanRendeer::~VulkanRendeer()
    {
        
    }

    void VulkanRendeer::BuildOrthographicTransform(Mat4Ref matrix, float size, float aspect, float nearPlane,
        float farPlane)
    {
    }

    void VulkanRendeer::BuildPerspectiveTransform(Mat4Ref matrix, float fov, float aspect, float nearPlane,
        float farPlane)
    {
    }

    bool VulkanRendeer::YInvertImageData(TextureType::Enum type)
    {
        return false;
    }

    MaterialRenderData* VulkanRendeer::CreateMaterialRenderData()
    {
        return new MaterialRenderData();
    }

    MeshRenderData* VulkanRendeer::CreateMeshRenderData()
    {
        return new MeshRenderData();
    }

    TextureRenderData* VulkanRendeer::CreateTextureRenderData()
    {
        return new TextureRenderData();
    }

    void VulkanRendeer::AddMaterial(AddMaterialInfo* info)
    {
    }

    void VulkanRendeer::AddMesh(AddMeshInfo* info)
    {
    }

    void VulkanRendeer::AddTexture(AddTextureInfo* info)
    {
    }

    void VulkanRendeer::RemoveMaterial(MaterialRenderData* data)
    {
    }

    void VulkanRendeer::RemoveMesh(MeshRenderData* data)
    {
    }

    void VulkanRendeer::RemoveTexture(TextureRenderData* data)
    {
    }

    bool VulkanRendeer::GetLazyShaderCompilation()
    {
    }

    void VulkanRendeer::SetLazyShaderCompilation(bool isLazy)
    {
    }

    void VulkanRendeer::AddShaders(Array<ShaderEntry>& entries, uint forceCompileBatchCount)
    {
    }

    void VulkanRendeer::RemoveShaders(Array<ShaderEntry>& entries)
    {
    }

    void VulkanRendeer::SetVSync(bool vsync)
    {
    }

    void VulkanRendeer::GetTextureData(GetTextureDataInfo* info)
    {
    }

    void VulkanRendeer::ShowProgress(ShowProgressInfo* info)
    {
    }

    void VulkanRendeer::DoRenderTasks(RenderTasks* renderTasks, RenderQueues* renderQueues)
    {
    }

    Renderer* CreateRenderer(RendererInitializeData initializeData, String& error)
    {
        return new VulkanRendeer(initializeData, error);
    }
   
}