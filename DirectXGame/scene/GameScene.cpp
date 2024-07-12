#include "GameScene.h"
#include "myMath.h"
#include "TextureManager.h"
#include <cassert>
#include <cstdint>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete player_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
	}
	delete modelEnemy_;
	delete model_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameraController_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 3Dモデルの生成
	model_ = Model::CreateFromOBJ("player");
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("sphere", true);

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 16);
	player_->Initialize(model_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	worldTransform_.Initialize();

	GenerateBlocks();

	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	// 敵の生成
	newEnemy_ = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18);
	newEnemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

	enemies_.push_back(newEnemy_);
}

void GameScene::Update() {

	worldTransform_.UpdateMatrix();

	// 自キャラの更新
	player_->Update();

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	cameraController_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		// フラグをトグル
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	// カメラ処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	}

	// 縦横ブロック更新
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->UpdateMatrix();
		}
	}
	CheckAllCollisions();
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
	// 天球の描画
	modelSkydome_->Draw(worldTransform_, viewProjection_);

	for (vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)

				continue;

			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 自キャラの描画
	player_->Draw();
	// 敵の描画
	newEnemy_->Draw();

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

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetkNumkBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetkNumkBlockHorizontal();


	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定 (横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
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

void GameScene::CheckAllCollisions() { 

	// 判定対象1と2座標
	AABB aabb1, aabb2;

	#pragma region 自キャラと敵キャラの当たり判定
	{ 
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと擲弾全ての当たり判定
		for (Enemy* enemy : enemies_) {
			// 擲弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 擲弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}
	}
	#pragma endregion
}
