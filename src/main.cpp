#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

const int pixelLength = 540, pixelHeight = 960;

class Game {

protected:
  Sprite player;
  Texture playerModelRight;
  Texture playerModelLeft;
  Texture playerModelShoot;
  IntRect playerRect;
  vector<Sprite *> backgroundVector;
  Texture background;
  IntRect backgroundRect;
  Sprite gameOverBackground;
  Texture tGameOverBackground;
  vector<Sprite *> tileVector;
  Texture tileModel;
  IntRect tileRect;
  vector<Sprite *> blatVector;
  vector<IntRect *> blatRectVector;
  vector<Sprite *> enemyVector;
  IntRect enemyRect;
  vector<Sprite *> bulletVector;
  Texture bullet;

  Sprite mouseBoundingBox;
  View view, currentView;

  Text score;
  Text scoreForGameOver;
  Text highscore;
  Text displayName;
  Mouse mouse;
  Font font;

public:
  virtual void updateGameView(RenderWindow &win) = 0; // Абстрактные методы
  virtual void updateMouse(RenderWindow &win) = 0; // +1
  virtual void gameOver(RenderWindow &win) = 0;    // +1

  Sprite getPlayer() const { return player; } // Геттеры
  Sprite getGameOverBackground() const { return gameOverBackground; }

  vector<Sprite *> getTileVector() const { return tileVector; }
  vector<Sprite *> getBackgroundVector() const { return backgroundVector; }
  vector<Sprite *> getBulletVector() const { return bulletVector; }
  vector<Sprite *> getEnemyVector() const { return enemyVector; }
  vector<Sprite *> getBlatVector() const { return blatVector; }

  View getView() const { return view; }
  Text getScore() const { return score; }
  Text getScoreForGameOver() const { return scoreForGameOver; }
  Text getHighScore() const { return highscore; }
  Text getDisplayName() const { return displayName; }
};

class Sources : public Game {

protected:
  Clock clockForTiles, clockForBlat, clockForBackground, clockForEnemy,
      clockForEnemy2, clockForBullets, clockForGameOver;

  SoundBuffer lostBuffer;
  Sound lostSound;
  SoundBuffer jumpBuffer;
  Sound jumpSound;
  SoundBuffer breakBuffer;
  Sound breakSound;
  SoundBuffer shoot1Buffer, shoot2Buffer;
  Sound shootSound;
  SoundBuffer monsterBuffer;
  Sound monsterSound;
  SoundBuffer monsterDeathBuffer;
  Sound monsterDeathSound;
  SoundBuffer blatBuffer;
  Sound blatSound;

  float yAccelerationDown, yAccelerationUp;
  bool isJumping, isFalling, isMoving, isLost, canMove, looksLeft, looksRight,
      isShooting, playJumpSound;
  float xVelocity, bulletVelocity, enemyVelocity;
  int gapBetweenTiles, scoreInt, highscoreInt;

public:
  Sources(RenderWindow &win)
      : yAccelerationDown{0}, xVelocity{6}, enemyVelocity{10},
        yAccelerationUp{15}, bulletVelocity{20}, scoreInt{0}, highscoreInt{0},
        gapBetweenTiles{60}, isFalling{false}, playJumpSound{true},
        isShooting{false}, canMove{true}, isJumping{true}, isLost{false},
        isMoving{false}, looksRight{true}, looksLeft{
                                               false} { // Список инизиализаций

    playerModelRight.loadFromFile("img/right.png");
    playerModelRight.setSmooth(true);
    playerModelLeft.loadFromFile("img/left.png");
    playerModelLeft.setSmooth(true);
    playerModelShoot.loadFromFile("img/shoot.png");
    playerModelShoot.setSmooth(true);

    playerRect.left = 0;
    playerRect.top = 0;
    playerRect.width = 62;
    playerRect.height = 170;
    string who = "красава";
    player.setTexture(playerModelRight);
    player.setTextureRect(playerRect);
    player.setPosition(pixelLength / 2, 0);
    player.setScale(0.7, 0.7);

    currentView = win.getView();

    background.loadFromFile("img/background.png");
    backgroundRect.left = 0;
    backgroundRect.top = 0;
    backgroundRect.width = pixelLength;
    backgroundRect.height = pixelHeight;

    tileModel.loadFromFile("img/game-tiles.png");
    tileRect.left = 0;
    tileRect.top = 0;
    tileRect.width = 58;
    tileRect.height = 16;

    bullet.loadFromFile("img/bullet.png");

    view.setCenter(pixelLength / 2, pixelHeight / 2);
    view.setSize(pixelLength, pixelHeight);

    font.loadFromFile("font/Repka.ttf");
    score.setString(to_string(scoreInt));
    score.setFont(font);
    score.setPosition(30, 300);
    score.setCharacterSize(40);
    score.setFillColor(Color::Black);

    scoreForGameOver.setString(to_string(scoreInt));
    scoreForGameOver.setFont(font);
    scoreForGameOver.setPosition(400, 100);
    scoreForGameOver.setCharacterSize(40);
    scoreForGameOver.setFillColor(Color::Black);

    highscore.setFont(font);
    highscore.setString(to_string(highscoreInt));
    highscore.setPosition(400, 100);
    highscore.setCharacterSize(40);
    highscore.setFillColor(Color::Black);

    displayName.setString(to_string(777));
    displayName.setFont(font);
    displayName.setPosition(350, 100);
    displayName.setCharacterSize(40);
    displayName.setFillColor(Color::Black);

    tGameOverBackground.loadFromFile("img/gameOverBackground.png");
    gameOverBackground.setTexture(tGameOverBackground);
    gameOverBackground.setPosition(0, 0);

    lostBuffer.loadFromFile("audio/lost.wav");
    lostSound.setBuffer(lostBuffer);
    lostSound.setVolume(7);

    jumpBuffer.loadFromFile("audio/jump.wav");
    jumpSound.setBuffer(jumpBuffer);
    jumpSound.setVolume(25);

    shoot1Buffer.loadFromFile("audio/shoot1.wav");
    shoot2Buffer.loadFromFile("audio/shoot2.wav");
    shootSound.setBuffer(shoot1Buffer);
    shootSound.setVolume(8);

    monsterBuffer.loadFromFile("audio/monster.wav");
    monsterSound.setBuffer(monsterBuffer);
    monsterSound.setVolume(3);

    monsterDeathBuffer.loadFromFile("audio/monsterdeath.wav");
    monsterDeathSound.setBuffer(monsterDeathBuffer);
    monsterDeathSound.setVolume(50);

    blatBuffer.loadFromFile("audio/blat.wav");
    blatSound.setBuffer(blatBuffer);
    blatSound.setVolume(13);
  }
  Sources(RenderWindow &win, bool caption) {
    background.loadFromFile("img/bratva.png");
    backgroundRect.left = 0;
    backgroundRect.top = 0;
    backgroundRect.width = 1000;
    backgroundRect.height = 720;
  }
  virtual void checkCollision() {}
  virtual void addItem() {}
  virtual void move() {}
  virtual void close() {}
  void updateGameView(RenderWindow &win) override {
    if (!isLost)
      view.setCenter(pixelLength / 2, player.getPosition().y);
    currentView = win.getView();
  }
  void updateMouse(RenderWindow &win) override {
    mouseBoundingBox.setPosition(mouse.getPosition(win).x,
                                 mouse.getPosition(win).y);
  }
  void gameOver(RenderWindow &win) override;
  void loadSafetyTiles(); // Определения функций вне класса
  void updateAssets();
  void updateScore();
  void checkIfLost();
  void deleteAll();
  bool getIsLost() const { return isLost; }

  ~Sources() { deleteAll(); };
};

class Background : public virtual Sources {

private:
  void addItem() override {
    if (!isLost &&
        player.getPosition().y <
            backgroundVector[backgroundVector.size() - 1]->getPosition().y +
                800 &&
        clockForBackground.getElapsedTime().asSeconds() > 0.01) {
      Sprite *newBackground = new Sprite;
      newBackground->setTexture(background);
      newBackground->setTextureRect(backgroundRect);
      newBackground->setPosition(
          0,
          backgroundVector[backgroundVector.size() - 1]->getPosition().y - 800);

      backgroundVector.push_back(newBackground);
      clockForBackground.restart();
    }
  }
  void close() override {
    for (int i = 0; i < backgroundVector.size(); i++) {
      if (!isLost &&
          player.getPosition().y < backgroundVector[i]->getPosition().y - 500) {
        delete backgroundVector[i];
        backgroundVector.erase(backgroundVector.begin() + i);
      }
    }
  }

public:
  Background(RenderWindow &win) : Sources(win) {}
  friend class Crime;
};

class Platform : public virtual Sources,
                 public Background { // Ромбовидное наследование

private:
  void checkCollision() override {
    for (int i = 0; i < blatVector.size(); i++) {
      if (!isJumping && blatVector[i]->getGlobalBounds().intersects(
                            player.getGlobalBounds())) {
        blatRectVector[i]->top = 114;
        blatRectVector[i]->height = 29;
        blatVector[i]->setTextureRect(*(blatRectVector[i]));
        blatVector[i]->setPosition(blatVector[i]->getPosition().x,
                                   blatVector[i]->getPosition().y - 5);
        blatSound.play();
        yAccelerationUp = 25;
      }
    }
  }
  void addItem() override {
    if (!isLost && clockForTiles.getElapsedTime().asSeconds() > 0.1 &&
        player.getPosition().y < scoreInt + 5) {
      float rTilePositionX = rand() % (pixelLength - tileRect.width);

      Sprite *newTile = new Sprite;
      newTile->setTexture(tileModel);
      newTile->setTextureRect(tileRect);
      newTile->setPosition(rTilePositionX,
                           tileVector[tileVector.size() - 1]->getPosition().y -
                               gapBetweenTiles);

      tileVector.push_back(newTile);

      if (clockForBlat.getElapsedTime().asSeconds() > 1) {
        Sprite *newBlat = new Sprite;
        IntRect *newRect = new IntRect;

        newRect->left = 395;
        newRect->top = 100;
        newRect->width = 27;
        newRect->height = 20;

        newBlat->setPosition(newTile->getPosition().x + 16,
                             newTile->getPosition().y - 12);
        newBlat->setTextureRect(*newRect);
        newBlat->setTexture(tileModel);

        blatVector.push_back(newBlat);
        blatRectVector.push_back(newRect);
        clockForBlat.restart();
      }
      clockForTiles.restart();
    }
  }
  void close() override {
    for (int i = 0; i < tileVector.size(); i++) {
      if (!isLost &&
          player.getPosition().y < tileVector[i]->getPosition().y - 500) {
        delete tileVector[i];
        tileVector.erase(tileVector.begin() + i);
      }
    }
    for (int i = 0; i < blatVector.size(); i++) {
      if (!isLost &&
          player.getPosition().y < blatVector[i]->getPosition().y - 500) {
        delete blatVector[i];
        delete blatRectVector[i];
        blatVector.erase(blatVector.begin() + i);
        blatRectVector.erase(blatRectVector.begin() + i);
      }
    }
  }

public:
  Platform(RenderWindow &win) : Sources(win), Background(win) {}
  void drawGreenTiles(RenderWindow &window) {
    float viewY = currentView.getCenter().y - (pixelHeight / 2);
    for (int i = 0; i < tileVector.size(); i++) {
      if (tileVector[i]->getPosition().y > viewY - 100) {
        window.draw(*tileVector[i]);
      } else if (tileVector[i]->getPosition().y < viewY - 100)
        break;
    }
  }
  friend class Crime;
};

class Player : public virtual Sources,
               public Platform { // Второе ромбовидное наследование

private:
  void checkCollision() override {
    if (player.getPosition().x < -playerRect.width && !isLost) {
      player.setPosition(pixelLength, player.getPosition().y);
    }
    if (player.getPosition().x > pixelLength && !isLost) {
      player.setPosition(-playerRect.width, player.getPosition().y);
    }
  }
  void move() override {
    if (Keyboard::isKeyPressed(Keyboard::Key::A) && canMove) {
      player.setPosition(player.getPosition().x - xVelocity,
                         player.getPosition().y);
      isMoving = true;
      looksLeft = true;
      looksRight = false;
      if (!isShooting)
        player.setTexture(playerModelLeft);
    } else
      isMoving = false;
    if (Keyboard::isKeyPressed(Keyboard::Key::D) && canMove) {
      player.setPosition(player.getPosition().x + xVelocity,
                         player.getPosition().y);
      isMoving = true;
      looksLeft = false;
      looksRight = true;
      if (!isShooting)
        player.setTexture(playerModelRight);
    } else
      isMoving = false;
  }

public:
  Player(RenderWindow &win) : Sources(win), Platform(win) {}
  void shoot() {
    if (!isLost && Keyboard::isKeyPressed(Keyboard::Key::Space) &&
        clockForBullets.getElapsedTime().asSeconds() > 0.1) {
      Sprite *newBullet = new Sprite;
      newBullet->setTexture(bullet);
      newBullet->setPosition(player.getPosition().x + 12,
                             player.getPosition().y);

      bulletVector.push_back(newBullet);
      player.setTexture(playerModelShoot);
      isShooting = true;
      int rShootingSound = rand() % 2 + 1;
      switch (rShootingSound) {
      case 1:
        shootSound.setBuffer(shoot1Buffer);
        break;
      case 2:
        shootSound.setBuffer(shoot2Buffer);
        break;
      }
      shootSound.play();
      clockForBullets.restart();
    } else if (!isLost && clockForBullets.getElapsedTime().asSeconds() > 0.3) {
      isShooting = false;
      if (looksLeft) {
        player.setTexture(playerModelLeft);
      } else {
        player.setTexture(playerModelRight);
      }
    }
    for (int i = 0; i < bulletVector.size(); i++) {
      bulletVector[i]->setPosition(bulletVector[i]->getPosition().x,
                                   bulletVector[i]->getPosition().y -
                                       bulletVelocity);
    }
    for (int i = 0; i < bulletVector.size(); i++) {
      if (bulletVector[i]->getPosition().y + 500 < player.getPosition().y) {
        delete bulletVector[i];
        bulletVector.erase(bulletVector.begin() + i);
      }
    }
  }
  void jump() {
    if (isJumping && !isLost) {
      player.setPosition(player.getPosition().x,
                         player.getPosition().y - yAccelerationUp);
      yAccelerationUp -= 0.5;

      if (yAccelerationUp <= 0) {
        isJumping = false;
        isFalling = true;
        playJumpSound = true;
        yAccelerationUp = 15;
      }
    }
  }
  void gravity() {
    for (int i = 0; i < tileVector.size(); i++) {
      if (isLost || !tileVector[i]->getGlobalBounds().intersects(
                        player.getGlobalBounds()) &&
                        !isJumping) {
        player.setPosition(player.getPosition().x,
                           player.getPosition().y + yAccelerationDown);
        yAccelerationDown += 0.55;
        isFalling = true;
        break;
      } else if (!isLost) {
        yAccelerationDown = 0;
        isFalling = false;
        isJumping = true;
        if (playJumpSound) {
          jumpSound.play();
          playJumpSound = false;
        }
      }
    }
    for (int i = 0; i < tileVector.size(); i++) {
      if (!isLost && tileVector[i]->getGlobalBounds().intersects(
                         player.getGlobalBounds())) {
        yAccelerationDown = 0;
        isFalling = false;
        isJumping = true;
        break;
      }
    }
  }
  friend class Crime;
};

class Enemy : public virtual Sources {

private:
  void move() override {
    for (int i = 0; i < enemyVector.size(); i++) {
      if (clockForEnemy2.getElapsedTime().asSeconds() > 0.2) {
        enemyVector[i]->setPosition(enemyVector[i]->getPosition().x +
                                        enemyVelocity,
                                    enemyVector[i]->getPosition().y);
        enemyVelocity *= -1;
        monsterSound.play();
        clockForEnemy2.restart();
      }
    }
  }
  void close() override {
    for (int i = 0; i < enemyVector.size(); i++) {
      for (int j = 0; j < bulletVector.size(); j++) {
        if (bulletVector[j]->getGlobalBounds().intersects(
                enemyVector[i]->getGlobalBounds())) {
          delete enemyVector[i];
          enemyVector.erase(enemyVector.begin() + i);
          monsterDeathSound.play();
          break;
        }
      }
    }
    for (int i = 0; i < enemyVector.size(); i++) {
      if (enemyVector[i]->getPosition().y > player.getPosition().y + 500) {
        delete enemyVector[i];
        enemyVector.erase(enemyVector.begin() + i);
      }
    }
  }
  void checkCollision() override {
    for (int i = 0; i < enemyVector.size(); i++) {
      if (enemyVector[i]->getGlobalBounds().intersects(
              player.getGlobalBounds()) &&
          isFalling) {
        delete enemyVector[i];
        enemyVector.erase(enemyVector.begin() + i);
        yAccelerationUp = 15;
        isFalling = false;
        isJumping = true;
        monsterDeathSound.play();
        break;
      }
      if (player.getGlobalBounds().intersects(
              enemyVector[i]->getGlobalBounds())) {
        isLost = true;
        lostSound.play();
        break;
      }
    }
  }

public:
  Enemy(RenderWindow &win) : Sources(win) {}
  void addEnemy() {

    if (clockForEnemy.getElapsedTime().asSeconds() > 4 &&
        player.getPosition().y < scoreInt - 1) {
      int rEnemyModel = rand() % 2 + 1;
      float rEnemyPositionX = rand() % 450 + 50;

      Sprite *newEnemy = new Sprite;
      newEnemy->setPosition(rEnemyPositionX, player.getPosition().y - 550);
      newEnemy->setTexture(tileModel);

      switch (rEnemyModel) {
      case 1:
        enemyRect.left = 148;
        enemyRect.top = 265;
        enemyRect.height = 46;
        enemyRect.width = 56;
        break;
      case 2:
        enemyRect.left = 336;
        enemyRect.top = 169;
        enemyRect.height = 100;
        enemyRect.width = 56;
        break;
      }
      newEnemy->setTextureRect(enemyRect);

      enemyVector.push_back(newEnemy);
      clockForEnemy.restart();
    }
  }
  friend class Crime;
}; // Дружественный класс

class Crime : public Player,
              public Enemy { // Перегрузка конструктора при ↓наследовании

private:
  bool bratva = true;

public:
  Crime(RenderWindow &win, bool titre)
      : Sources(win, bratva), Player(win), Enemy(win) {}
  Crime(RenderWindow &win) : Sources(win), Player(win), Enemy(win) {}
  void checkCollision() override {
    ::Player::checkCollision();
    ::Platform::checkCollision();
    ::Enemy::checkCollision();
  }
  void addItem() override {
    ::Platform::addItem();
    ::Background::addItem();
  }
  void move() override {
    ::Player::move();
    ::Enemy::move();
  }
  void close() override {
    ::Enemy::close();
    ::Platform::close();
    ::Background::close();
  }
};

bool startEngine() {
  bool titre;
  RenderWindow window(VideoMode(1080, 1920), "Repka bandit");
  window.setFramerateLimit(35);
  srand(time(NULL));
  Crime game(window); // Включить титры?
  game.loadSafetyTiles();

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
        if (event.key.code == sf::Keyboard::Tab)
          window.close();
    }

    game.updateMouse(window);
    game.gameOver(window);

    window.setView(game.getView());
    game.updateGameView(window);
    game.addItem();
    game.addEnemy();
    game.checkIfLost();
    game.checkCollision();
    game.updateScore();
    game.updateAssets();
    game.close();
    game.gravity();
    game.jump();
    game.shoot();
    game.move();

    window.clear();

    vector<Sprite *> tileVector = game.getTileVector();
    vector<Sprite *> backgroundVector = game.getBackgroundVector();
    vector<Sprite *> bulletVector = game.getBulletVector();
    vector<Sprite *> enemyVector = game.getEnemyVector();
    vector<Sprite *> blatVector = game.getBlatVector();

    for (int i = 0; i < backgroundVector.size(); i++) {
      window.draw((*backgroundVector[i]));
    }
    for (int i = 0; i < blatVector.size(); i++) {
      window.draw(*(blatVector[i]));
    }
    for (int i = 0; i < tileVector.size(); i++) {
      window.draw(*(tileVector[i]));
    }
    game.drawGreenTiles(window);
    for (int i = 0; i < bulletVector.size(); i++) {
      window.draw((*bulletVector[i]));
    }
    for (int i = 0; i < enemyVector.size(); i++) {
      window.draw(*(enemyVector[i]));
    }
    window.draw(game.getScore());

    if (game.getIsLost()) {
      window.draw(game.getGameOverBackground());
      window.draw(game.getHighScore());
      window.draw(game.getScoreForGameOver());
      window.draw(game.getDisplayName());
    }

    window.draw(game.getPlayer());

    window.display();
  }

  return true;
}

void engineRunning() {
  if (startEngine())
    engineRunning();
}

int main() {
  srand(time(NULL));
  Music music;
  try { // Обработка исключений
    music.openFromFile("audio/bumer.wav");
  } catch (const exception &ex) {
    cout << ex.what() << endl;
    cout << "Музыки не будет" << endl;
  }
  music.play();
  music.setLoop(true);
  music.setVolume(10);
  engineRunning();

  return 0;
}

void Sources::loadSafetyTiles() {
  for (int i = 0; i < 3; i++) {
    Sprite *newBackground = new Sprite;
    newBackground->setTexture(background);
    newBackground->setTextureRect(backgroundRect);
    if (i == 0) {
      newBackground->setPosition(0, 500);
    } else {
      newBackground->setPosition(0, backgroundVector[i - 1]->getPosition().y -
                                        500);
    }

    backgroundVector.push_back(newBackground);
  }

  for (int i = 0; i < 50; i++) {
    float rTilePositionX = rand() % (pixelLength - tileRect.width);

    Sprite *newTile = new Sprite;
    newTile->setTexture(tileModel);
    newTile->setTextureRect(tileRect);

    i != 0 ? newTile->setPosition(rTilePositionX,
                                  tileVector[i - 1]->getPosition().y -
                                      gapBetweenTiles)
           : newTile->setPosition(rTilePositionX, 0);

    tileVector.push_back(newTile);
  }
}
void Sources::deleteAll() {
  for (int i = 0; i < backgroundVector.size(); i++) {
    delete backgroundVector[i];
  }
  for (int i = 0; i < tileVector.size(); i++) {
    delete tileVector[i];
  }
  for (int i = 0; i < bulletVector.size(); i++) {
    delete bulletVector[i];
  }
  for (int i = 0; i < enemyVector.size(); i++) {
    delete enemyVector[i];
  }
  for (int i = 0; i < blatVector.size(); i++) {
    delete blatVector[i];
  }
  for (int i = 0; i < blatRectVector.size(); i++) {
    delete blatRectVector[i];
  }
}
void Sources::updateAssets() {
  score.setPosition(25, currentView.getCenter().y - (pixelHeight / 2));
  scoreForGameOver.setPosition(currentView.getCenter().x + 110,
                               currentView.getCenter().y - 160);
  highscore.setPosition(currentView.getCenter().x + 110,
                        currentView.getCenter().y - 108);
  displayName.setPosition(currentView.getCenter().x + 78,
                          currentView.getCenter().y - 75);
  gameOverBackground.setPosition(currentView.getCenter().x - (pixelLength / 2),
                                 currentView.getCenter().y - (pixelHeight / 2));
}
void Sources::updateScore() {
  if (scoreInt > player.getPosition().y) {
    scoreInt = player.getPosition().y;
  }
  score.setString(to_string(scoreInt * -1));
  highscore.setString(to_string(highscoreInt));
  scoreForGameOver.setString(to_string(scoreInt * -1));

  if (isLost) {
    ifstream highscoreFileRead("docs/highscore.txt");
    string tmp = "", tmp2 = "0";
    if (highscoreFileRead.is_open()) {
      while (getline(highscoreFileRead, tmp)) {
        tmp2 = tmp;
      }
      highscoreFileRead.close();
    }
    ofstream highscoreFileWrite("docs/highscore.txt");
    if (stoi(tmp2) < scoreInt * -1) {
      highscoreFileWrite << scoreInt * -1 << endl;
      highscoreInt = scoreInt * -1;
    } else {
      highscoreFileWrite << stoi(tmp2);
      highscoreInt = stoi(tmp2);
    }
  }
}
void Sources::gameOver(RenderWindow &win) {

  if (isLost) {
    canMove = false;
    if (mouse.isButtonPressed(Mouse::Left) &&
        mouseBoundingBox.getPosition().x > 190 &&
        mouseBoundingBox.getPosition().x < 450 &&
        mouseBoundingBox.getPosition().y > 560 &&
        mouseBoundingBox.getPosition().y < 640 &&
        clockForGameOver.getElapsedTime().asSeconds() > 1) {
      isLost = false;
      canMove = true;
      player.setPosition(pixelLength / 2, 0);
      yAccelerationUp = 15;
      yAccelerationDown = 0;
      isJumping = true;
      scoreInt = 0;

      deleteAll();
      lostSound.stop();

      backgroundVector.resize(0);
      tileVector.resize(0);
      bulletVector.resize(0);
      enemyVector.resize(0);
      blatVector.resize(0);
      blatRectVector.resize(0);

      clockForEnemy.restart();

      loadSafetyTiles();
      clockForGameOver.restart();

    } else if (mouse.isButtonPressed(Mouse::Left) &&
               mouseBoundingBox.getPosition().x > 300 &&
               mouseBoundingBox.getPosition().x < 530 &&
               mouseBoundingBox.getPosition().y > 680 &&
               mouseBoundingBox.getPosition().y < 770) {
      isLost = false;
      canMove = false;
      player.setPosition(pixelLength / 2, 0);
      yAccelerationUp = 15;
      yAccelerationDown = 0;
      isJumping = true;
      scoreInt = 0;

      deleteAll();
      lostSound.stop();

      backgroundVector.resize(0);
      tileVector.resize(0);
      bulletVector.resize(0);
      enemyVector.resize(0);
      blatVector.resize(0);
      blatRectVector.resize(0);

      clockForEnemy.restart();

      loadSafetyTiles();
    }
  }
}
void Sources::checkIfLost() {
  if (player.getPosition().y > tileVector[0]->getPosition().y + 200 &&
      !isLost) {
    isLost = true;
    lostSound.play();
  }
}
