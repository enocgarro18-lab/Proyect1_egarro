#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>


Gem* Board::createGem(int type) {
    if (type == 5) {
        return new BombGem(); // Gema poderosa
    }
    else {
        return new NormalGem(type); // Gema normal
    }
}

//Constructor
Board::Board() : score(0), obstacleCount(0), maxObstacles(MAX_OBSTACLES),
iceCount(0), maxIce(10), iceDestroyed(0),
nivelActual(1), movimientosRestantes(0),
nivelCompletado(false), nivelFallido(false),
objetivoPuntaje(0), objetivoHielo(0) {

    gems = new Gem * *[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        gems[i] = new Gem * [BOARD_SIZE];
        for (int j = 0; j < BOARD_SIZE; ++j) {
            gems[i][j] = nullptr;
        }
    }

    for (int i = 0; i < 5; ++i) {
        gemsCollected[i] = 0;
    }

    gemTextures = new sf::Texture * [NUM_GEMS];
    for (int i = 0; i < NUM_GEMS; ++i) {
        gemTextures[i] = new sf::Texture();
    }

    obstacles = new Obstacle * [maxObstacles];
    for (int i = 0; i < maxObstacles; ++i) {
        obstacles[i] = nullptr;
    }

    obstacleTextures = new sf::Texture * [5];
    for (int i = 0; i < 5; ++i) {
        obstacleTextures[i] = new sf::Texture();
    }

    iceObstacles = new Obstacle * [maxIce];
    for (int i = 0; i < maxIce; ++i) {
        iceObstacles[i] = nullptr;
    }
    for (int i = 0; i < 5; ++i) {
        gemsCollected[i] = 0;
        objetivoGemas[i] = 0;
        tieneObjetivoGemas[i] = false;
    }
}

// DESTRUCTOR
Board::~Board() {
    //Liberar gemas
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            delete gems[i][j];
        }
        delete[] gems[i];
    }
    delete[] gems;

    //Liberar Obstaculos
    for (int i = 0; i < maxObstacles; ++i) {
        if (obstacles[i] != nullptr) {
            delete obstacles[i];
        }
    }
    delete[] obstacles;

    //Liberar Texturas
    for (int i = 0; i < NUM_GEMS; ++i) {
        delete gemTextures[i];
    }
    delete[] gemTextures;

    for (int i = 0; i < 5; ++i) {
        delete obstacleTextures[i];
    }
    delete[] obstacleTextures;

    for (int i = 0; i < maxIce; ++i) {
        if (iceObstacles[i] != nullptr) {
            delete iceObstacles[i];
        }
    }
    delete[] iceObstacles;
}

void Board::cargarNivel(int numeroNivel) {
    nivelActual = numeroNivel;
    nivelCompletado = false;
    nivelFallido = false;

    // Reiniciar contadores
    resetCounters();

    switch (numeroNivel) {
    case 1: // Nivel facil
        movimientosRestantes = 15;
        objetivoPuntaje = 700;
        objetivoHielo = 3;
        break;

    case 2: // Nivel Intermedio
        movimientosRestantes = 12;
        objetivoPuntaje = 1000; 
        objetivoHielo = 5;      
        break;

    case 3: // Nivel Dificil
        movimientosRestantes = 10;
        objetivoPuntaje = 1000;
        objetivoHielo = 6;
        break;
    }

    // Reiniciar objetivos de gemas
    for (int i = 0; i < 5; i++) {
        objetivoGemas[i] = 0;
        tieneObjetivoGemas[i] = false;
    }

    // Generar hielo según el nivel
    generarHieloAleatorio(objetivoHielo);

    std::cout << "Nivel " << numeroNivel << " cargado: "
        << movimientosRestantes << " movimientos, "
        << objetivoPuntaje << " puntos, "
        << objetivoHielo << " hielos" << std::endl;
}




bool Board::verificarVictoria() {
    // Verificar puntaje
    bool puntajeCumplido = (score >= objetivoPuntaje);

    // Verificar gemas
    bool gemasCumplidas = true;
    for (int i = 0; i < 5; ++i) {
        if (tieneObjetivoGemas[i] && gemsCollected[i] < objetivoGemas[i]) {
            gemasCumplidas = false;
            break;
        }
    }

    // Verificar hielo
    bool hieloCumplido = (iceDestroyed >= objetivoHielo);

    nivelCompletado = puntajeCumplido && gemasCumplidas && hieloCumplido;

    if (nivelCompletado && !nivelFallido) {
        std::cout << "Nivel " << nivelActual << " completado" << std::endl;
        std::cout << "Puntaje: " << score << "/" << objetivoPuntaje << std::endl;
        std::cout << "Gemas recolectadas: ";
        for (int i = 0; i < 5; ++i) {
            if (tieneObjetivoGemas[i]) {
                std::cout << "Tipo " << i << ": " << gemsCollected[i] << "/" << objetivoGemas[i] << " ";
            }
        }
        std::cout << std::endl;
        std::cout << "Hielo destruido: " << iceDestroyed << "/" << objetivoHielo << std::endl;
    }

    return nivelCompletado;
}

bool Board::verificarDerrota() {
    nivelFallido = (movimientosRestantes <= 0 && !nivelCompletado);

    if (nivelFallido) {
        std::cout << "Nivel " << nivelActual << " fallido!" << std::endl;
        std::cout << "Progreso:" << std::endl;
        std::cout << "   - Puntaje: " << score << "/" << objetivoPuntaje << std::endl;
        for (int i = 0; i < 5; ++i) {
            if (tieneObjetivoGemas[i]) {
                std::cout << "   - Gemas tipo " << i << ": " << gemsCollected[i] << "/" << objetivoGemas[i] << std::endl;
            }
        }
        std::cout << "   - Hielo: " << iceDestroyed << "/" << objetivoHielo << std::endl;
    }

    return nivelFallido;
}

void Board::usarMovimiento() {
    if (movimientosRestantes > 0 && !nivelCompletado && !nivelFallido) {
        movimientosRestantes--;
        std::cout << "Movimientos restantes: " << movimientosRestantes << std::endl;

        // Verificar si se perdió después de usar movimiento
        verificarDerrota();
    }
}



// Load textures
bool Board::cargarTexturas() {
    bool todasCargadas = true;

    // Load gems
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../assets/gem" + std::to_string(i) + ".png";
        if (!gemTextures[i]->loadFromFile(filename)) {
            std::cerr << "Error cargando: " << filename << std::endl;
            todasCargadas = false;
        }
    }

    // Load bmob
    if (!gemTextures[5]->loadFromFile("../assets/bom.png")) {
        std::cerr << "Error cargando bom.png" << std::endl;
        todasCargadas = false;
    }

    // Load iceTexture
    if (!obstacleTextures[0]->loadFromFile("../assets/ice.png")) {
        std::cerr << "Error cargando ice.png" << std::endl;
        todasCargadas = false;
    }

    return todasCargadas;
}

// GENERAR GEMAS
void Board::generarGemas() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            int tipo;
            bool valido;

            do {
                valido = true;
                tipo = std::rand() % 5;

                // Evitar combinaciones al generar
                if (col >= 2 && gems[row][col - 1] && gems[row][col - 2]) {
                    if (gems[row][col - 1]->getType() == tipo &&
                        gems[row][col - 2]->getType() == tipo) {
                        valido = false;
                    }
                }
                if (row >= 2 && gems[row - 1][col] && gems[row - 2][col]) {
                    if (gems[row - 1][col]->getType() == tipo &&
                        gems[row - 2][col]->getType() == tipo) {
                        valido = false;
                    }
                }
            } while (!valido);
            if (gems[row][col]) {
                delete gems[row][col];
            }
            gems[row][col] = createGem(tipo);
            gems[row][col]->setType(tipo, gemTextures, NUM_GEMS);
            gems[row][col]->setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
        }
    }

    score = 0;
    resetCounters();
}

//Obtener gema en posicion
Gem* Board::getGemAt(int row, int col) {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return gems[row][col];
    }
    return nullptr;
}


void Board::generarGemaPoderosa(int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        std::cout << "Posición invalida para bomba: (" << row << ", " << col << ")" << std::endl;
        return;
    }

    std::cout << "Generando bomba en (" << row << ", " << col << ")..." << std::endl;

    // Eliminar gema existente si hay
    if (gems[row][col]) {
        std::cout << "   Eliminando gema existente tipo " << gems[row][col]->getType() << std::endl;
        delete gems[row][col];
        gems[row][col] = nullptr;
    }

    // Crear nueva bomba
    gems[row][col] = createGem(5);
    if (gems[row][col]) {
        std::cout << "Bomba creada... llamando setType..." << std::endl;
        gems[row][col]->setType(5, gemTextures, NUM_GEMS);
        gems[row][col]->setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
        std::cout << "   Tipo después de setType: " << gems[row][col]->getType() << std::endl;
        std::cout << "   Bomba creada exitosamente" << std::endl;
    }
    else {
        std::cout << "   Error creando bomba" << std::endl;
    }
}


bool Board::esGemaPoderosa(int row, int col) const {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && gems[row][col]) {
        return gems[row][col]->getType() == 5;
    }
    return false;
}


void Board::activarGemaPoderosa(int row, int col) {
    if (!esGemaPoderosa(row, col)) return;

    std::cout << "Explosion de bomba en (" << row << ", " << col << ")!" << std::endl;

    if (gems[row][col]) {
        gems[row][col]->onMatch();
    }

    eliminarAreaBomba(row, col);
 
    delete gems[row][col];
    gems[row][col] = nullptr;

    aplicarGravedad();
}


int Board::getGemsCollected(int gemType) const {
    if (gemType >= 0 && gemType < 5) {
        return gemsCollected[gemType];
    }
    return 0;
}

void Board::resetCounters() {
    iceDestroyed = 0;
    for (int i = 0; i < 5; i++) {
        gemsCollected[i] = 0;
    }
}

void Board::destruirHieloEnPosicion(int row, int col) {
    for (int i = 0; i < iceCount; ++i) {
        if (iceObstacles[i] != nullptr &&
            iceObstacles[i]->getBoardPosition().x == col &&
            iceObstacles[i]->getBoardPosition().y == row &&
            iceObstacles[i]->isVisible()) {

            iceObstacles[i]->hide();
            iceDestroyed++; 
            std::cout << "Hielo destruido en: (" << row << ", " << col << ")\n";
            break;
        }
    }
}

void Board::obtenerSprites(sf::Sprite* output[], int& count) const {
    count = 0;
    int bombasCount = 0;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (gems[row][col] && gems[row][col]->getType() != -1) {
               
                if (gems[row][col]->getType() == 5) {
                    bombasCount++;
                    std::cout << "BOMBA DETECTADA en (" << row << ", " << col << ")" << std::endl;
                }

                output[count] = const_cast<sf::Sprite*>(&gems[row][col]->getSprite());
                count++;
            }
        }
    }

    if (bombasCount > 0) {
        std::cout << "Total de bombas en tablero: " << bombasCount << std::endl;
    }
    std::cout << "Total sprites obtenidos: " << count << std::endl;
}

// MÉTODOS EXISTENTES (adaptados para el nuevo sistema)

bool Board::intercambiarSoloVisual(int x1, int y1, int x2, int y2) {
    if (!esMovimientoValido(x1, y1, x2, y2)) {
        return false;
    }

    // Solo intercambiar punteros de gemas
    Gem* temp = gems[y1][x1];
    gems[y1][x1] = gems[y2][x2];
    gems[y2][x2] = temp;

    return true;
}

void Board::agregarObstaculo(ObstacleType type, int row, int col) {
    if (obstacleCount < maxObstacles && obstacleTextures[0] != nullptr) {
        obstacles[obstacleCount] = new Obstacle(type, *obstacleTextures[0], row, col);
        obstacles[obstacleCount]->setPosition(col * CELL_SIZE, row * CELL_SIZE);
        obstacles[obstacleCount]->setScale(
            static_cast<float>(CELL_SIZE) / obstacleTextures[0]->getSize().x,
            static_cast<float>(CELL_SIZE) / obstacleTextures[0]->getSize().y
        );
        obstacleCount++;
    }
}

void Board::dibujarObstaculos(sf::RenderWindow& window) const {
    for (int i = 0; i < obstacleCount; ++i) {
        if (obstacles[i] != nullptr) {
            obstacles[i]->draw(window);
        }
    }
}

void Board::generarHieloAleatorio(int cantidad) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int hielosGenerados = 0;
    while (hielosGenerados < cantidad && iceCount < maxIce) {
        int row = std::rand() % BOARD_SIZE;
        int col = std::rand() % BOARD_SIZE;

        if (!hayHieloEnPosicion(row, col) && gems[row][col] && gems[row][col]->getType() != -1) {
            if (obstacleTextures[0] != nullptr) {
                iceObstacles[iceCount] = new Obstacle(ObstacleType::ICE, *obstacleTextures[0], row, col);
                iceObstacles[iceCount]->setPosition(col * CELL_SIZE, row * CELL_SIZE);
                iceObstacles[iceCount]->setScale(
                    static_cast<float>(CELL_SIZE) / obstacleTextures[0]->getSize().x,
                    static_cast<float>(CELL_SIZE) / obstacleTextures[0]->getSize().y
                );
                iceCount++;
                hielosGenerados++;
            }
        }
    }
}

bool Board::hayHieloEnPosicion(int row, int col) const {
    for (int i = 0; i < iceCount; ++i) {
        if (iceObstacles[i] != nullptr &&
            iceObstacles[i]->isVisible() &&
            iceObstacles[i]->getBoardPosition().x == col &&
            iceObstacles[i]->getBoardPosition().y == row) {
            return true;
        }
    }
    return false;
}

void Board::dibujarHielo(sf::RenderWindow& window) const {
    for (int i = 0; i < iceCount; ++i) {
        if (iceObstacles[i] != nullptr && iceObstacles[i]->isVisible()) {
            iceObstacles[i]->draw(window);
        }
    }
}

void Board::verificarCombinacionesCercaDeHielo(int row, int col) {
    for (int r = row - 1; r <= row + 1; ++r) {
        for (int c = col - 1; c <= col + 1; ++c) {
            if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
                if (hayHieloEnPosicion(r, c)) {
                    destruirHieloEnPosicion(r, c);
                }
            }
        }
    }
}

void Board::destruirHieloAdyacente(int row, int col) {
    int direcciones[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (int i = 0; i < 4; ++i) {
        int newRow = row + direcciones[i][0];
        int newCol = col + direcciones[i][1];

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
            if (hayHieloEnPosicion(newRow, newCol)) {
                destruirHieloEnPosicion(newRow, newCol);
            }
        }
    }
}

bool Board::esMovimientoValido(int x1, int y1, int x2, int y2) const {
    if (x1 < 0 || x1 >= BOARD_SIZE || y1 < 0 || y1 >= BOARD_SIZE ||
        x2 < 0 || x2 >= BOARD_SIZE || y2 < 0 || y2 >= BOARD_SIZE) {
        return false;
    }

    int dx = std::abs(x1 - x2);
    int dy = std::abs(y1 - y2);

    return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
}

bool Board::esIntercambioValido(int x1, int y1, int x2, int y2) const {
    if (!esMovimientoValido(x1, y1, x2, y2))
        return false;

    if (hayHieloEnPosicion(y1, x1) || hayHieloEnPosicion(y2, x2))
        return false;

    
    int tempTab[BOARD_SIZE][BOARD_SIZE];
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            tempTab[r][c] = (gems[r][c] ? gems[r][c]->getType() : -1);

   
    std::swap(tempTab[y1][x1], tempTab[y2][x2]);

    auto hayCombinacion = [&](int row, int col) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
            return false;
        int tipo = tempTab[row][col];
        if (tipo == -1) return false;

        // Revisar  horizontal
        int countH = 1;
        for (int c = col - 1; c >= 0 && tempTab[row][c] == tipo; --c) countH++;
        for (int c = col + 1; c < BOARD_SIZE && tempTab[row][c] == tipo; ++c) countH++;

        // Revisar vertical
        int countV = 1;
        for (int r = row - 1; r >= 0 && tempTab[r][col] == tipo; --r) countV++;
        for (int r = row + 1; r < BOARD_SIZE && tempTab[r][col] == tipo; ++r) countV++;

        return countH >= 3 || countV >= 3;
        };

    for (int dy = -2; dy <= 2; ++dy)
        for (int dx = -2; dx <= 2; ++dx)
            if (hayCombinacion(y1 + dy, x1 + dx) || hayCombinacion(y2 + dy, x2 + dx))
                return true;

    return false;
}

bool Board::verificarCombinacionEnPosicion(int row, int col) const {
    if (!gems[row][col]) return false;
    int tipo = gems[row][col]->getType();
    if (tipo == -1) return false;

    // Revisar horizontal
    int countHorizontal = 1;
    for (int c = col - 1; c >= 0 && gems[row][c] && gems[row][c]->getType() == tipo; c--) countHorizontal++;
    for (int c = col + 1; c < BOARD_SIZE && gems[row][c] && gems[row][c]->getType() == tipo; c++) countHorizontal++;

    // Revisarr vertical  
    int countVertical = 1;
    for (int r = row - 1; r >= 0 && gems[r][col] && gems[r][col]->getType() == tipo; r--) countVertical++;
    for (int r = row + 1; r < BOARD_SIZE && gems[r][col] && gems[r][col]->getType() == tipo; r++) countVertical++;

    return (countHorizontal >= 3) || (countVertical >= 3);
}

//Eliminar combinaciones...
int Board::eliminarCombinaciones() {
    bool** marcas = new bool* [BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        marcas[i] = new bool[BOARD_SIZE]();
    }

    int puntos = 0;
    const int MAX_BOMBAS = 10;
    struct BombaInfo {
        int row, col;
        int length;
    };
    BombaInfo bombasGenerar[MAX_BOMBAS];
    int numBombas = 0;

    std::cout << " BUSCANDO COMBINACIONES " << std::endl;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE - 2; ++col) {
            if (gems[row][col] && gems[row][col]->getType() != -1) {
                int tipo = gems[row][col]->getType();
                int count = 1;

                for (int c = col + 1; c < BOARD_SIZE; ++c) {
                    if (gems[row][c] && gems[row][c]->getType() == tipo) {
                        count++;
                    }
                    else {
                        break;
                    }
                }

                if (count >= 3) {
                    std::cout << "Combinacion horizontal: " << count << " gemas en fila " << row
                        << ", columnas " << col << " a " << (col + count - 1) << std::endl;

                    for (int c = col; c < col + count; ++c) {
                        marcas[row][c] = true;
                    }
                    puntos += count * 10;

                    //Gemas 4
                    if (count >= 4 && numBombas < MAX_BOMBAS) {
                        bombasGenerar[numBombas].row = row;
                        bombasGenerar[numBombas].col = col + (count - 1) / 2;
                        bombasGenerar[numBombas].length = count;
                        numBombas++;

                        std::cout << "COMBINACION DE " << count << " GEMAS! - ";
                        std::cout << "Bomba programada en (" << row << ", " << (col + (count - 1) / 2) << ")" << std::endl;
                    }

                    col += count - 1;
                }
            }
        }
    }
    for (int col = 0; col < BOARD_SIZE; ++col) {
        for (int row = 0; row < BOARD_SIZE - 2; ++row) {
            if (gems[row][col] && gems[row][col]->getType() != -1) {
                int tipo = gems[row][col]->getType();
                int count = 1;

                for (int r = row + 1; r < BOARD_SIZE; ++r) {
                    if (gems[r][col] && gems[r][col]->getType() == tipo) {
                        count++;
                    }
                    else {
                        break;
                    }
                }
                if (count >= 3) {
                    std::cout << "Combinación vertical: " << count << " gemas en columna " << col
                        << ", filas " << row << " a " << (row + count - 1) << std::endl;

                    for (int r = row; r < row + count; ++r) {
                        marcas[r][col] = true;
                    }
                    puntos += count * 10;

                    if (count >= 4 && numBombas < MAX_BOMBAS) {
                        bombasGenerar[numBombas].row = row + (count - 1) / 2;
                        bombasGenerar[numBombas].col = col;
                        bombasGenerar[numBombas].length = count;
                        numBombas++;

                        std::cout << "combinacion " << count << " gemas";
                        std::cout << "Bomba programada en (" << (row + (count - 1) / 2) << ", " << col << ")" << std::endl;
                    }

                    row += count - 1;
                }
            }
        }
    }
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (marcas[row][col] && esGemaPoderosa(row, col)) {
                std::cout << "Bomba existente activada en(" << row << ", " << col << ")" << std::endl;
                activarGemaPoderosa(row, col);
                marcas[row][col] = false;
            }
        }
    }
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (marcas[row][col]) {
                activarBombasAdyacentes(row, col);
            }
        }
    }

    //
    for (int i = 0; i < numBombas; ++i) {
        int bombaRow = bombasGenerar[i].row;
        int bombaCol = bombasGenerar[i].col;
        int length = bombasGenerar[i].length;

        std::cout << "Procesando bomba " << (i + 1) << "/" << numBombas;
        std::cout << " en (" << bombaRow << ", " << bombaCol << ") de línea " << length << std::endl;

        if (bombaRow >= 0 && bombaRow < BOARD_SIZE && bombaCol >= 0 && bombaCol < BOARD_SIZE) {
            if (marcas[bombaRow][bombaCol]) {
                std::cout << "Posición marcada para eliminación, generando bomba igualmente..." << std::endl;
            }

            if (gems[bombaRow][bombaCol] &&
                gems[bombaRow][bombaCol]->getType() != -1 &&
                !esGemaPoderosa(bombaRow, bombaCol)) {

                int gemTypeOriginal = gems[bombaRow][bombaCol]->getType();
                std::cout << "Convirtiendo gema tipo " << gemTypeOriginal << " en BOMBA" << std::endl;

                generarGemaPoderosa(bombaRow, bombaCol);
                marcas[bombaRow][bombaCol] = false;

                std::cout << "Bomba generada en (" << bombaRow << ", " << bombaCol << ")!" << std::endl;
            }
            else {
                std::cout << "No se pudo generar bomba - Razón: ";
                if (!gems[bombaRow][bombaCol]) {
                    std::cout << "No hay gema";
                }
                else if (gems[bombaRow][bombaCol]->getType() == -1) {
                    std::cout << "Gema vacia";
                }
                else if (esGemaPoderosa(bombaRow, bombaCol)) {
                    std::cout << "es poderosa";
                }
                std::cout << std::endl;
            }
        }
    }
    int gemasEliminadas = 0;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (marcas[row][col] && gems[row][col]) {
                gemasEliminadas++;
                int gemType = gems[row][col]->getType();

                if (gemType >= 0 && gemType < 5) {
                    gemsCollected[gemType]++;
                    std::cout << "Gema tipo " << gemType << " recolectada. Total: "
                        << gemsCollected[gemType] << std::endl;
                }
                gems[row][col]->onMatch();

                if (hayHieloEnPosicion(row, col)) {
                    destruirHieloEnPosicion(row, col);
                    iceDestroyed++; 
                    std::cout << "Hielo destruido en (" << row << ", " << col
                        << "). Total destruido: " << iceDestroyed << std::endl;
                }
                verificarCombinacionesCercaDeHielo(row, col);

                delete gems[row][col];
                gems[row][col] = nullptr;
            }
        }
    }
    score += puntos;
    std::cout << "Puntos: " << puntos << " - Puntaje total: " << score << std::endl;

    std::cout << "Resumen: " << gemasEliminadas << " gemas eliminadas, "
        << puntos << " puntos, " << numBombas << " bombas generadas" << std::endl;
    std::cout << " FIN BUSQUEDA COMBINACIONES " << std::endl;

    if (gemasEliminadas > 0) {
        verificarVictoria();
    }

    // Liberar memoria
    for (int i = 0; i < BOARD_SIZE; ++i) {
        delete[] marcas[i];
    }
    delete[] marcas;

    return puntos;
}

void Board::aplicarGravedad() {
    for (int col = 0; col < BOARD_SIZE; ++col) {
        int writeIndex = BOARD_SIZE - 1;

        // compactar gemas
        for (int row = BOARD_SIZE - 1; row >= 0; --row) {
            if (gems[row][col] && gems[row][col]->getType() != -1) {
                if (row != writeIndex) {
                    delete gems[writeIndex][col];
                    gems[writeIndex][col] = gems[row][col];
                    gems[row][col] = nullptr;
                    gems[writeIndex][col]->setPosition(sf::Vector2f(col * CELL_SIZE, writeIndex * CELL_SIZE));
                }
                writeIndex--;
            }
        }

        //Rellenar espacios
        for (int row = writeIndex; row >= 0; --row) {
            if (gems[row][col]) {
                delete gems[row][col];
            }
            int newType = std::rand() % 5;
            gems[row][col] = createGem(newType);
            gems[row][col]->setType(newType, gemTextures, NUM_GEMS);
            gems[row][col]->setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
        }
    }
}

bool Board::intercambiar(int x1, int y1, int x2, int y2, bool actualizarPosiciones) {
    if (!esMovimientoValido(x1, y1, x2, y2)) {
        return false;
    }

    //Intercambiar punteros
    Gem* temp = gems[y1][x1];
    gems[y1][x1] = gems[y2][x2];
    gems[y2][x2] = temp;

    if (actualizarPosiciones) {
        if (gems[y1][x1]) {
            gems[y1][x1]->setPosition(sf::Vector2f(x1 * CELL_SIZE, y1 * CELL_SIZE));
        }
        if (gems[y2][x2]) {
            gems[y2][x2]->setPosition(sf::Vector2f(x2 * CELL_SIZE, y2 * CELL_SIZE));
        }
    }

    return true;
}

bool Board::intercambiar(int x1, int y1, int x2, int y2) {
    if (intercambiar(x1, y1, x2, y2, true)) {
        usarMovimiento();
        return true;
    }
    return false;
}

int Board::getGemType(int row, int col) const {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && gems[row][col]) {
        return gems[row][col]->getType();
    }
    return -1;
}

void Board::reset() {
    score = 0;

    //limpiar gemas
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            delete gems[row][col];
            gems[row][col] = nullptr;
        }
    }

    //Eliminar hielo
    for (int i = 0; i < iceCount; ++i) {
        if (iceObstacles[i]) {
            delete iceObstacles[i];
            iceObstacles[i] = nullptr;
        }
    }
    iceCount = 0;

    resetCounters();
    generarGemas();
}

bool Board::hayCombinaciones() {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE - 2; ++col) {
            if (gems[row][col] && gems[row][col + 1] && gems[row][col + 2]) {
                int tipo = gems[row][col]->getType();
                if (tipo != -1 &&
                    gems[row][col + 1]->getType() == tipo &&
                    gems[row][col + 2]->getType() == tipo) {
                    return true;
                }
            }
        }
    }

    for (int col = 0; col < BOARD_SIZE; ++col) {
        for (int row = 0; row < BOARD_SIZE - 2; ++row) {
            if (gems[row][col] && gems[row + 1][col] && gems[row + 2][col]) {
                int tipo = gems[row][col]->getType();
                if (tipo != -1 &&
                    gems[row + 1][col]->getType() == tipo &&
                    gems[row + 2][col]->getType() == tipo) {
                    return true;
                }
            }
        }
    }

    return false;
}

void Board::activarBombasAdyacentes(int row, int col) {
    std::cout << "Buscando bombas adyacentes a (" << row << ", " << col << ")" << std::endl;

    int bombasActivadas = 0;
    for (int r = row - 1; r <= row + 1; ++r) {
        for (int c = col - 1; c <= col + 1; ++c) {
            if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
                if (esGemaPoderosa(r, c) && gems[r][c] != nullptr) {
                    std::cout << " Activando bomba adyacente en (" << r << ", " << c << ")" << std::endl;
                    activarGemaPoderosa(r, c);
                    bombasActivadas++;
                }
            }
        }
    }

    if (bombasActivadas > 0) {
        std::cout << "Se activaron " << bombasActivadas << " bombas adyacentes" << std::endl;
    }
}

void Board::eliminarAreaBomba(int centerRow, int centerCol) {
    int puntosGanados = 0;
    int gemasEliminadas = 0;

    std::cout << " Area de explosion 3x3 alrededor de (" << centerRow << ", " << centerCol << "):" << std::endl;

    for (int r = centerRow - 1; r <= centerRow + 1; ++r) {
        for (int c = centerCol - 1; c <= centerCol + 1; ++c) {
            if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
                if (gems[r][c] && gems[r][c]->getType() != -1) {
                    gemasEliminadas++;

                    std::cout << "Eliminando gema en (" << r << ", " << c << ") - Tipo: "
                        << gems[r][c]->getType() << std::endl;

                    // Contar gema recolectada
                    int gemType = gems[r][c]->getType();
                    if (gemType >= 0 && gemType < 5) {
                        gemsCollected[gemType]++;
                    }

               
                    gems[r][c]->onMatch();

                    // Destruir hielo
                    if (hayHieloEnPosicion(r, c)) {
                        destruirHieloEnPosicion(r, c);
                    }

                    puntosGanados += 10;
                    delete gems[r][c];
                    gems[r][c] = nullptr;
                }
            }
        }
    }

    score += puntosGanados;
    std::cout << "Bomba elimino " << gemasEliminadas << " gemas - " << puntosGanados << " puntos!" << std::endl;
}

void Board::eliminarLineaHorizontal(int row) {
    int puntosGanados = 0;

    for (int col = 0; col < BOARD_SIZE; ++col) {
        if (gems[row][col] && gems[row][col]->getType() != -1) {
            puntosGanados += 10;
            int gemType = gems[row][col]->getType();
            if (gemType >= 0 && gemType < 5) {
                gemsCollected[gemType]++;
            }
            gems[row][col]->onMatch();

            if (hayHieloEnPosicion(row, col)) {
                destruirHieloEnPosicion(row, col);
            }

            delete gems[row][col];
            gems[row][col] = nullptr;
        }
    }

    score += puntosGanados;
}

void Board::eliminarLineaVertical(int col) {
    int puntosGanados = 0;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        if (gems[row][col] && gems[row][col]->getType() != -1) {
            puntosGanados += 10;

            int gemType = gems[row][col]->getType();
            if (gemType >= 0 && gemType < 5) {
                gemsCollected[gemType]++;
            }
            gems[row][col]->onMatch();

            if (hayHieloEnPosicion(row, col)) {
                destruirHieloEnPosicion(row, col);
            }

            delete gems[row][col];
            gems[row][col] = nullptr;
        }
    }

    score += puntosGanados;
}

void Board::eliminarPorColor(int colorType) {
    int puntosGanados = 0;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (gems[row][col] && gems[row][col]->getType() == colorType) {
                puntosGanados += 10;

                //Contar gemas
                if (colorType >= 0 && colorType < 5) {
                    gemsCollected[colorType]++;
                }
                gems[row][col]->onMatch();

                if (hayHieloEnPosicion(row, col)) {
                    destruirHieloEnPosicion(row, col);
                }

                delete gems[row][col];
                gems[row][col] = nullptr;
            }
        }
    }

    score += puntosGanados;
}