#include "Player.h"

#include "Program.h"
#include "Bullet.h"
#include "BulletJamScene.h"
#include "Cursor.h"
#include "Bomb.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using namespace ld42_bulletjam;

Player* Player::Instance()
{
	if (ms_instance == nullptr) {
		ms_instance = Create();
	}
	return ms_instance;
}

Player* Player::Create()
{
	Player* player = new Player();
	player->m_bulletAlarm = Alarm::Create(AlarmMode::Persist, [player]()
	{
		player->m_canShoot = true;
	}, 0.1f);
	player->Add(player->m_bulletAlarm);
	player->m_playerFootSprite = new Sprite(GameApp::GetAtlas());
	player->m_playerFootSprite->AddLoop("stand", "playerfoot/playerfoot1");
	player->m_playerFootSprite->AddLoop("walk", "playerfoot/playerfoot", 1, 1);
	player->m_playerFootSprite->Play("stand");
	player->m_playerFootSprite->CenterOrigin();
	player->m_playerFootSprite->SetSpriteEffects(SpriteEffects::RoundRenderingPosition);
	player->m_playerSprite = new Sprite(GameApp::GetAtlas());
	player->m_playerSprite->AddLoop("right",     "player/player1");
	player->m_playerSprite->AddLoop("rightdown", "player/player2");
	player->m_playerSprite->AddLoop("down",      "player/player3");
	player->m_playerSprite->AddLoop("leftdown",  "player/player4");
	player->m_playerSprite->AddLoop("left",      "player/player5");
	player->m_playerSprite->AddLoop("leftup",    "player/player6");
	player->m_playerSprite->AddLoop("up",        "player/player7");
	player->m_playerSprite->AddLoop("rightup",   "player/player8");
	player->m_playerSprite->Play("right");
	player->m_playerSprite->CenterOrigin();
	player->m_playerSprite->SetSpriteEffects(SpriteEffects::RoundRenderingPosition);
	player->Add(player->m_playerFootSprite);
	player->Add(player->m_playerSprite);
	player->SetCollider(new Circle(8.f));
	player->Position(Math::Vec2(300.f, 200.f));
	// player->m_hitShaker = new Shaker(false, player.OnShake);
	// player->Add(player->m_hitShaker);
	player->m_HP = 6;
	return player;
}

void Player::Update()
{
	base::Update();

	Math::Vec2 vector = Calc::EightWayNormal(Math::Vec2(Math_Normalize(Cursor::Instance()->Position() - Position())));
	if (vector == Vec2_YUp)
	{
		m_playerSprite->Play("up");
	}
	else if (vector == -Vec2_YUp)
	{
		m_playerSprite->Play("down");
	}
	else if (vector == -Vec2_XUp)
	{
		m_playerSprite->Play("left");
	}
	else if (vector == Vec2_XUp)
	{
		m_playerSprite->Play("right");
	}
	else if (vector.x == -vector.y && vector.x < 0.f)
	{
		m_playerSprite->Play("leftup");
	}
	else if (vector.x == -vector.y && vector.x > 0.f)
	{
		m_playerSprite->Play("rightdown");
	}
	else if (vector.x == vector.y && vector.x < 0.f)
	{
		m_playerSprite->Play("leftdown");
	}
	else if (vector.x == vector.y && vector.x > 0.f)
	{
		m_playerSprite->Play("rightup");
	}

	Math::Vec2 move(0.f);
	if (MInput::Keyboard()->Check(Keys::W))
	{
		move.y += 1.f;
	}
	if (MInput::Keyboard()->Check(Keys::A))
	{
		move.x -= 1.f;
	}
	if (MInput::Keyboard()->Check(Keys::S))
	{
		move.y -= 1.f;
	}
	if (MInput::Keyboard()->Check(Keys::D))
	{
		move.x += 1.f;
	}
	move = Calc::SafeNormalize(move);
	if (move == Vec2_Zero) {
		m_playerFootSprite->Play("stand");
	}
	else 
	{
		m_playerFootSprite->Play("walk");
		if (move.x > 0.f) {
			m_playerFootSprite->FlipX(false);
		}
		else if (move.x < 0.f) {
			m_playerFootSprite->FlipX(true);
		}
	}

	for (auto item : CollideAll(BitTag::Get("enemybullet")))
	{
		Hit();
		item->RemoveSelf();
	}

	Move(move);

	if (MInput::Mouse()->CheckLeftButton())
	{
		Shoot();
	}
	if (MInput::Mouse()->PressedRightButton())
	{
		PlaceBomb();
	}
}

void Player::Move(const Math::Vec2& move)
{
	const auto lastPos2D = Position2D();
	Position2D(lastPos2D + Engine::Instance()->DeltaTime() * 180.f * move);
	if (CollideCheck(BitTag::Get("deadbullet"))) {
		Position2D(lastPos2D);
	}
	auto pos2D = Position2D();
	BulletJamScene::CheckOutsideOfPlayZone(pos2D, true, 30.f);
	Position(pos2D);
	Depth(Math_Round(PositionY() - 700.0f));
}

void Player::Shoot()
{
	if (m_canShoot && !m_isDead)
	{
		GetScene()->Add(Bullet::Create(Position(), 600.f * Calc::SafeNormalize(Cursor::Instance()->Position() - Position(), Vec2_YUp)));
		m_canShoot = false;
		m_bulletAlarm->Start();
	}
}

void Player::PlaceBomb()
{
	if (m_bombCount > 0) {
		--m_bombCount;	
		GetScene()->Add(Bomb::CreateAt(Position2D() - Vec2_YUp * 10.f));
		GetSceneAs<BulletJamScene>()->SetPlayerBomb(m_bombCount);
	}
}

void Player::Hit()
{
	if (m_POW || m_isDead) {
		return;
	}
	if (m_HP > 1) {
		m_HP -= 2;
		GetSceneAs<BulletJamScene>()->SetPlayerHP(m_HP);
	}
	if (m_HP > 0) {
		m_POW = true;
		m_playerFootSprite->SetColor(Color::Red);
		m_playerFootSprite->SetColorA(64);
		m_playerSprite->SetColor(Color::Red);
		m_playerSprite->SetColorA(64);
		Add(Alarm::Create(AlarmMode::Oneshot, [this]()
		{
			m_POW = false;
			m_playerFootSprite->SetColor(Color::White);
			m_playerSprite->SetColor(Color::White);
		}, 0.5f, true));
		// HitShaker.ShakeFor(0.5f, removeOnFinish: false);
	}
	else {
		Die();
	}
}

void Player::Die()
{
	m_isDead = true;
	RemoveSelf();

	GetSceneAs<BulletJamScene>()->GameOver();
}

Player* Player::ms_instance = nullptr;
