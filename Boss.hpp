#ifndef BOSS_HPP
#define BOSS_HPP

#include <SFML/Graphics.hpp>

class Boss {
public:
    sf::Sprite sprite;
    bool active = false;
    int hp = 50;
    float speed = 2.0f;

    void init(sf::Texture& tex) {
        sprite.setTexture(tex);
        sprite.setScale(0.4f, 0.4f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        sprite.setPosition(400, -200);
        hp = 50;
        active = true;
    }

    void update() {
        if (!active) return;
        if (sprite.getPosition().y < 150) sprite.move(0, speed);
    }

    void draw(sf::RenderWindow& window) {
        if (active) window.draw(sprite);
    }

    void reset() {
        active = false;
        hp = maxHp;
        sprite.setPosition(400, -300);
    }
};
#endif