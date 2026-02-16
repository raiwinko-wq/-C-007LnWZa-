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

    const float SPAWN_TIME = 15.f; // เปลี่ยนเป็น 15 วินาที ระเบิดจะตกลงมาเร็วขึ้น 

public:
    Bomb()
    {
        texture.loadFromFile("assets/bomb.png");
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

    void reset()
    {
        active = false;
        spawnClock.restart();
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

    bool update(const sf::Sprite& player, int windowHeight)
{
    if (!active) return false;

    sprite.move(0, speed);

    if (sprite.getGlobalBounds().intersects(player.getGlobalBounds()))
    {
        active = false;
        spawnClock.restart();
        return true;  // 🔥 บอกว่าโดนชน
    }

    if (sprite.getPosition().y > windowHeight + 100)
    {
        active = false;
        spawnClock.restart();
    }

    return false;
}

    void draw(sf::RenderWindow& window)
    {
        if (active)
            window.draw(sprite);
    }
};

#endif
