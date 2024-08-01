/**
* @file Enemy.cpp
*/

#include "Enemy.h"
#include "../Actor/Item/Bullet.h"
#include "../GameEngine.h"
#include "../Intersect.h"
#include "../GameManager.h"
#include <glm/ext.hpp>

/**
* コンストラクタ
*/
Enemy::Enemy(glm::vec3 position,
	glm::vec3 scale,
	float rotation,
	TankType type)
	:NonPlayerCharacter{position,
	scale,
	rotation,
	type}
{
	tag = "enemy";
	enemyTag = "player";
}

/**
* 敵の動き
*/
void Enemy::Move(float deltaTime)
{
	// 砲の位置の設定
	tankTop.position = position;
	// 砲の回転の設定
	tankTop.rotation = rotation;

	// 動ける状態でなければ動かさない
	if (!canMoving)
	{
		return;
	}

	// 取得する情報
	Hit hit;

	// 戦車の向きベクトル
	glm::vec3 tankFront = glm::vec3(0, 0, 1);
	// 回転させる回転行列
	const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
	// 向きベクトルの回転
	tankFront = matRot * glm::vec4(0, 0, 1, 1);

	// レイの最初の位置(地面に当たらないようにyに1足す,自分に当たらないように少し前に出す)
	glm::vec3 start = position + glm::vec3(0, 1, 0) + tankFront * 2.6f;
	// レイの最後の位置(地面に当たらないようにyに1足す,自分に当たらないように少し前に出す,50m先まで飛ばす)
	glm::vec3 end = position + tankFront * 50.0f + glm::vec3(0, 1, 0);

	// 敵を探すために回転するので最初の回転を保存する変数
	static float rota;

	// レイを飛ばす

	// アクターにレイが当たっていたら敵か調べる
	if (RayCast(start, end, hit))
	{
		// 敵にレイが当たっていたらターゲットに入れる
		if (hit.hitActor)
		{
			if (hit.hitActor->tag == enemyTag)
			{
				target = hit.hitActor.get();
				// 残り時間を設定
				interval = intervalTime;
			}
			else
			{
				// 見失うまでの時間を入れる
				interval -= deltaTime;
				// 残り時間が無くなったらターゲットを空にする
				if (interval <= 0)
				{
					target = nullptr;
					// 敵を見失ったので現在の回転を保存
					rota = rotation;
				}
			}
		}
	}
	// ターゲットがいたら追いかける
	if (target != nullptr)
	{
		// 正面方向のベクトルを計算
		glm::mat4 matR = glm::rotate(
			glm::mat4(1), rotation, glm::vec3(0, 1, 0));
		glm::vec3 t34Front = matR * glm::vec4(0, 0, 1, 1);

		// 自分からからターゲットへのベクトルdを計算
		glm::vec3 d = target->position - position;

		// 自分からターゲットへの距離を計算
		float length = glm::length(d);

		// ベクトルdを正規化
		d = glm::normalize(d);

		// 自分の正面ベクトルと、ターゲットへのベクトルの角度差
		float r = std::acos(glm::dot(tankFront, d));

		// 自分の正面とターゲットのいる方向の角度が10度未満の場合
		if (r < glm::radians(10.0f))
		{
			// ターゲットまでの距離が15mより遠い場合は前に加速
			if (length > 15.0f)
			{
				// 最大速度より遅い場合少しずつ加速
				if (!(glm::length(velocity) >= maxSpeed))
				{
					velocity += tankFront * 0.3f;
				}
				// 最大速度を超えると最大速度を代入
				else
				{
					velocity = tankFront * maxSpeed;
				}
			}
			// 15m未満の場合は減速→停止
			else
			{
				// ベロシティのt34Front方向の長さを計算
				float v = glm::dot(tankFront, velocity);
				// 長さが0.2以上なら0.2を減速、それ以下なら長さ分を減速(=停止)する
				velocity -= tankFront * glm::clamp(v, -0.2f, 0.2f);
			}
		}
		// 角度が10度以上の場合
		else
		{
			// 自分の正面ベクトルと、ターゲットへのベクトルの外積を計算
			glm::vec3 n = glm::cross(tankFront, d);
			// yが0以上なら反時計回り、0未満なら時計回りに回転するほうが近い
			if (n.y >= 0)
			{
				rotation += glm::radians(90.0f) * deltaTime;
			}
			else
			{
				rotation -= glm::radians(90.0f) * deltaTime;
			}
		}

		// インターバルを減らす
		shotTimer -= deltaTime;
		// 弾を発射する
		if (shotTimer <= 0)
		{
			// インターバルの設定
			shotTimer = 5;
			// 発射位置を砲の先端に設定
			glm::vec3 position = this->position + tankFront * 3.0f;
			position.y += 2.0f;

			// ゲームエンジンの取得
			GameEngine& engine = GameEngine::Get();
			// 弾アクターを作成
			std::shared_ptr<Actor> bullet(new Bullet{ position ,attack });
			// 1.5秒後に弾を消す
			bullet->lifespan = 1.5f;
			// 戦車の向いている方向に20m/sの速度で移動させる
			bullet->velocity = tankFront * 20.0f;
			// アクターを追加
			engine.AddActors(bullet);
		}
	}
	// ターゲットがいなければターゲットを探す
	else
	{
		// 曲がる方向
		static float dir = 0;
		// 回転が終わる角度
		static float finishRotation = 0;
		// 前進する時間
		static float time = moveTime;
		if (isMoving)
		{
			// 決めた方向に向く
			if (dir < 0)
			{
				// 真横を向くまで回転させる
				if (rotation > finishRotation)
				{
					rotation -= glm::radians(90.0f) * deltaTime;
				}
				else
				{
					// 前進するようにする
					dir = 0;
				}
			}
			else if (dir > 0)
			{
				// 真横を向くまで回転させる
				if (rotation < finishRotation)
				{
					rotation += glm::radians(90.0f) * deltaTime;
				}
				else
				{
					// 前進するようにする
					dir = 0;
				}
			}
			// 前方向であれば前に進む
			else
			{
				// 最大速度になるまで速度を加算
				if (glm::length(velocity) < maxSpeed)
				{
					velocity += tankFront * 0.3f;
				}

				if (time < 0)
				{
					// 動けない状態にする
					isMoving = false;
					// 動ける時間を初期化
					time = moveTime;
				}
				else
				{
					// 動ける時間を減らす
					time -= deltaTime;
				}
			}
		}
		else
		{
			// 動くことのできる方向
			bool canMoveFront = false;
			bool canMoveRight = false;
			bool canMoveLeft = false;

			// 回転角度を入れる変数
			std::vector<float> direction;

			// 右方向を求める
			glm::vec3 right(1, 0, 0);
			// 回転行列
			const glm::mat4 matRotR = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
			right = matRotR * glm::vec4(right, 1);

			// レイを飛ばす最初の位置を設定(自分に当たらないように少し右に出して、地面に当たらないようにyに1足す)
			glm::vec3 startRight = position + glm::vec3(0, 1, 0) + right * 3.0f;
			// レイの終了位置を設定(地面に当たらないようにyに1足す,右に100m飛ばす)
			glm::vec3 endRight = position + glm::vec3(0, 1, 0) + right * 5.0f;

			// 左方向を求める
			glm::vec3 left(-1, 0, 0);
			// 回転行列
			const glm::mat4 matRotL = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
			left = matRotL * glm::vec4(left, 1);
			// レイを飛ばす最初の位置を設定(自分に当たらないように少し左に出して、地面に当たらないようにyに1足す)
			glm::vec3 startLeft = position + glm::vec3(0, 1, 0) + left * 3.0f;
			// レイの終了位置を設定(地面に当たらないようにyに1足す,左に100m飛ばす)
			glm::vec3 endLeft = position + glm::vec3(0, 1, 0) + left * 5.0f;

			// 前方のレイの情報
			Hit frontHit;
			// 右方向のレイの情報
			Hit rightHit;
			// 左方向のレイの情報
			Hit leftHit;

			// 前方に敵戦車がいないことは調べてあるので敵戦車かは調べない
			// 前にオブジェクトがなければ前に動ける
			if (!RayCast(start, end, frontHit))
			{
				canMoveFront = true;
				direction.push_back(0);
			}
			// 右にオブジェクトがなければ右に動ける
			if (!RayCast(startRight, endRight, rightHit))
			{
				canMoveRight = true;
				direction.push_back(-90);
			}
			else
			{
				// 右にあるオブジェクトが敵戦車であればターゲットに入れる
				if (rightHit.hitActor->tag == enemyTag)
				{
					target = rightHit.hitActor.get();
				}
			}

			// 右に敵戦車がいなければ左を調べる
			// 左に敵戦車がいた場合 target に nullptr 以外が入っている
			if (target == nullptr)
			{
				// 左にオブジェクトが無ければ左に動ける
				if (!RayCast(startLeft, endLeft, leftHit))
				{
					canMoveLeft = true;
					direction.push_back(90);
				}
				else
				{
					// 左にあるオブジェクトが敵戦車であればターゲットに入れる
					if (leftHit.hitActor->tag == enemyTag)
					{
						target = leftHit.hitActor.get();
					}
				}
			}

			// 右にも左にも敵戦車がない場合のみ動く
			// 左右どちらかに敵戦車がいた場合 target に nullptr 以外が入っている
			if (target == nullptr)
			{
				// 左右どちらかに動ける状態であれば、前左右にランダムで動く
				if (canMoveFront || canMoveRight || canMoveLeft)
				{
					// 非決定的な乱数を生成
					std::random_device rand;
					// メルセンヌ・ツイスタ生成
					std::mt19937 mt(rand());
					// 曲る方向をランダムで決定
					int turn = mt() % direction.size();
					// 曲がる方向を保存
					dir = glm::radians(direction[turn]);
					// 曲がる方向を初期化
					direction.clear();
					// 回転している状態にする
					isMoving = true;
					// 回転の終わりを設定
					finishRotation = rotation + dir;
				}
			}
		}
	}
}

/**
* 攻撃
* 
* @param deltaTime 前回からの経過時間
*/
void Enemy::Attack(float deltaTime)
{
	shotTimer -= deltaTime;
	// 発射できるまでの時間が0でないなら攻撃できない
	if (shotTimer >= 0)
	{
		return;
	}
	// 戦車の向きベクトル
	glm::vec3 tankFront = glm::vec3(0, 0, 1);
	// 回転させる回転行列
	const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
	// 向きベクトルの回転
	tankFront = matRot * glm::vec4(0, 0, 1, 1);
	
	// インターバルの設定
	shotTimer = shotMaxTimer;
	// 発射位置を砲の先端に設定
	glm::vec3 position = this->position + tankFront * 3.0f;
	position.y += 2.0f;

	// ゲームエンジンの取得
	GameEngine& engine = GameEngine::Get();
	// 弾アクターを作成
	std::shared_ptr<Actor> bullet(new Bullet{ position ,attack });
	// 1.5秒後に弾を消す
	bullet->lifespan = 1.5f;
	// 戦車の向いている方向に20m/sの速度で移動させる
	bullet->velocity = tankFront * 20.0f;
	// アクターを追加
	engine.AddActors(bullet);
}

/** 
* 戦車の更新
*/
void Enemy::OnUpdate(float deltaTime)
{	
	Move(deltaTime);
	Attack(deltaTime);
}