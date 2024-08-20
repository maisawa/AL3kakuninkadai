#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
using namespace std;

namespace { 
    map<string, MapChipType> mapChipTable = { 
        {"0", MapChipType::kBlank},
        {"1", MapChipType::kBlock},
		//2を追加
    };
}

void MapChipField::ResetMapChipData() {
    mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
    }
}

void MapChipField::LoadMapChipCsv(const string& filePath) {

	ResetMapChipData();
	ifstream file;
	file.open(filePath);
	assert(file.is_open());
	stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();
	ResetMapChipData();
	string line;
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		getline(mapChipCsv, line);
		istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { 
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

MapChipType MapChipField::GetMapChipTypeByPosition(const Vector3& position) { 
	IndexSet indexSet = GetMapChipIndexSetByPosition(position); 

	return GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) { 
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>(position.y + kBlockHeight / 2.0f / kBlockHeight);
	return indexSet;
}

uint32_t MapChipField::GetNumBlockVirtical(){
	return kNumBlockVirtical;
}

uint32_t MapChipField::GetNumBlockHorizontal(){
	return kNumBlockHorizontal; 
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