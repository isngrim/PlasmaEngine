// MIT Licensed (see LICENSE.md).
#pragma once

namespace Plasma
{
    class VKMaterialRenderData : MaterialRenderData
    {
        
    };

    class VKMeshRenderData : MeshRenderData
    {
        
    };

    class VKTextureRenderData : TextureRenderData
    {
        
    };

    class VulkanRenderer : public Renderer
    {
    public:
      VulkanRenderer(RendererInitData initData, String& error);
      ~VulkanRenderer();

      // This must be called by the derived class after the OpenGL context has been
      // created.
      void Initialize(RendererInitData initData, String& error);

      // This must be called by the derived class before the OpenGL context has been
      // destroyed.
      void Shutdown();

        void BuildOrthographicTransform(Mat4Ref matrix, float size, float aspect, float nearPlane,
            float farPlane) override;
        void BuildPerspectiveTransform(Mat4Ref matrix, float fov, float aspect, float nearPlane,
            float farPlane) override;
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
        void DoRenderTasks(RenderTasks* renderTasks, RenderQueues* renderQueues) override;
    };
}
