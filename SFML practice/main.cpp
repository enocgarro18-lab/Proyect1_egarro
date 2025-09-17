#include "Game.h"
#include "Gem.h"
#include <iostream>

int main() {
    if (!Gem::emptyTexture.loadFromFile("assets/empty.png")) {
        std::cerr << "No se pudo cargar empty.png\n";
        return -1;
    }

    Game juego;
    juego.run();
    return 0;
}
