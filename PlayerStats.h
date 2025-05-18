#pragma once
#include "PlayerType.h"
#include "ClampUtils.h"
#include <string>

enum class StatType {
    ENERGY,
    HUNGER,
    FUN,
    SOCIAL,
    HEALTH,
    SUCCESS,
    HAPPINESS,
    MONEY
};

class PlayerStats {
public:
    int energy = 80;
    int hunger = 50;
    int fun = 50;
    int social = 50;
    int health = 80;
    int success = 40;
    int happiness = 50;
    int money = 100;
    bool schoolMenuTriggered = false;
    bool examProcessed = false;
    int totalExamScore = 0;
    int examCount = 0;
    float academicAverage = 0.0f;
    int currentTerm = 1;
    int repeatCount = 0;


    int currentDay = 1;
    int currentPeriod = 1;


    int prevEnergy = 80;
    int prevHunger = 50;
    int prevFun = 50;
    int prevSocial = 50;
    int prevHealth = 80;
    int prevSuccess = 40;
    int prevHappiness = 50;
    int prevMoney = 100;

    PlayerType type = PlayerType::NORMAL;

    void CapStats();
    void AdvanceDay(std::string &message, int &messageTimer);

    int ApplyStatModifier(StatType stat, int baseValue);

    void Snapshot();

};
