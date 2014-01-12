Hex: Implement of a 11 * 11 hex game 
Author: Zhao Yang (zhaoyang8778@gmail.com)

== Description ==

The code under this directory implements a 11 * 11 hex game which can be played 
between human and a very strong AI.

Hex is a strategy board game played on a hexagonal grid, theoretically of any size
and several possible shapes, but traditionally as an 11×11 rhombus.

Each player has an allocated color, Red and Blue. Players take turns placing a 
stone of their color on a single cell within the overall playing board. The goal 
for each player is to form a connected path of their own stones linking the oppo-
-sing sides of the board marked by their colors, before their opponent connects 
his or her sides in a similar fashion. The first player to complete his or her 
connection wins the game. 

e.g:Computer win the game by forming a complete path between north and south firstly.

****************************************************
* Computer *  O  * connects North-South, moves 1st *
*    Human *  X  * connects West-East              *
****************************************************

  A  B  C  D  E  F  G  H  I  J  K 

1  o  o  x  x  x  o  x  x  o  x  o  1
                                /
 2  x  x  o  .  x  .  o  x  .  o  x  2
                              /
  3  o  .  x  x  .  o  x  x  o  o  o  3
                            /
   4  o  .  o  o  o  o  o  o  x  o  o  4
                 /
    5  o  o  .  o  x  x  .  x  x  x  x  5
               / 
     6  x  .  o  x  o  x  x  .  o  x  x  6
              \
      7  o  x  o  o  .  o  x  .  o  x  .  7
                  \
       8  .  x  o  o  o  x  o  x  x  o  x  8
                      \
        9  x  x  .  x  o  o  o  x  o  o  x  9
                       \
         10 o  .  o  x  o  o  .  x  .  o  x  10
                       /
          11 o  x  x  o  x  x  .  o  x  o  o  11

              A  B  C  D  E  F  G  H  I  J  K 

== Runnig info ==

Usage: <command> easy|normal|hard
easy|normal|hard choose the degree of game difficulty

== version == 
 
1.0.0-alpha1

