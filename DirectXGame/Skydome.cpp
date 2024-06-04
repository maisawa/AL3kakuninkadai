#include <Skydome.h>
#include <cassert>
void Skydome::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

//Update関数の定義
void Skydome::Update()
{
	worldTransform_.TransferMatrix();
}

//Draw関数の定義
void Skydome::Draw()
{
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_);
}