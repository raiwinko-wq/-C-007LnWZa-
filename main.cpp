#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Player.hpp"
#include "Enemy.hpp"
#include "bomb.hpp"
#include "Freez.hpp"
#include "Boss.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 1000), "SKY WARRIOR: GHOST PROTOCOL");
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(NULL)));

    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    sf::Texture p1, p2, bTex, eTex, rTex, bgTex, btnTex, hTex;
    p1.loadFromFile("assets/pc-walk-up-1.png");
    p2.loadFromFile("assets/pc-walk-up-2.png");
    bTex.loadFromFile("assets/bullet.png");
    eTex.loadFromFile("assets/enemy.png");
    rTex.loadFromFile("assets/rock.png");
    bgTex.loadFromFile("assets/map.png");
    btnTex.loadFromFile("assets/button_start.png");
    hTex.loadFromFile("assets/heart.png");

    Player player; player.init(p1);
    std::vector<Enemy> enemies;
    std::vector<sf::Sprite> bullets;
    Bomb bomb; Freez freez;

    sf::Sprite bg1(bgTex), bg2(bgTex);
    float bgScaleX = 800.0f / bgTex.getSize().x;
    float bgScaleY = 1000.0f / bgTex.getSize().y;
    bg1.setScale(bgScaleX, bgScaleY); bg2.setScale(bgScaleX, bgScaleY);
    bg2.setPosition(0, -1000);

    sf::Sprite btnStart(btnTex);
    btnStart.setScale(0.35f, 0.35f);
    btnStart.setOrigin(btnStart.getLocalBounds().width/2, btnStart.getLocalBounds().height/2);
    btnStart.setPosition(400, 650);

    sf::Sprite heartUI(hTex); heartUI.setScale(0.8f, 0.8f);

    bool isGameRunning = false, isGameOver = false;
    int score = 0, combo = 0, spawnTimer = 0, shootTimer = 0, rockCooldown = 0;
    float lastHitTime = 0.0f;
    bool rapidFire = false, freezeSkillActive = false;
    float rapidFireEnd = 0.0f, freezeSkillEnd = 0.0f;
    sf::Clock gameClock;

    sf::Text scoreText("Score: 0", font, 24); scoreText.setPosition(10, 10);
    sf::Text comboText("", font, 40); comboText.setFillColor(sf::Color::Yellow);

    while (window.isOpen()) {
        float currentTime = gameClock.getElapsedTime().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            // [แก้แล้ว] ระบบคลิกปุ่ม Start Screen ทำงานปกติ
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if ((!isGameRunning || isGameOver) && btnStart.getGlobalBounds().contains(mousePos)) {
                    isGameRunning = true; isGameOver = false; score = 0; combo = 0;
                    bomb.reset(); player.hp = 3; player.iFrames = 0;
                    enemies.clear(); bullets.clear();
                    player.sprite.setPosition(400, 850);
                    gameClock.restart();
                }
            }
        }

        if (isGameRunning && !isGameOver) {
            bg1.move(0, 3); bg2.move(0, 3);
            if (bg1.getPosition().y >= 1000) bg1.setPosition(0, -1000);
            if (bg2.getPosition().y >= 1000) bg2.setPosition(0, -1000);

            player.handleInput(800, 1000);
            player.updateAnimation(p1, p2);

            if (rockCooldown > 0) rockCooldown--;
            if (combo > 0 && currentTime - lastHitTime > 3.0f) combo = 0;
            if (rapidFire && currentTime > rapidFireEnd) rapidFire = false;
            if (freezeSkillActive && currentTime > freezeSkillEnd) freezeSkillActive = false;

            int cooldown = rapidFire ? 6 : 12;
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

            spawnTimer++;
            float difficultyScale = std::min(currentTime / 90.0f, 1.0f);
            if (spawnTimer >= (50 - (int)(difficultyScale * 35))) {
                Enemy e;
                int r = rand() % 100;
                int type = (r < 25 && rockCooldown <= 0) ? 1 : (r < 30 ? 3 : 0);
                if (type == 1) rockCooldown = 60;
                e.init(type == 1 ? rTex : eTex, type, rand() % 740 + 30, 4.0f + (difficultyScale * 4.0f));
                enemies.push_back(e); spawnTimer = 0;
            }

            for (size_t i = 0; i < enemies.size(); i++) {
                enemies[i].update(player.sprite.getPosition());
                
                // [แก้แล้ว] ใช้ระบบ Hitbox ตามเดิม
                if (enemies[i].getHitbox().intersects(player.getHitbox()) && player.iFrames <= 0) {
                    if (enemies[i].type != 2) { 
                        player.hp--; player.iFrames = 90;
                        if (player.hp <= 0) isGameOver = true;
                    }
                }
                for (size_t k = 0; k < bullets.size(); k++) {
                    if (i < enemies.size() && enemies[i].getHitbox().intersects(bullets[k].getGlobalBounds())) {
                        bullets.erase(bullets.begin() + k);
                        if (enemies[i].type != 1) { 
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
                if (i < enemies.size() && enemies[i].sprite.getPosition().y > 1100) { enemies.erase(enemies.begin() + i); i--; }
            }

            bomb.trySpawn(800);
            if (bomb.update(player.sprite, 1000)) enemies.clear();

            scoreText.setString("Score: " + std::to_string(score));
        }

        window.clear();
        window.draw(bg1); window.draw(bg2);
        
        // [แก้แล้ว] วาดหน้าจอ Start และ Game Over 
        if (!isGameRunning || isGameOver) {
            window.draw(btnStart);
            if (isGameOver) {
                sf::Text gameOverText("GAME OVER\nClick Start to Restart", font, 40);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setOrigin(gameOverText.getLocalBounds().width/2, gameOverText.getLocalBounds().height/2);
                gameOverText.setPosition(400, 400);
                window.draw(gameOverText);
            }
        } else {
            player.draw(window); 
            freez.updateAndDraw(window, player.sprite, enemies);
            for(auto &e : enemies) e.draw(window); 
            for(auto &b : bullets) window.draw(b);
            bomb.draw(window); 
            window.draw(scoreText);
            
            if (combo > 0) { 
                comboText.setString("COMBO x" + std::to_string(combo));
                comboText.setOrigin(comboText.getLocalBounds().width/2, 0);
                comboText.setPosition(400, 80);
                window.draw(comboText);
            }
            for (int i = 0; i < player.hp; i++) {
                heartUI.setPosition(800 - 60 - (i * 60), 30); 
                window.draw(heartUI);
            }
        }
        window.display();
    }
    return 0;
}