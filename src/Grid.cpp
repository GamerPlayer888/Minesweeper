#include "Grid.h"
#include "algorithm"
#include "Minesweeper.h"
#include "Gamestate.h"
#include <random>
#include <iostream>

Grid::Grid(sf::RenderWindow *window, int size, AssetManager *assetManager) : size(size),
                                                                             window(window) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Cell c = Cell(sf::Vector2i(j * 32, i * 32), Cell::State::CLOSED, assetManager);
            grid.push_back(c);
        }
    }
}

void Grid::handleEvent(sf::Event event) {
    /*
     * Toggle variable for flags
     */
    bool tick = true;
    bool click = false;
    bool toggle = false;

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Right) {
        click = true;
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right) {
        click = false;
    }

    if (click && tick) {
        tick = false;
        toggle = true;
    } else if (!click) {
        tick = true;
    }

    /*
     * Input and Event handling for interacting with the grid
     */
    sf::Vector2f window_relative_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window), window->getView());

    if (window_relative_pos.x >= 0 && window_relative_pos.x < size * 32 && window_relative_pos.y >= 0 &&
        window_relative_pos.y < size * 32) {

        int xCoord = int(window_relative_pos.x / 32);
        int yCoord = int(window_relative_pos.y / 32);

        Cell *cell_hover = &grid[yCoord * size + xCoord];

        sf::FloatRect bounds = cell_hover->sprite.getGlobalBounds();

        // Handling Input
        if (Gamestate::currentGamestate == Gamestate::PLAYING) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !cell_hover->flagged) {
                window->setTitle("Minesweeper | Playing | " + std::to_string(bombs.size()) + " bombs.");
                if (bounds.contains(window_relative_pos)) {
                    if (cell_hover->currentState == Cell::State::BOMB) {
                        cell_hover->changeState(Cell::State::BOMB_FAIL);
                        Gamestate::setGameState(Gamestate::GAMEOVER);
                        showAllBomb();
                        checkForWrongFlags();

                        window->setTitle("Minesweeper | GameOver | You Lost");
                    }

                    openCellAtX(yCoord * size + xCoord);

                    if (CheckWinCondition()) {
                        Gamestate::setGameState(Gamestate::WIN);
                        winFlagChange();

                        window->setTitle(
                                "Minesweeper | GameOver | You Won.");
                    }
                }
            }

            // Actual flag toggle logic
            if (toggle) {
                if (bounds.contains(window_relative_pos)) {
                    cell_hover->setFlag(!cell_hover->flagged);

                    if (!cell_hover->isOpen) {
                        if (cell_hover->flagged) {
                            flagsCount += -1;
                        } else {
                            flagsCount += +1;
                        }
                    }

                    window->setTitle(
                            "Minesweeper | Playing | " + std::to_string(flagsCount) + " flags remaining.");

                    if (CheckWinCondition()) {
                        Gamestate::setGameState(Gamestate::WIN);
                        winFlagChange();

                        window->setTitle(
                                "Minesweeper | GameOver | You Won.");
                    }
                }
            }
        }
    }
}

void Grid::generateBombs(int x) {
    bombs.clear();
    for (Cell *cell: getXCellRandomly(x)) {
        cell->changeState(Cell::State::BOMB);
        bombs.push_back(cell);
    }

    flagsCount = bombs.size();
}

void Grid::reset() {
    for (Cell &cell: grid) {
        cell.changeState(Cell::State::CLOSED);
        cell.close();
        cell.setFlag(false);
    }

    generateBombs(bombs.size());
    generateBombCountForEveryCell();

    visitedPos.clear();
}

void Grid::showAllBomb() {
    for (Cell *bomb: bombs) {
        if (!bomb->flagged) {
            bomb->open();
        }
    }
}

void Grid::checkForWrongFlags() {
    for (Cell &cell: grid) {
        if (cell.flagged && cell.currentState != Cell::State::BOMB) {
            cell.FlagFail();
        }
    }
}

bool Grid::CheckWinCondition() {
    int flaggedBombs = 0;
    int totalFlaggedCells = 0;

    for (const Cell &cell: grid) {
        if (cell.flagged) {
            totalFlaggedCells++;
            if (cell.currentState == Cell::State::BOMB) {
                flaggedBombs++;
            } else {
                return false;
            }
        }
    }

    return grid.size() - bombs.size() == visitedPos.size() &&
           (flaggedBombs == bombs.size() && totalFlaggedCells == bombs.size());
}

void Grid::winFlagChange() {
    for (Cell *cell: bombs) {
        cell->FlagWin();
    }
}

std::vector<Cell *> Grid::getNeighborgFromCell(int pos) {
    std::vector<Cell *> res;

    if (pos - size >= 0) res.push_back(&grid[pos - size]);
    if (pos + size < grid.size()) res.push_back(&grid[pos + size]);
    if (pos % size != 0) res.push_back(&grid[pos - 1]); // check for left edge
    if (pos % size != size - 1) res.push_back(&grid[pos + 1]); // check for right edge
    if (pos - size + 1 >= 0 && pos % size != size - 1) res.push_back(&grid[pos - size + 1]);
    if (pos + size - 1 < grid.size() && pos % size != 0) res.push_back(&grid[pos + size - 1]);
    if (pos - size - 1 >= 0 && pos % size != 0) res.push_back(&grid[pos - size - 1]);
    if (pos + size + 1 < grid.size() && pos % size != size - 1) res.push_back(&grid[pos + size + 1]);

    return res;
}

void Grid::generateBombCountForEveryCell() {
    int bombCount = 6;
    for (int i = 0; i < grid.size(); i++) {
        if (grid[i].currentState == Cell::State::CLOSED) {
            for (Cell *cell: getNeighborgFromCell(i)) {
                if (cell->currentState == Cell::State::BOMB) {
                    bombCount++;
                }
            }
            grid[i].changeState(Cell::State(bombCount));
            bombCount = 6;
        }
    }
}

std::vector<Cell *> Grid::getXCellRandomly(int x) {
    std::vector<Cell *> res;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, grid.size() - 1);

    std::unordered_set<int> selectedPositions;

    while (res.size() < x) {
        int pos = dist(gen);
        if (selectedPositions.find(pos) == selectedPositions.end()) {
            res.push_back(&grid[pos]);
            selectedPositions.insert(pos);
        }
    }

    return res;
}

void Grid::openCellAtX(int x) {
    if (visitedPos.find(x) != visitedPos.end()) {
        return;
    }

    visitedPos.insert(x);

    if (grid[x].currentState != Cell::State::EMPTY) {
        grid[x].open();
        return;
    }

    if (grid[x].currentState == Cell::State::BOMB) {
        return;
    }

    grid[x].open();

    std::vector<Cell *> neighbors = getNeighborgFromCell(x);

    for (Cell *neighbor: neighbors) {
        int pos = neighbor->pos.y / 32 * size + neighbor->pos.x / 32;

        if (visitedPos.find(pos) == visitedPos.end()) {
            openCellAtX(pos);
        }
    }
}
