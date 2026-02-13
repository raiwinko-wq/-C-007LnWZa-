#ifndef FREEZ_HPP
#define FREEZ_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include "Enemy.hpp"

class Freez {
private:
    sf::Sprite item;
    sf::Texture texture;

    sf::Clock spawnClock;         // นับเวลา 2 นาที
    const float spawnInterval = 120.f;

    const int freezeFrames = 600; // 10 วิ (60 FPS)

    bool itemActive = false;

public:

    // Constructor โหลดรูปทันที
    Freez() {
        texture.loadFromFile("assets/freeze.png"); // <<< ใส่รูปตรงนี้
        item.setTexture(texture);
        item.setScale(0.08f, 0.08f);
        item.setOrigin(
            item.getLocalBounds().width / 2,
            item.getLocalBounds().height / 2
        );
    }

    void updateAndDraw(sf::RenderWindow& window,
                       sf::Sprite& player,
                       std::vector<Enemy>& enemies)
    {
        // ===== สร้างไอเทมทุก 120 วิ =====
        if (!itemActive &&
            spawnClock.getElapsedTime().asSeconds() >= spawnInterval)
        {
            item.setPosition(rand() % 700 + 50, -50);
            itemActive = true;
            spawnClock.restart();
        }

        if (itemActive) {

            // ล่วงลง
            item.move(0, 3.f);

            // ชนผู้เล่น
            if (item.getGlobalBounds().intersects(
                player.getGlobalBounds()))
            {
                for (auto& e : enemies) {
                    if (e.type != 1) { // Rock ไม่โดน
                        e.freezeTimer = freezeFrames;
                    }
                }
                itemActive = false;
            }

            // หลุดจอ
            if (item.getPosition().y > 1100)
                itemActive = false;

            window.draw(item);
        }
    }
};

#endif