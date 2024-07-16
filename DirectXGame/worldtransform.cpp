#include "WorldTransform.h"
#include "math.h"

void WorldTransform::UpdateMatrix() {
    matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	TransferMatrix();
}