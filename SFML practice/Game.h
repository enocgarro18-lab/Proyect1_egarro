#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Game {
private:
    enum class GameState { Inicio, Jugando, Fin };
    GameState estado;

    sf::RenderWindow window;
    sf::Font font;

    // UI y textos
    sf::Text gameOverText;
    sf::Text finalScoreText;
    sf::Text playButtonText;
    sf::Text resetButtonText;
    sf::Text exitButtonText;
    sf::Text scoreText;
    sf::Text movimientosText;

    sf::RectangleShape playButton;
    sf::RectangleShape resetButton;
    sf::RectangleShape exitButton;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<sf::Texture> gemTextures;
    std::vector<sf::Sprite> gems;

    Board board;
    int movimientosRestantes;
    int selectedX;
    int selectedY;

public:
    Game();
    void run();

private:
    void procesarEventos();
    void procesarClick(int col, int row);
    void reiniciarJuego();
    void actualizarTexto();
    void actualizarSpritesDesdeBoard();
    void dibujar();
};

#endif
