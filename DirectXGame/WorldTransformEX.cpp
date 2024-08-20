#include"WorldTransform.h"
#include"myMath.h"

void WorldTransform::UpdateMatrix(Vector3 blockposition) {
	scale_=blockposition;
	matWorld_=MakeAffineMatrix(scale_,rotation_,translation_);
	TransferMatrix();
}
void WorldTransform::UpdateMatrix() {
	matWorld_=MakeAffineMatrix(scale_,rotation_,translation_);
	
	TransferMatrix();
}