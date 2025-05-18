
#pragma once
#include "raylib.h"
#include "GameState.h"

class InteractableObject {
public:
    Rectangle bounds;
    ActionType action;

    bool CheckClicked();
};
