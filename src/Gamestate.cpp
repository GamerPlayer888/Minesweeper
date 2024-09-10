//
// Created by User on 26/02/2024.
//

#include "Gamestate.h"
#include "Minesweeper.h"

Gamestate::State Gamestate::currentGamestate = Gamestate::PLAYING;

void Gamestate::setGameState(State gamestate) {
    switch (gamestate) {
        case Gamestate::State::GAMEOVER:
            break;
        case Gamestate::State::WIN:
            break;
        case Gamestate::State::PLAYING:
            break;
    }
    currentGamestate = gamestate;
}