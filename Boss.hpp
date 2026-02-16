#pragma once
#include <SFML/Graphics.hpp>

class Boss
{
public:
    sf::Sprite sprite;

    bool active = false;
    int hp = 50;
    float speed = 2.0f;

    void init(sf::Texture& tex)
    {
        sprite.setTexture(tex);
        sprite.setScale(sf::Vector2f(0.4f, 0.4f));

        auto bounds = sprite.getLocalBounds();
        sprite.setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));

        sprite.setPosition(sf::Vector2f(400, -200));

        hp = 50;
        active = true;
    }

    void update()
    {
        if (!active) return;

        if (sprite.getPosition().y < 150)
            sprite.move(sf::Vector2f(0, speed));
    }

    void draw(sf::RenderWindow& window)
    {
        if (active)
            window.draw(sprite);
    }

    void reset()
    {
        active = false;
        hp = 50;
        sprite.setPosition(sf::Vector2f(400, -200));
    }
};