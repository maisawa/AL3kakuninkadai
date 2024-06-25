#include "Skydome.h"

/// 初期化
void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
    // 引数の内容をメンバ変数に記録
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
};

/// 毎フレーム処理
void Skydome::Update(){};

/// 描画
void Skydome::Draw() {
	//3Dモデル描画
	model_->Draw(worldTransform_,*viewProjection_); };