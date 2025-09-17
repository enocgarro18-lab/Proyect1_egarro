#include "Game.h"
#include <iostream>
#include <optional>
#include <cstdlib>
#include <ctime>

Game::Game()
    : window(sf::VideoMode({ 800 , 600 }), "Match-3 SFML 3.0")
    , backgroundSprite(Gem::emptyTexture)
    , gameOverText(font)
    , finalScoreText(font)
    , scoreText(font)
    , playButtonText(font)
    , exitButtonText(font)
    , resetButtonText(font)
    , movimientosText(font)
{
    std::cout << "Inicializando juego\n";
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!font.openFromFile("assets/arial.ttf")) {
        std::cerr << "Error cargando la fuente\n";
    }

    resetButton.setSize(sf::Vector2f(100.f, 40.f));
    resetButton.setPosition(sf::Vector2f(float(BOARD_SIZE * CELL_SIZE) - 140.f, 555.f));
    resetButton.setFillColor(sf::Color::Red);
    resetButtonText.setFont(font);
    resetButtonText.setString("Reiniciar");
    resetButtonText.setCharacterSize(20);
    resetButtonText.setFillColor(sf::Color::White);
    resetButtonText.setPosition(sf::Vector2f(resetButton.getPosition().x + 10.f, resetButton.getPosition().y + 5.f));

    gameOverText = sf::Text(font, "Juego terminado", 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({ 200.f, 200.f });

    finalScoreText = sf::Text(font, "", 30);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition({ 220.f, 280.f });

    playButton.setSize({ 200.f, 60.f });
    playButton.setPosition({ 300.f, 250.f });
    playButton.setFillColor(sf::Color::Blue);
    playButtonText.setFont(font);
    playButtonText.setString("Jugar");
    playButtonText.setCharacterSize(30);
    playButtonText.setFillColor(sf::Color::White);
    playButtonText.setPosition(sf::Vector2f(playButton.getPosition().x + 60.f, playButton.getPosition().y + 15.f));

    exitButton.setSize(sf::Vector2f(100.f, 40.f));
    exitButton.setPosition(sf::Vector2f(float(BOARD_SIZE * CELL_SIZE) - 10.f, 555.f));
    exitButton.setFillColor(sf::Color::Red);
    exitButtonText.setFont(font);
    exitButtonText.setString("Salir");
    exitButtonText.setCharacterSize(20);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setPosition(sf::Vector2f(exitButton.getPosition().x + 30.f, exitButton.getPosition().y + 5.f));

    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition({ 230.f, 550.f });

    movimientosText.setCharacterSize(24);
    movimientosText.setFillColor(sf::Color::Yellow);
    movimientosText.setPosition({ 10.f, 550.f });

    if (backgroundTexture.loadFromFile("assets/background.png")) {
        backgroundSprite.setTexture(backgroundTexture);
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        float scaleX = float(windowSize.x) / textureSize.x * 1.3f;
        float scaleY = float(windowSize.y) / textureSize.y * 1.1f;
        backgroundSprite.setScale(sf::Vector2f(scaleX, scaleY));
        std::cout << "Background cargado y ajustado\n";
    }
    else {
        std::cerr << "Error cargando background.png\n";
    }

    std::cout << "Cargando texturas de gemas...\n";
    gemTextures.resize(NUM_GEMS);
    for (int i = 0; i < NUM_GEMS; ++i) {
        std::string filename = "assets/gem" + std::to_string(i) + ".png";
        if (!gemTextures[i].loadFromFile(filename))
            std::cerr << "Error cargando " << filename << "\n";
        else
            std::cout << filename << " cargada\n";
    }

    std::cout << "Inicializando tablero...\n";
    std::cout << "Tablero inicializado\n";
    actualizarSpritesDesdeBoard();
    std::cout << "Sprites actualizados\n";

    movimientosRestantes = 20;
    selectedX = -1;
    selectedY = -1;
}

void Game::run() {
    std::cout << "Inicio del ciclo principal\n";
    while (window.isOpen()) {
        std::cout << "Nuevo ciclo\n";
        procesarEventos();
        actualizarTexto();
        dibujar();
    }
    std::cout << "Fin del ciclo principa              l\n";
}

void Game::procesarEventos() {
    while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
        const sf::Event& event = *eventOpt;
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }
        else if (event.is<sf::Event::MouseButtonPressed>()) {
            auto mousePress = event.getIf<sf::Event::MouseButtonPressed>();
            if (mousePress->button == sf::Mouse::Button::Left) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                if (estado == GameState::Inicio && playButton.getGlobalBounds().contains(worldPos)) {
                    estado = GameState::Jugando;
                }
                else if (estado == GameState::Jugando) {
                    if (resetButton.getGlobalBounds().contains(worldPos)) {
                        reiniciarJuego();
                    }
                    else if (exitButton.getGlobalBounds().contains(worldPos)) {
                        window.close();
                    }
                    else {
                        int col = int(worldPos.x) / CELL_SIZE;
                        int row = int(worldPos.y) / CELL_SIZE;
                        procesarClick(col, row);
                        if (movimientosRestantes <= 0) {
                            estado = GameState::Fin;
                        }
                    }
                }
                else if (estado == GameState::Fin) {
                    if (resetButton.getGlobalBounds().contains(worldPos)) {
                        reiniciarJuego();
                        estado = GameState::Inicio; // O jugamos automáticamente con GameState::Jugando
                    }
                    else if (exitButton.getGlobalBounds().contains(worldPos)) {
                        window.close();
                    }
                }
            }
        }
    }
}


void Game::procesarClick(int col, int row) {
    if (col < 0 || col >= BOARD_SIZE || row < 0 || row >= BOARD_SIZE)
        return;

    if (movimientosRestantes <= 0) {
        std::cout << "¡Sin movimientos restantes!\n";
        return;
    }

    if (selectedX == -1 && selectedY == -1) {
        selectedX = col;
        selectedY = row;
        std::cout << "Seleccionado tile: (" << selectedX << "," << selectedY << ")\n";
    }
    else {
        int dx = std::abs(col - selectedX);
        int dy = std::abs(row - selectedY);
        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            if (board.intercambiar(selectedX, selectedY, col, row)) {
                std::cout << "Movimiento válido, actualizando sprites\n";
                actualizarSpritesDesdeBoard();
                movimientosRestantes--;
            }
            else {
                std::cout << "Movimiento inválido, sin combinaciones.\n";
            }
        }
        selectedX = -1;
        selectedY = -1;
    }
}

void Game::reiniciarJuego() {
    movimientosRestantes = 20;
    board.generarGemas();
    actualizarSpritesDesdeBoard();
    board.resetScore();
    actualizarSpritesDesdeBoard();
    actualizarTexto();
    std::cout << "Juego reiniciado, puntaje reseteado\n";
}

void Game::actualizarTexto() {
    scoreText.setString("Score: " + std::to_string(board.getScore()));
    movimientosText.setString("Movimientos: " + std::to_string(movimientosRestantes));
}

void Game::actualizarSpritesDesdeBoard() {
    gems.clear();
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int tipo = board.getGemType(r, c);
            if (tipo >= 0 && tipo < static_cast<int>(gemTextures.size())) {
                sf::Sprite sprite(gemTextures[tipo]);
                sprite.setScale(sf::Vector2f(
                    float(CELL_SIZE) / gemTextures[tipo].getSize().x,
                    float(CELL_SIZE) / gemTextures[tipo].getSize().y
                ));
                sprite.setPosition(sf::Vector2f(
                    float(c * CELL_SIZE),
                    float(r * CELL_SIZE)
                ));
                gems.push_back(sprite);
            }
        }
    }
}

void Game::dibujar() {
    window.clear();
    if (estado == GameState::Inicio) {
        window.draw(playButton);
        window.draw(playButtonText);
    }
    else if (estado == GameState::Jugando) {
        window.draw(backgroundSprite);
        for (const auto& gemSprite : gems) {
            window.draw(gemSprite);
        }
        window.draw(scoreText);
        window.draw(movimientosText);
        window.draw(resetButton);
        window.draw(resetButtonText);
        window.draw(exitButton);
        window.draw(exitButtonText);
    }
    else if (estado == GameState::Fin) {
        gameOverText.setString("Juego terminado");
        window.draw(gameOverText);
        finalScoreText.setString("Puntaje final: " + std::to_string(board.getScore()));
        window.draw(finalScoreText);
        window.draw(resetButton);
        window.draw(resetButtonText);
        window.draw(exitButton);
        window.draw(exitButtonText);
    }
    window.display();
}
