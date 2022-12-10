#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

const int pixelLength = 540, pixelHeight = 960;

class Game {};

class Sources : public Game {};

class Background : public virtual Sources {};

class Platform : public virtual Sources, public Background {};

class Player : public virtual Sources, public Platform {};

class Enemy : public virtual Sources {};

class Crime : public Player, public Enemy {};

bool startEngine(){};
