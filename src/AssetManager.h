#pragma once
#ifndef MINESWEEPER_ASSETMANAGER_H
#define MINESWEEPER_ASSETMANAGER_H

#include "map"
#include "SFML/Graphics.hpp"

 class AssetManager {
public:
     AssetManager();
     explicit AssetManager(const std::string& path);

    sf::Texture &getTexture(const std::string& name);
private:
     void loadTexture(const std::string& name, const std::string& fileName);
    std::map<std::string, sf::Texture> _Textures;
};


#endif
