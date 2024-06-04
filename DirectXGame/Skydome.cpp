#include "Skydome.h"
#include <cassert>
void Skydome::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

//UpdateŠÖ”‚Ì’è‹`
void Skydome::Update()
{
	worldTransform_.TransferMatrix();
}

//DrawŠÖ”‚Ì’è‹`
void Skydome::Draw()
{
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_);
}