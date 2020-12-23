// MIT Licensed (see LICENSE.md).

#include "Precompiled.hpp"
#include "bgfx/bgfx.h"
#include "BGFXRenderer.hpp"

#include <bx/bx.h>



#include "EulerOrder.hpp"
#include "bgfx/platform.h"

// temporary to prevent string constructions every frame
// RenderQueue structures should have semantics for setting shader parameters
namespace
{
	const Plasma::String cFrameTime("FrameData.FrameTime");
	const Plasma::String cLogicTime("FrameData.LogicTime");
	const Plasma::String cNearPlane("CameraData.NearPlane");
	const Plasma::String cFarPlane("CameraData.FarPlane");
	const Plasma::String cViewportSize("CameraData.ViewportSize");
	const Plasma::String cInverseViewportSize("CameraData.InverseViewportSize");
	const Plasma::String cObjectWorldPosition("CameraData.ObjectWorldPosition");

	const Plasma::String cLocalToWorld("TransformData.LocalToWorld");
	const Plasma::String cWorldToLocal("TransformData.WorldToLocal");
	const Plasma::String cWorldToView("TransformData.WorldToView");
	const Plasma::String cViewToWorld("TransformData.ViewToWorld");
	const Plasma::String cLocalToView("TransformData.LocalToView");
	const Plasma::String cLastLocalToView("TransformData.LastLocalToView");
	const Plasma::String cViewToLocal("TransformData.ViewToLocal");
	const Plasma::String cLocalToWorldNormal("TransformData.LocalToWorldNormal");
	const Plasma::String cWorldToLocalNormal("TransformData.WorldToLocalNormal");
	const Plasma::String cLocalToViewNormal("TransformData.LocalToViewNormal");
	const Plasma::String cViewToLocalNormal("TransformData.ViewToLocalNormal");
	const Plasma::String cLocalToPerspective("TransformData.LocalToPerspective");
	const Plasma::String cViewToPerspective("TransformData.ViewToPerspective");
	const Plasma::String cPerspectiveToView("TransformData.PerspectiveToView");
	const Plasma::String cPlasmaPerspectiveToApiPerspective("TransformData.PlasmaPerspectiveToApiPerspective");

	const Plasma::String cSpriteSource("SpriteSource_SpriteSourceColor");
	const Plasma::String cSpriteSourceCubePreview("SpriteSource_TextureCubePreview");
} // namespace

namespace Plasma
{
	bgfx::TextureFormat::Enum BGFXInternalFormat(TextureCompression::Enum compression)
	{
		switch (compression)
		{
		case TextureCompression::BC1:
			return bgfx::TextureFormat::BC1;
		case TextureCompression::BC2:
			return bgfx::TextureFormat::BC2;
		case TextureCompression::BC3:
			return bgfx::TextureFormat::BC3;
		case TextureCompression::BC4:
			return bgfx::TextureFormat::BC4;
		case TextureCompression::BC5:
			return bgfx::TextureFormat::BC5;
		case TextureCompression::BC6:
			return bgfx::TextureFormat::BC6H;
		case TextureCompression::None:
		default:
			return bgfx::TextureFormat::Unknown;
		}
	}

	bgfx::TextureFormat::Enum BGFXTextureFormat(TextureFormat::Enum format)
	{
		switch (format)
		{
		case TextureFormat::R8: 
			return bgfx::TextureFormat::R8;
		case TextureFormat::RG8: 
			return bgfx::TextureFormat::RG8;
		case TextureFormat::RGB8: 
			return bgfx::TextureFormat::RGB8;
		case TextureFormat::RGBA8: 
			return bgfx::TextureFormat::RGBA8;
		case TextureFormat::R16: 
			return bgfx::TextureFormat::R16;
		case TextureFormat::RG16: 
			return bgfx::TextureFormat::RG16;
		case TextureFormat::RGB16: 
			return bgfx::TextureFormat::RGBA16;
		case TextureFormat::RGBA16: 
			return bgfx::TextureFormat::RGBA16;
		case TextureFormat::R16f: 
			return bgfx::TextureFormat::R16F;
		case TextureFormat::RG16f: 
			return bgfx::TextureFormat::RG16F;
		case TextureFormat::RGB16f: 
			return bgfx::TextureFormat::RGBA16F;
		case TextureFormat::RGBA16f: 
			return bgfx::TextureFormat::RGBA16F;
		case TextureFormat::R32f: 
			return bgfx::TextureFormat::R32F;
		case TextureFormat::RG32f:
			return bgfx::TextureFormat::RG32F;
		case TextureFormat::RGB32f: 
			return bgfx::TextureFormat::RGBA32F;
		case TextureFormat::RGBA32f: 
			return bgfx::TextureFormat::RGBA32F;
		case TextureFormat::SRGB8: 
			return bgfx::TextureFormat::RGB8S;
		case TextureFormat::SRGB8A8: 
			return bgfx::TextureFormat::RGBA8S;
		case TextureFormat::Depth16: 
			return bgfx::TextureFormat::D16;
		case TextureFormat::Depth24: 
			return bgfx::TextureFormat::D24;
		case TextureFormat::Depth32: 
			return bgfx::TextureFormat::D32;
		case TextureFormat::Depth32f:
			return bgfx::TextureFormat::D32F;
		case TextureFormat::Depth24Stencil8: 
			return bgfx::TextureFormat::D24S8;
		case TextureFormat::Depth32fStencil8Pad24: 
			return bgfx::TextureFormat::D24S8;
		case TextureFormat::None: 
		default:
			return bgfx::TextureFormat::Count;
		}
	}

	bgfx::Attrib::Enum ToBGFXAttrib(VertexSemantic::Enum attrib)
	{
		switch (attrib)
		{
		case VertexSemantic::Position: 
			return bgfx::Attrib::Position;
		case VertexSemantic::Normal: 
			return bgfx::Attrib::Normal;
		case VertexSemantic::Tangent: 
			return bgfx::Attrib::Tangent;
		case VertexSemantic::Bitangent: 
			return bgfx::Attrib::Bitangent;
		case VertexSemantic::Uv: 
			return bgfx::Attrib::TexCoord0;
		case VertexSemantic::UvAux: 
			return bgfx::Attrib::TexCoord1;
		case VertexSemantic::Color: 
			return bgfx::Attrib::Color0;
		case VertexSemantic::ColorAux: 
			return bgfx::Attrib::Color1;
		case VertexSemantic::BoneWeights: 
			return bgfx::Attrib::Weight;
		case VertexSemantic::BoneIndices: 
			return bgfx::Attrib::Indices;
		case VertexSemantic::Aux0: 
			return bgfx::Attrib::TexCoord2;
		case VertexSemantic::Aux1: 
			return bgfx::Attrib::TexCoord3;
		case VertexSemantic::Aux2: 
			return bgfx::Attrib::TexCoord4;
		case VertexSemantic::Aux3: 
			return bgfx::Attrib::TexCoord5;
		case VertexSemantic::Aux4: 
			return bgfx::Attrib::TexCoord6;
		case VertexSemantic::Aux5: 
			return bgfx::Attrib::TexCoord7;
		case VertexSemantic::None: 
			return bgfx::Attrib::Count;
		default: ;
		}
	}

	bgfx::AttribType::Enum ToBGFXType(VertexElementType::Enum type)
	{
		switch (type)
		{
		case VertexElementType::Byte:
		case VertexElementType::NormByte:
			return bgfx::AttribType::Uint8;
		case VertexElementType::Short:
		case VertexElementType::NormShort:
			return bgfx::AttribType::Int16;
		case VertexElementType::Half:
			return bgfx::AttribType::Half;
		case VertexElementType::Real:
			return bgfx::AttribType::Float;
		default:
			return bgfx::AttribType::Count;
		}
	}

	bgfx::Topology::Enum BGFXPrimitiveType(PrimitiveType::Enum value)
	{
		switch (value)
		{
		case PrimitiveType::Points:
			return bgfx::Topology::PointList;
		case PrimitiveType::Lines:
			return bgfx::Topology::LineList;
		case PrimitiveType::Triangles:
			return bgfx::Topology::TriList;
		default:
			return bgfx::Topology::Count;
		}
	}

	BGFXRenderer::BGFXRenderer(OsHandle windowHandle, String& error)
	{
		mWindowHandle = static_cast<HWND>(windowHandle);
		mDeviceContext = GetDC(mWindowHandle);

		RECT rect;
		GetClientRect(mWindowHandle, &rect);
		IntVec2 size = IntVec2(rect.right - rect.left, rect.bottom - rect.top);

		bgfx::PlatformData pd;
		bx::memSet(&pd, 0, sizeof(pd));
		pd.nwh = windowHandle;
		setPlatformData(pd);

		bgfx::Resolution resolution;
		resolution.width = size.x;
		resolution.height = size.y;

		bgfx::Init init;
		init.platformData = pd;
		init.resolution = resolution;
		init.type = bgfx::RendererType::Direct3D12;

		bgfx::init(init);
		bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS | BGFX_DEBUG_PROFILER);

		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);


		bgfx::reset(size.x, size.y);

		bgfx::frame();
	}

	BGFXRenderer::~BGFXRenderer()
	{
		bgfx::shutdown();
	}

	void BGFXRenderer::BuildOrthographicTransform(
    Mat4Ref matrix, float size, float aspect, float nearPlane, float farPlane)
	{
		BuildOrthographicTransformGl(matrix, size, aspect, nearPlane, farPlane);
	}

	void BGFXRenderer::BuildPerspectiveTransform(Mat4Ref matrix, float fov, float aspect, float nearPlane,
                                                   float farPlane)
	{
		BuildPerspectiveTransformGl(matrix, fov, aspect, nearPlane, farPlane);
	}

	MaterialRenderData* BGFXRenderer::CreateMaterialRenderData()
	{
		BGFXMaterialData* renderData = new BGFXMaterialData();
		return reinterpret_cast<MaterialRenderData*>(renderData);
		
	}

	MeshRenderData* BGFXRenderer::CreateMeshRenderData()
	{
		BGFXMeshData* renderData = new BGFXMeshData();
		renderData->mVertexBufferHandle.idx = 0;
		renderData->mIndexBufferHandle.idx = 0;
		renderData->mIndexCount = 0;
		return reinterpret_cast<MeshRenderData*>(renderData);
	}

	TextureRenderData* BGFXRenderer::CreateTextureRenderData()
	{
		BGFXTextureData* renderData = new BGFXTextureData();
		renderData->mTextureHandle.idx = 0;
		renderData->mFormat = TextureFormat::None;
		return reinterpret_cast<TextureRenderData*>(renderData);
	}

	void BGFXRenderer::AddMaterial(AddMaterialInfo* info)
	{
		BGFXMaterialData* renderData = static_cast<BGFXMaterialData*>(info->mRenderData);
		renderData->mCompositeName = info->mCompositeName;
		renderData->mResourceId = info->mMaterialId;
	}

	void BGFXRenderer::AddMesh(AddMeshInfo* info)
	{
		BGFXMeshData* renderData = static_cast<BGFXMeshData*>(info->mRenderData);
		if(renderData->mVertexBufferHandle.idx != 0)
		{
			bgfx::destroy(renderData->mVertexBufferHandle);
			bgfx::destroy(renderData->mIndexBufferHandle);
		}

		if(info->mVertexData == nullptr)
		{
			renderData->mVertexBufferHandle.idx = 0;
			renderData->mIndexBufferHandle.idx = 0;
			renderData->mIndexCount = info->mIndexCount;
			renderData->mPrimitiveType = info->mPrimitiveType;
			return;
		}

		renderData->mVertexLayout = renderData->mVertexLayout.begin();

		for(int i = 0; i < info->mVertexAttributes.Size()+1; i++)
		{
			VertexSemantic::Enum symantic = (VertexSemantic::Enum)info->mVertexAttributes[i].mSemantic.mValue;
			VertexElementType::Enum type = (VertexElementType::Enum)info->mVertexAttributes[i].mType.mValue;
			byte offset = info->mVertexAttributes[i].mOffset;
			renderData->mVertexLayout = renderData->mVertexLayout.add(ToBGFXAttrib(symantic), offset,  ToBGFXType(type));
		}
		renderData->mVertexLayout.end();

		if(info->mVertexData != nullptr)
			renderData->mVertexBufferHandle = bgfx::createVertexBuffer(bgfx::makeRef(info->mVertexData, info->mVertexCount * info->mVertexSize), renderData->mVertexLayout);

		if(info->mIndexData !=  nullptr)
			renderData->mIndexBufferHandle = bgfx::createIndexBuffer(bgfx::makeRef(info->mIndexData, info->mIndexCount * info->mIndexSize), BGFX_BUFFER_NONE );

		renderData->mIndexCount = info->mIndexCount;
		renderData->mPrimitiveType = info->mPrimitiveType;

		delete[] info->mVertexData;
		delete[] info->mIndexData;

		renderData->mBones.Assign(info->mBones.All());
	}

	void BGFXRenderer::AddTexture(AddTextureInfo* info)
	{
		BGFXTextureData* renderData = static_cast<BGFXTextureData*>(info->mRenderData);

		if(info->mFormat == TextureFormat::None)
		{
			if(renderData->mTextureHandle.idx != 0)
				bgfx::destroy(renderData->mTextureHandle);
		}
		else
		{
			if(info->mType != renderData->mType && renderData->mTextureHandle.idx != 0)
				bgfx::destroy(renderData->mTextureHandle);

			if(renderData->mTextureHandle.idx == 0)
			{
				bgfx::TextureInfo texInfo;
				texInfo.width = info->mWidth;
				texInfo.height = info->mHeight;
				texInfo.format = BGFXTextureFormat(info->mFormat);

				uint64_t flags = 0;

				const char* empty = "";
				if(info->mImageData != (byte*)empty)
				{
					texInfo.numMips = info->mMipCount;
					texInfo.cubeMap = info->mType ? TextureType::TextureCube : true , false;

					switch (info->mAddressingX)
					{
					case TextureAddressing::Clamp:
						flags |= BGFX_SAMPLER_U_CLAMP;
						break;
					case TextureAddressing::Repeat:
						// default is repeat
						break;
					case TextureAddressing::Mirror:
						flags |= BGFX_SAMPLER_U_MIRROR;
						break;
					}

					switch (info->mAddressingY)
					{
					case TextureAddressing::Clamp:
						flags |= BGFX_SAMPLER_V_CLAMP;
						break;
					case TextureAddressing::Repeat:
						// default is repeat
						break;
					case TextureAddressing::Mirror:
						flags |= BGFX_SAMPLER_V_MIRROR;
						break;
					}

					if(info->mCompression != TextureCompression::None)
						texInfo.format = BGFXInternalFormat(info->mCompression);
					
					if(info->mAnisotropy != TextureAnisotropy::x1)
						flags |= BGFX_SAMPLER_MAG_ANISOTROPIC | BGFX_SAMPLER_MIN_ANISOTROPIC;

					if(flags == 0)
					{
						flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;
					}

					renderData->mTextureHandle = bgfx::createTexture(bgfx::makeRef(info->mImageData, sizeof(info->mTotalDataSize)), flags, 0, &texInfo);
				}
			}
		}
	}

	void BGFXRenderer::RemoveMaterial(MaterialRenderData* data)
	{
		mMaterialRenderDataToDestroy.PushBack(static_cast<BGFXMaterialData*>(data));
	}

	void BGFXRenderer::RemoveMesh(MeshRenderData* data)
	{
		mMeshRenderDataToDestroy.PushBack(static_cast<BGFXMeshData*>(data));
	}

	void BGFXRenderer::RemoveTexture(TextureRenderData* data)
	{
		mTextureRenderDataToDestroy.PushBack(static_cast<BGFXTextureData*>(data));
	}
	
	void BGFXRenderer::ShowProgress(ShowProgressInfo* info)
	{
		bgfx::frame();
	}

	void BGFXRenderer::DoRenderTasks(RenderTasks* renderTasks, RenderQueues* renderQueues)
	{
		bgfx::frame();
	}
} // namespace Plasma
