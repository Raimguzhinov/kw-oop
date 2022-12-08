#include <unistd.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <iostream>

#include "figures.cpp"

using namespace sf;
using namespace std;

class MainWindow {
   private:
    const int width = 1220;
    const int height = 820;
    Texture texture;
    Sprite background;

   public:
    MainWindow() {
        texture.loadFromFile("img/back2.jpeg");
        background.setTexture(texture);
        background.setScale(0.4, 0.4);
    }
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    Sprite getBackground() { return background; }
};

class Figures {
   public:
    void DrawTraffic(RenderWindow &window, tPoint *Points) {
        Points->move();
        Points->draw(window);
    }
    void DrawTraffic(RenderWindow &window, tPoint *Points, bool rotation) {
        Points->rotate();
        Points->move();
        Points->draw(window);
    }
};

bool startEngine() {
    MainWindow mainWin;
    RenderWindow window(VideoMode(mainWin.GetWidth(), mainWin.GetHeight()),
                        "Figures");
    Color colorArray[7] = {Color::Black, Color::Red,     Color::Green,
                           Color::Blue,  Color::Magenta, Color::Yellow,
                           Color::Cyan};
    window.setFramerateLimit(100);

    int count = 700;
    int arr[2][count];
    bool rotation = true;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < count; j++) arr[i][j] = rand() % 700 + 50;

    tPoint *Points[count];
    Figures figures;

    for (int i = 0; i < count; i += 7) {
        Points[i] = new tPoint(
             //window, colorArray[rand()%7] //RUSSIA IS A POINT?
        );
        Points[i]->setPos(arr[0][i], arr[1][i]);
        Points[i + 1] = new tLine(window, colorArray[rand() % 7]);
        Points[i + 1]->setPos(arr[0][i], arr[1][i]);
        Points[i + 2] = new tCircle(window, colorArray[rand() % 7]);
        Points[i + 2]->setPos(arr[0][i], arr[1][i]);
        Points[i + 3] = new tRectangle(window, colorArray[rand() % 7]);
        Points[i + 3]->setPos(arr[0][i], arr[1][i]);
        Points[i + 4] = new tTriangle(window, colorArray[rand() % 7]);
        Points[i + 4]->setPos(arr[0][i], arr[1][i]);
        Points[i + 5] = new tEllipse(window, colorArray[rand() % 7]);
        Points[i + 5]->setPos(arr[0][i], arr[1][i]);
        Points[i + 6] = new tRhomb(window, colorArray[rand() % 7]);
        Points[i + 6]->setPos(arr[0][i], arr[1][i]);
    }

    while (window.isOpen()) {
        Event event;
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            return true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Tab) ||
            Keyboard::isKeyPressed(Keyboard::Escape)) {
            return false;
        }
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                if (event.key.code == sf::Keyboard::Tab) window.close();
        }
        window.clear();
        window.draw(mainWin.getBackground());
        for (int i = 0; i < count; i++) {
            figures.DrawTraffic(
                window, Points[i],
                rotation);  // Points, Lines, Circles, Rectangles, Triangles,
                            // Ellipses, Rhombus
        }                   // and rotation overload
        window.display();
    }

    for (int i = 0; i < count; i++) {
        delete Points[i];
    }

    return true;
}

void engineRunning() {
    if (startEngine()) engineRunning();
}

int main() {
    srand(time(NULL));
    Music music;
    if (!music.openFromFile("audio/music.ogg")) return -1;
    music.play();
    music.setLoop(true);
    music.setVolume(20);
    engineRunning();

    return 0;
}
