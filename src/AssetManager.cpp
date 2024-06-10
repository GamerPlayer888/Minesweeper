#include "AssetManager.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;


std::string extractFileName(const std::string& filePath) {
    size_t lastSlashPos = filePath.find_last_of("/\\");
    std::string fileName;
    if (lastSlashPos == std::string::npos) {
        fileName = filePath;
    } else {
        fileName = filePath.substr(lastSlashPos + 1);
    }
    size_t lastDotPos = fileName.find_last_of('.');
    if (lastDotPos != std::string::npos) {
        fileName = fileName.substr(0, lastDotPos);
    }

    return fileName;
}

AssetManager::AssetManager() = default;

AssetManager::AssetManager(const std::string& path) {
    for (const auto & entry : fs::directory_iterator(path))
    {
        loadTexture(extractFileName(entry.path().string()), entry.path().string());
    }
}

void AssetManager::loadTexture(const std::string& name, const std::string& fileName){
    auto texture = sf::Texture();
    if( texture.loadFromFile(fileName)){
        this->_Textures[name] = texture;
    } else {
        std::cerr << "Impossibile caricare la texture da " << fileName << std::endl;
    }
    std::cout << fileName << " loaded as " << name << std::endl;
}

sf::Texture &AssetManager::getTexture(const std::string& name) {
    if (this->_Textures.find(name) != this->_Textures.end()) {
        return this->_Textures.at(name);
    } else {
        std::cerr << "Texture '" << name << "' non trovata!" << std::endl;
        return this->_Textures["default"];
    }
}