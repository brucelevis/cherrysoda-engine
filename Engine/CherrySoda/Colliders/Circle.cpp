#include <CherrySoda/Colliders/Circle.h>

#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Circle;

using cherrysoda::Camera;
using cherrysoda::Color;
using cherrysoda::Math;

bool Circle::Collide(const Circle* circle) const
{
	const Math::Vec2 vec = AbsolutePosition2D() - circle->AbsolutePosition2D();
	return Math_LengthSq(vec) <= (Radius() + circle->Radius()) * (Radius() + circle->Radius());
}

bool Circle::Collide(const Math::Vec2& point) const
{
	const Math::Vec2 vec = AbsolutePosition2D() - point;
	return Math_LengthSq(vec) <= RadiusSq();
}

void Circle::Render(const Camera* camera, const Color& color) const
{
	// TODO:
}
