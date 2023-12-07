#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace sf;
using namespace std;
int main(){
    RenderWindow window(VideoMode(600, 900), "Avoid the Meteorites!");
    srand((unsigned int)time(NULL));

    // Fonts
    Font font;
    if (!font.loadFromFile("C:\\Users\\wlsgu\\OneDrive\\바탕 화면\\컴프실\\KdamThmorPro-Regular.ttf"))
    {
        cerr << "폰트 파일 못 읽음" << endl;
        return -1;
    }

    // Texts
    Text gameOverText, pressRtoRestart, scoreNUM, scoreTEXT;
    scoreNUM.setFont(font);
    scoreNUM.setCharacterSize(40);
    scoreNUM.setFillColor(Color::White);
    scoreNUM.setPosition(275, 150);

    scoreTEXT.setFont(font);
    scoreTEXT.setCharacterSize(60);
    scoreTEXT.setString("SCORE");
    scoreTEXT.setFillColor(Color::White);
    scoreTEXT.setPosition(207, 70);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setString("GAME OVER!");
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(150, 320);

    pressRtoRestart.setFont(font);
    pressRtoRestart.setCharacterSize(20);
    pressRtoRestart.setString("Press R to Restart");
    pressRtoRestart.setFillColor(Color::White);
    pressRtoRestart.setPosition(220, 380);

    // Background
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\Users\\wlsgu\\OneDrive\\background_image.jpg"))
    {
        cerr << "배경 이미지 못 읽음" << endl;
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);

    // Player
    RectangleShape player(Vector2f(50, 50));
    player.setFillColor(Color::Green);
    player.setPosition(275, 800);
    float playerSpeed = 0.1f;

    // Lives
    Texture heartTexture;
    if (!heartTexture.loadFromFile("C:\\Users\\wlsgu\\OneDrive\\heart_image.png"))
    {
        cerr << "목숨 이미지 파일 못 읽음" << endl;
        return -1;
    }

    Sprite heartSprite;
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.1f, 0.1f);
    int lives = 3;

    // Meteorite
    CircleShape meteorite(25);
    meteorite.setFillColor(Color::Red);
    Vector2f meteoritePosition(rand() % 525, 0);
    float meteoriteSpeed = 0.2f;

    // Buff
    Texture buffIcon;
    if (!buffIcon.loadFromFile("C:\\Users\\wlsgu\\OneDrive\\buffIcon_image.png"))
    {
        cerr << "버프 이미지 파일 못 읽음" << endl;
        return -1;
    }
    Sprite buff;
    buff.setTexture(buffIcon);
    buff.setScale(0.3f, 0.3f);
    float buffSpeed = 0.1f;
    Clock buffClock; //buff가 20초마다 등장할 수 있도록 buffinterver과 비교하기위한 개체
    float buffinterval = 20.0f;
    bool buffDropped = false;


    // Life
    Sprite life;
    life.setTexture(heartTexture);
    life.setScale(0.1f, 0.1f);
    float lifeSpeed = 0.1f;
    Clock lifeClock; //life가 15초마다 등장할 수 있도록 lifeInterver과 비교하기위한 개체
    float lifeInterval = 15.0f;
    bool lifeDropped = false;

    RectangleShape bullet(Vector2f(0.1f, 0.3f));
    bullet.setFillColor(Color::Yellow);

    // Game State
    bool gamePaused = false;
    int consecutiveDodges = 0; //연속으로 피한 운석 갯수를 담는 변수
    int score = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (gamePaused && event.type == Event::KeyPressed && event.key.code == Keyboard::R)
            {
                // Restart the game
                player.setPosition(275, 800);
                meteorite.setPosition(rand() % 525, 0);
                gamePaused = false;
                score = 0;
                lives = 3;
                consecutiveDodges = 0;
                lifeClock.restart();
                buffClock.restart();
                life.setPosition(rand() % 525, -30);
                buff.setPosition(rand() % 525, -30);
                lifeDropped = false;
                buffDropped = false;
            }
        }

        if (!gamePaused)
        {
            // Player Movement
            if (Keyboard::isKeyPressed(Keyboard::Left) && player.getPosition().x > 0)
                player.move(-playerSpeed, 0);

            if (Keyboard::isKeyPressed(Keyboard::Right) && player.getPosition().x < 550)
                player.move(playerSpeed, 0);

            // Meteorite Movement
            meteorite.move(0, meteoriteSpeed);

            // Player and Meteorite Collision
            if (player.getGlobalBounds().intersects(meteorite.getGlobalBounds()))
            {
                lives--;

                if (lives <= 0)
                    gamePaused = true;
                else
                {
                    meteorite.setPosition(rand() % 525, 0);
                    consecutiveDodges = 0;
                }
            }

            // Meteorite Reset
            if (meteorite.getPosition().y > 1000)
            {
                meteorite.setPosition(rand() % 525, 0);
                score += 3;
                consecutiveDodges++;

                if (consecutiveDodges == 5) //5개를 연속으로 피했을 시 + 5점
                {
                    score += 2; //기본 점수 +3과 추가 점수 +2
                    consecutiveDodges = 0;
                }
            }

            // Life Movement and Collision
            if (!lifeDropped)
            {
                life.move(0, lifeSpeed);

                if (lifeClock.getElapsedTime().asSeconds() >= lifeInterval)
                {
                    life.setPosition(rand() % 525, -30);

                    lifeClock.restart();
                    lifeDropped = false;
                }

                if (player.getGlobalBounds().intersects(life.getGlobalBounds()))
                {
                    lives++;
                    life.setPosition(-300, -300);
                    lifeDropped = false;
                }
            }

            // Buff Movement and Collision
            if (!buffDropped)
            {
                buff.move(0, buffSpeed);
                if (buffClock.getElapsedTime().asSeconds() >= buffinterval)
                {
                    buff.setPosition(rand() % 525, -30);
                    buffClock.restart();
                    buffDropped = false;
                }

                if (player.getGlobalBounds().intersects(buff.getGlobalBounds()))
                {
                    bullet.setPosition(player.getPosition().x + player.getSize().x / 2 - bullet.getSize().x / 2,player.getPosition().y);
                    buff.setPosition(-300, -300);
                    buffDropped = false;
                }
                if (buffDropped) {
                    if (Keyboard::isKeyPressed(Keyboard::Up) && bullet.getPosition().y > 0)
                    {
                        bullet.move(0, -0.3f);
                    }
                }
            }       

        }

        window.clear();

        // Draw Background, Player, Meteorite
        window.draw(backgroundSprite);
        window.draw(player);
        window.draw(meteorite);
        window.draw(scoreTEXT);
        window.draw(bullet);
        // Draw Life and Buff if not dropped
        if (!lifeDropped)
            window.draw(life);

        if (!buffDropped)
            window.draw(buff);

        // Draw Lives
        for (int i = 0; i < lives; ++i)
        {
            heartSprite.setPosition(10 + i * 40, 10);
            window.draw(heartSprite);
        }

        // Draw Score
        scoreNUM.setString(to_string(score));
        window.draw(scoreNUM);
        window.draw(scoreTEXT);

        // Draw Game Over and Restart Text
        if (gamePaused)
        {
            window.draw(gameOverText);
            window.draw(pressRtoRestart);
        }
      
        window.display();
    }

    return 0;
}