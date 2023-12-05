#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;
int main()
{
    RenderWindow window(VideoMode(600, 900), "Avoid the Meteorites!");
    Font font;

    if (!font.loadFromFile("C:\\Users\\wlsgu\\OneDrive\\���� ȭ��\\������\\KdamThmorPro-Regular.ttf"))
    {
        cerr << "��Ʈ ���� �� ����" << endl;
        return -1;
    }

    // ��� �̹��� �ε�
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\Users\\wlsgu\\OneDrive\\background_image.jpg")) {
        cerr << "��� �̹��� �� ����" << endl;
        return -1;
    }

    // ��� ����
    Sprite backgroundSprite(backgroundTexture);


    Text gameOverText; //GAME OVER! �ؽ�Ʈ ����
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setString("GAME OVER!");
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(150, 200);

    Text pressRtoRestart; //Press R to Restart �ؽ�Ʈ ����
    pressRtoRestart.setFont(font);
    pressRtoRestart.setCharacterSize(20);
    pressRtoRestart.setString("Press R to Restart");
    pressRtoRestart.setFillColor(Color::White);
    pressRtoRestart.setPosition(220, 260);

    Text scoreNUM; //���� �ؽ�Ʈ ����
    scoreNUM.setFont(font);
    scoreNUM.setCharacterSize(40);
    scoreNUM.setFillColor(Color::White);
    scoreNUM.setPosition(280, 150);

    Text scoreTEXT; //score �ؽ�Ʈ ����
    scoreTEXT.setFont(font);
    scoreTEXT.setCharacterSize(40);
    scoreTEXT.setFillColor(Color::White);
    scoreTEXT.setPosition(280, 150);


    int score = 0;
    int remainingLives = 3; // �ʱ� ��� ����

    Texture heartTexture;
    if (!heartTexture.loadFromFile("C:\\Users\\wlsgu\\OneDrive\\heart_image.jpg"))
    {
        cerr << "��� �̹��� ���� �� ����" << endl;
        return -1;
    }

    

    Sprite heartSprite;
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.1f, 0.1f);

    RectangleShape player(Vector2f(50, 50)); //�÷��̾� ����
    player.setFillColor(Color::Green);
    player.setPosition(275, 800);

    CircleShape meteorite(25); //� ����
    meteorite.setFillColor(Color::Red);
    Vector2f meteoritePosition(rand() % 525, 0);

    RectangleShape life(Vector2f(30, 30)); // ��� ����
    life.setFillColor(Color::Blue);
    life.setPosition(rand() % 525, -30); // ȭ�� ��� �ۿ��� ����

    float playerSpeed = 0.1f;
    float meteoriteSpeed = 0.2f;
    float lifeSpeed = 0.1f;

    bool gamePaused = false; //��� ������ �ε����� true�� ��
    int consecutiveDodges = 0; // �������� ���� Ƚ��

    Clock lifeClock; // ����� �������� �ֱ⸦ �����ϱ� ���� Clock ��ü
    float lifeInterval = 3.0f; // ����� �������� �ֱ� (��)
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
                meteorite.setPosition(rand() % 525, 0);
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

            meteorite.move(0, meteoriteSpeed);

            if (player.getGlobalBounds().intersects(meteorite.getGlobalBounds()))
            {
                remainingLives--;

                if (remainingLives <= 0)
                {
                    gamePaused = true;
                }
                else
                {
                    meteorite.setPosition(rand() % 525, 0);
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
                    lifeDropped = true; // ����� ������ ���·� ����
                }

                if (player.getGlobalBounds().intersects(life.getGlobalBounds()))
                {
                    remainingLives++;
                    life.setPosition(rand() % 525, -30);
                    lifeDropped = true; // ����� ������ ���·� ����
                }
            }

            if (meteorite.getPosition().y > 1000)
            {
                meteorite.setPosition(rand() % 525, 0);
                score++; // ��� ���� ������ ���ھ� ����
                consecutiveDodges++;

                if (consecutiveDodges == 5)
                {
                    score += 2; // 5�� �������� ���� ��� 3�� �߰��� ���� 2�� ����, �⺻ � ���� + �߰� ���� 2��
                    consecutiveDodges = 0; // �������� ���� Ƚ�� �ʱ�ȭ
                }
            }
        }

        window.clear();

        //��� & ĳ���� & � �׸���
        window.draw(backgroundSprite);
        window.draw(player);
        window.draw(meteorite);

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
