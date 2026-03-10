#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <SFML/Graphics.hpp>
#include <string>

class GameOver {
private:
    sf::Sprite bg;
    sf::Texture bgTex;
    sf::Sprite btnPlay;
    sf::Sprite btnBack;
    sf::Texture texPlay;
    sf::Texture texBack;
    sf::Font font;
    sf::Text textGameOver;
    sf::Text textScore;  
    int lastScore = 0;  

public:
    enum State {
        NONE,
        PLAY,
        BACK
    };

    State state = NONE;

    void init() {
        bgTex.loadFromFile("assets/spacebg.jpg");
        bg.setTexture(bgTex);

        float scaleX = 800.0f / bgTex.getSize().x;
        float scaleY = 1000.0f / bgTex.getSize().y;
        bg.setScale(scaleX, scaleY);

        font.loadFromFile("assets/WowDino-G33vP.ttf");

        textGameOver.setFont(font);
        textGameOver.setString("GAME OVER");
        textGameOver.setCharacterSize(60);
        textGameOver.setFillColor(sf::Color::Red);

        sf::FloatRect bounds = textGameOver.getLocalBounds();
        textGameOver.setOrigin(
            bounds.left + bounds.width / 2.0f,
            bounds.top + bounds.height / 2.0f
        );
        textGameOver.setPosition(800 / 2.0f, 350);

        textScore.setFont(font);
        textScore.setCharacterSize(40);
        textScore.setFillColor(sf::Color::White);
        textScore.setPosition(800 / 2.0f - 100, 450);

        texBack.loadFromFile("assets/back.png");
        btnBack.setTexture(texBack);
        btnBack.setOrigin(
            texBack.getSize().x / 2.0f,
            texBack.getSize().y / 2.0f
        );
        btnBack.setScale(0.35f, 0.35f);
        btnBack.setPosition(250, 700);

        texPlay.loadFromFile("assets/play.png");
        btnPlay.setTexture(texPlay);
        btnPlay.setOrigin(
            texPlay.getSize().x / 2.0f,
            texPlay.getSize().y / 2.0f
        );
        btnPlay.setScale(0.35f, 0.35f);
        btnPlay.setPosition(550, 695);
    }

    void setScore(int score) {
        lastScore = score;
        textScore.setString("Score : " + std::to_string(lastScore));
    }

    void handleClick(sf::Vector2f mousePos) {
        if (btnPlay.getGlobalBounds().contains(mousePos)) {
            state = PLAY;
        }

        if (btnBack.getGlobalBounds().contains(mousePos)) {
            state = BACK;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(bg);
        window.draw(textGameOver);
        window.draw(textScore); 
        window.draw(btnBack);
        window.draw(btnPlay);
    }

    bool isPlay() {
        return state == PLAY;
    }

    bool isBack() {
        return state == BACK;
    }

    void reset() {
        state = NONE;
    }
};
#endif