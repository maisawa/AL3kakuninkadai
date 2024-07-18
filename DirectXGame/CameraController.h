#pragma once

#include "ViewProjection.h"
#include "myMath.h"

class Player;

class CameraController {

public:
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	void Initialize();
	void Update();

	void SetTarget(Player* target);
	void Reset();

	ViewProjection& GetViewProjection();

	void SetMovableArea(Rect area);
	float Lerp(float x1, float x2, float t);

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -30.0f};
	Rect movableArea_ = {0, 100, 0, 100};
	Vector3 destination_;
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
	static inline const float kInterpolationRate_ = 0.1f;
	static inline const float kVelocityBias_ = 30.0f;
};