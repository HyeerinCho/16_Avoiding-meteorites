#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;
using namespace st;

Meteor_Gen meteor;

class Meteor{
private:
    float x;
    float speed;
    //Texture ;
    //Sprite meteor;
    Clock clock;
public:
    bool hit = false;
    bool destroyed  = false;
    bool destroyed_hit = false;
    Meteor(float _x, float _speed);
    void draw(RenderWindow& _window);
    FloatRect getArea();
}

Meteor::Meteor(float _x, float _speed) : x(_x), speed(_speed){
    //assert(meteor_image.loadFromFile(""));
    //meteor.setTexture();
    meteor.setPosition(_x, 0.0f);
}

void Meteor::update(){
    if (destroyed == true){
        return;
    }

    Vector2f pos = meteor.getPosition();
    //if (pos.y > 900.0f - ?f){
        //meteor.setTexture("")
        //speed = 0;
        //destroyed = true;

        //clock.restart();
    //}
    meteor.move(0.0f, 1.0f * speed);
}

void Meteor::draw(RenderWindow& _window){
    if(destroyed == true){
        if(clock.getElapsedTime().asMilliseconds() > 500){
            return;
        }
    }
}

class Meteor_Gen{
private:
    vector<Meteor> meteors;
    float _x = 0;
    float _speed = 0;
    int period;
    Clock clock;
public:
    Meteor_Gen();
    void update();
    void draw(RenderWindow& );
    bool checkHit(FloatRect );
}

Meteor_Gen::Metoer_Gen(){
    period = 0;
}

void Meteor_Gen::update(){
    period = clock.getEllapsedTime().asMilliseconds();

    if (period > rand() % 1000 + 300){
        _x = (float)(rand() % 3300) / 10;
        _speed = (float)((rand() % 50) / 10) + 1;
        Meteor* meteor = new Meteor(_x, _speed);
        meteors.push_back(*meteor);
        clock.restart();
    }

    for (auto& _e : meteors){
        _e.update();
    }
}

void Meteor_Gen::draw(RenderWindow& _window){
    for(auto& _e : meteors){
        _e.draw(_window);
    }
}

int main(){
    RenderWindow window(VideoMode(600, 900),"운석 피하기");
    window.setFramerateLimit(60);

    srand((unsigned int)time(NULL));
    Meteor metoer;
    meteor.draw(window);
    meteor.update();
}