#include <iostream>
#include "Minesweeper.h"
#include "Gamestate.h"

int Size = 16;
int bombsCount = 4;

int main() {
    const auto videoMode = sf::VideoMode(Size * 32, Size * 32);
    sf::RenderWindow GameWindow;
    GameWindow.create(videoMode, "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    AssetManager Assets = *new AssetManager("../resources");
    auto grid = Grid(Size, &Assets);
    grid.generateBombs(bombsCount);
    grid.generateBombCountForEveryCell();

    bool tick = true;
    bool click = false;

    while(GameWindow.isOpen()){
        sf::Event event{};
        while (GameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                GameWindow.close();
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Right){
                click = true;
                std::cout<<"Ciao in!";
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right){
                click = false;
                std::cout<<"Ciao out!";
            }
        }

        GameWindow.clear(sf::Color::White);

        bool toggle = false;
        if (click && tick)
        {
            tick = false;
            toggle = true;
        } else if (!click)
        {
            tick = true;
        }

        for(auto i : grid.grid){
            GameWindow.draw(i.sprite);
        }

        sf::Vector2f mouse = GameWindow.mapPixelToCoords(sf::Mouse::getPosition(GameWindow));

        if(!(mouse.x < 0 or mouse.x > GameWindow.getSize().x or mouse.y < 0 or mouse.y > GameWindow.getSize().y)){
            int xCoord = (mouse.x * grid.size) / GameWindow.getSize().x;
            int yCoord = (mouse.y * grid.size) / GameWindow.getSize().y;

            Cell* cell_hover = &grid.grid[yCoord * grid.size + xCoord];

            sf::FloatRect bounds = cell_hover->sprite.getGlobalBounds();

            //Handling Input
            if(Gamestate::currentGamestate == Gamestate::PLAYING) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    std::cout<<"Grid coordinates: ("<<xCoord<<", "<<yCoord<<")"<< std::endl;
                    std::cout<<"Grid 1D array: "<<yCoord * grid.size + xCoord<<"; ";
                    if (bounds.contains(mouse)) {
                        std::cout<<"State: "<<Cell::stateToString(cell_hover->currentState)<<" ";
                        if (cell_hover->currentState == Cell::State::BOMB) {
                            cell_hover->changeState(Cell::State::BOMB_FAIL);
                            Gamestate::setGameState(Gamestate::GAMEOVER, &grid);
                        }
                        grid.openCellAtX(cell_hover->pos.y / 32 * grid.size + cell_hover->pos.x / 32);
                        if(grid.CheckWinCondition()){
                            Gamestate::setGameState(Gamestate::WIN, &grid);
                        }
                    }
                }

                if (toggle) {
                    if (bounds.contains(mouse)) {
                        cell_hover->setFlag(true);
                        if(grid.CheckWinCondition()){
                            Gamestate::setGameState(Gamestate::WIN, &grid);
                        }
                    }
                }
            }
        }



        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            grid.reset();
            Gamestate::setGameState(Gamestate::PLAYING, &grid);
        }

        GameWindow.display();
    }
}