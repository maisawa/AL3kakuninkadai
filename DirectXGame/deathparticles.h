#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <array>
#include <numbers>
using namespace std;

class DeathParticles{
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
private:
	Model *model_=nullptr;
	ViewProjection* viewProjection=nullptr;
	static inline const uint32_t kNumParticles=8;
	array<WorldTransform,kNumParticles>worldTransforms_;
	static inline const float kDuration=3;
	static inline const float kSpeed=3;
	static inline const float kAngleUnit=2.0f *numbers::pi_v<float> / kNumParticles;
	bool isFinished_=false;
	float counter_=0.0f;
	ObjectColor objectColor_;
	Vector4 color_;
};