#include <iostream>
#include "Minesweeper.h"
#include "Gamestate.h"

int grid_size = 50;
int bombs_count = 500;

int main() {
    const auto videoMode = sf::VideoMode(512, 512);
    sf::RenderWindow GameWindow;
    GameWindow.create(videoMode, "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    GameWindow.setFramerateLimit(30);
    auto window_view = GameWindow.getDefaultView();

    AssetManager Assets = *new AssetManager("./resources");

    auto grid = Grid(&GameWindow, grid_size, &Assets);
    grid.generateBombs(bombs_count);
    grid.generateBombCountForEveryCell();

    int center = (grid_size / 2) * 32;

    window_view.move(sf::Vector2f(center, center));

    while (GameWindow.isOpen()) {
        sf::Event event{};
        while (GameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                GameWindow.close();
            grid.handleEvent(event);
        }

        GameWindow.clear(sf::Color::White);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            window_view.move(sf::Vector2f(32, 0));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            window_view.move(sf::Vector2f(0, -32));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            window_view.move(sf::Vector2f(0, 32));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            window_view.move(sf::Vector2f(-32, 0));
        }

        GameWindow.setView(window_view);

        for (auto i: grid.grid) {
            GameWindow.draw(i.sprite);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            Gamestate::setGameState(Gamestate::State::PLAYING);

            grid.reset();
        }

        GameWindow.display();
    }
}