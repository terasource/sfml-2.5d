#include "TileMap.hpp"
#include <iostream>

TileMap::TileMap() {

};

bool TileMap::addTileset(int firstGid, const std::string& imagePath, sf::Vector2f tileSize) {
    TilesetData tData;
    tData.mVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    tData.firstGid = firstGid;
    tData.mTileSize = tileSize;

    if (!tData.mTexture.loadFromFile(imagePath)) {
        std::cout << "image texture could not be loaded! " << std::endl;
        return false;
    }

    TilesetsData.push_back(tData);

    return true;
}

void TileMap::buildMap(std::vector<int>& mapData, int width, int height) {

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            int index = i + (j * width);
            int gid = mapData[index];
            gid &= 0x1FFFFFFF;
            if (gid == 0) continue;

            TilesetData* currentTileSet = nullptr;

            for (size_t i = TilesetsData.size() - 1; i >= 0; i--) {
                if (gid >= TilesetsData[i].firstGid) {
                    currentTileSet = &TilesetsData[i];
                    break;
                }
            }

            int localid = gid - currentTileSet->firstGid; // gid - firstgid

            int row = localid % (int)(currentTileSet->mTexture.getSize().x / currentTileSet->mTileSize.x);
            int column = localid / (int)(currentTileSet->mTexture.getSize().x / currentTileSet->mTileSize.x);

            int quadSize = 6;
            sf::Vertex quad[quadSize];
            float x = currentTileSet->mTileSize.x;
            float y = currentTileSet->mTileSize.y;

            quad[0].position = sf::Vector2f({ i * x, j * y });
            quad[1].position = sf::Vector2f({ (i + 1) * x, j * y });
            quad[2].position = sf::Vector2f({ (i + 1) * x, (j + 1) * y });
            quad[3].position = sf::Vector2f({ i * x, (j + 1) * y });

            quad[0].texCoords = sf::Vector2f({ row * x, column * y });
            quad[1].texCoords = sf::Vector2f({ (row + 1) * x, column * y });
            quad[2].texCoords = sf::Vector2f({ (row + 1) * x, (column + 1) * y });
            quad[3].texCoords = sf::Vector2f({ row * x, (column + 1) * y });

            currentTileSet->mVertices.append(quad[0]);
            currentTileSet->mVertices.append(quad[1]);
            currentTileSet->mVertices.append(quad[2]);

            currentTileSet->mVertices.append(quad[2]);
            currentTileSet->mVertices.append(quad[3]);
            currentTileSet->mVertices.append(quad[0]);
        }
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates state) const {

    state.transform *= getTransform();

    for (auto& tileSet : TilesetsData) {

        if (tileSet.mVertices.getVertexCount() > 0) {
            state.texture = &tileSet.mTexture;
            target.draw(tileSet.mVertices, state);
        }

    }
}



