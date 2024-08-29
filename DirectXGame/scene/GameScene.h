#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <vector>

#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "Player.h"
#include "Goal.h"

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

	bool IsFinished();

	void ChangePhase();

	void GenerateBlocks();

	void UpdateCamera();

	void UpdateBlocks();

	void CheckAllCollisions();

private: // メンバ変数

	enum class Phase {
		kPlay, // ゲームプレイ
		kDeath, // デス演出
		kClear,//クリア
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
	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticle_ = nullptr;
	Model* modelGoal_ = nullptr;

	vector<vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform worldTransformSkydome_;
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	MapChipField* mapChipField_;
	CameraController* cameraController = nullptr;
	list<Enemy*> enemies_;
	bool finished_ = false;
	Phase phase_;
	DeathParticles* deathParticles_ = nullptr;
	Vector3 scale={3,3,3};
	Goal* goal_;
};