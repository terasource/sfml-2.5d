#include <SFML/Graphics.hpp>
#include "Game.hpp"
/*
TO-DO's
(i) Fix the missing probs, check the json to figure out why their path is not loaded into proplist
(ii) Fix the camera with clamping in order to prevent to go beyond the out of map bounds
(iii) Seperate the probs.cpp into 2 class which are probs and probsLoader. probs should be responsible from prob object itself and,
the prob loader should be responsible for json parsing and loading the data to probs class.
(iv) refactor dummy texture approach with the valid flag in the prob struct (!c)
*/
int main() {
   Game game;

   game.run();

   return 0;
}