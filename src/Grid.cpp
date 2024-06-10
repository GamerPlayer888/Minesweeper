#include "Grid.h"
#include "algorithm"
#include <random>

Grid::Grid(int size, AssetManager* assetManager) : size(size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            Cell c = Cell(sf::Vector2i(j * 32, i * 32), Cell::State::CLOSED, assetManager);
            grid.push_back(c);
        }
    }
}

void Grid::generateBombs(int x){
    bombs.clear();
    for (Cell *cell: getXCellRandomly(x)) {
        cell->changeState(Cell::State::BOMB);
        bombs.push_back(cell);
    }
}

void Grid::reset(){
    for (Cell& cell : grid) {
        cell.changeState(Cell::State::CLOSED);
        cell.close();
        cell.setFlag(false);
    }

    generateBombs(bombs.size());
    generateBombCountForEveryCell();

    visitedPos.clear();
}

void Grid::showAllBomb(){
    for(Cell *bomb : bombs){
        if(!bomb->flagged){
            bomb->open();
        }
    }
}

void Grid::checkForWrongFlags(){
    for(Cell &cell : grid){
        if(cell.flagged && cell.currentState != Cell::State::BOMB){
            cell.FlagFail();
        }
    }
}

bool Grid::CheckWinCondition() {
    int flaggedBombs = 0;
    int totalFlaggedCells = 0;

    for (const Cell& cell : grid) {
        if (cell.flagged) {
            totalFlaggedCells++;
            if (cell.currentState == Cell::State::BOMB) {
                flaggedBombs++;
            } else {
                return false;
            }
        }
    }

    return grid.size() - bombs.size() == visitedPos.size() && (flaggedBombs == bombs.size() && totalFlaggedCells == bombs.size());
}

void Grid::winFlagChange(){
    for(Cell* cell : bombs){
        cell->FlagWin();
    }
}

std::vector<Cell*> Grid::getNeighborgFromCell(int pos){
    std::vector<Cell*> res;

    if(pos - size >= 0) res.push_back(&grid[pos - size]);
    if(pos + size < grid.size()) res.push_back(&grid[pos + size]);
    if(pos % size != 0) res.push_back(&grid[pos - 1]); // check for left edge
    if(pos % size != size - 1) res.push_back(&grid[pos + 1]); // check for right edge
    if(pos - size + 1 >= 0 && pos % size != size - 1) res.push_back(&grid[pos - size + 1]);
    if(pos + size - 1 < grid.size() && pos % size != 0) res.push_back(&grid[pos + size - 1]);
    if(pos - size - 1 >= 0 && pos % size != 0) res.push_back(&grid[pos - size - 1]);
    if(pos + size + 1 < grid.size() && pos % size != size - 1) res.push_back(&grid[pos + size + 1]);

    return res;
}

void Grid::generateBombCountForEveryCell(){
    int bombCount = 6;
    for(int i = 0; i < grid.size(); i++){
        if(grid[i].currentState == Cell::State::CLOSED){
            for(Cell* cell: getNeighborgFromCell(i)){
                if(cell->currentState == Cell::State::BOMB){
                    bombCount++;
                }
            }
            grid[i].changeState(Cell::State(bombCount));
            bombCount = 6;
        }
    }
}

std::vector<Cell*> Grid::getXCellRandomly(int x) {
    std::vector<Cell*> res;

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

    std::vector<Cell*> neighbors = getNeighborgFromCell(x);

    for (Cell* neighbor : neighbors) {
        int pos = neighbor->pos.y/32 * size + neighbor->pos.x/32;

        if (visitedPos.find(pos) == visitedPos.end()) {
            openCellAtX(pos);
        }
    }
}