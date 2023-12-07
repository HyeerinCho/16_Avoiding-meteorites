#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

// 운석 클래스 선언
class Asteroid {
public:
    CircleShape shape;  // 운석 모양
    float speed;  // 운석 이동 속도

    // 생성자: 초기 위치를 입력받아 운석 생성
    Asteroid(float startX, float startY) {
        shape.setPosition(startX, startY);
        shape.setRadius(25);
        shape.setFillColor(Color::Gray);

        speed = (rand() % 5) + 2;  // 운석 속도 랜덤 설정
    }

    // 운석 위치 업데이트
    void update() {
        shape.move(0, speed);
    }
};

int main() {
    srand(time(0));  // 랜덤 시드 설정

    RenderWindow window(VideoMode(600, 900), "Asteroid Dodge");  // 게임 창 생성
    vector<Asteroid> asteroids;  // 운석 저장을 위한 벡터

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            // 창 닫기 이벤트 처리
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // 일정 확률로 운석 생성
        if (rand() % 100 < 2) {
            asteroids.push_back(Asteroid(rand() % 600, 0));
        }

        // 각 운석 위치 업데이트
        for (auto& asteroid : asteroids) {
            asteroid.update();
        }

        window.clear();  // 화면 클리어
        // 각 운석 그리기
        for (const auto& asteroid : asteroids) {
            window.draw(asteroid.shape);
        }
        window.display();  // 화면에 그림 표시
    }

    return 0;
}
