#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "Player.hpp" // [CHECK] ดึงข้อมูลตัวละคร 7.5x มาใช้
#include "Enemy.hpp"  // [CHECK] ดึงสมอง AI และประเภทศัตรูมาใช้
#include "Bomb.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 1000;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SKY WARRIOR: MODULAR EDITION");
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(NULL)));

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) std::cout << "Error: arial.ttf not found" << std::endl;

    // --- LOAD ASSETS ---
    sf::Texture p1, p2, bTex, eTex, rTex, bgTex, btnTex, hTex;
    p1.loadFromFile("assets/pc-walk-up-1.png");
    p2.loadFromFile("assets/pc-walk-up-2.png");
    bTex.loadFromFile("assets/bullet.png");
    eTex.loadFromFile("assets/enemy.png");
    rTex.loadFromFile("assets/rock.png");
    bgTex.loadFromFile("assets/map.png");
    btnTex.loadFromFile("assets/button_start.png");
    hTex.loadFromFile("assets/heart.png");

    // --- SETUP OBJECTS ---
    Player player; // [CHECK] เรียกใช้จาก Player.hpp
    player.init(p1); 

    std::vector<Enemy> enemies; // [CHECK] เรียกใช้จาก Enemy.hpp
    std::vector<sf::Sprite> bullets;

    sf::Sprite bg1(bgTex), bg2(bgTex);
    float bgScaleX = (float)WINDOW_WIDTH / bgTex.getSize().x;
    float bgScaleY = (float)WINDOW_HEIGHT / bgTex.getSize().y;
    bg1.setScale(bgScaleX, bgScaleY); bg2.setScale(bgScaleX, bgScaleY);
    bg2.setPosition(0, -WINDOW_HEIGHT);

    sf::Sprite btnStart(btnTex);
    btnStart.setScale(0.35f, 0.35f);
    btnStart.setOrigin(btnStart.getLocalBounds().width/2, btnStart.getLocalBounds().height/2);
    btnStart.setPosition(WINDOW_WIDTH/2, 650);

    sf::Sprite heartUI(hTex);
    heartUI.setScale(0.8f, 0.8f);

    // --- GAME STATE ---
    bool isGameRunning = false, isGameOver = false;
    int score = 0, spawnTimer = 0, shootTimer = 0, rockCooldown = 0, combo = 0;
    float lastHitTime = 0.0f;
    bool rapidFire = false, freezeSkillActive = false;
    float rapidFireEnd = 0.0f, freezeSkillEnd = 0.0f;
    sf::Clock gameClock;

    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setPosition(10, 10);
    sf::Text comboText("", font, 40);
    comboText.setFillColor(sf::Color::Yellow);

    // ================= LOOP GAME =================
    while (window.isOpen()) {
        float currentTime = gameClock.getElapsedTime().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(sf::Mouse::getPosition(window));
                if ((!isGameRunning || isGameOver) && btnStart.getGlobalBounds().contains(mousePos)) {
                    isGameRunning = true; isGameOver = false; score = 0; combo = 0;
                    player.hp = 3; player.iFrames = 0;
                    enemies.clear(); bullets.clear();
                    player.sprite.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 150);
                    gameClock.restart();
                }
            }
        }

        if (isGameRunning && !isGameOver) {
            // [CHECK] Map ไหลตลอดเวลา
            bg1.move(0, 3); bg2.move(0, 3);
            if (bg1.getPosition().y >= WINDOW_HEIGHT) bg1.setPosition(0, -WINDOW_HEIGHT);
            if (bg2.getPosition().y >= WINDOW_HEIGHT) bg2.setPosition(0, -WINDOW_HEIGHT);

            // [CHECK] เรียกคำสั่งจาก Player.hpp
            player.handleInput(WINDOW_WIDTH, WINDOW_HEIGHT);
            player.updateAnimation(p1, p2);

            if (rockCooldown > 0) rockCooldown--;
            if (combo > 0 && currentTime - lastHitTime > 3.0f) combo = 0;
            if (rapidFire && currentTime > rapidFireEnd) rapidFire = false;
            if (freezeSkillActive && currentTime > freezeSkillEnd) freezeSkillActive = false;

            // ยิงกระสุน
            int cooldown = rapidFire ? 8 : 15;
            if (shootTimer < cooldown) shootTimer++;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= cooldown) {
                sf::Sprite b(bTex); b.setScale(0.5f, 0.5f);
                b.setOrigin(b.getLocalBounds().width/2, b.getLocalBounds().height/2);
                b.setPosition(player.sprite.getPosition().x, player.sprite.getPosition().y - 90);
                bullets.push_back(b); shootTimer = 0;
            }
            for (size_t i = 0; i < bullets.size(); i++) {
                bullets[i].move(0, -12);
                if (bullets[i].getPosition().y < 0) { bullets.erase(bullets.begin() + i); i--; }
            }

            // [CHECK] เสกศัตรูพร้อม AI Chaser
            spawnTimer++;
            float difficultyScale = std::min(currentTime / 90.0f, 1.0f);
            if (spawnTimer >= (50 - (int)(difficultyScale * 35))) {
                Enemy e;
                int r = rand() % 100;
                int type = (r < 25 && rockCooldown <= 0) ? 1 : (r < 40 ? 3 : 0);
                if (type == 1) rockCooldown = 60;
                
                e.init(type == 1 ? rTex : eTex, type, rand() % 740 + 30, 4.0f + (difficultyScale * 4.0f));
                enemies.push_back(e);
                spawnTimer = 0;
            }

            // [CHECK] อัปเดตศัตรู (ส่งตำแหน่งผู้เล่นไปให้ AI Chaser)
            for (size_t i = 0; i < enemies.size(); i++) {
                enemies[i].update(player.sprite.getPosition());

                // เช็คชนผู้เล่น
                if (enemies[i].sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()) && player.iFrames <= 0) {
                    if (enemies[i].type != 2) { 
                        player.hp--; player.iFrames = 90;
                        if (player.hp <= 0) { isGameOver = true; enemies.clear(); bullets.clear(); }
                    }
                }

                // เช็คโดนยิง
                for (size_t k = 0; k < bullets.size(); k++) {
                    if (i < enemies.size() && enemies[i].sprite.getGlobalBounds().intersects(bullets[k].getGlobalBounds())) {
                        bullets.erase(bullets.begin() + k);
                        if (enemies[i].type != 1) { // หินอมตะ
                            if (freezeSkillActive) enemies[i].freezeTimer = 180;
                            enemies[i].hp--; enemies[i].flashTimer = 5;
                            if (enemies[i].hp <= 0) {
                                score += 30 + (combo * 10); combo++; lastHitTime = currentTime;
                                if (combo == 5) { rapidFire = true; rapidFireEnd = currentTime + 4.0f; }
                                if (combo == 10) { freezeSkillActive = true; freezeSkillEnd = currentTime + 5.0f; }
                                enemies.erase(enemies.begin() + i); i--;
                            }
                        }
                        break;
                    }
                }
                if (i < enemies.size() && enemies[i].sprite.getPosition().y > WINDOW_HEIGHT + 100) { enemies.erase(enemies.begin() + i); i--; }
            }
            scoreText.setString("Score: " + std::to_string(score));
        }

        // --- DRAWING ---
        window.clear();
        window.draw(bg1); window.draw(bg2);
        
        if (!isGameRunning || isGameOver) {
            window.draw(btnStart);
        } else {
            player.draw(window); // [CHECK] วาดตัวละครพร้อมระบบกะพริบ
            for(auto &e : enemies) e.draw(window); // [CHECK] วาดมอนสเตอร์พร้อมสีสถานะ
            for(auto &b : bullets) window.draw(b);
            
            window.draw(scoreText);
            if (combo > 0) { // [CHECK] วาด Combo ขึ้นจอ
                comboText.setString("COMBO x" + std::to_string(combo));
                comboText.setOrigin(comboText.getLocalBounds().width/2, 0);
                comboText.setPosition(WINDOW_WIDTH/2, 80);
                window.draw(comboText);
            }
            for (int i = 0; i < player.hp; i++) {
                heartUI.setPosition(WINDOW_WIDTH - 60 - (i * 60), 30); 
                window.draw(heartUI);
            }
        }
        window.display();
    }
    return 0;
}