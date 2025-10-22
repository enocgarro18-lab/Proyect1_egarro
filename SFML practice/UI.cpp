#include "UI.h"
#include <iostream>

UI::UI()
    : font()
    , scoreText(font)
    , movimientosText(font)
    , gameOverText(font)
    , finalScoreText(font)
    , playButtonText(font)
    , resetButtonText(font)
    , exitButtonText(font)
    , nivel1Text(font)
    , nivel2Text(font)
    , nivel3Text(font)
    , tituloNivelesText(font)
    , playButton()
    , resetButton()
    , exitButton()
    , nivel1Button()
    , nivel2Button()
    , nivel3Button()
    , backgroundTexture()
    , backgroundSprite(backgroundTexture)
{
    if (!cargarFuente()) {
        std::cerr << "Error: No se pudo cargar la fuente\n";
    }

    if (!cargarTexturas()) {
        std::cerr << "Error: No se pudieron cargar las texturas\n";
    }

    inicializarBotones();
    inicializarTextos();
    inicializarBotonesNiveles();
}

bool UI::cargarFuente() {
    return font.openFromFile("../assets/arial.ttf");
}

bool UI::cargarTexturas() {
    if (!backgroundTexture.loadFromFile("../assets/background.png")) {
        std::cerr << "Error cargando background.png\n";
        return false;
    }

    backgroundSprite = sf::Sprite(backgroundTexture);
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize(800, 600);
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale({ scaleX, scaleY });

    return true;
}

void UI::inicializarBotones() {
    // Botón Play
    playButton.setSize(sf::Vector2f(200, 60));
    playButton.setPosition(sf::Vector2f(300, 250));
    playButton.setFillColor(sf::Color::Blue);

    playButtonText.setString("Jugar");
    playButtonText.setCharacterSize(30);
    playButtonText.setFillColor(sf::Color::White);
    playButtonText.setPosition(sf::Vector2f(playButton.getPosition().x + 60,
        playButton.getPosition().y + 15));

    // Botón Reset
    resetButton.setSize(sf::Vector2f(120, 40));
    resetButton.setPosition(sf::Vector2f(10, 550));
    resetButton.setFillColor(sf::Color::Green);

    resetButtonText.setString("Reiniciar");
    resetButtonText.setCharacterSize(20);
    resetButtonText.setFillColor(sf::Color::White);
    resetButtonText.setPosition(sf::Vector2f(resetButton.getPosition().x + 10,
        resetButton.getPosition().y + 8));

    // Botón Exit
    exitButton.setSize(sf::Vector2f(100, 40));
    exitButton.setPosition(sf::Vector2f(690, 550));
    exitButton.setFillColor(sf::Color::Red);

    exitButtonText.setString("Salir");
    exitButtonText.setCharacterSize(20);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setPosition(sf::Vector2f(exitButton.getPosition().x + 25,
        exitButton.getPosition().y + 8));
}

void UI::inicializarBotonesNiveles() {
    // Botón Nivel 1
    nivel1Button.setSize(sf::Vector2f(300, 100));
    nivel1Button.setPosition(sf::Vector2f(250, 200));
    nivel1Button.setFillColor(sf::Color(0, 150, 0));

    nivel1Text.setString("NIVEL 1 - FACIL\n• 15 movimientos\n• 700 puntos\n• 3 bloques de hielo");
    nivel1Text.setCharacterSize(18);
    nivel1Text.setFillColor(sf::Color::White);
    nivel1Text.setPosition(sf::Vector2f(nivel1Button.getPosition().x + 10,
        nivel1Button.getPosition().y + 10));

    // Botón Nivel 2 
    nivel2Button.setSize(sf::Vector2f(300, 100));
    nivel2Button.setPosition(sf::Vector2f(250, 300));
    nivel2Button.setFillColor(sf::Color(200, 130, 0));

    nivel2Text.setString("NIVEL 2 - INTERMEDIO\n• 12 movimientos\n• 1000 puntos\n• 5 bloques de hielo");
    nivel2Text.setCharacterSize(18);
    nivel2Text.setFillColor(sf::Color::White);
    nivel2Text.setPosition(sf::Vector2f(nivel2Button.getPosition().x + 10,
        nivel2Button.getPosition().y + 10));

    // Botón Nivel 3
    nivel3Button.setSize(sf::Vector2f(300, 120));
    nivel3Button.setPosition(sf::Vector2f(250, 400));
    nivel3Button.setFillColor(sf::Color(150, 0, 0));

    nivel3Text.setString("NIVEL 3 - DIFICIL\n• 10 movimientos\n• 1000 puntos\n• 6 bloques de hielo");
    nivel3Text.setCharacterSize(18);
    nivel3Text.setFillColor(sf::Color::White);
    nivel3Text.setPosition(sf::Vector2f(nivel3Button.getPosition().x + 10,
        nivel3Button.getPosition().y + 10));

    // Título de selección de niveles
    tituloNivelesText.setString("GEM MATCH - SELECCIONA NIVEL");
    tituloNivelesText.setCharacterSize(32);
    tituloNivelesText.setFillColor(sf::Color::White);
    tituloNivelesText.setPosition(sf::Vector2f(180, 100));
}

void UI::inicializarTextos() {
    // Texto de score
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(sf::Vector2f(570, 50));

    // Texto de movimientos
    movimientosText.setCharacterSize(24);
    movimientosText.setFillColor(sf::Color::White);
    movimientosText.setPosition(sf::Vector2f(570, 10));

    // Texto de game over
    gameOverText.setString("Juego Terminado");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(sf::Vector2f(200, 200));

    // Texto de puntaje final
    finalScoreText.setCharacterSize(30);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(sf::Vector2f(300, 280));
}

void UI::setScore(int score) {
    scoreText.setString("Puntaje: " + std::to_string(score));
}

void UI::setMovimientos(int movimientos) {
    movimientosText.setString("Movimientos: " + std::to_string(movimientos));
}

void UI::actualizarTexto(int score, int movimientos) {
    setScore(score);
    setMovimientos(movimientos);
}

bool UI::isPlayButtonClicked(const sf::Vector2f& pos) const {
    return playButton.getGlobalBounds().contains(pos);
}

bool UI::isResetButtonClicked(const sf::Vector2f& pos) const {
    return resetButton.getGlobalBounds().contains(pos);
}

bool UI::isExitButtonClicked(const sf::Vector2f& pos) const {
    return exitButton.getGlobalBounds().contains(pos);
}

bool UI::isNivel1ButtonClicked(const sf::Vector2f& pos) const {
    return nivel1Button.getGlobalBounds().contains(pos);
}

bool UI::isNivel2ButtonClicked(const sf::Vector2f& pos) const {
    return nivel2Button.getGlobalBounds().contains(pos);
}

bool UI::isNivel3ButtonClicked(const sf::Vector2f& pos) const {
    return nivel3Button.getGlobalBounds().contains(pos);
}

void UI::drawInicio(sf::RenderWindow& window) const {
    window.draw(backgroundSprite);
    window.draw(playButton);
    window.draw(playButtonText);
}

void UI::drawSeleccionNivel(sf::RenderWindow& window) const {
    window.draw(backgroundSprite);
    window.draw(tituloNivelesText);

    window.draw(nivel1Button);
    window.draw(nivel1Text);
    window.draw(nivel2Button);
    window.draw(nivel2Text);
    window.draw(nivel3Button);
    window.draw(nivel3Text);

    window.draw(exitButton);
    window.draw(exitButtonText);
}

void UI::drawJugando(sf::RenderWindow& window) const {
    window.draw(backgroundSprite);
    window.draw(scoreText);
    window.draw(movimientosText);
    window.draw(resetButton);
    window.draw(resetButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);
}

void UI::drawFin(sf::RenderWindow& window, int finalScore) {
    window.draw(backgroundSprite);

    finalScoreText.setString("Puntaje Final: " + std::to_string(finalScore));

    window.draw(gameOverText);
    window.draw(finalScoreText);
    window.draw(resetButton);
    window.draw(resetButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);
}

void UI::drawObjetivosNivel(sf::RenderWindow& window, int nivel,
    int objetivoPuntaje, int objetivoHielo,
    const int* objetivoGemas, const bool* tieneObjetivoGemas,
    int puntajeActual, int hieloDestruido,
    const int* gemasRecolectadas) const {

    // Fondo semi-transparente para objetivos
    sf::RectangleShape fondoObjetivos(sf::Vector2f(230, 300));
    fondoObjetivos.setPosition({ 555, 100 });
    fondoObjetivos.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(fondoObjetivos);

    // Textos temporales para objetivos
    sf::Text tituloObjetivos(font);
    tituloObjetivos.setString("OBJETIVOS NIVEL " + std::to_string(nivel));
    tituloObjetivos.setCharacterSize(20);
    tituloObjetivos.setPosition({ 570, 110 });
    tituloObjetivos.setFillColor(sf::Color::Yellow);
    window.draw(tituloObjetivos);

    sf::Text textoPuntaje(font);
    textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual) + "/" + std::to_string(objetivoPuntaje));
    textoPuntaje.setCharacterSize(16);
    textoPuntaje.setPosition({ 560, 150 });
    textoPuntaje.setFillColor(puntajeActual >= objetivoPuntaje ? sf::Color::Green : sf::Color::White);
    window.draw(textoPuntaje);

    sf::Text textoHielo(font);
    textoHielo.setString("Hielo: " + std::to_string(hieloDestruido) + "/" + std::to_string(objetivoHielo));
    textoHielo.setCharacterSize(16);
    textoHielo.setPosition({ 560, 180 });
    textoHielo.setFillColor(hieloDestruido >= objetivoHielo ? sf::Color::Green : sf::Color::White);
    window.draw(textoHielo);

    int yPos = 210;
    std::string nombresGemas[5] = { "Rojas", "Azules", "Verdes", "Amarillas", "Moradas" };
    sf::Color coloresGemas[5] = { sf::Color::Red, sf::Color::Blue, sf::Color::Green,
                                sf::Color::Yellow, sf::Color(128, 0, 128) };

    for (int i = 0; i < 5; ++i) {
        if (tieneObjetivoGemas && tieneObjetivoGemas[i] && objetivoGemas && gemasRecolectadas) {
            sf::Text textoGema(font);
            textoGema.setString("Gemas " + nombresGemas[i] + ": " + std::to_string(gemasRecolectadas[i]) + "/" + std::to_string(objetivoGemas[i]));
            textoGema.setCharacterSize(16);
            textoGema.setPosition({ 540, static_cast<float>(yPos) });
            textoGema.setFillColor(gemasRecolectadas[i] >= objetivoGemas[i] ? sf::Color::Green : coloresGemas[i]);
            window.draw(textoGema);
            yPos += 25;
        }
    }
}