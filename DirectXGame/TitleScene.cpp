#define NOMINMAX

#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

using namespace std;

TitleScene::~TitleScene() { 
	delete modelTitle_;
	delete modelPlayer_;
}

void TitleScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("text", true); 
	modelPlayer_ = Model::CreateFromOBJ("player");
	viewProjection_.Initialize();

	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * numbers::pi_v<float>;
	worldTransformPlayer_.translation_.y = -9.0f;

	const float kTextTitle = 10.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kTextTitle, kTextTitle, kTextTitle};
	worldTransformTitle_.rotation_.y = 0.99f * numbers::pi_v<float>;
}

void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		finished_ = true;
	}
	counter_ += 1.0f / 60.0f;
	counter_ = fmod(counter_, kTimeTitleMove);

	float angle = counter_ / kTimeTitleMove * 2.0f * numbers::pi_v<float>;

	worldTransformTitle_.translation_.y = sin(angle) + 10.0f;

	viewProjection_.TransferMatrix();
	worldTransformTitle_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
}

void TitleScene::Draw() { 
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);
	Model::PostDraw();
}

bool TitleScene::IsFinished(){ 
	return finished_;
}
