#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DebugCamera.h"
#include "DeathParticles.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <vector>
using namespace std;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void ChangePhase();

	void GenerateBlocks();

	void UpdateCamera();

	void UpdateBlocks();

	void CheckAllCollisions();

private: // メンバ変数

	enum class Phase {
		kPlay, // ゲームプレイ
		kDeath, // デス演出
	};

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	ViewProjection viewProjection_;
	uint32_t textureHandle_ = 0;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticle_ = nullptr;
	vector<vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform worldTransformSkydome_;
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	MapChipField* mapChipField_;
	CameraController* cameraController = nullptr;
	list<Enemy*> enemies_;
	Enemy* newEnemy_ = nullptr;
	Phase phase_;
	DeathParticles* deathParticles_ = nullptr;
};