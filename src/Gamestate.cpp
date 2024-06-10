//
// Created by User on 26/02/2024.
//

#include "Gamestate.h"
#include "Minesweeper.h"

Gamestate::State Gamestate::currentGamestate = Gamestate::PLAYING;

void Gamestate::setGameState(State gamestate, Grid* grid) {
    switch (gamestate) {
        case Gamestate::State::GAMEOVER:
            grid->showAllBomb();
            grid->checkForWrongFlags();
            break;
        case Gamestate::State::WIN:
            grid->winFlagChange();
            break;
        case Gamestate::State::PLAYING:
            break;
    }
    currentGamestate = gamestate;
}