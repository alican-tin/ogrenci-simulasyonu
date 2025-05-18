
#pragma once
#include "GameEvent.h"
#include <vector>

class EventManager {
private:
    std::vector<GameEvent> events;

public:
    void AddEvent(const GameEvent& event);
    void TriggerRandomEvent(EventType type, PlayerStats& player, std::string& message, int& messageTimer);
};
