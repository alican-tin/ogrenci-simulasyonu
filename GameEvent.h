
#pragma once
#include "PlayerStats.h"
#include <string>
#include <functional>

enum class EventType {
    GENERIC,
    OUTSIDE,
    EAT,
    SLEEP,
    STUDY,
    SOCIAL,
    SPORTS,
    SCHOOL,
    WORK
};



struct GameEvent {
    std::string description;
    std::function<void(PlayerStats&)> effect;
    EventType type;
    int triggerChance; // 1-100 arası tetiklenme şansı

    GameEvent(const std::string& desc, std::function<void(PlayerStats&)> eff, EventType t, int chance)
        : description(desc), effect(eff), type(t), triggerChance(chance) {}
};

