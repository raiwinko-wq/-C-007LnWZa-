#ifndef BOSS_HPP
#define BOSS_HPP
#include <SFML/Graphics.hpp>

class Boss {
public:
    sf::Sprite sprite;

    bool active = false;
    int hp = 50;
    float speed = 2.0f;

    // ===== INIT =====
    void init(sf::Texture& tex) {

        sprite.setTexture(tex);

        sprite.setScale(0.4f, 0.4f);

        sprite.setOrigin(
            sprite.getLocalBounds().width / 2,
            sprite.getLocalBounds().height / 2
        );

        sprite.setPosition(400, -200);

        hp = 50;
        active = true;
    }

    // ===== UPDATE =====
    void update(sf::Vector2f playerPos) {

    if (!active) return;

    // เข้าฉากก่อน
    if (sprite.getPosition().y < 150) {
        sprite.move(0, speed);
        return;
    }

    // ===== FOLLOW PLAYER =====
    float dx = playerPos.x - sprite.getPosition().x;

    if (abs(dx) > 5)
        sprite.move(dx * 0.02f, 0);   // ความแรงการตาม
}

    // ===== DRAW =====
    void draw(sf::RenderWindow& window) {

        if (!active) return;

        window.draw(sprite);
    }

    // ===== RESET =====
    void reset() {

        active = false;
        hp = 50;

        // ย้ายออกจอ
        sprite.setPosition(400, -300);
    }

    // ===== HARD RESET (ใช้ตอนเริ่มเกมใหม่) =====
    void hardReset() {

        active = false;
        hp = 50;

        sprite = sf::Sprite();   // ⭐ ล้าง sprite memory
    }
};

#endif