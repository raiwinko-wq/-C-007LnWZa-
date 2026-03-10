#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Player.hpp"
#include "Enemy.hpp"
#include "bomb.hpp"
#include "Freeze.hpp"
#include "Boss.hpp"
#include "RapidFire.hpp"
#include "Heal.hpp"
#include "Menu.hpp"
#include "HighScore.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 1000), "DINO SHOOTING GAME");
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(NULL)));

    Menu menu;
    menu.init();

    sf::Font font;
    font.loadFromFile("assets/WowDino-G33vP.ttf");

    sf::Texture p1, p2, bTex, eTex, rTex, bgTex, hTex, bossTex;
    p1.loadFromFile("assets/pc-walk-up-1.png");
    p2.loadFromFile("assets/pc-walk-up-2.png");
    bTex.loadFromFile("assets/bullet.png");
    eTex.loadFromFile("assets/enemy.png");
    rTex.loadFromFile("assets/rock.png");
    bgTex.loadFromFile("assets/spacebg.jpg");
    hTex.loadFromFile("assets/heart.png");
    bossTex.loadFromFile("assets/boss.png");

    sf::Texture playTex, backTex;
    playTex.loadFromFile("assets/play.png");
    backTex.loadFromFile("assets/back.png");

    sf::Sprite btnPlay(playTex);
    sf::Sprite btnBack(backTex);

    btnPlay.setOrigin(playTex.getSize().x / 2.0f, playTex.getSize().y / 2.0f);
    btnBack.setOrigin(backTex.getSize().x / 2.0f, backTex.getSize().y / 2.0f);

    btnPlay.setScale(0.35f, 0.35f);
    btnBack.setScale(0.35f, 0.35f);

    btnPlay.setPosition(400, 550);
    btnBack.setPosition(400, 700);

    Player player;
    player.init(p1);
    Boss boss;
    float nextBossTime = 60.0f;
    bool bossSpawned = false;
    bool bossWarning = false;
    float bossWarningTimer = 0;

    std::vector<Enemy> enemies;
    std::vector<sf::Sprite> bullets;
    Bomb bomb;
    Freeze freeze;
    RapidFire rapid;
    Heal heal;

    std::vector<sf::Sprite> bossBullets;
    int bossShootTimer = 0;

    sf::Sprite bg1(bgTex), bg2(bgTex);
    float bgScaleX = 800.0f / bgTex.getSize().x;
    float bgScaleY = 1000.0f / bgTex.getSize().y;
    bg1.setScale(bgScaleX, bgScaleY);
    bg2.setScale(bgScaleX, bgScaleY);
    bg2.setPosition(0, -1000);

    sf::Sprite heartUI(hTex);
    heartUI.setScale(0.8f, 0.8f);

    bool isGameRunning = false, isGameOver = false;
    int score = 0, combo = 0, spawnTimer = 0, shootTimer = 0, rockCooldown = 0;
    float lastHitTime = 0.0f;
    bool freezeSkillActive = false;
    float freezeSkillEnd = 0.0f;
    sf::Clock gameClock;

    sf::View gameView = window.getDefaultView();
    gameView.setSize(800, 1000);

    float shakeTimer = 0.0f;
    float shakeDuration = 0.25f;
    float shakeMagnitude = 8.0f;

    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setPosition(10, 10);
    sf::Text comboText("", font, 40);
    comboText.setFillColor(sf::Color::Yellow);
    sf::Text warningText("⚠ WARNING ⚠", font, 80);
    warningText.setFillColor(sf::Color::Red);
    warningText.setOutlineColor(sf::Color::Black);
    warningText.setOutlineThickness(5);

    warningText.setOrigin(
        warningText.getLocalBounds().width / 2,
        warningText.getLocalBounds().height / 2
    );

    warningText.setPosition(
        window.getSize().x / 2,
        window.getSize().y / 2
    );

    HighScore highScore;
    highScore.init(font);

    auto resetGame = [&]() {
        isGameRunning = true;
        isGameOver = false;
        score = 0;
        combo = 0;
        player.hp = 3;
        player.iFrames = 0;
        player.sprite.setPosition(400, 850);
        spawnTimer = 0;
        shootTimer = 0;
        rockCooldown = 0;
        bossShootTimer = 0;
        freezeSkillActive = false;
        freezeSkillEnd = 0;
        lastHitTime = 0;
        boss.active = false;
        bossSpawned = false;
        bossWarning = false;
        bossWarningTimer = 0;
        bossBullets.clear();
        nextBossTime = 30.0f;
        enemies.clear();
        bullets.clear();
        bomb.reset();
        rapid.reset();
        heal.reset();
        freeze.reset();
        bg1.setPosition(0, 0);
        bg2.setPosition(0, -1000);

        highScore.resetColor();
        gameClock.restart();
        shakeTimer = 0;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

                if (!isGameRunning) {
                    menu.handleClick(mousePos);
                    if (menu.isPlay()) {
                        resetGame();
                    }
                }
                if (isGameOver) {
                    if (btnPlay.getGlobalBounds().contains(mousePos)) {
                        resetGame();
                    }
                    if (btnBack.getGlobalBounds().contains(mousePos)) {
                        isGameRunning = false;
                        isGameOver = false;
                        menu.resetToMenu();
                    }
                }
            }
        }

        if (isGameRunning && !isGameOver) {
            float currentTime = gameClock.getElapsedTime().asSeconds();
            bg1.move(0, 3);
            bg2.move(0, 3);

            if (bg1.getPosition().y >= 1000) bg1.setPosition(0, -1000);
            if (bg2.getPosition().y >= 1000) bg2.setPosition(0, -1000);

            player.handleInput(800, 1000);
            player.updateAnimation(p1, p2);

            rapid.update(1.0f / 60.0f, freezeSkillActive, window);
            rapid.checkCollision(player.sprite);

            if (rockCooldown > 0) rockCooldown--;
            if (combo > 0 && currentTime - lastHitTime > 3.0f) combo = 0;
            if (freezeSkillActive && currentTime > freezeSkillEnd) freezeSkillActive = false;

            int cooldown = rapid.active() ? 6 : 12;

            if (shootTimer < cooldown) shootTimer++;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= cooldown) {
                sf::Sprite b(bTex);
                b.setScale(0.5f, 0.5f);
                b.setOrigin(b.getLocalBounds().width / 2, b.getLocalBounds().height / 2);
                b.setPosition(player.sprite.getPosition().x, player.sprite.getPosition().y - 90);
                bullets.push_back(b);
                shootTimer = 0;
            }

            for (size_t i = 0; i < bullets.size(); i++) {
                bullets[i].move(0, -12);
                if (bullets[i].getPosition().y < 0) {
                    bullets.erase(bullets.begin() + i);
                    i--;
                }
            }

            spawnTimer++;
            float difficultyScale = std::min(currentTime / 90.0f, 1.0f);
            if (!boss.active && spawnTimer >= (50 - (int)(difficultyScale * 35))) {
                Enemy e;
                int r = rand() % 100;
                int type = (r < 25 && rockCooldown <= 0) ? 1 : (r < 30 ? 3 : 0);
                if (type == 1) rockCooldown = 60;
                e.init(type == 1 ? rTex : eTex, type, rand() % 740 + 30, 4.0f + (difficultyScale * 4.0f));
                enemies.push_back(e);
                spawnTimer = 0;
            }

            if (bossWarning) {
                bool blink = ((int)(currentTime * 4) % 2 == 0);
                warningText.setFillColor(blink ? sf::Color::Red : sf::Color::White);
            } else {
                warningText.setFillColor(sf::Color::Red);
            }

            if (!bossSpawned && !bossWarning && !boss.active && currentTime > nextBossTime) {
                bossWarning = true;
                bossWarningTimer = currentTime + 2.5f;
            }

            if (bossWarning && currentTime > bossWarningTimer) {
                bossWarning = false;
                boss.init(bossTex);
                bossSpawned = true;
                enemies.clear();
                bossBullets.clear();
                bossShootTimer = 0;
            }

            if (boss.active) {
                boss.update(player.sprite.getPosition());
                if (boss.sprite.getGlobalBounds().intersects(player.getHitbox()) && player.iFrames <= 0) {
                    player.hp--;
                    shakeTimer = shakeDuration;
                    player.sprite.move(0, 40);
                    player.iFrames = 90;
                    if (player.hp <= 0) isGameOver = true;
                }

                bossShootTimer++;
                if (bossShootTimer > 90) {
                    for (int i = -2; i <= 2; i++) {
                        sf::Sprite bb(bTex);
                        bb.setScale(0.6f, 0.6f);
                        bb.setOrigin(bb.getLocalBounds().width / 2, bb.getLocalBounds().height / 2);
                        bb.setPosition(boss.sprite.getPosition());
                        bossBullets.push_back(bb);
                    }
                    bossShootTimer = 0;
                }
            }

            for (size_t i = 0; i < enemies.size(); i++) {
                enemies[i].update(player.sprite.getPosition());
                if (enemies[i].getHitbox().intersects(player.getHitbox()) && player.iFrames <= 0) {
                    if (enemies[i].type != 2) {
                        player.hp--;
                        shakeTimer = shakeDuration;
                        player.iFrames = 90;
                        if (player.hp <= 0) isGameOver = true;
                    }
                }
                for (size_t k = 0; k < bullets.size(); k++) {
                    if (i < enemies.size() && enemies[i].getHitbox().intersects(bullets[k].getGlobalBounds())) {
                        bullets.erase(bullets.begin() + k);
                        if (enemies[i].type != 1) {
                            if (freezeSkillActive) enemies[i].freezeTimer = 180;
                            enemies[i].hp--;
                            enemies[i].flashTimer = 5;
                            if (enemies[i].hp <= 0) {
                                score += 30 + (combo * 10);
                                combo++;
                                lastHitTime = currentTime;
                                highScore.checkAndSave(score);

                                if (combo == 10) {
                                    freezeSkillActive = true;
                                    freezeSkillEnd = currentTime + 5.0f;
                                }
                                enemies.erase(enemies.begin() + i);
                                i--;
                            }
                        }
                        break;
                    }
                }
                if (i < enemies.size() && enemies[i].sprite.getPosition().y > 1100) {
                    enemies.erase(enemies.begin() + i);
                    i--;
                }
            }

            if (boss.active) {
                for (size_t k = 0; k < bullets.size(); k++) {
                    if (boss.sprite.getGlobalBounds().intersects(bullets[k].getGlobalBounds())) {
                        bullets.erase(bullets.begin() + k);
                        boss.hp--;
                        boss.flashTimer = 6;
                        if (boss.hp <= 0) {
                            boss.active = false;
                            bossSpawned = false;
                            score += 500;
                            heal.spawnFromBoss(boss.sprite.getPosition().x, boss.sprite.getPosition().y);
                            highScore.checkAndSave(score);
                            bossBullets.clear();
                            bossShootTimer = 0;

                            float randomDelay = 45 + rand() % 46;
                            nextBossTime = currentTime + randomDelay;
                        }
                        break;
                    }
                }
            }

            for (size_t i = 0; i < bossBullets.size(); i++) {
                float spread = (i % 5 - 2) * 1.5f;
                bossBullets[i].move(spread, 6);
                if (bossBullets[i].getGlobalBounds().intersects(player.getHitbox()) && player.iFrames <= 0) {
                    player.hp--;
                    shakeTimer = shakeDuration;
                    player.iFrames = 90;
                    if (player.hp <= 0) { isGameOver = true; }
                    bossBullets.erase(bossBullets.begin() + i);
                    i--;
                    continue;
                }
                if (bossBullets[i].getPosition().y > 1100) {
                    bossBullets.erase(bossBullets.begin() + i);
                    i--;
                }
            }

            bomb.trySpawn(800);
            if (bomb.update(player.sprite, 1000)) enemies.clear();
            heal.update(1000);

            if (heal.checkPlayerCollision(player.getHitbox())) {
                if (player.hp < 3) player.hp++;
            }

            scoreText.setString("Score: " + std::to_string(score));

            if (shakeTimer > 0) {
                shakeTimer -= 1.0f / 60.0f;
                float offsetX = (rand() % 21 - 10) * shakeMagnitude * 0.1f;
                float offsetY = (rand() % 21 - 10) * shakeMagnitude * 0.1f;
                gameView.setCenter(400 + offsetX, 500 + offsetY);
            } else {
                gameView.setCenter(400, 500);
            }
        }

        if (isGameOver) {
            boss.active = false;
            bossSpawned = false;
            bossBullets.clear();
        }

        window.clear();
        window.setView(gameView);

        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER");
        sf::Text finalScoreText;
finalScoreText.setFont(font);
finalScoreText.setCharacterSize(30);
finalScoreText.setFillColor(sf::Color::White);
finalScoreText.setOutlineColor(sf::Color::Black);
finalScoreText.setOutlineThickness(3);

finalScoreText.setString("Score : " + std::to_string(score));

finalScoreText.setOrigin(
    finalScoreText.getLocalBounds().width / 2,
    finalScoreText.getLocalBounds().height / 2
);

finalScoreText.setPosition(400, 425);
        gameOverText.setCharacterSize(60);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineColor(sf::Color::Black);
        gameOverText.setOutlineThickness(3);
        gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
        gameOverText.setPosition(400, 250);

        window.draw(bg1);
        window.draw(bg2);

        if (!isGameRunning) {
            menu.draw(window);
            if (menu.isMenu()) {
                highScore.drawMenu(window);
            }
        } else if (isGameOver) {
            window.draw(gameOverText);
            window.draw(finalScoreText); 
            window.draw(btnPlay);
            window.draw(btnBack);
            highScore.drawMenu(window);
        } else {
            player.draw(window);
            freeze.updateAndDraw(window, player.sprite, enemies);
            rapid.draw(window);
            for (auto &e : enemies) e.draw(window);
            boss.draw(window);
            for (auto &b : bullets) window.draw(b);
            for (auto &bb : bossBullets) window.draw(bb);

            if (boss.active) {
                float hpPercent = (float)boss.hp / 100.0f;
                float bossBarWidth = 300.0f;
                float bossBarX = (800.0f - bossBarWidth) / 2.0f;
                float bossBarY = 50.0f;

                sf::RectangleShape back(sf::Vector2f(bossBarWidth, 20));
                back.setFillColor(sf::Color(50, 50, 50));
                back.setPosition(bossBarX, bossBarY);

                sf::RectangleShape bar(sf::Vector2f(bossBarWidth * hpPercent, 20));
                bar.setFillColor(sf::Color::Red);
                bar.setPosition(bossBarX, bossBarY);

                window.draw(back);
                window.draw(bar);
            }

            bomb.draw(window);
            heal.draw(window);

            window.draw(scoreText);
            highScore.drawGameplay(window);

            if (bossWarning && !isGameOver) {
                window.draw(warningText);
            }

            if (combo > 0) {
                comboText.setString("COMBO x " + std::to_string(combo));
                comboText.setOrigin(comboText.getLocalBounds().width / 2, 0);
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