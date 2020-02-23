#ifndef _CHERRYSODA_GRAPHICS_TEXTURE_H_
#define _CHERRYSODA_GRAPHICS_TEXTURE_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Texture
{
public:
	static Texture FromFile(const String& filename);

protected:
	friend class Graphics;

	Graphics::TextureHandle m_texture = Graphics::InvalidHandle;
};

class Texture2D : public Texture
{
public:
	static Texture2D FromFile(const String& filename);

	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }
	inline const Math::IVec2 Size() const { return Math::IVec2(Width(), Height()); }

	virtual const Math::IRectangle* ClipRect() { return nullptr; }

private:
	int m_width  = 0;
	int m_height = 0;
};

class Subtexture2D : public Texture2D
{
public:
	const Math::IRectangle* ClipRect() override { return &m_clipRect; }

private:
	Math::IRectangle m_clipRect;
};

class TextureCube : public Texture
{
public:
	static TextureCube FromFile(const String& filename);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_TEXTURE_H_
