#ifndef _BULLETJAM_BULLET_H_
#define _BULLETJAM_BULLET_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {
class Image;
} // namespace cherrysoda

namespace ld42_bulletjam {

class Bullet : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	static Bullet* Create(cherrysoda::Math::Vec2 position, cherrysoda::Math::Vec2 speed, bool needRandom = true, bool whiteBullet = false);
	static void Destroy(Bullet* bullet);

	void Update() override;
	void Removed(cherrysoda::Scene* scene) override;
	int Damage() const { return m_damage; }

	cherrysoda::Image* BulletImage() { return m_bulletImage; }

	CHERRYSODA_GETTER_SETTER_OF_VEC2(Speed, m_speed);

private:
	Bullet() {};

	cherrysoda::Image* m_bulletImage = nullptr;
	cherrysoda::Math::Vec2 m_speed = Vec2_Zero;
	int m_damage = 40;

	static float ms_initialScale;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_BULLET_H_
