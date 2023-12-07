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

    Text timeText;
    int t = 0;
    timeText.setFont(font);
    timeText.setString(to_string(t) + " sec");
    timeText.setCharacterSize(20);
    timeText.setFillColor(Color::White);
    timeText.setPosition(window.getSize().x - 100, 10);

    clock_t time = clock();



    if (!font.loadFromFile("E:\\arial.ttf")) { //E:\\works\\practice\\ch7\\x64\\Debug
        return 42;
    }


    // 배경 이미지 로드
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("E:\\바탕화면\\컴프실 팀플\\12week\\Portfolio_img_128.png")) {
        cerr << "배경 이미지 못 읽음" << endl;
        return -1;
    }

    // 배경 생성
    Sprite backgroundSprite(backgroundTexture);

    Sprite background1(backgroundTexture);
    Sprite background2(backgroundTexture);

    // 초기 위치 설정
    background1.setPosition(0, 0);
    background2.setPosition(backgroundTexture.getSize().x, 0);


    Text gameOverText; //GAME OVER! 텍스트 설정
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setString("GAME OVER!");
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(150, 200);

    Text pressRtoRestart; //Press R to Restart 텍스트 설정
    pressRtoRestart.setFont(font);
    pressRtoRestart.setCharacterSize(20);
    pressRtoRestart.setString("Press R to Restart");
    pressRtoRestart.setFillColor(Color::White);
    pressRtoRestart.setPosition(220, 260);

    Text scoreNUM; //점수 텍스트 설정
    scoreNUM.setFont(font);
    scoreNUM.setCharacterSize(40);
    scoreNUM.setFillColor(Color::White);
    scoreNUM.setPosition(280, 150);

    Text scoreTEXT; //score 텍스트 설정
    scoreTEXT.setFont(font);
    scoreTEXT.setCharacterSize(40);
    scoreTEXT.setFillColor(Color::White);
    scoreTEXT.setPosition(280, 150);


    int score = 0;
    int remainingLives = 3; // 초기 목숨 개수

    Texture heartTexture;
    if (!heartTexture.loadFromFile("E:\\바탕화면\\컴프실 팀플\\12week\\heart-img.png"))
    {
        cerr << "목숨 이미지 파일 못 읽음" << endl;
        return -1;
    }


    Sprite heartSprite;
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.1f, 0.1f);

    RectangleShape player(Vector2f(50, 50)); //플레이어 설정
    player.setFillColor(Color::Green);
    player.setPosition(275, 800);

    CircleShape meteorite(25); //운석 설정
    meteorite.setFillColor(Color::Red);
    Vector2f meteoritePosition(rand() % 525, 0);

    RectangleShape life(Vector2f(30, 30)); // 목숨 설정
    life.setFillColor(Color::Blue);
    life.setPosition(rand() % 525, -30); // 화면 상단 밖에서 시작

    float playerSpeed = 0.1f;
    const float speedIncrease = 10.0f; // 속도를 증가시킬 간격(초)
    float timeSpeedIncrease = 0.0f;

    float meteoriteSpeed = 0.15f;
    float lifeSpeed = 0.1f;

    bool gamePaused = false; //운석과 유저가 부딪히면 true가 됨
    int consecutiveDodges = 0; // 연속으로 피한 횟수

    Clock lifeClock; // 목숨이 떨어지는 주기를 측정하기 위한 Clock 객체
    float lifeInterval = 3.0f; // 목숨이 떨어지는 주기 (초)
    bool lifeDropped = false; // 목숨이 떨어진 상태인지 여부

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (gamePaused && event.type == Event::KeyPressed && event.key.code == Keyboard::R) //R키를 눌러 게임 재시작하기
            {
                player.setPosition(275, 800);
                meteorite.setPosition(rand() % 525, 0);
                gamePaused = false;
                score = 0; // 게임 재시작시 스코어 초기화
                remainingLives = 3; // 게임 재시작시 목숨 초기화
                consecutiveDodges = 0; // 연속으로 피한 횟수 초기화
                lifeClock.restart(); // 목숨 떨어지는 주기를 초기화
                life.setPosition(rand() % 525, -30); // 화면 상단 밖에서 시작
                lifeDropped = false; // 목숨 떨어진 상태 초기화
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
                    consecutiveDodges = 0; // 피하다가 부딪히면 연속으로 피한 횟수 초기화
                }
            }

            if (!lifeDropped)
            {
                life.move(0, lifeSpeed);

                if (life.getPosition().y > 1000 || lifeClock.getElapsedTime().asSeconds() > lifeInterval)
                {
                    life.setPosition(rand() % 525, -30);
                    lifeClock.restart();
                    lifeDropped = true; // 목숨이 떨어진 상태로 설정
                }

                if (player.getGlobalBounds().intersects(life.getGlobalBounds()))
                {
                    remainingLives++;
                    life.setPosition(rand() % 525, -30);
                    lifeDropped = true; // 목숨이 떨어진 상태로 설정
                }
            }

            if (meteorite.getPosition().y > 1000)
            {
                meteorite.setPosition(rand() % 525, 0);
                score++; // 운석을 피할 때마다 스코어 증가
                consecutiveDodges++;

                if (consecutiveDodges == 5)
                {
                    score += 2; // 5번 연속으로 피한 경우 3점 추가를 위해 2로 설정, 기본 운석 점수 + 추가 점수 2점
                    consecutiveDodges = 0; // 연속으로 피한 횟수 초기화
                }
            }

            timeSpeedIncrease += clock_time.getElapsedTime().asSeconds();
            if (timeSpeedIncrease >= speedIncrease) {
                playerSpeed += 0.1f;
                timeSpeedIncrease = 0.0f; // 초기화
            }

            time = clock();
            time = time / CLOCKS_PER_SEC;
            timeText.setString(to_string(time) + " sec");

            // 배경 스크롤 속도 (조절 가능)
            float scrollSpeed = 70.0f;

            // 경과 시간을 초로 변환하여 이동 거리 계산
            float resetTime = clock_time.restart().asSeconds();
            float distance = scrollSpeed * resetTime;

            // 배경 이동
            background1.move(-distance, 0);
            background2.move(-distance, 0);

            // 첫 번째 배경이 왼쪽으로 사라지면 오른쪽으로 이동
            if (background1.getPosition().x + backgroundTexture.getSize().x < 0) {
                background1.setPosition(background2.getPosition().x + backgroundTexture.getSize().x, 0);
            }

            // 두 번째 배경이 왼쪽으로 사라지면 오른쪽으로 이동
            if (background2.getPosition().x + backgroundTexture.getSize().x < 0) {
                background2.setPosition(background1.getPosition().x + backgroundTexture.getSize().x, 0);
            }
        }

        window.clear();

        //배경 & 캐릭터 & 운석 그리기
        window.draw(background1);
        window.draw(background2);
        window.draw(player);
        window.draw(meteorite);
        window.draw(timeText);

        if (!lifeDropped)
        {
            window.draw(life);
        }

        //목숨 그리기
        for (int i = 0; i < remainingLives; ++i)
        {
            heartSprite.setPosition(10 + i * 40, 10);
            window.draw(heartSprite);
        }

        //점수 출력
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