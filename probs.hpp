#pragma once
#include <map>
#include "SFML/Graphics.hpp"


struct prop {
    unsigned int gid;
    std::string propPath;

    //if collection of images or not
    bool isGridProp = false;
    sf::IntRect textureRect;

    //tile attributes
    float x;
    float y;
    int height;
    int width;

    // flip flags
    bool flipH;
    bool flipV;
    bool flipD;

};

struct CollectionOfProps {
    unsigned int firstgid;
    unsigned int tilecount;
    std::string imagePath;
    int columns;
    int tilewidth;
    int tileheight;
};

struct spropList {
    std::vector<prop> props;
    std::vector<CollectionOfProps> coprops;
};


class probs {
private:

    spropList propList;
    std::map<int, std::string> probTileDict;

    std::map<std::string, sf::Texture> propTextures;
    std::vector<sf::Sprite> propSprites;
    sf::Texture emptyTexture;
public:
    void matchTextures(/*std::map<int, sf::Texture> probTextures, int id, int gid, int firstgid*/);
    void DrawProbs(sf::RenderWindow& window);
    probs();
};