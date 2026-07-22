#include "probs.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
/*
    match the textures with the calculated localid via formula gid-firstgid = localid.
*/
using json = nlohmann::json;

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;

probs::probs() {}

void probs::matchTextures() {

    std::string pathPrefix = "assets/";

    std::ifstream ifs("assets/Sample map.json");

    if (!ifs.is_open()) {
        std::cerr << "File could not be opened! " << std::endl;
        return;
    }

    json data = json::parse(ifs);


    //seperate tiles
    for (const auto& dict : data["tilesets"]) {

        if (dict.contains("image")) {

            CollectionOfProps copropsinfo;
            copropsinfo.firstgid = dict["firstgid"];
            copropsinfo.tilecount = dict["tilecount"];
            copropsinfo.imagePath = pathPrefix + dict["image"].get<std::string>();
            copropsinfo.columns = dict["columns"];
            copropsinfo.tilewidth = dict["tilewidth"];
            copropsinfo.tileheight = dict["tileheight"];
            propList.coprops.push_back(copropsinfo);
        }

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
            unsigned int rawGid = obj["gid"].get<unsigned int>();
            cprop.flipH = (rawGid & FLIPPED_HORIZONTALLY_FLAG) != 0;
            cprop.flipV = (rawGid & FLIPPED_VERTICALLY_FLAG) != 0;
            cprop.flipD = (rawGid & FLIPPED_DIAGONALLY_FLAG) != 0;
            cprop.gid = rawGid & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

            cprop.x = proplayers["objects"][i]["x"];
            cprop.y = proplayers["objects"][i]["y"];

            cprop.height = proplayers["objects"][i]["height"];
            cprop.width = proplayers["objects"][i]["width"];


            if (probTileDict.count(cprop.gid)) {
                cprop.propPath = probTileDict[cprop.gid];
            }
            else {
                for (const auto& info : propList.coprops) {
                    if (cprop.gid >= info.firstgid && cprop.gid < info.firstgid + info.tilecount) {
                        unsigned int localid = cprop.gid - info.firstgid;
                        int col = localid % info.columns;
                        int row = localid / info.columns;
                        cprop.isGridProp = true;
                        cprop.propPath = info.imagePath;
                        cprop.textureRect = sf::IntRect({ col * info.tilewidth, row * info.tileheight }, { info.tilewidth, info.tileheight });
                        /* test (3)
                            std::cout << "grid prop gid=" << cprop.gid
                            << " path=" << cprop.propPath
                            << " cell=(" << col << "," << row << ")"
                            << " rect=(" << col * info.tilewidth << "," << row * info.tileheight << ")"
                            << std::endl;
                        */
                        break;
                    }
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
        if (propList.props[i].isGridProp)
            csprite.setTextureRect(propList.props[i].textureRect);


        propSprites.push_back(csprite);
    }
}

void probs::DrawProbs(sf::RenderWindow& mWindow) {
    // test (1)
    //std::cout << "prop draw function is working! " << std::endl;
    for (int i = 0; i < propSprites.size(); i++) {

        if (propList.props[i].propPath.empty()) continue;

        prop cprop = propList.props[i];
        //bottom left
        float px = cprop.x;
        float py = cprop.y;

        sf::FloatRect bounds = propSprites[i].getLocalBounds();
        //changing origin to center from top left.
        propSprites[i].setOrigin({ bounds.size.x / 2 , bounds.size.y / 2 });
        //but the tiled gives the coordinates based on bottom left so i need to arrange my origin(middle point of the tile) in order to bottom left can be put coordinates given by tiled correctly.
        propSprites[i].setPosition({ px + bounds.size.x / 2 , py - bounds.size.y / 2 });

        //then flipping is gonna be done from middle to prevent shifting.
        if (cprop.flipH)
            propSprites[i].setScale({ -1,1 });
        if (cprop.flipV)
            propSprites[i].setScale({ 1,-1 });
        if (cprop.flipH && cprop.flipV)
            propSprites[i].setRotation(sf::degrees(180));
        if (cprop.flipH && cprop.flipD)
            propSprites[i].setRotation(sf::degrees(90));
        if (cprop.flipV && cprop.flipD)
            propSprites[i].setRotation(sf::degrees(-90));


        /*
        test (2)
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
