#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Player.hpp"
#include "Enemy.hpp"
#include "bomb.hpp"
#include "Freez.hpp"
#include "Boss.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 1000), "SKY WARRIOR");
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(NULL)));

    // ================= TEXTURES =================
    sf::Texture p1, p2, bTex, eTex, rTex, bgTex, btnTex, hTex, bossTex;

    p1.loadFromFile("assets/pc-walk-up-1.png");
    p2.loadFromFile("assets/pc-walk-up-2.png");
    bTex.loadFromFile("assets/bullet.png");
    eTex.loadFromFile("assets/enemy.png");
    rTex.loadFromFile("assets/rock.png");
    bgTex.loadFromFile("assets/map.png");
    btnTex.loadFromFile("assets/button_start.png");
    hTex.loadFromFile("assets/heart.png");
    bossTex.loadFromFile("assets/boss.png");

    // ================= OBJECTS =================
    Player player;
    player.init(p1);

    Boss boss;

    std::vector<Enemy> enemies;
    std::vector<sf::Sprite> bullets;
    std::vector<sf::Sprite> bossBullets;

    Bomb bomb;
    Freez freez;

    // ================= BOSS SYSTEM =================
    float nextBossTime = 60.0f;
    bool bossSpawned = false;
    int bossShootTimer = 0;

    // ================= UI =================
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setPosition(10, 10);

    sf::Sprite heartUI(hTex);
    heartUI.setScale(0.8f, 0.8f);

    // ================= BACKGROUND =================
    sf::Sprite bg1(bgTex), bg2(bgTex);

    float bgScaleX = 800.0f / bgTex.getSize().x;
    float bgScaleY = 1000.0f / bgTex.getSize().y;

    bg1.setScale(bgScaleX, bgScaleY);
    bg2.setScale(bgScaleX, bgScaleY);

    bg2.setPosition(0, -1000);

    // ================= START BUTTON =================
    sf::Sprite btnStart(btnTex);
    btnStart.setScale(0.35f, 0.35f);
    btnStart.setOrigin(
        btnStart.getLocalBounds().width / 2,
        btnStart.getLocalBounds().height / 2
    );
    btnStart.setPosition(400, 650);

    // ================= GAME STATE =================
    bool isGameRunning = false;
    bool isGameOver = false;

    int score = 0;
    int spawnTimer = 0;
    int shootTimer = 0;

    sf::Clock gameClock;

    // =====================================================
    // ================= GAME LOOP =========================
    // =====================================================

    while (window.isOpen())
    {
        float currentTime = gameClock.getElapsedTime().asSeconds();

        // ================= EVENTS =================
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // ===== CLICK START =====
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );

                if ((!isGameRunning || isGameOver) &&
                    btnStart.getGlobalBounds().contains(mousePos))
                {
                    // ===== GAME STATE =====
                    isGameRunning = true;
                    isGameOver = false;

                    // ===== PLAYER =====
                    player.hp = 3;
                    player.sprite.setPosition(400, 850);

                    // ===== SCORE =====
                    score = 0;

                    // ===== CLEAR OBJECTS =====
                    enemies.clear();
                    bullets.clear();
                    bossBullets.clear();

                    bomb.reset();

                    // ===== RESET BOSS =====
                    boss.hardReset();
                    boss.init(bossTex);
                    boss.reset();

                    bossSpawned = false;
                    bossShootTimer = 0;
                    nextBossTime = 60.0f;

                    // ===== TIMERS =====
                    spawnTimer = 0;
                    shootTimer = 0;

                    // ===== BACKGROUND =====
                    bg1.setPosition(0, 0);
                    bg2.setPosition(0, -1000);

                    // ===== CLOCK =====
                    gameClock.restart();
                }
            }
        }

        // =================================================
        // ================= UPDATE ========================
        // =================================================

        if (isGameRunning && !isGameOver)
        {
            // ===== BACKGROUND =====
            bg1.move(0, 3);
            bg2.move(0, 3);

            if (bg1.getPosition().y >= 1000) bg1.setPosition(0, -1000);
            if (bg2.getPosition().y >= 1000) bg2.setPosition(0, -1000);

            // ===== PLAYER =====
            player.handleInput(800, 1000);
            player.updateAnimation(p1, p2);

            // ===== SHOOT =====
            if (shootTimer < 12) shootTimer++;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
                shootTimer >= 12)
            {
                sf::Sprite b(bTex);
                b.setScale(0.5f, 0.5f);
                b.setOrigin(
                    b.getLocalBounds().width / 2,
                    b.getLocalBounds().height / 2
                );

                b.setPosition(
                    player.sprite.getPosition().x,
                    player.sprite.getPosition().y - 90
                );

                bullets.push_back(b);
                shootTimer = 0;
            }

            // ===== UPDATE BULLETS =====
            for (size_t i = 0; i < bullets.size(); i++)
            {
                bullets[i].move(0, -12);

                if (bullets[i].getPosition().y < 0)
                {
                    bullets.erase(bullets.begin() + i);
                    i--;
                }
            }

            // ===== SPAWN ENEMY =====
            spawnTimer++;

            if (!boss.active && spawnTimer > 40)
            {
                Enemy e;
                e.init(eTex, 0, rand() % 740 + 30, 4);
                enemies.push_back(e);

                spawnTimer = 0;
            }

            // ===== SPAWN BOSS =====
            if (!bossSpawned && !boss.active && currentTime > nextBossTime)
            {
                boss.init(bossTex);
                bossSpawned = true;

                enemies.clear();
                bossBullets.clear();
                bossShootTimer = 0;
            }

            // ===== UPDATE BOSS =====
            if (boss.active)
            {
                boss.update();

                bossShootTimer++;

                if (bossShootTimer > 90)
                {
                    sf::Sprite bb(bTex);

                    bb.setScale(0.6f, 0.6f);
                    bb.setOrigin(
                        bb.getLocalBounds().width / 2,
                        bb.getLocalBounds().height / 2
                    );

                    bb.setPosition(boss.sprite.getPosition());

                    bossBullets.push_back(bb);

                    bossShootTimer = 0;
                }
            }

            // ===== BULLET HIT BOSS =====
            if (boss.active)
            {
                for (size_t k = 0; k < bullets.size(); k++)
                {
                    if (boss.sprite.getGlobalBounds()
                        .intersects(bullets[k].getGlobalBounds()))
                    {
                        bullets.erase(bullets.begin() + k);

                        boss.hp--;

                        if (boss.hp <= 0)
                        {
                            boss.reset();
                            bossSpawned = false;

                            score += 500;

                            bossBullets.clear();
                            bossShootTimer = 0;

                            float delay = 45 + rand() % 46;
                            nextBossTime = currentTime + delay;
                        }
                        break;
                    }
                }
            }

            // ===== BOSS BULLETS =====
            for (size_t i = 0; i < bossBullets.size(); i++)
            {
                bossBullets[i].move(0, 6);

                if (bossBullets[i].getGlobalBounds()
                    .intersects(player.getHitbox()))
                {
                    player.hp--;

                    if (player.hp <= 0)
                        isGameOver = true;

                    bossBullets.erase(bossBullets.begin() + i);
                    i--;
                    continue;
                }

                if (bossBullets[i].getPosition().y > 1100)
                {
                    bossBullets.erase(bossBullets.begin() + i);
                    i--;
                }
            }

            // ===== GAME OVER CHECK =====
            if (player.hp <= 0)
                isGameOver = true;

            // ===== SCORE =====
            scoreText.setString("Score: " + std::to_string(score));
        }

        // ===== CLEANUP WHEN GAME OVER =====
        if (isGameOver)
        {
            boss.reset();
            bossSpawned = false;
            bossBullets.clear();
        }

        // =================================================
        // ================= DRAW ==========================
        // =================================================

        window.clear();

        window.draw(bg1);
        window.draw(bg2);

        if (!isGameRunning || isGameOver)
        {
            window.draw(btnStart);
        }
        else
        {
            player.draw(window);

            for (auto &e : enemies)
                e.draw(window);

            boss.draw(window);

            for (auto &b : bullets)
                window.draw(b);

            for (auto &bb : bossBullets)
                window.draw(bb);

            window.draw(scoreText);

            for (int i = 0; i < player.hp; i++)
            {
                heartUI.setPosition(800 - 60 - (i * 60), 30);
                window.draw(heartUI);
            }
        }

        window.display();
    }

    return 0;
}