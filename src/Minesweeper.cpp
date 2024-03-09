#include "Minesweeper.h"
#include "Gamestate.h"

int Size = 16;
int bombsCount = 40;

AssetManager Assets = *new AssetManager("../resources");
sf::RenderWindow GameWindow;
Grid grid(Size);

int main() {
    GameWindow.create(sf::VideoMode(Size * 32, Size * 32), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);

    grid.generateBombs(bombsCount);
    grid.generateBombCountForEveryCell();


    while(GameWindow.isOpen()){
        sf::Event event{};
        while (GameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                GameWindow.close();
        }

        GameWindow.clear(sf::Color::White);

        for(auto & i : grid.grid){
            i.update();

            sf::Vector2f mouse = GameWindow.mapPixelToCoords(sf::Mouse::getPosition(GameWindow));

            sf::FloatRect bounds = i.sprite.getGlobalBounds();

            //Handling Input
            if(Gamestate::currentGamestate == Gamestate::PLAYING) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    if (bounds.contains(mouse)) {
                        if (i.currentState == Cell::State::BOMB) {
                            i.changeState(Cell::State::BOMB_FAIL);
                            Gamestate::setGameState(Gamestate::GAMEOVER);
                        }
                        grid.openCellAtX(i.pos.y / 32 * grid.size + i.pos.x / 32);
                    }
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                    if (bounds.contains(mouse)) {
                        i.setFlag(true);
                        if(grid.CheckWinCondition()){
                            Gamestate::setGameState(Gamestate::WIN);
                        }
                    }
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) {
                    if (bounds.contains(mouse)) {
                        i.setFlag(false);
                    }
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            grid.reset();
            Gamestate::setGameState(Gamestate::PLAYING);
        }

        GameWindow.display();
    }
}