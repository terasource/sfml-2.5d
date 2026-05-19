#include <SFML/Graphics.hpp>
#include "Game.hpp"
/*
TO-DO's
(i) Fix the missing probs, check the json to figure out why their path is not loaded into proplist (done)
(ii) Fix the camera with clamping in order to prevent to go beyond the out of map bounds (done)
(iii) Seperate the probs.cpp into 2 class which are probs and probsLoader. probs should be responsible from prob object itself and,
the prob loader should be responsible for json parsing and loading the data to probs class.
(iv) refactor dummy texture approach with the valid flag in the prob struct (!c) ?
*/
/*

the bug in tilemap appeared recently after implemented the dynamic animated tiles.
at the specific position of a character on map
(x axis is the issue based on my observations at the points {438.498,y})
some black vertical lines appears on map not especially on the dynamic animated tiles but almost everywhere.
-i think it based on sprites because everyline is not black but has the same color with the textures of tiles itselfs.
-but it is actually not the same as the tiles itselfs because as you see on vertical_line_bug_example_2.png and the other picture asswell some grass sprites
-near the river have blue lines.
-added aexample photos about the bug. examine it and try to figure out what causes it and determine how to solve it.

*/
int main() {
   Game game;

   game.run();

   return 0;
}