#include "CameraController.h"
#include "Player.h"
#include <algorithm>
using namespace std;

void CameraController::Initialize() { viewProjection_.Initialize(); }
void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	Vector3 targetVelocity = target_->GetVelocity();

	destination_.x = targetWorldTransform.translation_.x + targetOffset_.x * targetVelocity.x + targetOffset_.x;
	destination_.y = targetWorldTransform.translation_.y + targetOffset_.y * targetVelocity.y + targetOffset_.y;
	destination_.z = targetWorldTransform.translation_.z + targetOffset_.z * targetVelocity.z + targetOffset_.z;

	viewProjection_.translation_.x = Lerp(viewProjection_.translation_.x, destination_.x, kInterpolationRate_);
	viewProjection_.translation_.y = Lerp(viewProjection_.translation_.y, destination_.y, kInterpolationRate_);
	viewProjection_.translation_.z = Lerp(viewProjection_.translation_.z, destination_.z, kInterpolationRate_);

	viewProjection_.translation_.x = clamp(viewProjection_.translation_.x, targetWorldTransform.translation_.x + targetMargin.left, targetWorldTransform.translation_.x + targetMargin.right);

	viewProjection_.translation_.y = clamp(viewProjection_.translation_.y, targetWorldTransform.translation_.y + targetMargin.bottom, targetWorldTransform.translation_.y + targetMargin.top);
	viewProjection_.translation_.x = clamp(viewProjection_.translation_.x, movableArea_.left, movableArea_.right);
	viewProjection_.translation_.y = clamp(viewProjection_.translation_.y, movableArea_.bottom, movableArea_.top);

	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}