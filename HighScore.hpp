#ifndef HIGHSCORE_HPP
#define HIGHSCORE_HPP

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

class HighScore {
private:
    int bestScore;
    sf::Text menuText;
    sf::Text gameText;

public:
    HighScore() : bestScore(0) {}

    void init(sf::Font& font) {
        // ดึงคะแนนจากไฟล์
        std::ifstream file("highscore.txt");
        if (file.is_open()) {
            file >> bestScore;
            file.close();
        }

        // [ปรับขนาดลงแล้ว!] ตัวหนังสือหน้าเมนูและตอนแพ้ ลดเหลือขนาด 30
        menuText.setFont(font);
        menuText.setCharacterSize(30);
        menuText.setFillColor(sf::Color::Cyan);

        // ตัวหนังสือมุมขวาบนตอนเล่น (เล็ก)
        gameText.setFont(font);
        gameText.setCharacterSize(24);
        gameText.setFillColor(sf::Color(200, 200, 200));

        updateText();
    }

    void checkAndSave(int currentScore) {
        if (currentScore > bestScore) {
            bestScore = currentScore;

            // ทำลายสถิติ เปลี่ยนเป็นสีทอง!
            gameText.setFillColor(sf::Color(255, 215, 0));
            menuText.setFillColor(sf::Color(255, 215, 0));

            // เซฟลงไฟล์
            std::ofstream file("highscore.txt");
            if (file.is_open()) {
                file << bestScore;
                file.close();
            }
        }
        updateText();
    }

    void resetColor() {
        gameText.setFillColor(sf::Color(200, 200, 200));
        menuText.setFillColor(sf::Color::Cyan);
        updateText();
    }

    void updateText() {
        // ตำแหน่งตอนเล่น (มุมขวาบน ใต้หลอดหัวใจ)
        gameText.setString("BEST: " + std::to_string(bestScore));
        gameText.setPosition(800 - gameText.getLocalBounds().width - 30, 60);

        // ตำแหน่งหน้าเมนู (อยู่ตรงกลางระหว่าง ชื่อเกม Y=250 กับ ปุ่ม Play Y=450)
        menuText.setString("HIGH SCORE: " + std::to_string(bestScore));
        menuText.setOrigin(menuText.getLocalBounds().width / 2.0f, menuText.getLocalBounds().height / 2.0f);
        menuText.setPosition(400, 350);
    }

    void drawMenu(sf::RenderWindow& window) {
        window.draw(menuText);
    }

    void drawGameplay(sf::RenderWindow& window) {
        window.draw(gameText);
    }
};

#endif