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
    //sf::VertexArray mVertices;
    sf::Vector2f mTileSize;
};

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    TileMap();
    bool addTileset(int firstGid, const std::string& imagePath, sf::Vector2f tileSize);
    void buildMap(std::vector<int>& mapData, int width, int height);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

private:
    std::vector <TilesetData> TilesetsData;
    std::vector <MapLayer> mapLayers;
};

