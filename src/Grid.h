#ifndef MINESWEEPER_GRID_H
#define MINESWEEPER_GRID_H

#include "Cell.h"
#include "vector"
#include "unordered_set"

class Grid {
public:
    explicit Grid(int size);
    std::vector<Cell> grid;
    int size;
    std::vector<Cell *> getXCellRandomly(int x);

    void generateBombs(int x);

    std::vector<Cell *> getNeighborgFromCell(int pos);

    void generateBombCountForEveryCell();

    void openCellAtX(int x);
    void reset();
    void showAllBomb();
    void checkForWrongFlags();
    bool CheckWinCondition();
    void winFlagChange();
private:
    std::unordered_set<int> visitedPos;
    std::vector<Cell*> bombs;

};


#endif //MINESWEEPER_GRID_H
