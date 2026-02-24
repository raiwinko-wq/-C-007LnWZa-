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

    enum State { MENU, HELP, PLAY};

    State state = MENU;

    void init()
    {
        // background
        bgTex.loadFromFile("assets/spacebg.jpg");
        bg.setTexture(bgTex);

        float scaleX = 800.0f / bgTex.getSize().x;
        float scaleY = 1000.0f / bgTex.getSize().y;
        bg.setScale(scaleX, scaleY);

        font.loadFromFile("assets/WowDino-G33vP.ttf");

        welcomeText.setFont(font);
        welcomeText.setString("WELCOME TO\n");
        welcomeText.setString("DINO SHOOTING GAME!");
        welcomeText.setCharacterSize(36);
        welcomeText.setFillColor(sf::Color::White);

        welcomeText.setOrigin(
            welcomeText.getLocalBounds().width/2,
            welcomeText.getLocalBounds().height/2
        );

        welcomeText.setPosition(400, 200);

        // start button
        texStart.loadFromFile("assets/play.png");
        btnStart.setTexture(texStart);
        btnStart.setOrigin(
            texStart.getSize().x / 2,
            texStart.getSize().y / 2
        );
        btnStart.setScale(0.35f, 0.35f);
        btnStart.setPosition(550, 700);

        // help button
        texHelp.loadFromFile("assets/help.png");
        btnHelp.setTexture(texHelp);
        btnHelp.setOrigin(
            texHelp.getSize().x / 2,
            texHelp.getSize().y / 2
        );
        btnHelp.setScale(0.35f, 0.35f);
        btnHelp.setPosition(250, 700);

        // help screen image
        helpTex.loadFromFile("assets/howto.png");
        helpScreen.setTexture(helpTex);

        float helpScaleX = 800.0f / helpTex.getSize().x;
        float helpScaleY = 1000.0f / helpTex.getSize().y;
        helpScreen.setScale(helpScaleX, helpScaleY);

        // exit button (top right)
        texExit.loadFromFile("assets/exit.png");
        btnExit.setTexture(texExit);

        btnExit.setScale(0.25f, 0.25f);

        btnExit.setPosition(
            800 - texExit.getSize().x * 0.25f - 20,
            20
        );
    }

    void handleClick(sf::Vector2f mousePos)
    {
        if (state == MENU)
        {
            if (btnStart.getGlobalBounds().contains(mousePos))
            {
                state = PLAY;
            }

            if (btnHelp.getGlobalBounds().contains(mousePos))
            {
                state = HELP;
            }
        }
        else if (state == HELP)
        {
            if (btnExit.getGlobalBounds().contains(mousePos))
            {
                state = MENU;
            }
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (state == MENU)
        {
            window.draw(bg);
            window.draw(welcomeText);
            window.draw(btnHelp);
            window.draw(btnStart);
        }
        else if (state == HELP)
        {
            window.draw(helpScreen);
            window.draw(btnExit);
        }
    }

    bool isPlay()
    {
        return state == PLAY;
    }

    bool isMenu()
    {
        return state == MENU;
    }

    void resetToMenu()
    {
        state = MENU;
    }

};

#endif