#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class ClearScene {
public:
	~ClearScene();

	void Initialize();
	void Update();
	void Draw();
	bool IsFinished();

private:
	static inline const float kTimeTitleMove = 2.0f;

	ViewProjection viewProjection_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelTitle_ = nullptr;

	float counter_ = 0.0f;
	bool finished_ = false;
};