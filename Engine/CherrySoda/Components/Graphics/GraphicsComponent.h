#ifndef _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class GraphicsComponent : public Component
{
public:
	CHERRYSODA_MATH_VEC3_GETTER_SETTER(Position, m_position);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER(Origin, m_origin);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER(Scale, m_scale);

	inline void ZRotation(float zRotation) { m_zRotation = zRotation; };
	inline float ZRotation() const { return m_zRotation; }

	inline void RenderPosition(Math::Vec3 pos3d) { Position(pos3d - EntityPos()); }
	inline const Math::Vec3 RenderPosition() const { return Position() + EntityPos(); }

	const Math::Mat4 GetTransformMatrix() const;

private:
	inline const Math::Vec3 EntityPos() const { return GetEntity() != nullptr ? GetEntity()->Position() : Math::Vec3(0.f); }

	Color m_color = Color::White;
	Math::Vec3 m_position = Math::Vec3(0.f);
	Math::Vec3 m_origin = Math::Vec3(0.f);
	Math::Vec3 m_scale = Math::Vec3(1.f);
	float m_zRotation = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_