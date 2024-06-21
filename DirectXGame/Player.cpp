#include "Player.h"
#include "myMath.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>
void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;
	viewProjection_ = viewProjection;
}
void Player::Update(){
if(onGround_){
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
    } 
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
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

  }
}
}

void Player::Draw(){

}