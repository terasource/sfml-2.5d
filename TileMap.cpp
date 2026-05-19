#include "TileMap.hpp"
#include <iostream>

TileMap::TileMap() {

};


bool TileMap::addTileset(int firstGid, const std::string& imagePath, sf::Vector2f tileSize) {
    TilesetData tData;
    tData.firstGid = firstGid;
    tData.mTileSize = tileSize;

    if (!tData.mTexture.loadFromFile(imagePath)) {
        std::cout << "image texture could not be loaded! " << std::endl;
        return false;
    }

    TilesetsData.push_back(tData);

    return true;
}

void TileMap::AddAnimation(unsigned int gid, const TileAnimation& animation) {
    mAnimations[gid] = animation;
}

void TileMap::buildMap(std::vector<int>& mapData, int width, int height) {
    MapLayer newLayer;
    newLayer.mVertices.resize(TilesetsData.size());
    for (size_t i = 0; i < TilesetsData.size(); i++)
        newLayer.mVertices[i].setPrimitiveType(sf::PrimitiveType::Triangles);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            int index = i + (j * width);
            int gid = mapData[index];

            gid &= 0x1FFFFFFF;

            if (gid == 0) continue;

            TilesetData* currentTileSet = nullptr;
            int textureIndex = -1;
            for (int k = TilesetsData.size() - 1; k >= 0; k--) {
                if (gid >= TilesetsData[k].firstGid) {
                    currentTileSet = &TilesetsData[k];
                    textureIndex = k;
                    break;
                }
            }


            int localid = gid - currentTileSet->firstGid; // gid - firstgid

            int row = localid % (int)(currentTileSet->mTexture.getSize().x / currentTileSet->mTileSize.x);
            int column = localid / (int)(currentTileSet->mTexture.getSize().x / currentTileSet->mTileSize.x);

            const int quadSize = 6;
            sf::Vertex quad[quadSize];

            float x = currentTileSet->mTileSize.x;
            float y = currentTileSet->mTileSize.y;

            auto it = mAnimations.find(gid);
            if (it != mAnimations.end()) {
                size_t vertexindex = newLayer.mVertices[textureIndex].getVertexCount();
                size_t layerindex = mapLayers.size();

                it->second.instances.push_back({ vertexindex, layerindex , textureIndex });
            }



            quad[0].position = sf::Vector2f({ i * x, j * y });
            quad[1].position = sf::Vector2f({ (i + 1) * x, j * y });
            quad[2].position = sf::Vector2f({ (i + 1) * x, (j + 1) * y });
            quad[3].position = sf::Vector2f({ i * x, (j + 1) * y });

            quad[0].texCoords = sf::Vector2f({ row * x, column * y });
            quad[1].texCoords = sf::Vector2f({ (row + 1) * x, column * y });
            quad[2].texCoords = sf::Vector2f({ (row + 1) * x, (column + 1) * y });
            quad[3].texCoords = sf::Vector2f({ row * x, (column + 1) * y });


            newLayer.mVertices[textureIndex].append(quad[0]);
            newLayer.mVertices[textureIndex].append(quad[1]);
            newLayer.mVertices[textureIndex].append(quad[2]);

            newLayer.mVertices[textureIndex].append(quad[2]);
            newLayer.mVertices[textureIndex].append(quad[3]);
            newLayer.mVertices[textureIndex].append(quad[0]);

        }
    }
    mapLayers.push_back(newLayer);

}

void TileMap::update(sf::Time dt) {

    for (auto& [gid, anim] : mAnimations) {
        anim.animtimer += dt.asSeconds();
        if (anim.animtimer >= anim.frames[anim.currentindex].duration / 1000.f) {
            anim.animtimer = 0.0f;
            anim.currentindex = (anim.currentindex + 1) % anim.frames.size();

            int local_id = anim.frames[anim.currentindex].localid; // new tile_localid

            for (const auto& instance : anim.instances) {

                const auto& currentTileset = TilesetsData[instance.tilesetindex];

                int columns = currentTileset.mTexture.getSize().x / currentTileset.mTileSize.x;

                int row = local_id / columns;
                int column = local_id % columns;

                float left = column * currentTileset.mTileSize.x;
                float top = row * currentTileset.mTileSize.y;
                float right = left + currentTileset.mTileSize.x;
                float bottom = top + currentTileset.mTileSize.y;

                auto& vertices = mapLayers[instance.layer_index].mVertices[instance.tilesetindex];
                vertices[instance.vertex_index + 0].texCoords = { left, top };
                vertices[instance.vertex_index + 1].texCoords = { right, top };
                vertices[instance.vertex_index + 2].texCoords = { right, bottom };

                vertices[instance.vertex_index + 3].texCoords = { right, bottom };
                vertices[instance.vertex_index + 4].texCoords = { left, bottom };
                vertices[instance.vertex_index + 5].texCoords = { left, top };

            }
        }

    }

}


void TileMap::draw(sf::RenderTarget& target, sf::RenderStates state) const {

    state.transform *= getTransform();

    for (const auto& layer : mapLayers) {

        for (size_t i = 0; i < TilesetsData.size(); i++) {
            if (layer.mVertices[i].getVertexCount() > 0) {
                state.texture = &TilesetsData[i].mTexture;
                target.draw(layer.mVertices[i], state);
            }
        }
    }
}



