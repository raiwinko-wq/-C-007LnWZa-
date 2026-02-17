#ifndef RAPIDFIRE_HPP
#define RAPIDFIRE_HPP

#include <SFML/Graphics.hpp>
#include <cstdlib>

class RapidFire {
private:
    sf::Sprite sprite;
    sf::Texture texture;

    sf::Clock spawnClock;     // นับเวลา 15 วิ
    sf::Clock effectClock;    // เวลาของเอฟเฟกต์

    const float spawnInterval = 15.f;
    const float effectDuration = 10.f; // เอฟเฟกต์อยู่ 10 วิ

    float fallSpeed = 150.f;

    bool isOnMap = false;
    bool isActive = false;

public:
    RapidFire() {
        texture.loadFromFile("assets/gun.png"); // รูปไอเท็ม
        sprite.setTexture(texture);
        sprite.setScale(0.3f, 0.3f);

        sprite.setOrigin(
        sprite.getLocalBounds().width / 2,
        sprite.getLocalBounds().height / 2
        );
    }

    void update(float deltaTime, bool otherEffectActive, sf::RenderWindow& window) {

        // ถ้ายังไม่มีเอฟเฟกต์ใดทำงาน และยังไม่มีไอเท็มบนแมพ
        if (!isOnMap && !isActive && !otherEffectActive) {
            if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
                spawn();
                spawnClock.restart();
            }
        }

        // ทำให้ไอเท็มร่วงลง
        if (isOnMap) {
            sprite.move(0, fallSpeed * deltaTime);

            if (sprite.getPosition().y > window.getSize().y) {
                isOnMap = false;
            }
        }

        // เช็คหมดเวลาเอฟเฟกต์
        if (isActive) {
            if (effectClock.getElapsedTime().asSeconds() >= effectDuration) {
                isActive = false;
            }
        }
    }

    void spawn() {
        float x = rand() % 700 + 50;
        sprite.setPosition(x, 0);
        isOnMap = true;
    }

    void draw(sf::RenderWindow& window) {
        if (isOnMap)
            window.draw(sprite);
    }

    void checkCollision(sf::Sprite& player) {
        if (isOnMap && sprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
            isOnMap = false;
            isActive = true;
            effectClock.restart();

        }
    }

    bool active() {
        return isActive;
    }

    void reset() {
        isOnMap = false;
        isActive = false;
        spawnClock.restart();
        effectClock.restart();
    }
};

#endif
