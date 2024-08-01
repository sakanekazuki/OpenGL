/**
* @file Player.cpp
*/

#include "Player.h"
#include "../GLContext.h"
#include "../Actor/Item/Bullet.h"
#include "../GameEngine.h"
#include "../Intersect.h"
#include "../GameCursor.h"
#include "../Effect/Explosion.h"
#include "../Audio/Audio.h"
#include "../Audio/SE.h"
#include <iostream>

/**
* コンストラクタ
*/
Player::Player(glm::vec3 position,
	glm::vec3 scale,
	float rotation,
	TankType type)
	:Tank{ position,
	scale,
	rotation,
	type }
	,oldShotButton(0)
{
	// タグの設定
	tag = "player";
	collider = Box::Create(
		glm::vec3(-1.8f, 0, -1.8f),
		glm::vec3(1.8f, 2.8f, 1.8f));
	interval = maxInterval;
}

/**
* 戦車の操作
*/
void Player::OnUpdate(float deltaTime)
{
	// ゲームエンジンの取得
	GameEngine& engine = GameEngine::Get();

	// 動けない状態であれば、何もしない
	if (!canMoving)
	{
		return;
	}	

	// AかDが押されたら旋回
	if (engine.GetKey(GLFW_KEY_A))
	{
		rotation += glm::radians(90.0f) * deltaTime;
		tankTop.rotation += glm::radians(90.0f) * deltaTime;
	}
	else if (engine.GetKey(GLFW_KEY_D))
	{
		rotation -= glm::radians(90.0f) * deltaTime;
		tankTop.rotation -= glm::radians(90.0f) * deltaTime;
	}

	// 戦車の向きベクトル
	glm::vec3 tankFront(0, 0, 1);
	// 回転行列
	const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
	tankFront = matRot * glm::vec4(tankFront, 1);

	// 戦車の加速度
	float tankAccel = 0.2f;

	// 移動
	if (engine.GetKey(GLFW_KEY_W))
	{
		if (!(glm::length(velocity) >= maxSpeed))
		{
			// 前進
			velocity += tankFront * tankAccel;
		}
	}
	else if (engine.GetKey(GLFW_KEY_S))
	{
		if (!(glm::abs(glm::length(velocity)) >= maxSpeed))
		{
			// 後退
			velocity -= tankFront * tankAccel;
		}
	}
	// 動いている状態だと音を出す
	if (std::abs(glm::length(velocity)) >= 1 && Audio::Get().IsStop(2))
	{
		Audio::Get().Play(2, CRI_SE_TANK_MOVE);
	}
	else if(std::abs(glm::length(velocity)) <= 1)
	{
		Audio::Get().Stop(2);
	}

	// 砲の回転

	// カーソル取得
	GameCursor& cursor = GameCursor::Get();

	tankTop.rotation -= cursor.GetCursorMovePos().x / 2000;

	// 砲の向きベクトル
	glm::vec3 cannonFront(0, 0, 1);
	// 回転行列
	const glm::mat4 matCannonRot = glm::rotate(glm::mat4(1), tankTop.rotation, glm::vec3(0, 1, 0));
	cannonFront = matCannonRot * glm::vec4(cannonFront, 1);

	// マウス左ボタンの状態を取得する
	int shotButton = engine.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);

	// マウス左ボタンが押された瞬間に弾アクターを追加する
	if (shotButton != 0 && oldShotButton == 0 && interval <= 0)
	{
		// 発射位置を砲の先端に設定
		glm::vec3 position = this->position + cannonFront * 7.5f;
		// 自分の少し前に出す
		position.y += 2.0f;
		// 弾の作成
		std::shared_ptr<Actor> bullet(new Bullet(position, attack));
		// 弾の移動を加える
		bullet->velocity = cannonFront * 30.0f;
		// 弾の追加
		GameEngine::Get().AddActors(bullet);

		const glm::vec3 pos = this->position + glm::vec3(0, 2.2f, 0) + cannonFront * 7.0f;
		std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>(new Explosion(pos, 0.05f));
		sprite->scale *= 0.2f;
		sprite->lifespan = 0.1f;
		sprite->gravityScale = 0;
		sprite->pixelsPerMeter = 10;
		engine.AddActors(sprite);

		interval = maxInterval;

		Audio::Get().Play(0, CRI_SE_SHOT2);
	}
	// インターバルを減らす
	interval -= deltaTime;

	//「前回のショットボタンの状態」を更新する
	oldShotButton = shotButton;

	// 砲の移動
	tankTop.position = position;

	CameraMove();
}

/**
* カメラの移動
*/
void Player::CameraMove()
{
	// ゲームエンジン取得
	GameEngine& engine = GameEngine::Get();
	// カメラ取得
	Camera& camera = engine.GetCamera();
	// カーソル取得
	GameCursor& cursor = GameCursor::Get();
	// 自分位置の少し上にカメラの位置を固定
	camera.position = this->position + cameraPosition;
	
	// 戦車確認用
	//camera.position = this->position + glm::vec3(0, 2, -10);

	// ALTが押されていたらカメラを回転
	if (engine.GetKey(GLFW_KEY_LEFT_ALT))
	{
		// カーソルの移動した分だけ回転
		camera.Angle_y -= static_cast<float>(cursor.GetCursorMovePos().x) / 2000;
		// カーソルの位置を固定
		engine.SetCursorPos(glm::vec2(0, 0));
	}
	else
	{
		// 押されていなければ固定
		camera.Angle_y = tankTop.rotation;
	}
	camera.rotation_y = glm::rotate(glm::mat4(1), camera.Angle_y, glm::vec3(0, 1, 0));
	camera.Angle_x += static_cast<float>(cursor.GetCursorMovePos().y) / 2000;

	camera.rotation_x = glm::rotate(glm::mat4(1), camera.Angle_x, glm::vec3(1, 0, 0));
}

/**
* 当たり判定
* 
* @param contact 衝突情報
*/
void Player::OnCollision(const Contact& contact)
{
	// 弾に当たったらダメージを受ける
	if (contact.a->tag == "bullet"|| contact.b->tag == "bullet")
	{
		// 弾に変換
		Bullet bullet = static_cast<Bullet&>(*contact.a);
		// 弾の威力のダメージを受ける
		AcceptDamage(bullet.GetPower());

		// HPが0であれば、動かないようにする
		if (health <= 0)
		{
			canMoving = false;
		}
	}
}