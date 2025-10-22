#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({ 800, 600 }), "Match-3 SFML 3.0")
    , board()
    , ui()
    , estado(GameState::Inicio)
    , nivelActual(0)
    , totalNiveles(3)
    , movimientosRestantes(0)
    , selectedX(-1)
    , selectedY(-1)
{
    procesandoCombinaciones = false;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!board.cargarTexturas()) {
        std::cerr << "Error: No se pudieron cargar las texturas del tablero\n";
    }

    cargarNiveles();
}

void Game::cargarNiveles() {
    niveles[0] = Level(1, "Nivel Fácil", 20);
    niveles[0].setIceCount(3);
    niveles[0].addObjective(ObjectiveType::CLEAR_ICE, 3, "Romper 3 bloques de hielo");
    niveles[0].addObjective(ObjectiveType::REACH_SCORE, 700, "Alcanzar 700 puntos");


    niveles[1] = Level(2, "Nivel Intermedio", 12);
    niveles[1].setIceCount(5);
    niveles[1].addObjective(ObjectiveType::CLEAR_ICE, 5, "Romper 5 bloques de hielo");
    niveles[1].addObjective(ObjectiveType::REACH_SCORE, 1000, "Alcanzar 1000 puntos");

    niveles[2] = Level(3, "Nivel Difícil", 10);
    niveles[2].setIceCount(6);
    niveles[2].addObjective(ObjectiveType::CLEAR_ICE, 6, "Romper 6 bloques de hielo");
    niveles[2].addObjective(ObjectiveType::REACH_SCORE, 1000, "Alcanzar 1000 puntos");
}

void Game::run() {
    while (window.isOpen()) {
        deltaTime = gameClock.restart().asSeconds();

        procesarEventos();
        actualizar();
        dibujar();
    }
}

void Game::iniciarNivel(int numeroNivel) {
    if (numeroNivel >= 1 && numeroNivel <= totalNiveles) {
        nivelActual = numeroNivel - 1;
        niveles[nivelActual].reset();

        board.reset();
        board.generarHieloAleatorio(niveles[nivelActual].getIceCount());

        movimientosRestantes = niveles[nivelActual].getMoveLimit();
        board.setScore(0);

        cambiarEstado(GameState::Jugando);
        std::cout << "=== INICIANDO NIVEL " << numeroNivel << " ===" << std::endl;
        std::cout << "Nombre: " << niveles[nivelActual].getName() << std::endl;
        std::cout << "Movimientos: " << movimientosRestantes << std::endl;
        std::cout << "Hielo: " << niveles[nivelActual].getIceCount() << std::endl;

        // Mostrar objetivos
        for (int i = 0; i < niveles[nivelActual].getObjectiveCount(); i++) {
            LevelObjective obj = niveles[nivelActual].getObjectives()[i];
            std::cout << "Objetivo " << (i + 1) << ": " << obj.getDescription() << std::endl;
        }
        std::cout << "====================" << std::endl;
    }
}

void Game::verificarObjetivosNivel() {
    if (estado != GameState::Jugando) return;

    Level* nivel = &niveles[nivelActual];

    nivel->updateObjective(ObjectiveType::REACH_SCORE, board.getScore());

    // Verificar si el nivel esta completado
    if (nivel->checkLevelCompleted()) {
        completarNivel();
    }
    else if (movimientosRestantes <= 0) {
        fallarNivel();
    }
}

void Game::completarNivel() {
    std::cout << "NIVEL " << (nivelActual + 1) << " COMPLETADO !" << std::endl;
    cambiarEstado(GameState::NivelCompletado);
}

void Game::fallarNivel() {
    std::cout << "Nivel " << (nivelActual + 1) << " fallado - Sin movimientos" << std::endl;
    cambiarEstado(GameState::NivelFallado);
}

void Game::updateAnimations(float deltaTime) {
    float correctedDelta = deltaTime;
    if (correctedDelta > 0.1f) {
        correctedDelta = 0.016f;
    }
    animations.update(correctedDelta);
}


void Game::procesarSeleccionNivel(const sf::Vector2f& worldPos) {
    if (ui.isNivel1ButtonClicked(worldPos)) {
        iniciarNivel(1);
    }
    else if (ui.isNivel2ButtonClicked(worldPos)) {
        iniciarNivel(2);
    }
    else if (ui.isNivel3ButtonClicked(worldPos)) {
        iniciarNivel(3);
    }
    else if (ui.isExitButtonClicked(worldPos)) {
        cambiarEstado(GameState::Inicio);
    }
}


void Game::procesarEventos() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                switch (estado) {
                case GameState::Inicio:
                    if (ui.isPlayButtonClicked(worldPos)) {
                        cambiarEstado(GameState::SeleccionNivel);
                    }
                    break;

                case GameState::SeleccionNivel:
                    if (ui.isNivel1ButtonClicked(worldPos)) {
                        iniciarNivel(1);
                    }
                    else if (ui.isNivel2ButtonClicked(worldPos)) {
                        iniciarNivel(2);
                    }
                    else if (ui.isNivel3ButtonClicked(worldPos)) {
                        iniciarNivel(3);
                    }
                    else if (ui.isExitButtonClicked(worldPos)) {
                        cambiarEstado(GameState::Inicio);
                    }
                    break;

                case GameState::Jugando:
                    if (ui.isResetButtonClicked(worldPos)) {
                        reiniciarJuego();
                    }
                    else if (ui.isExitButtonClicked(worldPos)) {
                        window.close();
                    }
                    else {
                        int col = static_cast<int>(worldPos.x) / CELL_SIZE;
                        int row = static_cast<int>(worldPos.y) / CELL_SIZE;
                        procesarClick(col, row);
                    }
                    break;

                case GameState::NivelCompletado:
                case GameState::NivelFallado:
                    if (ui.isResetButtonClicked(worldPos)) {
                        reiniciarJuego();
                    }
                    else if (ui.isExitButtonClicked(worldPos)) {
                        window.close();
                    }
                    break;
                }
            }
        }
    }
}

void Game::agregarAnimacionesCaidaSelectivas(int posicionesOriginales[BOARD_SIZE][BOARD_SIZE]) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (posicionesOriginales[r][c] == -1 && board.getGemType(r, c) != -1) {
                Gem* gem = board.getGemAt(r, c);
                if (gem) {
                    int filaOrigen = encontrarFilaOrigen(r, c, posicionesOriginales);
                    if (filaOrigen != -1) {
                        animations.addGemFallAnimation(
                            r * BOARD_SIZE + c,
                            sf::Vector2f(c * CELL_SIZE, filaOrigen * CELL_SIZE),
                            sf::Vector2f(c * CELL_SIZE, r * CELL_SIZE),
                            &gem->getSprite()
                        );
                    }
                    else {
                        animations.addGemFallAnimation(
                            r * BOARD_SIZE + c,
                            sf::Vector2f(c * CELL_SIZE, -CELL_SIZE),
                            sf::Vector2f(c * CELL_SIZE, r * CELL_SIZE),
                            &gem->getSprite()
                        );
                    }
                }
            }
        }
    }
}

int Game::encontrarFilaOrigen(int filaDest, int col, int posicionesOriginales[BOARD_SIZE][BOARD_SIZE]) {
    for (int fila = filaDest - 1; fila >= 0; fila--) {
        if (posicionesOriginales[fila][col] != -1) {
            return fila;
        }
    }
    return -1;
}

void Game::procesarCombinacionesDespuesDeAnimacion() {
    if (procesandoCombinaciones) {
        return;
    }

    board.verificarCombinacionesCercaDeHielo(selectedY, selectedX);
    procesandoCombinaciones = true;

    int posicionesAntes[BOARD_SIZE][BOARD_SIZE];
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            posicionesAntes[r][c] = board.getGemType(r, c);
        }
    }

    board.aplicarGravedad();
    agregarAnimacionesCaidaSelectivas(posicionesAntes);
}

void Game::procesarClick(int col, int row) {
    if (col < 0 || col >= BOARD_SIZE || row < 0 || row >= BOARD_SIZE) return;
    if (movimientosRestantes <= 0) return;
    if (animations.isAnimating()) {
        std::cout << "Esperando a que terminen las animaciones...\n";
        return;
    }

    if (selectedX == -1 && selectedY == -1) {
        selectedX = col;
        selectedY = row;
        std::cout << "Seleccionado: (" << col << ", " << row << ")\n";
    }
    else {
        int dx = std::abs(col - selectedX);
        int dy = std::abs(row - selectedY);

        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            if (board.esIntercambioValido(selectedX, selectedY, col, row)) {
                sf::Vector2f pos1Original(selectedX * CELL_SIZE, selectedY * CELL_SIZE);
                sf::Vector2f pos2Original(col * CELL_SIZE, row * CELL_SIZE);

                Gem* gem1Original = board.getGemAt(selectedY, selectedX);
                Gem* gem2Original = board.getGemAt(row, col);

                bool intercambioExitoso = board.intercambiar(selectedX, selectedY, col, row, false);

                if (intercambioExitoso && gem1Original && gem2Original) {
                    Gem* gem1Nueva = board.getGemAt(selectedY, selectedX);
                    Gem* gem2Nueva = board.getGemAt(row, col);

                    if (gem1Nueva && gem2Nueva) {
                        animations.addGemSwapAnimation(1,
                            pos2Original,
                            sf::Vector2f(selectedX * CELL_SIZE, selectedY * CELL_SIZE),
                            &gem1Nueva->getSprite());

                        animations.addGemSwapAnimation(2,
                            pos1Original,
                            sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE),
                            &gem2Nueva->getSprite());
                    }

                    movimientosRestantes--;
                    std::cout << "Movimiento valido! Movimientos restantes: " << movimientosRestantes << "\n";
                    procesarCombinacionesDespuesDeAnimacion();
                }
            }
            else {
                std::cout << "Movimiento invalido - no crea combinaciones\n";
            }
        }
        else {
            std::cout << "Movimiento no adyacente\n";
        }

        selectedX = -1;
        selectedY = -1;
    }
}

void Game::actualizar() {
    updateAnimations(deltaTime);

    if (procesandoCombinaciones && !animations.isAnimating()) {
        int puntosExtra = board.eliminarCombinaciones();

        if (puntosExtra > 0) {
            std::cout << "¡COMBINACION EN CADENA! Puntos: " << puntosExtra << std::endl;
            board.aplicarGravedad();
            procesandoCombinaciones = false;
            procesarCombinacionesDespuesDeAnimacion();
        }
        else {
            procesandoCombinaciones = false;

            for (int r = 0; r < BOARD_SIZE; r++) {
                for (int c = 0; c < BOARD_SIZE; c++) {
                    if (board.getGemType(r, c) == -1) {
                        board.verificarCombinacionesCercaDeHielo(r, c);
                    }
                }
            }

            verificarObjetivosNivel();
        }
    }

    ui.actualizarTexto(board.getScore(), movimientosRestantes);
}

void Game::dibujar() {
    window.clear();

    switch (estado) {
    case GameState::Inicio:
        ui.drawInicio(window);
        break;

    case GameState::SeleccionNivel:
        ui.drawSeleccionNivel(window);
        break;

    case GameState::Jugando:
    {
        sf::Sprite* sprites[BOARD_SIZE * BOARD_SIZE];
        int spriteCount;

        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            sprites[i] = nullptr;
        }

        board.obtenerSprites(sprites, spriteCount);
        ui.drawJugando(window);
        if (nivelActual >= 0 && nivelActual < 3) {
            int nivelNumero = nivelActual + 1;
            int objetivoPuntaje = 0;
            int objetivoHielo = 0;
            int objetivoGemas[5] = { 0 };
            bool tieneObjetivoGemas[5] = { false };

            switch (nivelNumero) {
            case 1: // Nivel 1
                objetivoPuntaje = 700;
                objetivoHielo = 3;
                break;
            case 2: // Nivel 2
                objetivoPuntaje = 1000;
                objetivoHielo = 5;
                break;
            case 3: // Nivel 3
                objetivoPuntaje = 1000;
                objetivoHielo = 6;
                break;
            }
            int puntajeActual = board.getScore();
            int hieloDestruido = board.getProgresoHielo();
            int gemasRecolectadas[5] = { 0, 0, 0, 0, 0 };

            // Dibujar objetivos
            ui.drawObjetivosNivel(window,
                nivelNumero,
                objetivoPuntaje,
                objetivoHielo,
                objetivoGemas,
                tieneObjetivoGemas,
                puntajeActual,
                hieloDestruido,
                gemasRecolectadas);
        }

        for (int i = 0; i < spriteCount; i++) {
            if (sprites[i] != nullptr && !animations.isSpriteAnimating(sprites[i])) {
                window.draw(*sprites[i]);
            }
        }

        animations.draw(window);
        board.dibujarHielo(window);
    }
    break;

    case GameState::NivelCompletado:
        ui.drawFin(window, board.getScore());
        break;

    case GameState::NivelFallado:
        ui.drawFin(window, board.getScore());
        break;
    }

    window.display();
}

void Game::reiniciarJuego() {
    iniciarNivel(nivelActual + 1);
    selectedX = -1;
    selectedY = -1;
    std::cout << "Juego reiniciado - Nivel " << (nivelActual + 1) << std::endl;
}

void Game::cambiarEstado(GameState nuevoEstado) {
    estado = nuevoEstado;
    std::cout << "Estado cambiado a: " << static_cast<int>(nuevoEstado) << "\n";
}