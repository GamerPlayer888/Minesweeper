//
// Created by User on 26/02/2024.
//

#ifndef MINESWEEPER_GAMESTATE_H
#define MINESWEEPER_GAMESTATE_H


class Grid;

class Gamestate {
public:

    enum State{
        PLAYING,
        GAMEOVER,
        WIN
    };
    static void setGameState(State gamestate);
    static State currentGamestate;
};


#endif //MINESWEEPER_GAMESTATE_H
