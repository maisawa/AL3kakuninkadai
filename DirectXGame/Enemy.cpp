#include "Enemy.h"
#include "myMath.h"
#include <cassert>
#include <numbers>
using namespace std;

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = numbers::pi_v<float> * 3.0f / 2.0f;
	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer = 0.0f;
}

void Enemy::Update() { 
	worldTransform_.translation_ += velocity_; 
	walkTimer += 1.0f / 60.0f;
	worldTransform_.rotation_.x = sin(numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	model_->Draw(worldTransform_, *viewProjection_); 
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { 
	(void)player; 
}
