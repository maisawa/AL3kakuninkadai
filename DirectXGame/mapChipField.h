
#pragma once
#include <cstdint>
#include <stdint.h>
#include <vector>
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
		float left;   //左端
		float right;  //右端
		float bottom; //下端
		float top;	  //上端
	};

	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update();
	void Draw();
	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetkNumkBlockVirtical();
	uint32_t GetkNumkBlockHorizontal();
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	static inline const uint32_t kNumkBlockVirtical = 20;
	static inline const uint32_t kNumkBlockHorizontal = 100;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	MapChipData mapChipData_;
};
