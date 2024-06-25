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

			if (turnTimer_ > 0.0f) {
				turnTimer_ = (1.0f / 60.0f);
				float destinationRotationYTable[] = {
				    std::numbers::pi_v<float> / 2.0f,
				    std::numbers::pi_v<float> * 3.0f / 2.0f,
				};
				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
			}
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAcceleration, 0);
			velocity_.x += 0;
			velocity_.y += kJumpAcceleration;
			velocity_.z += 0;
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.x += 0;
		velocity_.y += -kGravityAcceleration;
		velocity_.z += 0;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		landing = false;
		if (velocity_.y < 0) {

			if (worldTransform_.translation_.y <= 2.0f) {
				landing = true;
			}
		}
	}
	operator+=(worldTransform_.translation_, velocity_);
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAcceleratio);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

WorldTransform& Player::GetWorldTransform(){
      return worldTransform_;
}

Vector3& Player::GetVelocity(){
	return velocity_;
}