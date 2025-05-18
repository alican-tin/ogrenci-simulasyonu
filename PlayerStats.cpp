
#include "PlayerStats.h"
#include <string>

void PlayerStats::CapStats() {
    energy = Clamp(energy, 0, 100);
    hunger = Clamp(hunger, 0, 100);
    fun = Clamp(fun, 0, 100);
    social = Clamp(social, 0, 100);
    health = Clamp(health, 0, 100);
    success = Clamp(success, 0, 100);
    happiness = Clamp(happiness, 0, 100);
    money = Clamp(money, 0, 1000);
}

void PlayerStats::AdvanceDay(std::string &message, int &messageTimer) {
    currentDay++;
    energy = 80;
    money += 50; // Haftalık harçlık
    happiness += 1; // Moral toparlanır
    hunger -= 10;   
    fun -= 5;       
    social -= 5;    
    success -= 5;//unttu bir miktar
    if (success < 0) success = 0;
    schoolMenuTriggered = false;
    examProcessed = false;
    message = "Yeni gun basladi! Harclik alindi.";
    if (currentDay % 7 == 6) {//HAFTA SONU YAŞASIN!
        happiness += 5;
        fun += 5;
        social += 5;
    }
    messageTimer = 120;
}

void PlayerStats::Snapshot() {
    prevEnergy = energy;
    prevHunger = hunger;
    prevFun = fun;
    prevSocial = social;
    prevHealth = health;
    prevSuccess = success;
    prevHappiness = happiness;
    prevMoney = money;
}

int PlayerStats::ApplyStatModifier(StatType stat, int baseValue) {
    float modifier = 1.0f;

    if (happiness < 30) {
        modifier *= 0.5f; // Genel moral düşüşü
    }

    // Örnek: PlayerType bonusları
    if (type == PlayerType::NORMAL) {
        // Diğer oyuncu tipleri buraya (bonus/penaltı) eklenir
    }

    // İleride geçici event boostları buraya eklenir. 

    return static_cast<int>(baseValue * modifier);
}
