#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "ellipse.hpp"

using namespace sf;
using namespace std;

class tPoint {

  private:
    RectangleShape dot;
    Texture texture;
    Sprite ballSprite;

  protected:
    Color color;
    float speed;
    float posX;
	float posY;
	float dx;
	float dy;
    void LoadColor(Color color) {
        this->color = color; 
    }

  public:
    tPoint(RenderWindow &window, Color color) {
        int direction = rand() % 100;
	    speed = rand() % 4 + 1;
	    dx = cos(direction) * speed;
	    dy = sin(direction) * speed;
        LoadColor(color);
    }

    tPoint();

    void setPos(int x = 0, int y = 0) {
	    posX = x;
	    posY = y;
    }

    virtual void draw(RenderWindow& window) {
        if(!ballSprite.getTexture()) {
            dot.setSize(Vector2f(8.f, 8.f));
            dot.setOrigin(4.f, 4.f);
	        dot.setPosition(posX, posY);
            dot.setFillColor(color);
            window.draw(dot);
        } else {
            ballSprite.setPosition(posX, posY);
            ballSprite.setScale(0.09, 0.09);
            window.draw(ballSprite);
        }
    }

    virtual void move() {
    	posX += dx;
    	posY += dy;
    	if ((int)posX >= 1195 || (int)posX <= 5)
    		dx *= -1;
    	if ((int)posY >= 795 || (int)posY <= 5)
    		dy *= -1;
    }

    virtual void rotate() {
        dot.setRotation(dot.getRotation() + rand()%5-5);
    }

    int getPosX() {	return posX; }
    int getPosY() {	return posY; }
    virtual ~tPoint() {}

};

tPoint::tPoint(): dx(rand()%20-10), dy(rand()%20-10) {
    texture.loadFromFile("img/ball.png");
    ballSprite.setTexture(texture);
    cout << "RUSSIA IS NOT A POINT!!!" << endl;
}

class tLine : public tPoint {

  private:
    RectangleShape line;

  public:
    tLine(RenderWindow &window, Color color) : tPoint(window, color) {}
    
    void draw(RenderWindow &window) override {
        line.setSize(Vector2f(26.f, 4.f));
        line.setOrigin(13.f, 2.f);
        line.setFillColor(color);
	    line.setPosition(posX, posY);
        window.draw(line);
    }

    void rotate() override {
        line.rotate(rand()%5-5);
    }
};

class tCircle : public tPoint {

  private:
    CircleShape circle;

  public:
    tCircle(RenderWindow &window, Color color) : tPoint(window, color) {}
    
    void draw(RenderWindow &window) override {
        CircleShape circle(15.f);
        circle.setFillColor(Color::Transparent);
        circle.setOutlineThickness(3.f);
        circle.setOutlineColor(color);
	    circle.setPosition(posX, posY);
        window.draw(circle);
    }

    void rotate() override {
        circle.rotate(rand()%5-5);
    }
};

class tRectangle : public tLine {

  private:
    RectangleShape rect;

  public:
    tRectangle(RenderWindow &window, Color color) : tLine(window, color) {}
    
    void draw(RenderWindow &window) override {
        rect.setSize(Vector2f(28.f, 16.f));
        rect.setOrigin(14.f, 8.f);
        rect.setFillColor(Color::Transparent);
        rect.setOutlineThickness(3.f);
        rect.setOutlineColor(color);
	    rect.setPosition(posX, posY);
        window.draw(rect);
    }

    void rotate() override {
        rect.rotate(rand()%5-5);
    }
};

class tTriangle : public tLine {

  private:
    ConvexShape triangle;

  public:
    tTriangle(RenderWindow &window, Color color) : tLine(window, color) {}
    
    void draw(RenderWindow &window) override {
        triangle.setPointCount(3);
        triangle.setPoint(0, Vector2f(0, 0));
        triangle.setPoint(1, Vector2f(0, 20.f));
        triangle.setPoint(2, Vector2f(20.f, 10.f));
        triangle.setOrigin(10.f, 10.f);
        triangle.setFillColor(Color::Transparent);
        triangle.setOutlineThickness(3.f);
        triangle.setOutlineColor(color);
	    triangle.setPosition(posX, posY);
        window.draw(triangle);    
    }

    void rotate() override {
        triangle.rotate(rand()%15-15);
    }
};

class tEllipse : public tRectangle {

  private:
    EllipseShape ellipse;
    Transform t;
    int cnt = 5;

  public:

    tEllipse(RenderWindow &window, Color color) : tRectangle(window, color) {}
    
    void draw(RenderWindow &window) override {
        EllipseShape ellipse(Vector2f(26.f, 10.f));
        ellipse.setOrigin(13.f, 5.f);
        ellipse.setFillColor(Color::Transparent);
        ellipse.setOutlineThickness(3.f);
        ellipse.setOutlineColor(color);
	    ellipse.setPosition(posX, posY);
        Transform t; 
        t.rotate(cnt, posX, posY);
        window.draw(ellipse, t);
        cnt++;
    }
    
    void rotate() override {
        ellipse.rotate(rand()%5-5);
    }
};

class tRhomb : public tTriangle {

  private:
    ConvexShape rhomb;

  public:
    tRhomb(RenderWindow &window, Color color) : tTriangle(window, color) {}
    
    void draw(RenderWindow &window) override {
        rhomb.setPointCount(4);
        rhomb.setPoint(0, Vector2f(0, 0));
        rhomb.setPoint(1, Vector2f(0, 30.f));
        rhomb.setPoint(2, Vector2f(30.f, 45.f));
        rhomb.setPoint(3, Vector2f(30.f, 15.f));
        rhomb.setOrigin(15.f, 27.f);
        rhomb.setFillColor(Color::Transparent);
        rhomb.setOutlineThickness(3.f);
        rhomb.setOutlineColor(color);
	    rhomb.setPosition(posX, posY);
        window.draw(rhomb);    
    }

    void rotate() override {
        rhomb.rotate(rand()%5-5);
    }
};
