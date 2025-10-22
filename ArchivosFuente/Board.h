#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include "Gem.h"
#include "NormalGem.h"
#include "BombGem.h"
#include "Obstacle.h"
#include "Animations.h"

const int BOARD_SIZE = 8;
const int MAX_OBSTACLES = 20;
const int NUM_GEMS = 6; // 5 nomales y la bomba

class Board {
private:
    Gem*** gems;
    Obstacle** obstacles;
    sf::Texture** gemTextures;
    sf::Texture** obstacleTextures;
    int score;
    int obstacleCount;
    int maxObstacles;

    // Ice
    Obstacle** iceObstacles;
    int iceCount;
    int maxIce;

    // Accountants
    int iceDestroyed;
    int gemsCollected[5];

    // Level
    int nivelActual;
    int movimientosRestantes;
    bool nivelCompletado;
    bool nivelFallido;

    // Current goals
    int objetivoPuntaje;
    int objetivoGemas[5];
    int objetivoHielo;
    bool tieneObjetivoGemas[5]; 

    //Factory
    Gem* createGem(int type);

    bool esMovimientoValido(int x1, int y1, int x2, int y2) const;
    bool verificarCombinacionEnPosicion(int row, int col) const;

    void eliminarAreaBomba(int centerRow, int centerCol);
    void eliminarLineaHorizontal(int row);
    void eliminarLineaVertical(int col);
    void eliminarPorColor(int colorType);

public:
    Board();
    ~Board();

    bool intercambiarSoloVisual(int x1, int y1, int x2, int y2);

    bool cargarTexturas();
    void agregarObstaculo(ObstacleType type, int row, int col);
    void dibujarObstaculos(sf::RenderWindow& window) const;
    void generarGemas();
    void reset();
    bool hayCombinaciones();
    int eliminarCombinaciones();
    void aplicarGravedad();
    bool intercambiar(int x1, int y1, int x2, int y2);
    bool intercambiar(int x1, int y1, int x2, int y2, bool actualizarPosiciones);
    bool esIntercambioValido(int x1, int y1, int x2, int y2) const;
    int getGemType(int row, int col) const;
    int getScore() const { return score; }
    void setScore(int newScore) { score = newScore; }

    // Objetives
    int getIceDestroyed() const { return iceDestroyed; }
    int getGemsCollected(int gemType) const;
    void resetCounters();

    // Ice
    void destruirHieloAdyacente(int row, int col);
    void generarHieloAleatorio(int cantidad);
    bool hayHieloEnPosicion(int row, int col) const;
    void destruirHieloEnPosicion(int row, int col);
    void dibujarHielo(sf::RenderWindow& window) const;
    void verificarCombinacionesCercaDeHielo(int row, int col);

    // Animations
    void obtenerSprites(sf::Sprite* output[], int& count) const;
    Gem* getGemAt(int row, int col);

    // GemPowerful
    void generarGemaPoderosa(int row, int col);
    void activarGemaPoderosa(int row, int col);
    bool esGemaPoderosa(int row, int col) const;
    void activarBombasAdyacentes(int row, int col);

    // Levels
    void cargarNivel(int numeroNivel);
    void usarMovimiento();
    bool verificarVictoria();
    bool verificarDerrota();

    // Getters for interface
    int getNivelActual() const { return nivelActual; }
    int getMovimientosRestantes() const { return movimientosRestantes; }
    bool isNivelCompletado() const { return nivelCompletado; }
    bool isNivelFallido() const { return nivelFallido; }

    // Getters 
    int getObjetivoPuntaje() const { return objetivoPuntaje; }
    int getObjetivoGemas(int tipo) const {
        return (tipo >= 0 && tipo < 5) ? objetivoGemas[tipo] : 0;
    }
    int getObjetivoHielo() const { return objetivoHielo; }
    bool tieneObjetivoGema(int tipo) const {
        return (tipo >= 0 && tipo < 5) ? tieneObjetivoGemas[tipo] : false;
    }

    int getProgresoGemas(int tipo) const {
        return (tipo >= 0 && tipo < 5) ? gemsCollected[tipo] : 0;
    }
    int getProgresoHielo() const { return iceDestroyed; }

    // UI
    int getIceCount() const { return iceCount; }
    int getCollectedGems(int gemType) const {
        if (gemType >= 0 && gemType < 5) {
            return gemsCollected[gemType];
        }
        return 0;
    }

    void eliminarHielo(int row, int col) {
        destruirHieloEnPosicion(row, col);
    }

    bool tieneHielo(int row, int col) const {
        return hayHieloEnPosicion(row, col);
    }

    // accountants
    void actualizarContadoresGemas(int gemType) {
        if (gemType >= 0 && gemType < 5) {
            gemsCollected[gemType]++;
        }
    }
};

#endif