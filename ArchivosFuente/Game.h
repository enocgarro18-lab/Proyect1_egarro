#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "UI.h"
#include "Animations.h"
#include "Level.h"

enum class GameState {
    Inicio,
    SeleccionNivel,
    Jugando,
    NivelCompletado,
    NivelFallado
};

class Game {
private:
    sf::RenderWindow window;
    Board board;
    UI ui;
    Animations animations;
    sf::Clock gameClock;
    sf::Clock animationClock;
    float deltaTime;
    bool procesandoCombinaciones;
    GameState estado;
    int selectedX, selectedY;

    // Sistem levels
    Level niveles[3];
    int nivelActual;
    int totalNiveles;
    int movimientosRestantes;

    
    void procesarCombinacionesDespuesDeAnimacion();
    void updateAnimations(float deltaTime);
    void reiniciarJuego();
    void cambiarEstado(GameState nuevoEstado);
    void agregarAnimacionesCaidaSelectivas(int posicionesOriginales[BOARD_SIZE][BOARD_SIZE]);
    int encontrarFilaOrigen(int filaDest, int col, int posicionesOriginales[BOARD_SIZE][BOARD_SIZE]);

    
    void cargarNiveles();
    void iniciarNivel(int numeroNivel);
    void verificarObjetivosNivel();
    void completarNivel();
    void fallarNivel();
    void procesarSeleccionNivel(const sf::Vector2f& worldPos);
    void dibujarSeleccionNivel(sf::RenderWindow& window);
    void actualizarUI();

public:
    Game();
    void run();
    void procesarEventos();
    void procesarClick(int col, int row);
    void actualizar();
    void dibujar();
};

#endif