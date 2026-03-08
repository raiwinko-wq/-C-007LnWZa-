#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>

class Menu {
private:
    sf::Sprite bg;
    sf::Texture bgTex;

    sf::Sprite btnStart;
    sf::Sprite btnHelp;
    sf::Sprite btnExit;

    sf::Texture texStart;
    sf::Texture texHelp;
    sf::Texture texExit;

    sf::Sprite helpScreen;
    sf::Texture helpTex;

    sf::Font font;
    sf::Text welcomeText;

public:
    enum State { MENU, HELP, PLAY };
    State state = MENU;

    void init() {
        bgTex.loadFromFile("assets/spacebg.jpg");
        bg.setTexture(bgTex);
        bg.setScale(800.0f / bgTex.getSize().x, 1000.0f / bgTex.getSize().y);

        font.loadFromFile("assets/WowDino-G33vP.ttf");

        // [แก้ไข] เอาแค่คำว่า DINO SHOOTING GAME บรรทัดเดียวเน้นๆ
        welcomeText.setFont(font);
        welcomeText.setString("DINO SHOOTING GAME");
        welcomeText.setCharacterSize(55); // ปรับตัวใหญ่ขึ้นได้เพราะเหลือบรรทัดเดียว
        welcomeText.setFillColor(sf::Color::Cyan);
        welcomeText.setOutlineColor(sf::Color::Blue);
        welcomeText.setOutlineThickness(3);
        welcomeText.setOrigin(welcomeText.getLocalBounds().width / 2.0f, welcomeText.getLocalBounds().height / 2.0f);
        welcomeText.setPosition(400, 250);

        texStart.loadFromFile("assets/play.png");
        btnStart.setTexture(texStart);
        btnStart.setScale(0.35f, 0.35f);
        float playW = texStart.getSize().x * 0.35f;
        btnStart.setPosition(400.0f - (playW / 2.0f), 450.0f);

        texHelp.loadFromFile("assets/help.png");
        btnHelp.setTexture(texHelp);
        btnHelp.setScale(0.35f, 0.35f);
        float helpW = texHelp.getSize().x * 0.35f;
        btnHelp.setPosition(400.0f - (helpW / 2.0f), 600.0f);

        // หน้าจอสอนเล่น
        helpTex.loadFromFile("assets/howto.png");
        helpScreen.setTexture(helpTex);
        helpScreen.setScale(800.0f / helpTex.getSize().x, 1000.0f / helpTex.getSize().y);

        // ปุ่ม Exit
        texExit.loadFromFile("assets/exit.png");
        btnExit.setTexture(texExit);
        btnExit.setScale(0.25f, 0.25f);
        btnExit.setPosition(800.0f - (texExit.getSize().x * 0.25f) - 20.0f, 20.0f);
    }

    void handleClick(sf::Vector2f mousePos) {
        if (state == MENU) {
            if (btnStart.getGlobalBounds().contains(mousePos)) state = PLAY;
            if (btnHelp.getGlobalBounds().contains(mousePos)) state = HELP;
        } else if (state == HELP) {
            if (btnExit.getGlobalBounds().contains(mousePos)) state = MENU;
        }
    }

    void draw(sf::RenderWindow& window) {
        if (state == MENU) {
            window.draw(bg);
            window.draw(welcomeText);
            window.draw(btnStart);
            window.draw(btnHelp);
        } else if (state == HELP) {
            window.draw(helpScreen);
            window.draw(btnExit);
        }
    }

    bool isPlay() { return state == PLAY; }
    bool isMenu() { return state == MENU; }
    void resetToMenu() { state = MENU; }
};

#endif