#include "probs.hpp"
#include "include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
/*
    match the textures with the calculated localid via formula gid-firstgid = localid.
*/
using json = nlohmann::json;

probs::probs() {}

void probs::matchTextures() {

    std::string pathPrefix = "assets/";

    std::ifstream ifs("assets/Sample map.json");

    if (!ifs.is_open()) {
        std::cerr << "File could not be opened! " << std::endl;
        return;
    }

    json data = json::parse(ifs);


    for (const auto& dict : data["tilesets"]) {

        if (!dict.contains("tiles"))
            continue;

        for (int j = 0; j < dict["tiles"].size(); j++) {
            int localid = dict["tiles"][j]["id"];
            int firstgid = dict["firstgid"];

            if (!dict["tiles"][j].contains("image"))
                continue;

            probTileDict[localid + firstgid] = pathPrefix + dict["tiles"][j]["image"].get<std::string>();
        }
    }

    for (const auto& proplayers : data["layers"]) {
        if (proplayers["type"] != "objectgroup") {
            continue;
        }

        for (int i = 0; i < proplayers["objects"].size(); i++) {
            auto& obj = proplayers["objects"][i];

            if (obj.find("gid") == obj.end()) continue;

            prop cprop;
            cprop.gid = proplayers["objects"][i]["gid"];

            cprop.x = proplayers["objects"][i]["x"];
            cprop.y = proplayers["objects"][i]["y"];

            cprop.height = proplayers["objects"][i]["height"];
            cprop.width = proplayers["objects"][i]["width"];

            for (const auto& proptilesets : data["tilesets"]) {
                int currentfirstgid = proptilesets["firstgid"];
                int current_tilecount = proptilesets["tilecount"];
                if (cprop.gid >= currentfirstgid && cprop.gid < (currentfirstgid + current_tilecount)) {
                    int localid = cprop.gid - currentfirstgid;
                    cprop.propPath = probTileDict[localid + currentfirstgid];
                    break;
                }
            }

            propList.props.push_back(cprop);
        }
    }

    for (int i = 0; i < propList.props.size(); i++) {
        if (propTextures.find(propList.props[i].propPath) == propTextures.end()) {
            if (propList.props.at(i).propPath == "" && propList.props[i].propPath.empty()) {
                std::cerr << "empty or unknown path! at index of = " << i << std::endl;
                propSprites.push_back(sf::Sprite(emptyTexture));
                continue;
            }
            if (!propTextures[propList.props[i].propPath].loadFromFile(propList.props[i].propPath)) {
                std::cerr << "texture could not be loaded " << propList.props[i].propPath << std::endl;
            }
        }
        sf::Sprite csprite(propTextures[propList.props[i].propPath]);
        propSprites.push_back(csprite);
    }
}

void probs::DrawProbs(sf::RenderWindow& mWindow) {
    //std::cout << "prop draw function is working! " << std::endl;
    for (int i = 0; i < propSprites.size(); i++) {

        if (propList.props[i].propPath.empty()) continue;

        float px = propList.props[i].x;
        float py = propList.props[i].y;

        sf::FloatRect bounds = propSprites[i].getLocalBounds();

        propSprites[i].setPosition({ px, py - bounds.size.y });

        /*
        std::cout <<
            "path = " << propList.props[i].propPath <<
            " x = " << propList.props[i].x <<
            " y = " << propList.props[i].y <<
            " bounds.y = " << bounds.size.y <<
            " at index of = " << i << std::endl;
        */

        mWindow.draw(propSprites[i]);
    }
}
