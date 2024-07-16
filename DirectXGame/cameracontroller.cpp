#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include "myMath.h"
#include <algorithm>
using namespace std;

void CameraController::Initialize() {
	viewProjection_.Initialize();
}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	Vector3 targetVelocity = target_->GetVelocity();

	objective = targetWorldTransform.translation_ + targetVelocity * kVelocityBias + targetOffset_;

	viewProjection_.translation_ = Lerp(viewProjection_.translation_, objective, kInterpolationRate);

	viewProjection_.translation_.x = clamp(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin.left, targetWorldTransform.translation_.x + margin.right);
	viewProjection_.translation_.y = clamp(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin.bottom, targetWorldTransform.translation_.y + margin.top);

	viewProjection_.translation_.x = clamp(viewProjection_.translation_.x, movadeArea_.left, movadeArea_.right);
	viewProjection_.translation_.y = clamp(viewProjection_.translation_.y, movadeArea_.bottom, movadeArea_.top);

	viewProjection_.UpdateMatrix();
}

void CameraController::SetTarget(Player* target){
	{ target_ = target; }
}

void CameraController::SetMovableArea(const Rect& area){
	{ movadeArea_ = area; }
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}

ViewProjection& CameraController::GetViewProjection(){
	return viewProjection_;
}
