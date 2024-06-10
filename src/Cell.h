#ifndef MINESWEEPER_CELL_H
#define MINESWEEPER_CELL_H

#include "Cell.h"
#include "AssetManager.h"
#include "SFML/Graphics.hpp"

class Cell {
public:
    bool operator==(const Cell& other) const {
        return this->pos == other.pos;
    }
    AssetManager* assetManager;
    sf::Vector2f pos;
    enum State{
        BOMB,
        BOMB_FAIL,
        FLAG_FAIL,
        FLAG_WIN,
        FLAG,
        CLOSED,
        EMPTY,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT
    };
    Cell(sf::Vector2i vector2, State state, AssetManager* assetManager);
    void changeState(State state);
    void open();
    static std::string stateToString(State state);
    Cell::State currentState;
    sf::Sprite sprite;
    bool flagged;
    void setFlag(bool value);
    void close();
    void FlagFail();
    void FlagWin();
private:
    bool isOpen{};
};

#endif //MINESWEEPER_CELL_H
