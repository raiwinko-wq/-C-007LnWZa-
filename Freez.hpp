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
    sf::Clock spawnClock;
    const float spawnInterval = 60.f;
    const int freezeFrames = 600;
    bool itemActive = false;

public:
    Freez() {
        // [แก้แล้ว] โหลดให้ตรงกับชื่อไฟล์จริงคือ "freez.png" (ไม่มี e)
        texture.loadFromFile("assets/freez.png"); 
        item.setTexture(texture);
        item.setScale(0.3f, 0.3f);
        item.setOrigin(item.getLocalBounds().width / 2, item.getLocalBounds().height / 2);
    }

    void updateAndDraw(sf::RenderWindow& window, sf::Sprite& player, std::vector<Enemy>& enemies) {
        if (!itemActive && spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            item.setPosition(rand() % 700 + 50, -50);
            itemActive = true;
            spawnClock.restart();
        }

        if (itemActive) {
            item.move(0, 3.f);
            if (item.getGlobalBounds().intersects(player.getGlobalBounds())) {
                for (auto& e : enemies) {
                    if (e.type != 1) e.freezeTimer = freezeFrames;
                }
                itemActive = false;
            }
            if (item.getPosition().y > 1100) itemActive = false;
            window.draw(item);
        }
    }
};
#endif