#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
    
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
};

void Skydome::Update(){

};

void Skydome::Draw() {
	
	model_->Draw(worldTransform_,*viewProjection_);
};