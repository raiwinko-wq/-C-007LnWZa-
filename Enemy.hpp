#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <cmath> // จำเป็นสำหรับคำนวณ AI (sqrt)

class Enemy {
public:
    // --- Attributes (ข้อมูล) ---
    sf::Sprite sprite;
    float speed;
    int type;        // 0=Normal, 1=Rock, 3=AI Chaser
    int hp;
    int flashTimer = 0;  // ตัวจับเวลากะพริบสีแดงตอนโดนยิง
    int freezeTimer = 0; // ตัวจับเวลาแช่แข็ง

    // --- Methods (ความสามารถ) ---

    // 1. ตั้งค่าเริ่มต้น: เรียกใช้ตอนเสกศัตรูออกมา
    // tex: รูปภาพ, enemyType: ประเภท, startX: ตำแหน่งแกน X, baseSpeed: ความเร็วพื้นฐาน
    void init(sf::Texture& tex, int enemyType, float startX, float baseSpeed) {
        type = enemyType;
        sprite.setTexture(tex);
        sprite.setScale(0.05f, 0.05f); // ขนาดศัตรู (ปรับได้ถ้าอยากให้ใหญ่ขึ้น)
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
        sprite.setPosition(startX, -60); // เริ่มต้นที่ขอบบนจอ

        // กำหนดสเตตัสตามประเภท
        if (type == 1) { // หิน (Rock)
            speed = baseSpeed;
            hp = 9999; // เลือดเยอะมาก (ยิงไม่เข้า)
        } else if (type == 3) { // AI Chaser
            speed = baseSpeed * 0.6f; // เดินช้าหน่อยเพราะมันโกง (เดินตามเรา)
            hp = 3;
        } else { // Normal (Type 0)
            speed = baseSpeed - 0.5f;
            hp = 3;
        }
    }

    // 2. อัปเดตการเคลื่อนที่ (AI Logic อยู่ตรงนี้)
    // playerPos: ตำแหน่งผู้เล่น (ส่งมาจาก main)
    void update(sf::Vector2f playerPos) {
        // ถ้าถูกแช่แข็ง ให้หยุดเดิน
        if (freezeTimer > 0) {
            freezeTimer--;
            return;
        }

        if (type == 3) { 
            // --- [AI TRACKING LOGIC] ---
            // คำนวณระยะห่างระหว่างศัตรูกับผู้เล่น
            float dx = playerPos.x - sprite.getPosition().x;
            float dy = playerPos.y - sprite.getPosition().y;
            
            // สูตร Pythagoras หาความยาวเส้นตรง
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance != 0) {
                // Normalize (ทำให้เป็น 1 หน่วย) แล้วคูณด้วยความเร็ว
                // ผลลัพธ์คือทิศทางที่ถูกต้องในการพุ่งหาผู้เล่น
                float moveX = (dx / distance) * speed;
                float moveY = (dy / distance) * speed;
                sprite.move(moveX, moveY);
            }
        } else {
            // --- [NORMAL MOVEMENT] ---
            // เดินลงมาข้างล่างตรงๆ
            sprite.move(0, speed);
        }

        // ลดเวลาแฟลชสีแดงลงเรื่อยๆ
        if (flashTimer > 0) flashTimer--;
    }

    // 3. วาดและจัดการสี
    void draw(sf::RenderWindow& window) {
        // ลำดับความสำคัญของสี: แดง (โดนยิง) -> ฟ้า (แช่แข็ง) -> ม่วง (AI) -> ขาว (ปกติ)
        if (flashTimer > 0) 
            sprite.setColor(sf::Color::Red);
        else if (freezeTimer > 0) 
            sprite.setColor(sf::Color::Cyan);
        else if (type == 3) 
            sprite.setColor(sf::Color::Magenta); // สีม่วงให้รู้ว่าเป็นตัว AI
        else 
            sprite.setColor(sf::Color::White);

        window.draw(sprite);
    }
};

#endif