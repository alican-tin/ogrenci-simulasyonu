
#include "EventManager.h"
#include "raylib.h"

void EventManager::AddEvent(const GameEvent& event) {
    events.push_back(event);
}

void EventManager::TriggerRandomEvent(EventType type, PlayerStats& player, std::string& message, int& messageTimer) {
    std::vector<GameEvent> filteredEvents;

    for (auto& e : events) {
        if (e.type == type || e.type == EventType::GENERIC) {
            filteredEvents.push_back(e);
        }
    }

    if (filteredEvents.empty()) return;

    // Şans bazlı tetiklenebilir eventleri topla
    std::vector<GameEvent> validEvents;
    for (auto& e : filteredEvents) {
        if (GetRandomValue(1, 100) <= e.triggerChance) {
            validEvents.push_back(e);
        }
    }

    if (validEvents.empty()) {
        message = "Ozel bir sey olmadi.";
        messageTimer = 120;
        return;
    }

    int index = GetRandomValue(0, validEvents.size() - 1);
    GameEvent& selectedEvent = validEvents[index];

    message = selectedEvent.description;
    messageTimer = 180;

    selectedEvent.effect(player);
}

