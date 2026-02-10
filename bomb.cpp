#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 1000;

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Bomb Draft Test"
    );
    window.setFramerateLimit(60);

    srand(static_cast<unsigned>(time(nullptr)));

    // ===== LOAD TEXTURE =====
    sf::Texture bombTex;
    if (!bombTex.loadFromFile("bomb.png"))
        return 0;

    // ===== BOMB OBJECT =====
    sf::Sprite bomb;
    bomb.setTexture(bombTex);
    bomb.setScale(0.15f, 0.15f);
    bomb.setOrigin(
        bomb.getLocalBounds().width / 2,
        bomb.getLocalBounds().height / 2
    );

    bool bombActive = false;
    float bombSpeed = 5.0f;
    sf::Clock bombClock;   // นับเวลา 7 นาที

    // ===== PLAYER (ทดสอบชน) =====
    sf::RectangleShape player(sf::Vector2f(80, 80));
    player.setFillColor(sf::Color::Green);
    player.setOrigin(40, 40);
    player.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 150);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ===== PLAYER MOVE (TEST) =====
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.move(-6, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.move(6, 0);

        // ===== SPAWN BOMB (7 นาที = 420 วิ) =====
        if (!bombActive && bombClock.getElapsedTime().asSeconds() >= 420.f) {
            bomb.setPosition(
                rand() % (WINDOW_WIDTH - 100) + 50,
                -80
            );
            bombActive = true;
            bombClock.restart();
        }

        // ===== BOMB FALL =====
        if (bombActive) {
            bomb.move(0, bombSpeed);

            // ชนผู้เล่น
            if (bomb.getGlobalBounds().intersects(player.getGlobalBounds())) {
                bombActive = false; // (ในเกมจริงจะ enemies.clear())
            }

            // หลุดจอ
            if (bomb.getPosition().y > WINDOW_HEIGHT + 100) {
                bombActive = false;
            }
        }

        // ===== DRAW =====
        window.clear(sf::Color::Black);
        window.draw(player);
        if (bombActive)
            window.draw(bomb);
        window.display();
    }

    return 0;
}
