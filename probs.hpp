#pragma once
#include <map>
#include "SFML/Graphics.hpp"


struct prop {
    unsigned int gid;

    float x;
    float y;

    int height;
    int width;

    bool flipH;
    bool flipV;
    bool flipD;

    std::string propPath;
};

struct spropList {
    std::vector<prop> props;
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