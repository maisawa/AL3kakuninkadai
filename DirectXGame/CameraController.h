
#pragma once
#include "ViewProjection.h"

class Player;

class CameraController {
public:
	// 短形
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
	void SetMovableArea(const Rect& area);

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -15.0f};
	Rect movadeArea_ = {0, 100, 0, 100};
	Vector3 objective;
	static inline const Rect margin = {-9.0f, 9.0f, -5.0f, 5.0f};
	static inline const float kInterpolationRate = 0.1f;
	static inline const float kVelocityBias = 30.0f;
};
