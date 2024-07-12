#pragma once
#include "Vector3.h"
#include <cstdint>
#include <stdint.h>
#include <vector>
using namespace std;

enum class MapChipType {
	kBlank, //空白
	kBlock, //ブロック
};

struct MapChipData {
	vector<vector<MapChipType>> data;
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

	void ResetMapChipData();
	void LoadMapChipCsv(const string& filePath);
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
	MapChipData mapChipData_;
};
