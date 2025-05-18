
#include "InteractableObject.h"

bool InteractableObject::CheckClicked() {
    return CheckCollisionPointRec(GetMousePosition(), bounds) &&
           IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
