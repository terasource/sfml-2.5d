#pragma once
#include "texture.hpp"
#include <string>
#include <any>


struct _texture_types{
    struct assets{
        struct visual_asset{
            struct _material{

            };
            struct _2D_sprite{
                struct character{ 
                    struct hair{
                        enum class color {};
                    };
                    struct body{
                        enum class color {};
                    };
                    struct armour{
                        enum class color{};  
                    };
                };
                struct tileset {};
                struct prop{};
                struct sprite_sheet{};
            };
            struct _visual_effect{

            };
            struct shader{

            };
        };

        struct audio_asset{

        };

    };

};

using _tex_type = _texture_types::assets::visual_asset::_2D_sprite::character; // only handle 2d sprites now because my game only has sprites.
//this does not seperate the objects by their type but seperates the objects by their runtime value.
//so i think this is a wrong implementation choice. they need to be seperated by their type not runtime values.
//actually the wrong part is the structure.

class TextureManager{
    private:

    using _textures = std::tuple <
    std::vector<texture<_tex_type::body>>, 
    std::vector<texture<_tex_type::hair>>, 
    std::vector<texture<_tex_type::armour>>
                        >;

    _textures _textureslist;

    public:    
    TextureManager();
    void initialize_textures_sprites();


    void change_texture(texture<_tex_type> texture, sf::Texture& tex) noexcept;
    //add a function to set texturerect of a sprite for animations.
    void set_texture_rect(texture<_tex_type> texture ,sf::IntRect rect);

    //add wrapper methods for setscale setposition and etc methods that are used in game.cpp
    //or it can be doned as the calling the _spr with texturemanager->texture->_spr->methods(mentioned) i guess but im nor sure and,
    //this is a probably wrong method. there is no meaning in writing a custom wrapper texturemanager if im still gonna reach the sprites from objects.

    void set_scale(texture<_tex_type> texture, sf::Vector2f vec);
    void set_position(texture<_tex_type> texture, sf::Vector2f vec);
    // sf::Sprite get_texture_sprite();

    // actually i think it would be better if i handle the object of this class seperately like hair_texture object.
    // it should own its own texture sprites and methods and etc.
    
    template <typename T>
    void load_texture(const std::string& path){

     //should check if there is a texture with given type first.

        auto _tex_ref = T{};
        auto& _tex_vector = std::get<std::vector<T>>(_textureslist); // chosing the vector of a spesified type of texture<t> in order to insert correct texture into correct vector.


        if(!_tex_ref._tex.loadFromFile(path))
            std::cout << "texture could not be loaded!" << std::endl;
        else
            _tex_ref._tex_path = path;

        
        _tex_vector.push_back(std::move(_tex_ref)); 

        // these type conversations are fucked i guess. idk i just need it to be more explicit i guess.
        // should avoid use type cast always ig.
    };

    template <typename T>
    texture<T>& get_texture(){

        return std::get<std::vector<texture<T>>>(_textureslist).at(0);

        //this returns texture<_tex_type> but im not sure whether this should return sf::texture or texture<t> consider it later.
    };

    //std::shared_ptr<texture<_tex_type>> get_texture(); // there is no way this function works correctly. my idea was to return of the texture of a sprite that calls the function but i could not figure out how can i do that. maybe i wll look at if again later.

    /*
    template <typename T>
    void list_textures(T type){
    for(auto _pair : _texturelist){ 
        std::cout << "{" << typeid(_pair.first).name() 
                  << " ," << _pair.second._tex_path << "}" << std::endl;
        }
    };

        not in use for a while...
    */
   
    template <typename T>
    sf::Sprite& get_sprite(T& type){
        return std::get<std::vector<T>>(_textureslist)[0]._spr;
    };

    template <typename T>
    void assign_sprite_texture(texture<T>& texture){
        texture._spr.setTexture(texture._tex, true); // modify this wrapper in the future for giving user the choice of resetrect. u just need to add one more boolean parameter to the function signature.
    };

    template <typename T>
    void set_texture_rect(texture<T>& texture, sf::IntRect rect){
        texture._spr.setTextureRect(rect);
    };

    template <typename T>
    void change_texture(texture<T>& texture, sf::Texture& new_tex) noexcept{ 
        texture._tex.swap(new_tex);
    };

    template <typename T>   
    void set_scale(texture<T>& texture, sf::Vector2f vec){
        texture._spr.setScale(vec);
    };

    template <typename T>
    void set_position(texture<T>& texture ,sf::Vector2f vec){
        texture._spr.setPosition(vec);
    };

};  