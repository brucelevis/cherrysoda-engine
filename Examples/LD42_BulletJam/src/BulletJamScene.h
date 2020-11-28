#ifndef _BULLETJAMSCENE_H_
#define _BULLETJAMSCENE_H_

#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {
class Alarm;
class Entity;
class EverythingRenderer;
class Image;
class SingleTagRenderer;
class Tween;
} // namespace cherrysoda

namespace ld42_bulletjam {

class BulletJamScene : public cherrysoda::Scene
{
public:
	typedef cherrysoda::Scene base;

	void Begin() override;
	void Update() override;

	void GameStart();
	void GameRestart();
	void GameOver();
	void Win();

	void GenerateEnemy(int wave);
	void AddBoss();
	void AddEnemy(const cherrysoda::STL::Vector<int>& enemyType);
	void AddEnemy(int enemyType);
	void AddEnemyAt(int type, const cherrysoda::Math::Vec2& position);

	static bool CheckOutsideOfPlayZone(cherrysoda::Math::Vec2& position, bool normalize = false, float margin = 0.f);
	cherrysoda::Math::Vec2 GetValidSpawnPosition();

	void OnEnemyDead();

	void SetPlayerHP(int hp);
	void SetPlayerBomb(int bombCount);

private:
	cherrysoda::Alarm* m_enemyTimer = nullptr;
	cherrysoda::EverythingRenderer* m_renderer = nullptr;
	cherrysoda::Image* m_playerHPImage = nullptr;
	cherrysoda::Image* m_playerBombImage = nullptr;
	cherrysoda::Tween* m_openUITween = nullptr;
	int m_aliveEnemyCounter = 0;
	bool m_inGameProgress = false;
	bool m_gameHasEnded = false;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAMSCENE_H_
