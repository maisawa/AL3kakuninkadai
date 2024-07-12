#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "MapChipField.h"
using namespace std;

namespace {

map<string, MapChipType> mapChipType = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumkBlockVirtical);
	for (vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumkBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const string& filePath) {
	ifstream file;
	file.open(filePath);
	assert(file.is_open());
	stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();
	ResetMapChipData();
	string line;
	for (uint32_t i = 0; i < kNumkBlockVirtical; ++i) {
		getline(mapChipCsv, line);

		istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumkBlockHorizontal; ++j) {
			string word;
			getline(line_stream, word, ',');

			if (mapChipType.contains(word)) {
				mapChipData_.data[i][j] = mapChipType[word];
			}
		}
	}
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumkBlockVirtical - 1 - yIndex), 0); 
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumkBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumkBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

uint32_t MapChipField::GetkNumkBlockVirtical(){
	return kNumkBlockVirtical;
}

uint32_t MapChipField::GetkNumkBlockHorizontal(){
	return kNumkBlockHorizontal;
}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	MapChipField::IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = kNumkBlockVirtical - 1 - static_cast<uint32_t>(position.y + kBlockHeight / 2 / kBlockHeight);

	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockWidth / 2.0f;
	rect.top = center.y + kBlockWidth / 2.0f;

	return rect;
}