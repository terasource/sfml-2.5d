#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <iostream>
#include <memory>

struct _texture_types {
    struct assets {
        struct visual_asset {
            struct _material {

            };
            struct _2D_sprite {
                struct character {
                    struct hair {
                        enum class texture {
                            base,
                            alternative,
                        };

                        enum class color {};
                    };
                    struct body {
                        enum class texture {
                            base,
                            alternative,
                        };

                        enum class color {};
                    };
                    struct armour {
                        enum class texture {
                            base,
                            alternative,
                        };

                        enum class color {};
                    };
                };
                struct tileset {};
                struct prop {};
                struct sprite_sheet {};
            };
            struct _visual_effect {

            };
            struct shader {

            };
        };

        struct audio_asset {

        };

    };

};

//using _tex_type = _texture_types::assets::visual_asset::_2D_sprite::character; // only handle 2d sprites now because my game only has sprites.

class texture {

private:
    sf::Texture _tex;
    std::string _tex_path;
public:
    texture();

    bool load(const std::string& path);
    const sf::Texture& get_texture() const;
};