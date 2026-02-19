#ifndef HEAL_HPP
#define HEAL_HPP

#include <SFML/Graphics.hpp>
#include <cstdlib>

class Heal
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    bool active;
    sf::Clock spawnClock;

    const float SPAWN_TIME = 20.f; // ฮีลทุก 20 วิ

public:
    Heal()
    {
        texture.loadFromFile("assets/heel.png");
        sprite.setTexture(texture);
        sprite.setScale(0.4f, 0.4f);

        sprite.setOrigin(
            sprite.getLocalBounds().width / 2,
            sprite.getLocalBounds().height / 2
        );

        speed = 3.0f;
        active = false;

        spawnClock.restart();
    }

    void reset()
    {
        active = false;
        spawnClock.restart();
    }

    void trySpawn(int windowWidth)
    {
        if (!active && spawnClock.getElapsedTime().asSeconds() >= SPAWN_TIME)
        {
            sprite.setPosition(
                rand() % (windowWidth - 100) + 50,
                -80
            );

            active = true;
        }
    }

    // 🔥 เช็คว่ากระสุนยิงโดนไหม
    bool checkBulletHit(sf::FloatRect bulletBounds)
    {
        if (active && sprite.getGlobalBounds().intersects(bulletBounds))
        {
            active = false;
            spawnClock.restart();
            return true;
        }
        return false;
    }

    void update(int windowHeight)
    {
        if (!active) return;

        sprite.move(0, speed);

        if (sprite.getPosition().y > windowHeight + 100)
        {
            active = false;
            spawnClock.restart();
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (active)
            window.draw(sprite);
    }
};

#endif
