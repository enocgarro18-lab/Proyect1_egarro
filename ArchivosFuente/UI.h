#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>

class UI {
private:
    sf::Font font;
    sf::Text scoreText;
    sf::Text movimientosText;
    sf::Text gameOverText;
    sf::Text finalScoreText;
    sf::Text playButtonText;
    sf::Text resetButtonText;
    sf::Text exitButtonText;
    sf::Text nivel1Text;
    sf::Text nivel2Text;
    sf::Text nivel3Text;
    sf::Text tituloNivelesText;

    sf::RectangleShape playButton, resetButton, exitButton;
    sf::RectangleShape nivel1Button, nivel2Button, nivel3Button;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    bool cargarFuente();
    bool cargarTexturas();
    void inicializarBotones();
    void inicializarTextos();
    void inicializarBotonesNiveles();

public:
    UI();

    void setScore(int score);
    void setMovimientos(int movimientos);
    void actualizarTexto(int score, int movimientos);

    bool isPlayButtonClicked(const sf::Vector2f& pos) const;
    bool isResetButtonClicked(const sf::Vector2f& pos) const;
    bool isExitButtonClicked(const sf::Vector2f& pos) const;

    bool isNivel1ButtonClicked(const sf::Vector2f& pos) const;
    bool isNivel2ButtonClicked(const sf::Vector2f& pos) const;
    bool isNivel3ButtonClicked(const sf::Vector2f& pos) const;

    void drawInicio(sf::RenderWindow& window) const;
    void drawJugando(sf::RenderWindow& window) const;
    void drawFin(sf::RenderWindow& window, int finalScore);

    void drawSeleccionNivel(sf::RenderWindow& window) const;

    void drawObjetivosNivel(sf::RenderWindow& window, int nivel,
        int objetivoPuntaje, int objetivoHielo,
        const int* objetivoGemas, const bool* tieneObjetivoGemas,
        int puntajeActual, int hieloDestruido,
        const int* gemasRecolectadas) const;
};

#endif