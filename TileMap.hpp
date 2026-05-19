#pragma once
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include "Tile.hpp"
#include <SFML/Graphics/Drawable.hpp>

struct MapLayer {
    std::vector<sf::VertexArray> mVertices;

};

struct TilesetData {
    int firstGid;
    sf::Texture mTexture;
    sf::Vector2f mTileSize;
};

struct AnimatedIstance {
    size_t vertex_index;
    size_t layer_index;

    int tilesetindex;
};

struct AnimationFrame {
    float duration;
    unsigned int localid;
};

struct TileAnimation {
    std::vector<AnimationFrame> frames;
    unsigned int currentindex = 0;

    float animtimer;
    std::vector<AnimatedIstance> instances;
};

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    TileMap();
    bool addTileset(int firstGid, const std::string& imagePath, sf::Vector2f tileSize);
    void buildMap(std::vector<int>& mapData, int width, int height);
    void update(sf::Time dt);
    void AddAnimation(unsigned int gid, const TileAnimation& animation);
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

private:
    std::vector <TilesetData> TilesetsData;
    std::vector<MapLayer> mapLayers;
    std::unordered_map<unsigned int, TileAnimation> mAnimations;
};

 