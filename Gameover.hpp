#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <SFML/Graphics.hpp>

class GameOver
{
private:

    sf::Sprite bg;
    sf::Texture bgTex;

    sf::Sprite btnPlay;
    sf::Sprite btnBack;

    sf::Texture texPlay;
    sf::Texture texBack;

    sf::Font font;
    sf::Text textGameOver;

public:

    enum State
    {
        NONE,
        PLAY,
        BACK
    };

    State state = NONE;



    void init()
    {
        // =========================
        // BACKGROUND
        // =========================

        bgTex.loadFromFile("assets/spacebg.jpg");
        bg.setTexture(bgTex);

        float scaleX = 800.0f / bgTex.getSize().x;
        float scaleY = 1000.0f / bgTex.getSize().y;

        bg.setScale(scaleX, scaleY);



        // =========================
        // FONT + GAME OVER TEXT
        // =========================

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



        // =========================
        // BACK BUTTON (เหมือน Menu help)
        // =========================

        texBack.loadFromFile("assets/back.png");

        btnBack.setTexture(texBack);

        btnBack.setOrigin(
            texBack.getSize().x / 2.0f,
            texBack.getSize().y / 2.0f
        );

        btnBack.setScale(0.35f, 0.35f);

        btnBack.setPosition(250, 700);



        // =========================
        // PLAY BUTTON (เหมือน Menu play)
        // =========================

        texPlay.loadFromFile("assets/play.png");

        btnPlay.setTexture(texPlay);

        btnPlay.setOrigin(
            texPlay.getSize().x / 2.0f,
            texPlay.getSize().y / 2.0f
        );

        btnPlay.setScale(0.35f, 0.35f);

        btnPlay.setPosition(550, 695);

    }



    // =========================
    // HANDLE CLICK
    // =========================

    void handleClick(sf::Vector2f mousePos)
    {
        if (btnPlay.getGlobalBounds().contains(mousePos))
        {
            state = PLAY;
        }

        if (btnBack.getGlobalBounds().contains(mousePos))
        {
            state = BACK;
        }
    }



    // =========================
    // DRAW
    // =========================

    void draw(sf::RenderWindow& window)
    {
        window.draw(bg);
        window.draw(textGameOver);
        window.draw(btnBack);
        window.draw(btnPlay);
    }



    // =========================
    // CHECK STATE
    // =========================

    bool isPlay()
    {
        return state == PLAY;
    }

    bool isBack()
    {
        return state == BACK;
    }



    // =========================
    // RESET STATE
    // =========================

    void reset()
    {
        state = NONE;
    }

};

#endif