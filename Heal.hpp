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

public:
    Heal()
    {
        texture.loadFromFile("assets/heal.png");
        sprite.setTexture(texture);
        sprite.setScale(0.4f, 0.4f);

        sprite.setOrigin(
            sprite.getLocalBounds().width / 2,
            sprite.getLocalBounds().height / 2
        );

        speed = 3.0f;
        active = false;
    }

    void reset()
    {
        active = false;
    }

    //เรียกตอนบอสตาย
    void spawnFromBoss(float x, float y)
    {
        sprite.setPosition(x, y);
        active = true;
    }

    bool checkPlayerCollision(sf::FloatRect playerBounds)
    {
        if (active && sprite.getGlobalBounds().intersects(playerBounds))
        {
            active = false;
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
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (active)
            window.draw(sprite);
    }
};

#endif