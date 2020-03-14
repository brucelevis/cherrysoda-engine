#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Literals.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#if defined(CHERRYSODA_PLATFORM_EMSCRIPTEN)
#	define CHERRYSODA_SHADER_PLATFORM "nacl"
#elif defined(_WIN32)
#	define CHERRYSODA_SHADER_PLATFORM "windows"
#elif defined(__ANDROID__)
#	define CHERRYSODA_SHADER_PLATFORM "android"
#elif defined(__arm__)
#	define CHERRYSODA_SHADER_PLATFORM "nacl"
#else
#	define CHERRYSODA_SHADER_PLATFORM "linux"
#endif

namespace cherrysoda {

class Camera;
class Effect;
class MeshInterface;
class Texture;
class Texture2D;
class TextureCube;

class Graphics
{
public:
	static constexpr type::UInt32 EncodeNormalU32(const Math::Vec3& v)
	{
		type::UInt32 a = static_cast<type::UInt32>((v[0] + 1.0f) * 0.5f * 255.f + 0.5f);
		type::UInt32 b = static_cast<type::UInt32>((v[1] + 1.0f) * 0.5f * 255.f + 0.5f);
		type::UInt32 c = static_cast<type::UInt32>((v[2] + 1.0f) * 0.5f * 255.f + 0.5f);
		return a | b << 8 | c << 16;
	}

	struct PosColorVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;

		static void Init();
		static const PosColorVertex MakeVertex(const Math::Vec3& p, type::UInt32 c)
		{
			return { p[0], p[1], p[2], c };
		}
		static const PosColorVertex MakeVertex(const Math::Vec3& p, const Color& c)
		{
			return { p[0], p[1], p[2], c.U32ABGR() };
		}
	};

	struct PosColorNormalVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		type::UInt32 m_normal;

		static void Init();
		static const PosColorNormalVertex MakeVertex(const Math::Vec3& p, type::UInt32 c, const Math::Vec3& n)
		{
			return { p[0], p[1], p[2], c, EncodeNormalU32(n) };
		}
		static const PosColorNormalVertex MakeVertex(const Math::Vec3& p, const Color& c, const Math::Vec3& n)
		{
			return { p[0], p[1], p[2], c.U32ABGR(), EncodeNormalU32(n) };
		}
	};

	struct PosColorTexCoord0Vertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		float m_u, m_v;

		static void Init();
		static const PosColorTexCoord0Vertex MakeVertex(const Math::Vec3& p, type::UInt32 c, const Math::Vec2& uv)
		{
			return { p[0], p[1], p[2], c, uv[0], uv[1] };
		}
		static const PosColorTexCoord0Vertex MakeVertex(const Math::Vec3& p, const Color& c, const Math::Vec2& uv)
		{
			return { p[0], p[1], p[2], c.U32ABGR(), uv[0], uv[1] };
		}
	};

	struct TextureInfo
	{
		int m_width;
		int m_height;
		bool m_cubeMap;
	};

	using HandleType = type::UInt16;
	using BufferHandle = HandleType;
	using VertexBufferHandle = HandleType;
	using IndexBufferHandle  = HandleType;
	using DynamicVertexBufferHandle = HandleType;
	using DynamicIndexBufferHandle  = HandleType;
	using ShaderHandle = HandleType;
	using UniformHandle = HandleType;
	using TextureHandle = HandleType;
	static constexpr HandleType InvalidHandle = UINT16_MAX;

	static void Initialize();
	static void Terminate();

	void RenderFrame();

	static inline void BeginRenderPass(type::UInt16 renderPassId) { Instance()->RenderPass(renderPassId); }
	static inline void EndRenderPass(type::UInt16 renderPassId)
	{
		CHERRYSODA_ASSERT(Instance()->RenderPass() == renderPassId, \
			CHERRYSODA_FORMAT("Current RenderPass %u != %u!\n", Instance()->RenderPass(), renderPassId));
		Instance()->RenderPass(0);
	}
	static inline type::UInt16 CurrentRenderPass() { return Instance()->m_renderPassId; }
	static inline Graphics* UseRenderPass(type::UInt16 renderPassId) { ms_renderPassHelperInstance->RenderPass(renderPassId); return ms_renderPassHelperInstance; }
	static inline Graphics* UseCurrentRenderPass() { return Instance(); }

	static void UpdateView();
	void SetClearColor(const Color& color);
	void SetClearDiscard();
	static void SetRenderPassOrder(STL::Vector<type::UInt16> renderPassOrder);
	void Touch();
	static void SetVsyncEnabled(bool vsyncEnabled);
	void SetViewport(int x, int y, int w, int h);
	void SetCamera(Camera* camera);
	static void SetTransformMatrix(const Math::Mat4& transformMatrix);
	static void SetMesh(MeshInterface* mesh);
	static void SetVertexBuffer(VertexBufferHandle vertexBuffer);
	static void SetIndexBuffer(IndexBufferHandle indexBuffer);
	static void SetDynamicVertexBuffer(DynamicVertexBufferHandle vertexBuffer, size_t vertexAmount);
	static void SetDynamicIndexBuffer(DynamicIndexBufferHandle indexBuffer, size_t indexAmount);
	static void SetStateDefault();
	static void SetStateNoDepth();
	void Submit();
	void Submit(Effect* effect);
	static void Submit(type::UInt16 renderPass);
	static void Submit(type::UInt16 renderPass, Effect* effect);
	static void SubmitOnCurrentRenderPass();
	static void SubmitOnCurrentRenderPass(Effect* effect);

	static void ScreenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = false, float _width = 1.0f, float _height = 1.0f);

	static IndexBufferHandle CreateIndexBuffer(STL::Vector<type::UInt16>& indices);
	static DynamicIndexBufferHandle CreateDynamicIndexBuffer(STL::Vector<type::UInt16>& indices);

	static ShaderHandle CreateShaderProgram(const String& vs, const String& fs);
	static TextureHandle CreateTexture(const String& texture, Graphics::TextureInfo* info = nullptr);

	static UniformHandle CreateUniformVec4(const String& uniform, type::UInt16 num = 1U);
	static UniformHandle CreateUniformMat4(const String& uniform);
	static UniformHandle CreateUniformSampler(const String& sampler);

	static void UpdateDynamicIndexBuffer(DynamicIndexBufferHandle handle, int index, STL::Vector<cherrysoda::type::UInt16>& indices);

	static void DestroyVertexBuffer(VertexBufferHandle vertexBuffer);
	static void DestroyDynamicVertexBuffer(DynamicVertexBufferHandle vertexBuffer);
	static void DestroyIndexBuffer(IndexBufferHandle indexBuffer);
	static void DestroyDynamicIndexBuffer(DynamicIndexBufferHandle indexBuffer);
	static void DestroyShader(ShaderHandle shader);
	static void DestroyTexture(TextureHandle texture);
	static void DestroyUniform(UniformHandle uniform);

	static void SetShader(ShaderHandle shader) { ms_defaultShaderOverride = shader; }
	static void SetTexture(UniformHandle uniform, TextureHandle texture);
	static void SetTexture(type::UInt8 stage, UniformHandle uniform, TextureHandle texture);

	static void SetEffect(Effect* effect);
	static void SetTexture(Texture* texture);

	static void SetUniform(UniformHandle uniform, const void* value, type::UInt16 size = 1U);
	static void SetUniformCamPos(const Math::Vec3& camPos);
	static void SetUniformMaterial(const Math::Vec3& albedo, float metallics, float roughness, float ao);
	static void SetUniformLight(int index, const Math::Vec3& lightPos, const Math::Vec3& lightColor, bool submit = true);
	static void SubmitUniformLight();

	static void SetTextureCube(TextureCube* texture);
	static void SetTextureCubeIrr(TextureCube* texture);

private:
	friend class Engine;

	Graphics() {}

	inline void RenderPass(type::UInt16 renderPassId) { m_renderPassId = renderPassId; }
	inline type::UInt16 RenderPass() { return m_renderPassId; }	

	static inline ShaderHandle CurrentShader() { return ms_defaultShaderOverride != Graphics::InvalidHandle ? ms_defaultShaderOverride : ms_defaultShader; }

	static inline Graphics* Instance() { return ms_instance; }

	type::UInt16 m_renderPassId = 0;	

	static bool ms_vsyncEnabled;

	static ShaderHandle ms_defaultShader;
	static ShaderHandle ms_defaultShaderOverride;
	
	static UniformHandle ms_samplerTex;
	static UniformHandle ms_samplerTexCube;
	static UniformHandle ms_samplerTexCubeIrr;

	static UniformHandle ms_uniformCamPos;
	static UniformHandle ms_uniformLights;
	static UniformHandle ms_uniformMaterial;
	static UniformHandle ms_uniformParams;

	static Graphics* ms_instance;
	static Graphics* ms_renderPassHelperInstance;

public:
	#define CHERRYSODA_VERTEX_DECLARATION(VERTEX_T) \
	static VertexBufferHandle CreateVertexBuffer(STL::Vector<VERTEX_T>& vertices); \
	static DynamicVertexBufferHandle CreateDynamicVertexBuffer(STL::Vector<VERTEX_T>& vertices); \
	static void UpdateDynamicVertexBuffer(DynamicVertexBufferHandle handle, int index, STL::Vector<VERTEX_T>& vertices);

	CHERRYSODA_VERTEX_DECLARATION(PosColorVertex);
	CHERRYSODA_VERTEX_DECLARATION(PosColorNormalVertex);
	CHERRYSODA_VERTEX_DECLARATION(PosColorTexCoord0Vertex);

	#undef CHERRYSODA_VERTEX_DECLARATION
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_
