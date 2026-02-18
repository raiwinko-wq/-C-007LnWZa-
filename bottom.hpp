#ifndef BOTTOM_HPP
#define BOTTOM_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class BottomPopup
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Clock lifeClock;
    sf::Clock blinkClock;

    bool active = false;
    bool visible = true;

    const float LIFE_TIME  = 5.0f;   // แสดง 5 วินาที
    const float BLINK_TIME = 0.5f;  // ความเร็วกระพริบ

public:
    BottomPopup()
    {
        if (!texture.loadFromFile("assets/bottom.png"))
            std::cout << "bottom.png load failed\n";

        sprite.setTexture(texture);

        // ปรับขนาด (แก้ได้ตามต้องการ)
        sprite.setScale(0.4f, 0.4f);

        // มุมซ้ายบน
        sprite.setPosition(18.f, 80.f);
    }

    // เรียกเมื่อกด Start
    void trigger()
    {
        active = true;
        visible = true;

        lifeClock.restart();
        blinkClock.restart();
    }

    void update()
    {
        if (!active) return;

        // ครบเวลา → หาย
        if (lifeClock.getElapsedTime().asSeconds() >= LIFE_TIME)
        {
            active = false;
            return;
        }

        // เอฟเฟกต์กระพริบ
        if (blinkClock.getElapsedTime().asSeconds() >= BLINK_TIME)
        {
            visible = !visible;
            blinkClock.restart();
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (active && visible)
            window.draw(sprite);
    }
};

#endif
