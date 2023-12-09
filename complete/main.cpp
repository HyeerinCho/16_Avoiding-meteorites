#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(600, 900), "Avoid the Meteorites!");
    Font font;
    Clock clock_time;

    // Ÿ�̸� ����
    Text timeText;
    int t = 0;
    timeText.setFont(font);
    timeText.setString(to_string(t) + " sec");
    timeText.setCharacterSize(20);
    timeText.setFillColor(Color::White);
    timeText.setPosition(window.getSize().x - 100, 10);

    clock_t time = clock();


    // ��Ʈ ����
    font.loadFromFile("E:\\KdamThmorPro-Regular.ttf");

    // ��� �̹��� �ε�
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("E:\\����ȭ��\\������ ����\\12week\\Portfolio_img_128.png");

    // ��� ����
    Sprite backgroundSprite(backgroundTexture);

    Sprite background1(backgroundTexture);
    Sprite background2(backgroundTexture);

    // �ʱ� ��ġ ����
    background1.setPosition(0, 0);
    background2.setPosition(backgroundTexture.getSize().x, 0);


    // �ؽ�Ʈ ����
    Text gameOverText; //GAME OVER!
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setString("GAME OVER!");
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(150, 200);

    Text pressRtoRestart; // Press R to Restart
    pressRtoRestart.setFont(font);
    pressRtoRestart.setCharacterSize(20);
    pressRtoRestart.setString("Press R to Restart");
    pressRtoRestart.setFillColor(Color::White);
    pressRtoRestart.setPosition(220, 260);

    Text scoreNUM; // ����
    scoreNUM.setFont(font);
    scoreNUM.setCharacterSize(40);
    scoreNUM.setFillColor(Color::White);
    scoreNUM.setPosition(280, 150);

    Text scoreTEXT; // score
    scoreTEXT.setFont(font);
    scoreTEXT.setCharacterSize(40);
    scoreTEXT.setFillColor(Color::White);
    scoreTEXT.setPosition(280, 150);


    // ���(Life) �ؽ��� ����
    int score = 0;
    int remainingLives = 3; // �ʱ� ��� ����

    Texture heartTexture;
    heartTexture.loadFromFile("E:\\����ȭ��\\������ ����\\12week\\heart-img.png");

    Sprite heartSprite;
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.1, 0.1);

    // ��� ����
    Texture lifeTexture;
    lifeTexture.loadFromFile("E:\\����ȭ��\\������ ����\\12week\\potion.png");

    Sprite life;
    life.setTexture(lifeTexture);
    life.setScale(0.1f, 0.1f);
    life.setPosition(rand() % 525, -30); // ȭ�� ��� �ۿ��� ����

    // � ����
    Texture meteorTexture;
    meteorTexture.loadFromFile("E:\\����ȭ��\\������ ����\\12week\\meteor.png");

    Sprite _meteor;
    _meteor.setTexture(meteorTexture);
    _meteor.setPosition(rand() % 525, 0);

    // �÷��̾� ����
    Texture playerTexture;
    if (!playerTexture.loadFromFile("E:\\����ȭ��\\������ ����\\12week\\stone_3.png")) {
        cerr << "�÷��̾� �̹��� �� ����" << endl;
        return -1;
    }
    
    Sprite player;
    player.setTexture(playerTexture);
    player.setPosition(275, 800);


    //�÷��̾� �ӵ� ����
    float playerSpeed = 0.1;
    const float speedIncrease = 5.0; // �ӵ��� ������ų ����(��)
    float timeSpeedIncrease = 0.0;

    float meteoriteSpeed = 0.15;
    float lifeSpeed = 0.1;

    bool gamePaused = false; //��� ������ �ε����� true�� ��
    int consecutiveDodges = 0; // �������� ���� Ƚ��

    Clock lifeClock; // ����� �������� �ֱ⸦ ����
    float lifeInterval = 3.0; // ����� �������� �ֱ� (��)
    bool lifeDropped = false; // ����� ������ �������� ����

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (gamePaused && event.type == Event::KeyPressed && event.key.code == Keyboard::R) //RŰ�� ���� ���� ������ϱ�
            {
                player.setPosition(275, 800);
                _meteor.setPosition(rand() % 525, 0);
                gamePaused = false;
                score = 0; // ���� ����۽� ���ھ� �ʱ�ȭ
                remainingLives = 3; // ���� ����۽� ��� �ʱ�ȭ
                consecutiveDodges = 0; // �������� ���� Ƚ�� �ʱ�ȭ
                lifeClock.restart(); // ��� �������� �ֱ⸦ �ʱ�ȭ
                life.setPosition(rand() % 525, -30); // ȭ�� ��� �ۿ��� ����
                lifeDropped = false; // ��� ������ ���� �ʱ�ȭ
            }
        }

        if (!gamePaused)
        {
            if (Keyboard::isKeyPressed(Keyboard::Left) && player.getPosition().x > 0)
                player.move(-playerSpeed, 0);
            if (Keyboard::isKeyPressed(Keyboard::Right) && player.getPosition().x < 550)
                player.move(playerSpeed, 0);

            _meteor.move(0, meteoriteSpeed);

            if (_meteor.getPosition().y > 1000)
            {
                _meteor.setPosition(rand() % 525, 0);
                score++; // ��� ���� ������ ���ھ� ����
                consecutiveDodges++;

                if (consecutiveDodges == 5)
                {
                    score += 2; // 5�� �������� ���� ��� 3�� �߰��� ���� 2�� ����, �⺻ � ���� + �߰� ���� 2��
                    consecutiveDodges = 0; // �������� ���� Ƚ�� �ʱ�ȭ
                }
            }

            if (player.getGlobalBounds().intersects(_meteor.getGlobalBounds()))
            {
                remainingLives--;

                if (remainingLives <= 0)
                {
                    gamePaused = true;
                }
                else
                {
                    _meteor.setPosition(rand() % 525, 0);
                    consecutiveDodges = 0; // ���ϴٰ� �ε����� �������� ���� Ƚ�� �ʱ�ȭ
                }
            }

            if (!lifeDropped)
            {
                life.move(0, lifeSpeed);

                if (life.getPosition().y > 1000 || lifeClock.getElapsedTime().asSeconds() > lifeInterval)
                {
                    life.setPosition(rand() % 525, -30);
                    lifeClock.restart();
                    lifeDropped = false; 
                }

                if (player.getGlobalBounds().intersects(life.getGlobalBounds()))
                {
                    if (remainingLives < 5) {
                        remainingLives++;
                    }
                    life.setPosition(rand() % 525, -30);
                    lifeDropped = false; 
                }
            }

            timeSpeedIncrease += clock_time.getElapsedTime().asSeconds();
            if (timeSpeedIncrease >= speedIncrease) {
                playerSpeed += 0.05;
                timeSpeedIncrease = 0.0f; // �ʱ�ȭ
            }

            time = clock();
            time = time / CLOCKS_PER_SEC;
            timeText.setString(to_string(time) + " sec");

            // ��� ��ũ�� �ӵ� (���� ����)
            float scrollSpeed = 70.0;

            // ��� �ð��� �ʷ� ��ȯ�Ͽ� �̵� �Ÿ� ���
            float resetTime = clock_time.restart().asSeconds();
            float distance = scrollSpeed * resetTime;

            // ��� �̵�
            background1.move(-distance, 0);
            background2.move(-distance, 0);

            // ù ��° ����� �������� ������� ���������� �̵�
            if (background1.getPosition().x + backgroundTexture.getSize().x < 0) {
                background1.setPosition(background2.getPosition().x + backgroundTexture.getSize().x, 0);
            }

            // �� ��° ����� �������� ������� ���������� �̵�
            if (background2.getPosition().x + backgroundTexture.getSize().x < 0) {
                background2.setPosition(background1.getPosition().x + backgroundTexture.getSize().x, 0);
            }
        }

        window.clear();

        //��� & � & Ÿ�̸� �׸���
        window.draw(background1);
        window.draw(background2);
        window.draw(timeText);
        window.draw(_meteor);
        window.draw(player);

        if (!lifeDropped)
        {
            window.draw(life);
        }

        //��� �׸���
        for (int i = 0; i < remainingLives; ++i)
        {
            heartSprite.setPosition(10 + i * 40, 10);
            window.draw(heartSprite);
        }

        //���� ���
        scoreNUM.setString(to_string(score));
        window.draw(scoreNUM);

        if (gamePaused)
        {
            window.draw(gameOverText);
            window.draw(pressRtoRestart);
        }


        window.display();
    }

    return 0;
}