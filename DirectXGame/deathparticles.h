#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <array>
#include <numbers>
#include <Vector4.h>
#include <ObjectColor.h>
using namespace std;

class DeathParticles {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

private:
	
	static inline const uint32_t kNumParticles = 8;
	Model* model_ = nullptr;
	array<WorldTransform, kNumParticles> worldTransforms_;
	ViewProjection* viewProjection_ = nullptr;
	bool isFinished_ = false;
	float counter_ = 0.0f;
	ObjectColor objectColor_;
	Vector4 color_;
	
	static inline const float kDuration = 2.0f;
	static inline const float kSpeed = 0.05f;
	static inline const float kAngleUnit = 2.0f * numbers::pi_v<float> / kNumParticles;
};
