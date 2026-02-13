#ifndef BOMB_HPP
#define BOMB_HPP

#include <SFML/Graphics.hpp>
#include <cstdlib>

class Bomb
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    bool active;
    sf::Clock spawnClock;

    const float SPAWN_TIME = 180.f; // 3 นาที

public:
    Bomb()
    {
        texture.loadFromFile("bomb.png");
        sprite.setTexture(texture);
        sprite.setScale(0.15f, 0.15f);

        sprite.setOrigin(
            sprite.getLocalBounds().width / 2,
            sprite.getLocalBounds().height / 2
        );

        speed = 5.0f;
        active = false;

        spawnClock.restart(); // ✅ เริ่มนับเวลาตั้งแต่เริ่มเกม
    }

    void trySpawn(int windowWidth)
    {
        // รอครบ 5 นาที และต้องยังไม่มี bomb อยู่
        if (!active && spawnClock.getElapsedTime().asSeconds() >= SPAWN_TIME)
        {
            sprite.setPosition(
                rand() % (windowWidth - 100) + 50,
                -80
            );

            active = true;

            // ❌ ยังไม่ restart ตรงนี้
            // จะ restart ตอน bomb หายไป
        }
    }

    void update(const sf::RectangleShape& player, int windowHeight)
    {
        if (!active) return;

        sprite.move(0, speed);

        // ชนผู้เล่น
        if (sprite.getGlobalBounds().intersects(player.getGlobalBounds()))
        {
            active = false;
            spawnClock.restart(); // ✅ เริ่มนับใหม่หลัง bomb หาย
        }

        // หลุดจอ
        if (sprite.getPosition().y > windowHeight + 100)
        {
            active = false;
            spawnClock.restart(); // ✅ เริ่มนับใหม่หลัง bomb หาย
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (active)
            window.draw(sprite);
    }
};

#endif
