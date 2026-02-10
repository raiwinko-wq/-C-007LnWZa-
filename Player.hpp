#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Graphics.hpp>

class Player {
public:
    // --- ส่วนของข้อมูล (Attributes) ---
    sf::Sprite sprite;
    int hp = 3;             // เลือดเริ่มต้น
    int iFrames = 0;        // อมตะชั่วคราวหลังโดนชน
    int currentFrame = 1;   // สำหรับแอนิเมชันเดิน
    sf::Clock animClock;    // ตัวนับเวลาสลับท่าเดิน

    // --- ส่วนของความสามารถ (Methods) ---

    // 1. ตั้งค่าเริ่มต้นตัวละคร
    void init(sf::Texture& t1) {
        sprite.setTexture(t1);
        sprite.setScale(7.5f, 7.5f); // ขนาดพอดีที่เพื่อนสั่งไว้ล่าสุด
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
    }

    // 2. ระบบแอนิเมชันเดิน (สลับรูปทุก 0.15 วินาที)
    void updateAnimation(sf::Texture& t1, sf::Texture& t2) {
        if (animClock.getElapsedTime().asSeconds() > 0.15f) {
            if (currentFrame == 1) {
                sprite.setTexture(t2);
                currentFrame = 2;
            } else {
                sprite.setTexture(t1);
                currentFrame = 1;
            }
            animClock.restart();
        }
    }

    // 3. ระบบควบคุมการเดิน (WASD) และการบล็อกขอบจอ
    void handleInput(int windowWidth, int windowHeight) {
        float speed = 7.0f; // ความเร็วการเดิน
        float margin = 60.0f; // ขอบเขตที่ไม่ให้เดินเลยออกไป

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sprite.getPosition().x > margin) 
            sprite.move(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sprite.getPosition().x < windowWidth - margin) 
            sprite.move(speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sprite.getPosition().y > margin) 
            sprite.move(0, -speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sprite.getPosition().y < windowHeight - margin) 
            sprite.move(0, speed);
    }

    // 4. ฟังก์ชันสำหรับวาดตัวละคร (รวมระบบกะพริบตอนอมตะ)
    void draw(sf::RenderWindow& window) {
        if (iFrames > 0) {
            iFrames--;
            // ถ้า iFrames เหลือ จะกะพริบทุกๆ 10 เฟรม
            if ((iFrames / 10) % 2 == 0) return; 
        }
        window.draw(sprite);
    }
};

#endif