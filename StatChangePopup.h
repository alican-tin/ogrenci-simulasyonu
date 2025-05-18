
#pragma once
#include "raylib.h"
#include <string>

struct StatChangePopup {
    Vector2 position;
    std::string text;
    Color color;
    float lifetime; // saniye cinsinden kalan süre
    float alpha;    // saydamlık (0-255)

    StatChangePopup(Vector2 pos, const std::string& txt, Color col)
        : position(pos), text(txt), color(col), lifetime(2.0f), alpha(255.0f) {}

    void Update(float deltaTime) {
        lifetime -= deltaTime;
        if (lifetime < 0) lifetime = 0;
        alpha = (lifetime / 2.0f) * 255.0f; // 2 saniyede fade out
    }

    void Draw() const {
        Color drawColor = color;
        drawColor.a = (unsigned char)alpha;
        DrawText(text.c_str(), position.x, position.y, 20, drawColor);
    }

    bool IsDead() const {
        return lifetime <= 0;
    }
};
