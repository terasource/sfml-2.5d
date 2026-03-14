#include "TileMap.hpp"
#include <fstream>
#include <iostream>
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;

void loadMap(const std::string& jsonpath, TileMap& maps) {

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
        if (!ts.contains("image"))
            continue;
        int firstGid = ts["firstgid"];
        std::string jsonipath = ts["image"];
        std::string imagepath = pathPrefix + jsonipath;

        float tsw = ts["tilewidth"];
        float tsh = ts["tileheight"];

        if (!maps.addTileset(firstGid, imagepath, sf::Vector2f{ tsw,tsh }));
        //std::cout << "Tileset could not be added to the mwap!" << std::endl;
    }

    for (const auto& flayer : data["layers"]) {
        if (flayer["type"] != "tilelayer")
            continue;

        //auto& flayer = data["layers"][layer];

        //int datasize = data["layers"].size();
        //std::cout << "size = " << datasize << std::endl;

        std::vector<int> mapData = flayer["data"].get<std::vector<int>>();
        int lw = flayer["width"];
        int lh = flayer["height"];

        maps.buildMap(mapData, lw, lh);
    }
}