#include "SFML/Graphics.hpp"
#include "texturemanager.hpp"
#include <iostream> 
#include <string> //lazy loading - load the memory when it is only requested for first time and create a counter for its users.  //               if any sprite uses the texture, delete it. can done with smart pointers probably.  
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

void TextureManager::move_all_sprites(sf::Vector2f movement){

    //idea was from me but the implementation was from chatgpt cause i could not figure how to do this out.
    std::apply([&](auto&&... args) {

            auto move_vec = [&](auto& vec){
                for(auto& tex : vec)
                    tex._spr.move(movement);
            };

            (move_vec(args), ...);

        }, _textureslist);

    /*
        std::apply([&](auto&... args){
        
        ([&](auto& vec){
             for(auto& tex : vec)
                tex._spr.move(movement);
            }(args), ...);

        }, _textureslist);

        //tried to write it by scratch with myself understanding what is going on here and how to use all these parameter packing and folding mechanims.
        i just needed to research how to pass parameter into lambda function just after its definition from stackoverflow but it was simple. the lambda object
        returns a function so i just need to do [](){body; } () like lamda_func(parameter); 
        but im going to use the version placed above because it is more clear and readable.
    */
    
}

void TextureManager::setposition_all_sprites(sf::Vector2f position){

    std::apply([&](auto&&... args){
        auto set_pos = [&](auto& vec){
            for(auto& tex : vec)
                tex._spr.setPosition(position);
        };

        (set_pos(args), ...);

    }, _textureslist);
   
}
