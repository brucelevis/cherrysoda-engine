#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_IMAGE_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_IMAGE_H_

#include <CherrySoda/Components/Graphics/GraphicsComponent.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Graphics/SpriteBatch.h>

// TODO: Use shared_ptr for texture?

namespace cherrysoda {

class Image : public GraphicsComponent
{
public:
	typedef GraphicsComponent base;

	Image(const MTexture& texture)
	: base(false)
	, m_texture(texture)
	{
	}

	Image(const MTexture& texture, bool active)
	: base(active)
	, m_texture(texture)
	{
	}

	void Render() override
	{
		m_texture.Draw(RenderPosition(), Origin(), GetColor(), Scale(), ZRotation(), Effects());
	}

	inline float Width() const { return static_cast<float>(m_texture.Width()); }
	inline float Height() const { return static_cast<float>(m_texture.Height()); }
	inline const Math::Vec2 Size() const { return Math::Vec2(Width(), Height()); } 
	inline const Math::Vec2 HalfSize() const { return Size() * 0.5f; }
	inline SpriteEffects Effects() { return m_effects; }
	inline void Effects(SpriteEffects effects) { m_effects = effects; }

	Image& SetOrigin(float x, float y)
	{
		Origin(Math::Vec2(x, y));
		return *this;
	}

	Image& CenterOrigin()
	{
		Origin(HalfSize());
		return *this;
	}

	Image& JustifyOrigin(const Math::Vec2& at)
	{
		Origin(Size() * at);
		return *this;
	}

	Image& JustifyOrigin(float x, float y)
	{
		Origin(Size() * Math::Vec2(x, y));
		return *this;
	}

private:
	MTexture m_texture;
	SpriteEffects m_effects = SpriteEffects::None;
};

}; // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_IMAGE_H_
