#include "Level.h"
#include <sstream>

// Constructor
LevelObjective::LevelObjective()
    : type(ObjectiveType::REACH_SCORE), targetValue(0), currentValue(0),
    description(""), gemType(-1) {
}

LevelObjective::LevelObjective(ObjectiveType objType, int target, const std::string& desc, int gemType)
    : type(objType), targetValue(target), currentValue(0), description(desc), gemType(gemType) {
}

void LevelObjective::updateProgress(int value) {
    currentValue = value;
    if (currentValue > targetValue) {
        currentValue = targetValue;
    }
}

bool LevelObjective::isCompleted() const {
    return currentValue >= targetValue;
}

void LevelObjective::reset() {
    currentValue = 0;
}

std::string LevelObjective::getProgressText() const {
    std::stringstream ss;
    ss << description << ": " << currentValue << "/" << targetValue;
    if (isCompleted()) {
        ss << " ✓";
    }
    return ss.str();
}

// Conctructor
Level::Level()
    : levelNumber(0), name(""), moveLimit(0), objectiveCount(0), iceCount(0), completed(false) {
}

Level::Level(int number, const std::string& levelName, int moves)
    : levelNumber(number), name(levelName), moveLimit(moves),
    objectiveCount(0), iceCount(0), completed(false) {
}

void Level::addObjective(ObjectiveType type, int target, const std::string& desc, int gemType) {
    if (objectiveCount < 3) {
        objectives[objectiveCount] = LevelObjective(type, target, desc, gemType);
        objectiveCount++;
    }
}

bool Level::checkLevelCompleted() const {
    for (int i = 0; i < objectiveCount; i++) {
        if (!objectives[i].isCompleted()) {
            return false;
        }
    }
    return true;
}

void Level::updateObjective(ObjectiveType type, int value) {
    for (int i = 0; i < objectiveCount; i++) {
        if (objectives[i].getType() == type) {
            if (type == ObjectiveType::COLLECT_GEMS) {
                if (objectives[i].getProgress() < value) {
                    objectives[i].updateProgress(value);
                }
            }
            else {
                objectives[i].updateProgress(value);
            }
            break;
        }
    }

    completed = checkLevelCompleted();
}

void Level::reset() {
    for (int i = 0; i < objectiveCount; i++) {
        objectives[i].reset();
    }
    completed = false;
}