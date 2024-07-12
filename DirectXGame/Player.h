#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class MapChipField;

class Player {
public:
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};
	// マップチップとの当たり判定情報
	struct CollisionMapInfo {
		// 天井衝突フラグ
		bool ceiling = false;
		// 着地フラグ
		bool landing = false;
		// 壁接触フラグ
		bool hitWall = false;
		// 移動量
		Vector3 move;
	};

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
	const WorldTransform& GetWorldTransform();
	const Vector3& GetVelocity();
	void SetMapChipField(MapChipField* mapChipField);
	void InMovement();
	void TurningControl();
	void CollisionDetection(CollisionMapInfo& info);
	// 上
	void Top(CollisionMapInfo& info);
	// 下
	void Bottom(CollisionMapInfo& info);
	// 右
	void Right(CollisionMapInfo& info);
	// 左
	void Left(CollisionMapInfo& info);
	// 接地状態の切り替え処理
	void GroundingStateSwitching(const CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);

private:
	Model* model_ = nullptr;
	MapChipField* mapChipField_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	bool landing = false;
	bool onGround_ = true;

	static inline const float kAcceleratio = 0.1f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kAttenuationWall = 0.2f;
	static inline const float kAttenuationLanding = 0.0f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kLimitRunSpeed = 0.5f;
	static inline const float kTimeTurn = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.4f;
	static inline const float kGroundSearchHeight = 0.06f;
};
