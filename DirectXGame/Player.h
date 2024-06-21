#include "Model.h"
#include "WorldTransform.h"

enum class LRDirection {
	kRight,
	kLeft,
};

class Player{
public:

void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
void Update();
void Draw();
private:

// ワールドトランスフォームの初期化
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	LRDirection lrDirection_ = LRDirection::kRight;
	
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.7f;

	bool onGround_ = true;
	bool landing = false;

	static inline const float kAcceleratio = 1.0f;
	static inline const float kAttenuation = 0.1f;

	static inline const float kLimitRunSpeed = 0.1f;
	static inline const float kTimeTurn = 0.3f;

};