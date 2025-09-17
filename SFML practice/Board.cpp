#include "Board.h"
#include <iostream>
#include <cstdlib>

Board::Board() {
    gems = new Gem * [BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        gems[i] = new Gem[BOARD_SIZE];
    }
    gemTextures = new sf::Texture[NUM_GEMS];

    cargarTexturas();
    generarGemas();
    score = 0;
}

Board::~Board() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        delete[] gems[i];
    }
    delete[] gems;
    delete[] gemTextures;
}

void Board::setScore(int _score) { score = _score; }
int Board::getScore() const { return score; }

int Board::getGemType(int row, int col) const {
    return gems[row][col].getType();
}

void Board::cargarTexturas() {
    for (int i = 0; i < NUM_GEMS; ++i) {
        std::string baseName = "assets/gem" + std::to_string(i);
        bool cargada = false;
        if (gemTextures[i].loadFromFile(baseName + ".png")) {
            std::cout << "gem" << i << ".png cargada\n";
            cargada = true;
        }
        else if (gemTextures[i].loadFromFile(baseName + ".jpg")) {
            std::cout << "gem" << i << ".jpg cargada\n";
            cargada = true;
        }
        if (!cargada) {
            std::cerr << "Error cargando gem" << i << " (.png ni .jpg)\n";
        }
    }
}

void Board::generarGemas() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int nuevoTipo = std::rand() % NUM_GEMS;
            gems[i][j].setType(nuevoTipo, gemTextures, NUM_GEMS);
            gems[i][j].setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));
        }
    }
}

bool Board::intercambiar(int x1, int y1, int x2, int y2) {
    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 ||
        x1 >= BOARD_SIZE || y1 >= BOARD_SIZE ||
        x2 >= BOARD_SIZE || y2 >= BOARD_SIZE)
        return false;

    std::swap(gems[y1][x1], gems[y2][x2]);

    gems[y1][x1].setPosition({ float(x1 * CELL_SIZE), float(y1 * CELL_SIZE) });
    gems[y2][x2].setPosition({ float(x2 * CELL_SIZE), float(y2 * CELL_SIZE) });

    int puntos = eliminarCombinaciones();
    if (puntos == 0) {
        std::swap(gems[y1][x1], gems[y2][x2]);
        gems[y1][x1].setPosition({ float(x1 * CELL_SIZE), float(y1 * CELL_SIZE) });
        gems[y2][x2].setPosition({ float(x2 * CELL_SIZE), float(y2 * CELL_SIZE) });
        return false;
    }
    else {
        score += puntos;
        aplicarGravedad();
        return true;
    }
}

void Board::resetScore() { score = 0; }

int Board::eliminarCombinaciones() {
    int puntos = 0;

    // Horizontal
    for (int i = 0; i < BOARD_SIZE; i++) {
        int cont = 1;
        int pos = 0;
        for (int j = 0; j < BOARD_SIZE - 1; j++) {
            if (gems[i][j].getType() == gems[i][j + 1].getType() && gems[i][j].getType() != -1) {
                cont++;
                pos = j + 1;
            }
            else {
                if (cont >= 3) {
                    for (int k = pos; k >= pos - cont + 1; k--) {
                        gems[i][k].setType(-1, gemTextures, NUM_GEMS);
                    }
                    puntos += 10;
                }
                cont = 1;
            }
        }
        if (cont >= 3) {
            for (int k = pos; k >= pos - cont + 1; k--) {
                gems[i][k].setType(-1, gemTextures, NUM_GEMS);
            }
            puntos += 10;
        }
    }

    // Vertical
    for (int j = 0; j < BOARD_SIZE; j++) {
        int cont = 1;
        int pos = 0;
        for (int i = 0; i < BOARD_SIZE - 1; i++) {
            if (gems[i][j].getType() == gems[i + 1][j].getType() && gems[i][j].getType() != -1) {
                cont++;
                pos = i + 1;
            }
            else {
                if (cont >= 3) {
                    for (int k = pos; k >= pos - cont + 1; k--) {
                        gems[k][j].setType(-1, gemTextures, NUM_GEMS);
                    }
                    puntos += 10;
                }
                cont = 1;
            }
        }
        if (cont >= 3) {
            for (int k = pos; k >= pos - cont + 1; k--) {
                gems[k][j].setType(-1, gemTextures, NUM_GEMS);
            }
            puntos += 10;
        }
    }

    return puntos;
}

void Board::aplicarGravedad() {
    std::cout << "[Board] aplicarGravedad: aplicando gravedad a las gemas\n";
    for (int j = 0; j < BOARD_SIZE; j++) {
        int emptyRow = BOARD_SIZE - 1;
        for (int i = BOARD_SIZE - 1; i >= 0; i--) {
            if (gems[i][j].getType() != -1) {
                if (emptyRow != i) {
                    int tipo = gems[i][j].getType();
                    gems[emptyRow][j].setType(tipo, gemTextures, NUM_GEMS);
                    gems[emptyRow][j].setPosition(sf::Vector2f(j * CELL_SIZE, emptyRow * CELL_SIZE));
                    gems[i][j].setType(-1, gemTextures, NUM_GEMS);
                }
                emptyRow--;
            }
        }

        for (int i = emptyRow; i >= 0; i--) {
            int nuevoTipo;
            bool valido;
            do {
                valido = true;
                nuevoTipo = std::rand() % NUM_GEMS;
                if (j >= 2 &&
                    gems[i][j - 1].getType() == nuevoTipo &&
                    gems[i][j - 2].getType() == nuevoTipo)
                    valido = false;
                if (i <= BOARD_SIZE - 3 &&
                    gems[i + 1][j].getType() == nuevoTipo &&
                    gems[i + 2][j].getType() == nuevoTipo)
                    valido = false;
            } while (!valido);
            gems[i][j].setType(nuevoTipo, gemTextures, NUM_GEMS);
            gems[i][j].setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));
        }
    }
    std::cout << "[Board] aplicarGravedad: terminado\n";
}

void Board::dibujarTablero(sf::RenderWindow& window, int selectedX, int selectedY) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            gems[row][col].draw(window);
            if (col == selectedX && row == selectedY) {
                // extra: dibujar borde
            }
        }
    }
}
