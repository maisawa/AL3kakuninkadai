#include "GameScene.h"
#include "myMath.h"
#include "TextureManager.h"
#include <cassert>
#include <cstdint>
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete deathParticles_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete player_;
	for (vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
	}
	delete modelDeathParticle_;
	delete modelEnemy_;
	delete model_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameraController;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	model_ = Model::CreateFromOBJ("player");
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("sphere", true);
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);
	//modelGoal_=Model::OBJ関数呼んでファイルを読み込む

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 16);
	player_->Initialize(model_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	worldTransformSkydome_.Initialize();

	GenerateBlocks();

	cameraController = new CameraController;
	cameraController->Initialize();
	cameraController->SetTarget(player_);
	cameraController->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController->SetMovableArea(cameraArea);

	Enemy* newEnemy = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18);
	newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

	enemies_.push_back(newEnemy);

	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	ChangePhase();

	switch (phase_) { 
	case Phase::kPlay:
		worldTransformSkydome_.UpdateMatrix(scale);

		player_->Update();

		cameraController->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		UpdateCamera();

		UpdateBlocks();

		CheckAllCollisions();
		break;
	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		worldTransformSkydome_.UpdateMatrix(scale);

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

		UpdateCamera();
		break;
	}
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	modelSkydome_->Draw(worldTransformSkydome_, viewProjection_);

	for (vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)

				continue;

			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	if (!player_->IsDead()) {
		player_->Draw();
	}

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ChangePhase() { 
	switch (phase_) { 
	case Phase::kPlay:
		if (player_->IsDead()) {
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			
			deathParticles_ = new DeathParticles;

			deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:

		break;
	}
}

bool GameScene::IsFinished() { 
	return finished_; 
};

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::UpdateCamera() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = cameraController->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
}

void GameScene::UpdateBlocks() {
	for (vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->UpdateMatrix();
		}
	}
}

void GameScene::CheckAllCollisions() { 

	AABB aabb1, aabb2;

	#pragma region 自キャラと敵キャラの当たり判定
	{ 
		aabb1 = player_->GetAABB();

		for (Enemy* enemy : enemies_) {
			aabb2 = enemy->GetAABB();

			if (IsCollision(aabb1, aabb2)) {
				player_->OnCollision(enemy);
				enemy->OnCollision(player_);
			}
		}
	}
	#pragma endregion
}