#pragma once
#include "Model.h"
#include "WorldTransform.h"

class MapChipField;

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	// マップチップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		
		Vector3 move;
	};

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void InMovement();
	void TurningControl();
	void CollisionDetection(CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 上
	void Top(CollisionMapInfo& info);
	// 下
	void Bottom(CollisionMapInfo& info);
	// 右
	void Right(CollisionMapInfo& info);
	// 左
	void Left(CollisionMapInfo& info);

	// 判断結果を反映して移動させる
	void ReflectionMovement(const CollisionMapInfo& info);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	MapChipField* mapChipField_ = nullptr;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.7f;

	bool onGround_ = true;
	bool landing = false;

	static inline const float kAcceleratio = 1.0f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 100.0f;
	static inline const float kLimitRunSpeed = 0.1f;
	static inline const float kTimeTurn = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.4f;
};
