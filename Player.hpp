#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Graphics.hpp>

class Player {
public:
    sf::Sprite sprite;
    int hp = 3;
    int iFrames = 0;
    int currentFrame = 1;
    sf::Clock animClock;

    void init(sf::Texture& t1) {
        sprite.setTexture(t1);
        sprite.setScale(7.5f, 7.5f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
    }

    void updateAnimation(sf::Texture& t1, sf::Texture& t2) {
        if (animClock.getElapsedTime().asSeconds() > 0.15f) {
            sprite.setTexture((currentFrame == 1) ? t2 : t1);
            currentFrame = (currentFrame == 1) ? 2 : 1;
            animClock.restart();
        }
    }

    // [แก้ไข] สร้าง Hitbox ขนาดเล็ก 30% เพื่อความพริ้ว
    sf::FloatRect getHitbox() {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        float scale = 0.3f; 
        float width = bounds.width * scale;
        float height = bounds.height * scale;
        return sf::FloatRect(bounds.left + (bounds.width - width) / 2.0f, 
                             bounds.top + (bounds.height - height) / 2.0f, width, height);
    }

    void handleInput(int w, int h) {
        float speed = 7.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sprite.getPosition().x > 60) sprite.move(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sprite.getPosition().x < w - 60) sprite.move(speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sprite.getPosition().y > 60) sprite.move(0, -speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sprite.getPosition().y < h - 60) sprite.move(0, speed);
    }

    void draw(sf::RenderWindow& window) {
        if (iFrames > 0) {
            iFrames--;
            if ((iFrames / 10) % 2 == 0) return; 
        }
        window.draw(sprite);
    }
};
#endif