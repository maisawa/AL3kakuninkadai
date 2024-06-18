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
	/// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// 更新
	void Update();

	/// 描画
	void Draw();

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType MapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 MapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetkNumkBlockVirtical();
	uint32_t GetkNumkBlockHorizontal();

private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumkBlockVirtical = 20;
	static inline const uint32_t kNumkBlockHorizontal = 100;

	// ワールドトランスフォームの初期化
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	MapChipData mapChipData_;
};