#include "SFML/Graphics.hpp"
#include "texturemanager.hpp"
#include <iostream>
#include <string>
//lazy loading - load the memory when it is only requested for first time and create a counter for its users.
//               if any sprite uses the texture, delete it. can done with smart pointers probably.

TextureManager::TextureManager() {
    initialize_textures_sprites();
    static int debug = 0;
}

/*
std::shared_ptr<texture<_tex_type>> TextureManager::get_texture(){
    return _tex_ref;
};
*/

void TextureManager::initialize_textures_sprites(){

    load_texture<texture<_tex_type::body>>("assets/characters/char_a_p1_0bas_humn_v00.png");

    load_texture<texture<_tex_type::hair>>("assets/characters/char_a_p1_4har_bob1_v03.png");

    load_texture<texture<_tex_type::armour>>("assets/characters/char_a_p1_1out_pfpn_v05.png");

}