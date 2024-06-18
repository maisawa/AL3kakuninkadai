#include "Player.h"
#include <cassert>
#include<numbers>
#include <Input.h>

void Player::Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_=position;
	worldTransform_.rotation_.y=std::numbers::pi_v<float>/2.0f;

	// 引数の内容をメンバ変数に記録
	model_ = model;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
}

void Player::Update() {
	if(Input::GetInstance()->PushKey(DIK_RIGHT)||Input::GetInstance()->PushKey(DIK_LEFT)){
	     Vector3 acceleration={};
	     if(Input::GetInstance()->PushKey(DIK_RIGHT)){
	             acceleration.x+=kAcceleration;
	     }else if(Input::GetInstance()->PushKey(DIK_LEFT)){
	             acceleration.x-=kAcceleration;
		 }
		 velocity_+=acceleration;
	}
	worldTransform_.translation_+=velocity_;
	worldTransform_.UpdateMatrix();

}

void Player::Draw() {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}