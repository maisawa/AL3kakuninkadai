#include "Vector3.h"
#include <cstdint>
#include <vector>
#include <string>
using namespace std;

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {
	vector<vector<MapChipType>> data;
};

class MapChipField {
public:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipType GetMapChipTypeByPosition(const Vector3& position);
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};