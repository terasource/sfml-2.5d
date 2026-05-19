#include "TileMap.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
void loadMapData(const std::string& jsonpath, TileMap& maps) {

    std::string pathPrefix = "assets/";

    std::ifstream ifs("assets/Sample map.json");

    if (!ifs.is_open()) {
        std::cerr << "File could not be opened!" << std::endl;
        return;
    }

    json data = json::parse(ifs);
    int tw = data["tilewidth"];
    int th = data["tileheight"];

    for (const auto& ts : data["tilesets"]) {

        int firstGid = ts["firstgid"];

        if (!ts.contains("image"))
            continue;

        if (ts.contains("tiles")) {

            for (const auto& tile : ts["tiles"]) {

                if (!tile.contains("animation"))
                    continue;

                TileAnimation animation;
                unsigned int animgid;
                for (const auto& anim : tile["animation"]) {
                    animgid = firstGid + tile["id"].get<int>();
                    animation.frames.push_back({ anim["duration"], anim["tileid"] });
                }
                maps.AddAnimation(animgid, animation);
            }
        }

        std::string jsonipath = ts["image"];
        std::string imagepath = pathPrefix + jsonipath;

        float tsw = ts["tilewidth"];
        float tsh = ts["tileheight"];

        if (!maps.addTileset(firstGid, imagepath, sf::Vector2f{ tsw,tsh }));
        // std::cout << "Tileset could not be added to the mwap!" << std::endl; --ignore for now

    }

    for (const auto& flayer : data["layers"]) {

        if (flayer["type"] != "tilelayer")
            continue;


        std::vector<int> mapData = flayer["data"].get<std::vector<int>>();

        int lw = flayer["width"];
        int lh = flayer["height"];

        maps.buildMap(mapData, lw, lh);
    }
}