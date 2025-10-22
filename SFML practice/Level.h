#ifndef LEVEL_H
#define LEVEL_H

#include <string>

enum class ObjectiveType {
    CLEAR_ICE,
    REACH_SCORE,
    COLLECT_GEMS
};

class LevelObjective {
private:
    ObjectiveType type;
    int targetValue;
    int currentValue;
    std::string description;
    int gemType;

public:
    LevelObjective();
    LevelObjective(ObjectiveType objType, int target, const std::string& desc, int gemType = -1);

    void updateProgress(int value);
    bool isCompleted() const;
    void reset();
    ObjectiveType getType() const { return type; }
    int getTarget() const { return targetValue; }
    int getProgress() const { return currentValue; }
    std::string getDescription() const { return description; }
    std::string getProgressText() const;
};

class Level {
private:
    int levelNumber;
    std::string name;
    int moveLimit;
    LevelObjective objectives[3];
    int objectiveCount;
    int iceCount;
    bool completed;

public:
    Level();
    Level(int number, const std::string& levelName, int moves);

    void addObjective(ObjectiveType type, int target, const std::string& desc, int gemType = -1);
    bool checkLevelCompleted() const;
    void updateObjective(ObjectiveType type, int value);
    void reset();
    int getLevelNumber() const { return levelNumber; }
    std::string getName() const { return name; }
    int getMoveLimit() const { return moveLimit; }
    int getIceCount() const { return iceCount; }
    void setIceCount(int count) { iceCount = count; }
    bool isCompleted() const { return completed; }
    void setCompleted(bool comp) { completed = comp; }
    LevelObjective* getObjectives() { return objectives; }
    int getObjectiveCount() const { return objectiveCount; }
};

#endif