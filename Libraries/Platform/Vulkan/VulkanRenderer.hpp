#pragma once

namespace Plasma
{
    class VulkanRendeer : public Renderer
    {
    public:
        VulkanRendeer(RendererInitializeData initialiseData, String& error);
        ~VulkanRendeer() override;
        
        void BuildOrthographicTransform(Mat4Ref matrix, float size, float aspect, float nearPlane, float farPlane) override;
        void BuildPerspectiveTransform(Mat4Ref matrix, float fov, float aspect, float nearPlane, float farPlane) override;

        bool YInvertImageData(TextureType::Enum type) override;

        MaterialRenderData* CreateMaterialRenderData() override;
        MeshRenderData* CreateMeshRenderData() override;
        TextureRenderData* CreateTextureRenderData() override;

        void AddMaterial(AddMaterialInfo* info) override;
        void AddMesh(AddMeshInfo* info) override;
        void AddTexture(AddTextureInfo* info) override;

        void RemoveMaterial(MaterialRenderData* data) override;
        void RemoveMesh(MeshRenderData* data) override;
        void RemoveTexture(TextureRenderData* data) override;

        bool GetLazyShaderCompilation() override;
        void SetLazyShaderCompilation(bool isLazy) override;

        void AddShaders(Array<ShaderEntry>& entries, uint forceCompileBatchCount) override;
        void RemoveShaders(Array<ShaderEntry>& entries) override;

        void SetVSync(bool vsync) override;

        void GetTextureData(GetTextureDataInfo* info) override;

        void ShowProgress(ShowProgressInfo* info) override;

        void DoRenderTasks(RenderTasks* renderTasks, RenderQueues* renderQueues) override;

    private:
        VulkanRuntimeData mRuntimeData;
        VulkanFeatureData mFeatureData;
        VulkanPropertyData mPropertyData;
        uint mWidth;
        uint mHeight;
    };
}
