#define NOMINMAX
#include "Player.h"
#include "myMath.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;
	viewProjection_ = viewProjection;
}

void Player::Update() {

	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {

					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleratio;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleratio;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
				
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
	
			operator+=(velocity_, acceleration);

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

			if (acceleration.x >= 0.01f || acceleration.x <= -0.01f) {
				acceleration.x = 0;
			}

			// 旋回制御
			if (turnTimer_ > 0.0f) {
				// タイマーのカウントダウン
				turnTimer_ = (1.0f / 60.0f);

				// 左右の自キャラ角度テーブル
				float destinationRotationYTable[] = {
				    std::numbers::pi_v<float> / 2.0f,
				    std::numbers::pi_v<float> * 3.0f / 2.0f,
				};
				// 状態に応じた角度を取得する
				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				// 自キャラの角度を設定する
				worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
			}
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
			velocity_.x += 0;
			velocity_.y += kJumpAcceleration;
			velocity_.z += 0;
		}
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.x += 0;
		velocity_.y += -kGravityAcceleration;
		velocity_.z += 0;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		// 着地フラグ
		landing = false;

		// 地面との当たり判定
		// 落下中？
		if (velocity_.y < 0) {
			// Y座標が地面以下になったら着地
			if (worldTransform_.translation_.y <= 2.0f) {
				landing = true;
			}
		}
	}
	// 移動
	operator+=(worldTransform_.translation_, velocity_);
	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 接地判定
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAcceleratio);
			// 下方向速度のリセット
			velocity_.y = 0.0f;
			// 接地状態の移行
			onGround_ = true;
		}
	}
	// 行列計算
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}