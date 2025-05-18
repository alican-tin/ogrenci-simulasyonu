
#include "raylib.h"
#include "PlayerStats.h"
#include "InteractableObject.h"
#include "StatChangePopup.h"
#include "GameState.h"
#include "EventManager.h"

#include <string>
#include <algorithm>

// Obje sprite’ları
Texture2D bedIdle,      bedHover;
Texture2D fridgeIdle,   fridgeHover;
Texture2D doorIdle,     doorHover;
Texture2D shelfIdle,    shelfHover;
Texture2D pcIdle,       pcHover;

// Obje tıklama/draw alanları
Rectangle bedBounds;
Rectangle fridgeBounds;
Rectangle doorBounds;
Rectangle shelfBounds;
Rectangle pcBounds;

// Ders çalışma animasyonu
Texture2D studyAnim[3];
int studyAnimFrame = 0;
bool studyAnimPlaying = false;
int studyAnimTimer = 0;
int studyAnimLoops = 0;

// Uyuma animasyonu
Texture2D sleepAnim[3];
int sleepAnimFrame = 0;
bool sleepAnimPlaying = false;
int sleepAnimTimer = 0;
int sleepAnimLoops = 0;

// Yemek animasyonları
Texture2D cheapEatAnim[3];
Texture2D normalEatAnim[4];
Texture2D expensiveEatAnim[4];

int eatAnimFrame = 0;
bool eatAnimPlaying = false;
int eatAnimTimer = 0;
int eatAnimLoops = 0;
enum EatAnimType { NONE, CHEAP, NORMAL, EXPENSIVE } currentEatAnim = NONE;

Texture2D catSpriteSheet;
Rectangle catFrameRec;
int catFrame = 0;
int catTimer = 0;
int catFrameWidth = 0;
int catFrameHeight = 0;






bool CheckButton(Rectangle bounds) {
    return CheckCollisionPointRec(GetMousePosition(), bounds) &&
           IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void ShowRandomEvent(PlayerStats &player, std::string &message, int &messageTimer) {
    int event = GetRandomValue(1, 4);
    switch (event) {
        case 1: message = "Sokakta yeni biriyle tanistin!"; player.social += player.ApplyStatModifier(StatType::SOCIAL, 5); break;
        case 2: message = "Kotu hava enerjini dusurdu."; player.energy -= 10; break;
        case 3: message = "Bir arkadasin seni aradi."; player.happiness += 10; break;
        case 4: message = "Yolda dusup hafif yaralandin."; player.health -= 5; break;
    }
    messageTimer = 120;
}

std::string WrapText(const std::string& text, int maxLineLength) {
    std::string result;
    int lineLength = 0;

    for (size_t i = 0; i < text.length(); ++i) {
        result += text[i];
        lineLength++;

        if (lineLength >= maxLineLength && text[i] == ' ') {
            result += '\n';
            lineLength = 0;
        }
    }

    return result;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Mini oyun için state kontrolü
    CustomState miniGameState = STATE_MAIN_LOOP;

    // Mini oyun için UI butonları
    Rectangle pcMiniGamePromptYes = { 300, 250, 80, 40 };
    Rectangle pcMiniGamePromptNo = { 420, 250, 80, 40 };

    // Mini oyun için dinozor ve engel
    Rectangle dino = {100, 500, 40, 40};
    Rectangle obstacle = {800, 500, 20, 40};
    int score = 0;
    bool isJumping = false;
    int jumpVel = 0;


    InitWindow(screenWidth, screenHeight, "Ogrenci Simulasyonu - Raylib");
    SetExitKey(0);
    SetTargetFPS(60);

    GameState currentState = STATE_MAIN_MENU;
    bool isGameInProgress = false;

    Rectangle newGameBtn = { 300, 200, 200, 40 };
    Rectangle continueBtn = { 300, 260, 200, 40 };
    Rectangle exitBtn = { 300, 320, 200, 40 };

    std::string currentMessage = "";
    int messageTimer = 0;

    PlayerStats player;

    EventManager eventManager;
    
	std::vector<StatChangePopup> activePopups;


    Rectangle yesBtn = { 300, 250, 80, 40 };
    Rectangle noBtn = { 420, 250, 80, 40 };

    Rectangle outsideBtns[4] = {
        {300, 150, 200, 40}, {300, 200, 200, 40},
        {300, 250, 200, 40}, {300, 300, 200, 40}
    };

    Rectangle eatBtns[3] = {
        {300, 200, 200, 40}, {300, 250, 200, 40}, {300, 300, 200, 40}
    };

    Texture2D roomBg = LoadTexture("assets/backgrounds/ArkaPlan.png");

    // Hover Overlay görselleri (hepsi 800x600 boyutunda, transparan yerler alpha=0 olacak)
    Texture2D bedHover    = LoadTexture("assets/objects/Yatak_hover.png");
    Texture2D fridgeHover = LoadTexture("assets/objects/Dolap_hover.png");
    Texture2D doorHover   = LoadTexture("assets/objects/Kapi_hover.png");
    Texture2D shelfHover  = LoadTexture("assets/objects/Kitap_hover1.png");
    Texture2D pcHover     = LoadTexture("assets/objects/Pc_hover.png");
    Texture2D bedIdle     = LoadTexture("assets/objects/Yatak_idle.png");
    Texture2D fridgeIdle  = LoadTexture("assets/objects/Dolap_idle.png");
    Texture2D doorIdle    = LoadTexture("assets/objects/Kapi_idle.png");
    Texture2D shelfIdle   = LoadTexture("assets/objects/Kitap_idle.png");
    Texture2D pcIdle      = LoadTexture("assets/objects/Pc_idle.png");
    
    
    Rectangle bedBounds    = {  155, 400, 150, 70 };
    Rectangle fridgeBounds = { 243, 344,  43, 53 };
    Rectangle doorBounds   = { 455, 315,  50, 90 };
    Rectangle shelfBounds  = { 405, 225,  60, 60 };
    Rectangle pcBounds     = { 356, 289, 79, 60 };

    // Study anim frames
    studyAnim[0] = LoadTexture("assets/animations/Calis1.png");
    studyAnim[1] = LoadTexture("assets/animations/Calis2.png");
    studyAnim[2] = LoadTexture("assets/animations/Calis3.png");

    // Sleep anim frames
    sleepAnim[0] = LoadTexture("assets/animations/Uyku1.png");
    sleepAnim[1] = LoadTexture("assets/animations/Uyku2.png");
    sleepAnim[2] = LoadTexture("assets/animations/Uyku3.png");

    // Cheap Eat anim frames
    cheapEatAnim[0] = LoadTexture("assets/animations/UcuzYe1.png");
    cheapEatAnim[1] = LoadTexture("assets/animations/UcuzYe2.png");
    cheapEatAnim[2] = LoadTexture("assets/animations/UcuzYe3.png");

    // Normal Eat anim frames
    normalEatAnim[0] = LoadTexture("assets/animations/NormalYe1.png");
    normalEatAnim[1] = LoadTexture("assets/animations/NormalYe2.png");
    normalEatAnim[2] = LoadTexture("assets/animations/NormalYe3.png");
    normalEatAnim[3] = LoadTexture("assets/animations/NormalYe4.png");

    // Expensive Eat anim frames
    expensiveEatAnim[0] = LoadTexture("assets/animations/PahaliYe1.png");
    expensiveEatAnim[1] = LoadTexture("assets/animations/PahaliYe2.png");
    expensiveEatAnim[2] = LoadTexture("assets/animations/PahaliYe3.png");
    expensiveEatAnim[3] = LoadTexture("assets/animations/PahaliYe4.png");

    // Kedi sprite sheet
    catSpriteSheet = LoadTexture("assets/animations/Box3.png");
    catFrameWidth = catSpriteSheet.width / 4;   // 4 kare var
    catFrameHeight = catSpriteSheet.height;
    catFrameRec = { 0, 0, (float)catFrameWidth, (float)catFrameHeight };


    eventManager.AddEvent({ "Spor yaparken ayaginizi burktunuz.", [](PlayerStats& p) { p.health -= 10; }, EventType::SPORTS, 20 });
    eventManager.AddEvent({ "Spor sonrası kendinizi harika hissediyorsunuz!", [](PlayerStats& p) { p.happiness += 10; }, EventType::SPORTS, 30 });
    eventManager.AddEvent({ "Antrenman arkadasinizla tanistiniz.", [](PlayerStats& p) { p.social += 10; }, EventType::SPORTS, 25 });

    eventManager.AddEvent({ "Yeni bir arkadas edindiniz.", [](PlayerStats& p) { p.social += 15; }, EventType::SOCIAL, 30 });
    eventManager.AddEvent({ "Kotu bir sohbete maruz kaldiniz, modunuz dustu.", [](PlayerStats& p) { p.happiness -= 10; }, EventType::SOCIAL, 20 });
    eventManager.AddEvent({ "Guzel bir kafede zaman gecirdiniz.", [](PlayerStats& p) { p.happiness += 10; p.money -= 10; }, EventType::SOCIAL, 25 });
    eventManager.AddEvent({ "Arkadaslarinizla tartıştınız.", [](PlayerStats& p) { p.social -= 10; }, EventType::SOCIAL, 10 });

    eventManager.AddEvent({ "Patron size prim verdi!", [](PlayerStats& p) { p.money += 50; }, EventType::WORK, 15 });
    eventManager.AddEvent({ "İşten erken çıktınız, mutlu oldunuz.", [](PlayerStats& p) { p.happiness += 10; }, EventType::WORK, 5 });
    eventManager.AddEvent({ "Zor bir gün geçirdiniz, enerjiniz dustu.", [](PlayerStats& p) { p.energy -= 10; }, EventType::WORK, 15 });
    eventManager.AddEvent({ "İş arkadaşınızla tartıştınız.", [](PlayerStats& p) { p.social -= 10; }, EventType::WORK, 5 });

    eventManager.AddEvent({ "Yedigim yemek cok lezzetliydi, modum yukseldi.", [](PlayerStats& p) { p.happiness += 10; }, EventType::EAT, 25 });
    eventManager.AddEvent({ "Yediklerim midemi bozdu.", [](PlayerStats& p) { p.health -= 10; }, EventType::EAT, 20 });
    eventManager.AddEvent({ "Garson size ikram getirdi, moraliniz arttı.", [](PlayerStats& p) { p.happiness += 15; }, EventType::EAT, 10 });

    eventManager.AddEvent({ "Sokakta tanimadiginiz biriyle ilginç bir muhabbet ettiniz.", [](PlayerStats& p) { p.social += 10; }, EventType::OUTSIDE, 20 });

    eventManager.AddEvent({ "Kabus gördünüz.", [](PlayerStats& p) { p.happiness -= 10; p.energy -= 5; }, EventType::SLEEP, 5 });
    eventManager.AddEvent({ "Güzel bir rüya gördünüz.", [](PlayerStats& p) { p.happiness += 10; p.energy += 5; }, EventType::SLEEP, 10 });

    eventManager.AddEvent({ "Okulda yeni bir arkadaş edindiniz.", [](PlayerStats& p) { p.social += 10; }, EventType::SCHOOL, 20 });
    eventManager.AddEvent({ "Hoca zor bir konuya başladı.", [](PlayerStats& p) { p.energy -= 10; }, EventType::SCHOOL, 10 });
    eventManager.AddEvent({ "Okulda bir etkinlik var, güzel zaman geçirdin", [](PlayerStats& p) { p.fun += 10; }, EventType::SCHOOL, 5 });

    


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentState == STATE_MAIN_MENU) {
            DrawText("=== OGRENCI SIMULASYONU ===", 220, 100, 20, BLACK);
            DrawRectangleRec(newGameBtn, LIGHTGRAY);
            DrawText("Yeni Oyun", newGameBtn.x + 50, newGameBtn.y + 10, 20, BLACK);
            DrawRectangleRec(continueBtn, (isGameInProgress ? LIGHTGRAY : GRAY));
            DrawText("Devam Et", continueBtn.x + 50, continueBtn.y + 10, 20, BLACK);
            DrawRectangleRec(exitBtn, LIGHTGRAY);
            DrawText("Cikis", exitBtn.x + 70, exitBtn.y + 10, 20, BLACK);

            if (CheckButton(newGameBtn)) { isGameInProgress = true; currentState = STATE_PLAYING; }
            if (CheckButton(continueBtn) && isGameInProgress) { currentState = STATE_PLAYING; }
            if (CheckButton(exitBtn)) { currentState = STATE_EXIT; }
        }

        else if (currentState == STATE_SLEEP_ANIMATION) {
            DrawTexture(roomBg, 0, 0, WHITE);

            // Uyuma animasyonu oynat
            if (sleepAnimPlaying) {
                DrawTexture(sleepAnim[sleepAnimFrame], 0, 0, WHITE);
                sleepAnimTimer++;
                if (sleepAnimTimer > 10) {
                    sleepAnimTimer = 0;
                    sleepAnimFrame++;
                    if (sleepAnimFrame >= 3) {
                        sleepAnimFrame = 0;
                        sleepAnimLoops++;
                        if (sleepAnimLoops >= 2) {
                            sleepAnimPlaying = false;
                            currentState = STATE_PLAYING;
                        }
                    }
                }
            }

        }


        else if (currentState == STATE_PLAYING) {
            DrawTexture(roomBg, 0, 0, WHITE);
            DrawRectangle(0, 0, 800, 100, (Color){200, 200, 200, 230});
            DrawText(TextFormat("GUN: %d  PARA: %d", player.currentDay, player.money), 20, 10, 20, BLACK);
            DrawText(TextFormat("ENERJI: %d", player.energy), 20, 40, 20, BLACK);
            DrawText(TextFormat("ACLIK: %d", player.hunger), 20, 70, 20, BLACK);
            DrawText(TextFormat("EGLENCE: %d", player.fun), 200, 40, 20, BLACK);
            DrawText(TextFormat("SOSYAL: %d", player.social), 200, 70, 20, BLACK);
            DrawText(TextFormat("BASARI: %d", player.success), 400, 40, 20, BLACK);
            DrawText(TextFormat("MUTLULUK: %d", player.happiness), 400, 70, 20, BLACK);
            DrawText(TextFormat("SAGLIK: %d", player.health), 600, 40, 20, BLACK);
            DrawText(TextFormat("ORTALAMA: %.1f", player.academicAverage), 600, 70, 20, BLACK);
            DrawText(TextFormat("DONEM: %d", player.currentTerm), 600, 10, 20, BLACK);

            // ——— Obje hover/idle çizimi & tıklama ———
            Vector2 mpos = GetMousePosition();

            // YATAK
            if (CheckCollisionPointRec(mpos, bedBounds)) {
                DrawTexture(bedHover, 0, 0, WHITE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentState = STATE_CONFIRM_SLEEP;
                }
            } else {
                DrawTexture(bedIdle, 0, 0, WHITE);
            }

            // DOLAP (YEMEK)
            if (CheckCollisionPointRec(mpos, fridgeBounds)) {
                DrawTexture(fridgeHover, 0, 0, WHITE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentState = STATE_EAT_MENU;
                }
            } else {
                DrawTexture(fridgeIdle, 0, 0, WHITE);
            }

            // KAPI (DIŞARI)
            if (CheckCollisionPointRec(mpos, doorBounds)) {
                DrawTexture(doorHover, 0, 0, WHITE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentState = STATE_OUTSIDE_MENU;
                }
            } else {
                DrawTexture(doorIdle, 0, 0, WHITE);
            }

            // KİTAPLIK
            if (CheckCollisionPointRec(mpos, shelfBounds)) {
                DrawTexture(shelfHover, 0, 0, WHITE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                    // Sosyallik veya eğlence çok düşükse ders çalışamaz
                    if (player.fun < 20) {
                        currentMessage = "Cok sikildim, ders calismak istemiyorum.";
                        messageTimer = 180;
                    }

                    else if (player.social < 20) {
                        currentMessage = "Arkadaslarimi ozledim. Hayatta dersten baska seyler de var.";
                        messageTimer = 180;
                    }

                    
                    else{
                        player.success += player.ApplyStatModifier(StatType::SUCCESS, 15);
                        player.fun += player.ApplyStatModifier(StatType::FUN, -10);
                        player.energy -= 10;

                        currentMessage = "Ders calistiniz!";
                        messageTimer = 120;

                        studyAnimPlaying = true;
                        studyAnimFrame = 0;
                        studyAnimTimer = 0; 
                        studyAnimLoops = 0;
                    }
                }
            } else {
                DrawTexture(shelfIdle, 0, 0, WHITE);
            }



            // PC
            if (CheckCollisionPointRec(mpos, pcBounds)) {
                DrawTexture(pcHover, 0, 0, WHITE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    miniGameState = STATE_CONFIRM_MINIGAME;
                }
            } else {
                DrawTexture(pcIdle, 0, 0, WHITE);
            }

            /* ————————————————————————————————hitbox test kareleri:

            DrawRectangleLines(bedBounds.x, bedBounds.y, bedBounds.width, bedBounds.height, RED);
            DrawRectangleLines(fridgeBounds.x, fridgeBounds.y, fridgeBounds.width, fridgeBounds.height, RED);
            DrawRectangleLines(doorBounds.x, doorBounds.y, doorBounds.width, doorBounds.height, RED);
            DrawRectangleLines(shelfBounds.x, shelfBounds.y, shelfBounds.width, shelfBounds.height, RED);
            DrawRectangleLines(pcBounds.x, pcBounds.y, pcBounds.width, pcBounds.height, RED);
            ————————————————————————————————*/ 



            if (player.happiness < 30 && messageTimer <= 0) {
                currentMessage = "Mutlulugun cok dusuk! Depresyona girebilirsin!";
                messageTimer = 180;
            }
                //HENÜZ İMPLEMANTE DEĞİLLER.İLERİDE EKLENEBİLİR ETKİLERİ
                /*
            if (player.hunger < 20 && messageTimer <= 0) {
                currentMessage = "Açlik seviyen çok düşük! Sagligin etkilenebilir.";
                messageTimer = 180;
            }
            
            if (player.fun < 20 && messageTimer <= 0) {
                currentMessage = "Eğlence seviyen çok düşük! Eğlenmelisin.";
                messageTimer = 180;
            }
            if (player.social < 20 && messageTimer <= 0) {
                currentMessage = "Sosyal seviyen çok düşük! Sosyalleşmelisin.";
                messageTimer = 180;
            }
            if (player.health < 20 && messageTimer <= 0) {
                currentMessage = "Sağlık seviyen çok düşük! Dikkat etmelisin.";
                messageTimer = 180;
            }
                */
            


            if (IsKeyPressed(KEY_ESCAPE)) currentState = STATE_MAIN_MENU;
            if (player.energy <= 0) {
                player.AdvanceDay(currentMessage, messageTimer);
                eventManager.TriggerRandomEvent(EventType::SLEEP, player, currentMessage, messageTimer);

                if (player.hunger <= 0) {
                    player.health -= 5;
                    currentMessage = "Açlıktan zayıfladın!";
                    messageTimer = 120;
                }
                if (player.fun <= 0) {
                    player.happiness -= 5;
                    currentMessage = "Eğlence seviyen çok düşük!";
                    messageTimer = 120;
                }
                if (player.social <= 0) {
                    player.happiness -= 5;
                    currentMessage = "Sosyal seviyen çok düşük!";
                    messageTimer = 120;
                }

                sleepAnimPlaying = true;
                sleepAnimFrame = 0;
                sleepAnimTimer = 0;
                sleepAnimLoops = 0;


                currentState = STATE_SLEEP_ANIMATION;

            }
            

            if (player.currentDay % 7 != 0 && !player.schoolMenuTriggered) { // Hafta içi günlerinde
                player.schoolMenuTriggered = true;
                currentState = STATE_SCHOOL_MENU;
            }

            if (player.currentDay % 7 == 0 && !player.examProcessed) { // Sınav günü
                player.examProcessed = true;
                int examResult = player.success + GetRandomValue(-10, 10); // Başarıya bağlı ama rastgele sapma
                if (examResult > 100) examResult = 100;
                if (examResult < 0) examResult = 0;
                if (examResult >= 75) {
                    player.happiness += 10;
                    currentMessage = TextFormat("Sinav basarili! Notunuz: %d", examResult);
                    player.currentTerm++; // Başarılıysa dönem geçer
                }
                else if (examResult >= 50) {
                    player.happiness += 0; // Zar zor geçer
                    currentMessage = TextFormat("Sinav zorlu gecti ama gectin! Notunuz: %d", examResult);
                    player.currentTerm++; // Yine dönem geçer ama moral bozulmaz/artmaz
                }
                else {
                    player.happiness -= 10;
                    currentMessage = TextFormat("Sinav basarisiz! Dönemi tekrar edeceksin! Notunuz: %d", examResult);
                    player.repeatCount++;// currentTerm artmaz → dönem tekrarı
                }


                player.totalExamScore += examResult;
                player.examCount++;
                player.success = 40; // Yeni döneme başlangıç seviyesi


                player.academicAverage = (float)player.totalExamScore / player.examCount;

                messageTimer = 180;
            }

            if (player.currentTerm > 8) {
                if (player.academicAverage >= 85) {
                    currentMessage = "Onur Belgesi ile Mezun Oldun!";
                } else if (player.academicAverage >= 70) {
                    currentMessage = "Iyi bir dereceyle mezun oldun.";
                } else if (player.academicAverage >= 50) {
                    currentMessage = "Zar zor mezun oldun, ama basardin.";
                } else {
                    currentMessage = "Diploman var ama ici bos... zor bela mezun oldun.";
                }

                messageTimer = 300;
                currentState = STATE_EXIT;
            }
            else if (player.repeatCount >= 4) {
                currentMessage = "Okuldan atildin! Mezun olamadin.";
                messageTimer = 300;
                currentState = STATE_EXIT;
            }


            if (player.happiness == 0) {
                currentMessage = "Depresyona girdin! Oyun bitti.";
                messageTimer = 300;
                currentState = STATE_EXIT;
            }
            if (player.health == 0) {
                currentMessage = "Saglik sorunlarindan dolayi hastaneye kaldirildin! Oyun bitti.";
                messageTimer = 300;
                currentState = STATE_EXIT;
            }


            // Mini oyun soru ekranı
            if (miniGameState == STATE_CONFIRM_MINIGAME) {
                DrawRectangle(250, 200, 340, 200, DARKGRAY);
                DrawText("Mini oyun oynamak ister misin?", 270, 220, 20, WHITE);
                DrawRectangleRec(pcMiniGamePromptYes, LIGHTGRAY);
                DrawText("Evet", pcMiniGamePromptYes.x + 10, pcMiniGamePromptYes.y + 10, 20, BLACK);
                DrawRectangleRec(pcMiniGamePromptNo, LIGHTGRAY);
                DrawText("Hayir", pcMiniGamePromptNo.x + 10, pcMiniGamePromptNo.y + 10, 20, BLACK);

                if (CheckButton(pcMiniGamePromptYes)) { 
                    miniGameState = STATE_MINIGAME; 
                    score = 0; 
                    dino.y = 500; 
                    obstacle.x = 800; 
                    isJumping = false; 
                }
                if (CheckButton(pcMiniGamePromptNo)) { 
                    player.fun += player.ApplyStatModifier(StatType::FUN, 10); 
                    currentMessage = "Direkt oyun oynadiniz!"; 
                    messageTimer = 120; 
                    miniGameState = STATE_MAIN_LOOP; 
                }
            }

            // Mini oyun ekranı
            if (miniGameState == STATE_MINIGAME) {
                ClearBackground(RAYWHITE);
                DrawText("Mini Oyun: Dinozor Runner", 300, 50, 20, DARKGRAY);
                DrawText(TextFormat("Skor: %d", score), 20, 20, 20, BLACK);
                DrawRectangleRec(dino, DARKGRAY);
                DrawRectangleRec(obstacle, RED);

                obstacle.x -= 10;
                if (obstacle.x < -20) {
                    obstacle.x = 800;
                    score += 1;
                }

                if (isJumping) {
                    dino.y += jumpVel;
                    jumpVel += 2;
                    if (dino.y >= 500) {
                        dino.y = 500;
                        isJumping = false;
                    }
                }

                if (IsKeyPressed(KEY_SPACE) && !isJumping) {
                    isJumping = true;
                    jumpVel = -20;
                }

                if (CheckCollisionRecs(dino, obstacle)) {
                    currentMessage = TextFormat("Oyun bitti! Skor: %d", score);
                    player.fun += player.ApplyStatModifier(StatType::FUN, score * 5);
                    messageTimer = 180;
                    miniGameState = STATE_MAIN_LOOP;
                }

                // Mini oyun sırasında diğer şeyleri dondur:
                EndDrawing();
                continue;
            }

            // Eğer mini oyun veya soru ekranı aktifse, ana oyun loopunu dondur
            if (miniGameState != STATE_MAIN_LOOP) {
                EndDrawing();
                continue;
            }



#define CHECK_AND_POPUP(current, previous, posX, posY, displayName, offsetX, offsetY) \
            if (player.current != player.previous) { \
                int diff = player.current - player.previous; \
                Color c = (diff < 0) ? RED : GREEN; \
                std::string text = std::string(displayName) + (diff > 0 ? " +" : " ") + std::to_string(diff); \
                activePopups.push_back(StatChangePopup({ posX + offsetX, posY + offsetY }, text, c)); \
            }







            CHECK_AND_POPUP(health, prevHealth, 600, 40, "Health", 0, -15);
            CHECK_AND_POPUP(happiness, prevHappiness, 400, 70, "Happiness", 0, -15);
            CHECK_AND_POPUP(social, prevSocial, 200, 70, "Social", 0, -15);
            CHECK_AND_POPUP(fun, prevFun, 200, 40, "Fun", 0, -15);
            CHECK_AND_POPUP(energy, prevEnergy, 20, 40, "Energy", 0, -15);
            CHECK_AND_POPUP(hunger, prevHunger, 20, 70, "Hunger", 0, -15);
            CHECK_AND_POPUP(success, prevSuccess, 400, 40, "Success", 0, -15); 
            CHECK_AND_POPUP(money, prevMoney, 600, 70, "Money", 15, 0);       // para üstte, sağa kaydır
			
			            player.Snapshot();
			
			
			            if (messageTimer > 0) {
                std::string wrappedMessage = WrapText(currentMessage, 30);

                int lineCount = 1;
                for (char c : wrappedMessage) {
                    if (c == '\n') lineCount++;
                }

                int boxHeight = std::max(50, lineCount * 25);

                DrawRectangle(200, 500, 400, boxHeight, (Color){0,0,0,150});
                DrawText(wrappedMessage.c_str(), 220, 510, 20, WHITE);

                messageTimer--;
            }


            // Ders Çalışma Animasyonu
            if (studyAnimPlaying) {
                DrawTexture(studyAnim[studyAnimFrame], 0, 0, WHITE);

                studyAnimTimer++;
                if (studyAnimTimer > 10) {
                    studyAnimTimer = 0;
                    studyAnimFrame++;
                    if (studyAnimFrame >= 3) {
                        studyAnimFrame = 0;
                        studyAnimLoops++;
                        if (studyAnimLoops >= 2) {
                            studyAnimPlaying = false;
                        }
                    }
                }
            }

            // Uyuma Animasyonu
            if (sleepAnimPlaying) {
                DrawTexture(sleepAnim[sleepAnimFrame], 0, 0, WHITE);
                sleepAnimTimer++;

                if (sleepAnimTimer > 10) {
                    sleepAnimTimer = 0;
                    sleepAnimFrame++;
                    if (sleepAnimFrame >= 3) {
                        sleepAnimPlaying = false;
                    }
                }
            }



            // Yemek Animasyonu
            if (eatAnimPlaying) {
                Texture2D *animArray = nullptr;
                int animLength = 0;
                int delay = 10;

                if (currentEatAnim == CHEAP) {
                    animArray = cheapEatAnim;
                    animLength = 3;
                    delay = 20; // Yavaşlatılmış
                } else if (currentEatAnim == NORMAL) {
                    animArray = normalEatAnim;
                    animLength = 4;
                } else if (currentEatAnim == EXPENSIVE) {
                    animArray = expensiveEatAnim;
                    animLength = 4;
                }

                DrawTexture(animArray[eatAnimFrame], 0, 0, WHITE);

                eatAnimTimer++;
                if (eatAnimTimer > delay) {
                    eatAnimTimer = 0;
                    eatAnimFrame++;
                    if (eatAnimFrame >= animLength) {
                        eatAnimFrame = 0;
                        eatAnimLoops++;
                        if (eatAnimLoops >= 2) {
                            eatAnimPlaying = false;
                            currentEatAnim = NONE;
                        }
                    }
                }
            }

            // Kedi animasyonu
            Vector2 catPos = { 500, 415 }; // Pozisyonu

            DrawTextureRec(catSpriteSheet, catFrameRec, catPos, WHITE);

            // Timer
            catTimer++;
            if (catTimer > 10) { // 10 frame'de bir değişim
                catTimer = 0;
                catFrame++;
                if (catFrame > 3) catFrame = 0;

                catFrameRec.x = (float)catFrame * catFrameWidth; // yeni kare
            }
        }

        else if (currentState == STATE_SCHOOL_MENU) {
            DrawRectangle(250, 200, 300, 200, DARKGRAY);
            DrawText("Okul Secimi:", 270, 210, 20, WHITE);

            Rectangle focusBtn = { 300, 250, 200, 40 };
            Rectangle chillBtn = { 300, 300, 200, 40 };
            Rectangle skipBtn = { 300, 350, 200, 40 };

            DrawRectangleRec(focusBtn, LIGHTGRAY);
            DrawText("Derslere Odaklan", focusBtn.x + 10, focusBtn.y + 10, 20, BLACK);

            DrawRectangleRec(chillBtn, LIGHTGRAY);
            DrawText("Arkadaslarla Takil", chillBtn.x + 10, chillBtn.y + 10, 20, BLACK);

            DrawRectangleRec(skipBtn, LIGHTGRAY);
            DrawText("Okulu As", skipBtn.x + 10, skipBtn.y + 10, 20, BLACK);

            if (CheckButton(focusBtn)) {
                player.energy -= 20;
                player.fun += player.ApplyStatModifier(StatType::FUN, -10);
                player.social += player.ApplyStatModifier(StatType::SOCIAL, -10);
                player.success += player.ApplyStatModifier(StatType::SUCCESS, 15);
                currentState = STATE_PLAYING;
            }

            if (CheckButton(chillBtn)) {
                player.energy -= 10;
                player.fun += player.ApplyStatModifier(StatType::FUN, 10);
                player.social += player.ApplyStatModifier(StatType::SOCIAL, 10);
                // success artmaz
                currentState = STATE_PLAYING;
            }

            if (CheckButton(skipBtn)) {
                // energy harcamaz, serbest zaman kazanır
                player.success += player.ApplyStatModifier(StatType::SUCCESS, -10);
                currentState = STATE_PLAYING;
            }
        }


        else if (currentState == STATE_CONFIRM_SLEEP) {
            DrawRectangle(250, 200, 330, 200, DARKGRAY);
            DrawText("Erken yatmak istiyor musun?", 270, 220, 20, WHITE);
            DrawRectangleRec(yesBtn, LIGHTGRAY);
            DrawText("Evet", yesBtn.x + 10, yesBtn.y + 10, 20, BLACK);
            DrawRectangleRec(noBtn, LIGHTGRAY);
            DrawText("Hayir", noBtn.x + 10, noBtn.y + 10, 20, BLACK);

            if (CheckButton(yesBtn)) { 
                player.AdvanceDay(currentMessage, messageTimer); 
                eventManager.TriggerRandomEvent(EventType::SLEEP, player, currentMessage, messageTimer);


                if (player.hunger < 10) {
                    player.health -= 5;
                    currentMessage = "Açlıktan zayıfladın!";
                    messageTimer = 120;
                }
                if (player.fun < 10) {
                    player.happiness -= 5;
                    currentMessage = "Eğlence seviyen çok düşük!";
                    messageTimer = 120;
                }
                if (player.social < 10) {
                    player.happiness -= 5;
                    currentMessage = "Sosyal seviyen çok düşük!";
                    messageTimer = 120;
                }

                sleepAnimPlaying = true;
                sleepAnimFrame = 0;
                sleepAnimTimer = 0;
                sleepAnimLoops = 0;

                currentState = STATE_SLEEP_ANIMATION;           
				}
            if (CheckButton(noBtn)) { currentState = STATE_PLAYING; }
        }

        else if (currentState == STATE_EAT_MENU) {
            DrawRectangle(250, 160, 300, 200, DARKGRAY);
            DrawText("Yemek Secimi:", 270, 180, 20, WHITE);
            DrawRectangleRec(eatBtns[0], LIGHTGRAY);
            DrawText("Ucuz Yemek", eatBtns[0].x + 10, eatBtns[0].y + 10, 20, BLACK);
            DrawRectangleRec(eatBtns[1], LIGHTGRAY);
            DrawText("Normal Yemek", eatBtns[1].x + 10, eatBtns[1].y + 10, 20, BLACK);
            DrawRectangleRec(eatBtns[2], LIGHTGRAY);
            DrawText("Pahali Yemek", eatBtns[2].x + 10, eatBtns[2].y + 10, 20, BLACK);

            if (CheckButton(eatBtns[0])) {
                player.hunger += 20;
                player.health -= 5;
                player.money -= 10;
                currentMessage = "Ucuz yemek yediniz!";
                messageTimer = 120;

                // Ucuz yemek animasyonu başlat
                currentEatAnim = CHEAP;
                eatAnimPlaying = true;
                eatAnimFrame = 0;
                eatAnimTimer = 0;
                eatAnimLoops = 0;

                currentState = STATE_PLAYING;
            }

            if (CheckButton(eatBtns[1])) {
                player.hunger += 30;
                player.money -= 20;
                currentMessage = "Normal yemek yediniz!";
                messageTimer = 120;

                // Normal yemek animasyonu başlat
                currentEatAnim = NORMAL;
                eatAnimPlaying = true;
                eatAnimFrame = 0;
                eatAnimTimer = 0;
                eatAnimLoops = 0;

                currentState = STATE_PLAYING;
            }

            if (CheckButton(eatBtns[2])) {
                player.hunger += 40;
                player.health += 5;
                player.money -= 30;
                currentMessage = "Pahali yemek yediniz!";
                messageTimer = 120;

                // Pahalı yemek animasyonu başlat
                currentEatAnim = EXPENSIVE;
                eatAnimPlaying = true;
                eatAnimFrame = 0;
                eatAnimTimer = 0;
                eatAnimLoops = 0;

                currentState = STATE_PLAYING;
            }

        }

        else if (currentState == STATE_OUTSIDE_MENU) {
            DrawRectangle(250, 100, 300, 250, DARKGRAY);
            DrawText("Disari Aktiviteleri:", 270, 110, 20, WHITE);

            const char* labels[] = { "Spor Yap", "Sosyalles", "Part-time Is", "Disarida Yemek" };
            for (int i = 0; i < 4; i++) {
                DrawRectangleRec(outsideBtns[i], LIGHTGRAY);
                DrawText(labels[i], outsideBtns[i].x + 10, outsideBtns[i].y + 10, 20, BLACK);
            }

            if (CheckButton(outsideBtns[0])) { // Spor Yap
                player.health += 10;
                player.energy -= 20;
                player.money -= 20;

                eventManager.TriggerRandomEvent(EventType::SPORTS, player, currentMessage, messageTimer);

                currentState = STATE_PLAYING;
            }

            if (CheckButton(outsideBtns[1])) { // Sosyalles
                player.social += player.ApplyStatModifier(StatType::SOCIAL, 15);
                player.energy -= 15;
                player.money -= 15;

                eventManager.TriggerRandomEvent(EventType::SOCIAL, player, currentMessage, messageTimer);

                currentState = STATE_PLAYING;
            }

            if (CheckButton(outsideBtns[2])) { // Part-time is
                player.happiness -= 10;
                player.energy -= 20;
                player.money += 30;

                eventManager.TriggerRandomEvent(EventType::WORK, player, currentMessage, messageTimer);

                currentState = STATE_PLAYING;
            }

            if (CheckButton(outsideBtns[3])) { // Disarida Yemek
                player.hunger += 40;
                player.energy -= 5;
                player.money -= 30;

                eventManager.TriggerRandomEvent(EventType::EAT, player, currentMessage, messageTimer);

                currentState = STATE_PLAYING;
            }


        }

        player.CapStats();

        float deltaTime = GetFrameTime();
        for (auto& popup : activePopups) {
            popup.Update(deltaTime);
        }

        activePopups.erase(
            std::remove_if(activePopups.begin(), activePopups.end(), [](const StatChangePopup& p) {
                return p.IsDead();
            }), activePopups.end()
        );

        for (const auto& popup : activePopups) {
            popup.Draw();
        }


        EndDrawing();

        if (currentState == STATE_EXIT) break;
    }

        // … diğer unload’lar …
    UnloadTexture(bedIdle);    UnloadTexture(bedHover);
    UnloadTexture(fridgeIdle); UnloadTexture(fridgeHover);
    UnloadTexture(doorIdle);   UnloadTexture(doorHover);
    UnloadTexture(shelfIdle);  UnloadTexture(shelfHover);
    UnloadTexture(pcIdle);     UnloadTexture(pcHover);
    UnloadTexture(catSpriteSheet);
    UnloadTexture(studyAnim[0]); UnloadTexture(studyAnim[1]); UnloadTexture(studyAnim[2]);



    UnloadTexture(roomBg);

    CloseWindow();
    return 0;
}
