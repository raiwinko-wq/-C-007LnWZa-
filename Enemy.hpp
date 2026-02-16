#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SFML/Graphics.hpp>
#include <cmath>

class Enemy {
public:
    sf::Sprite sprite;
    float speed;
    int type, hp, flashTimer = 0, freezeTimer = 0;

    void init(sf::Texture& tex, int enemyType, float startX, float baseSpeed) {
        type = enemyType;
        sprite.setTexture(tex);
        // [แก้ไข] ปรับขนาดศัตรูให้เหมาะสม
        sprite.setScale(0.10f, 0.10f); 
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
        sprite.setPosition(startX, -60);

        if (type == 1) { speed = baseSpeed; hp = 9999; }
        else if (type == 3) { speed = baseSpeed * 0.6f; hp = 2; }
        else { speed = baseSpeed - 0.5f; hp = 1; }
    }

    // [แก้ไข] แยก Hitbox: หิน 40%, แมงมุม 80%
    sf::FloatRect getHitbox() {
        sf::FloatRect b = sprite.getGlobalBounds();
        float s = (type == 1) ? 0.4f : 0.8f; 
        float w = b.width * s, h = b.height * s;
        return sf::FloatRect(b.left + (b.width - w)/2, b.top + (b.height - h)/2, w, h);
    }

    void update(sf::Vector2f playerPos) {
        if (freezeTimer > 0) { freezeTimer--; return; }
        if (type == 3) {
            float dx = playerPos.x - sprite.getPosition().x, dy = playerPos.y - sprite.getPosition().y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist != 0) sprite.move((dx/dist)*speed, (dy/dist)*speed);
        } else sprite.move(0, speed);
        if (flashTimer > 0) flashTimer--;
    }

    void draw(sf::RenderWindow& window) {
        if (flashTimer > 0) sprite.setColor(sf::Color::Red);
        else if (freezeTimer > 0) sprite.setColor(sf::Color::Cyan);
        else if (type == 3) sprite.setColor(sf::Color::Magenta);
        else sprite.setColor(sf::Color::White);
        window.draw(sprite);
    }
};
#endif