#ifndef BOARD_H
#define BOARD_H

#include "Gem.h"

const int BOARD_SIZE = 8;
const int NUM_GEMS = 5;

class Board {
private:
    Gem** gems;
    sf::Texture* gemTextures;
    int score;

public:
    Board();
    ~Board();

    void setScore(int _score);
    int getScore() const;
    int getGemType(int row, int col) const;

    void cargarTexturas();
    void generarGemas();
    bool intercambiar(int x1, int y1, int x2, int y2);
    void resetScore();
    int eliminarCombinaciones();
    void aplicarGravedad();
    void dibujarTablero(sf::RenderWindow& window, int selectedX, int selectedY);
};

#endif

