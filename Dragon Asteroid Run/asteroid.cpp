#include <sl.h>
#include <stdlib.h>
#include <time.h>
#include "asteroid.h"
#include <cmath>
#include <cstdio>
#include <string>

using namespace std;

// static definitions
bool Asteroid::texturesLoaded = false;
int Asteroid::tex[5];

void Asteroid::load()
{
    // Load textures only once for all asteroid instances
    if (!texturesLoaded)
    {
        for (int i = 0; i < 5; i++)
        {
            string path = "assets/asteroid/" + to_string(i) + ".png";
            tex[i] = slLoadTexture(path.c_str());
        }
        texturesLoaded = true;
    }
    // initialize this asteroid
    reset();
}

void Asteroid::update(double dt, float speedMultiplier)
{
    posX -= (speed * speedMultiplier) * dt;

    // Jika keluar layar → reset ke default start (will be handled by game for waves)
    if (posX < -50)
        reset();
}

void Asteroid::render()
{
    slSprite(tex[currentTex], posX, posY, 80, 80);
}

void Asteroid::reset()
{
    posX = 900;
    posY = 100 + (rand() % 400); // posisi acak
    currentTex = rand() % 5;
    passed = false;
}

void Asteroid::reset(float startX, float newSpeed)
{
    posX = startX;
    posY = 400.0f + rand() % 400; // posisi acak di gelombang
    speed = newSpeed;
    currentTex = rand() % 5;
    passed = false;
}

bool Asteroid::checkCollision(float x, float y)
{
    // Deteksi tabrakan sederhana (jarak < threshold)
    float dx = x - posX;
    float dy = y - posY;
    float distance = sqrt(dx * dx + dy * dy);
    return (distance < 60);
}
