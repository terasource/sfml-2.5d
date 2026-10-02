#include "playerState.hpp"

playerState::playerState(unsigned int player_id, float pl_pos_x, float pl_pos_y){
    id = player_id;
    playerpos.x = pl_pos_x;
    playerpos.y = pl_pos_y;
}
void playerState::set_player_position(position pos){
    playerpos = pos;
}

position& playerState::get_player_position(){
    return playerpos;
}
