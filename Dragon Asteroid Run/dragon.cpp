#include <sl.h>
#include "dragon.h"
#include <string>
using namespace std;

void Dragon::load()
{
    for (int i = 0; i < 9; i++)
    {
        string path = "assets/dragon/" + to_string(i) + ".png";
        tex[i] = slLoadTexture(path.c_str());
    }
}

void Dragon::update(double dt)
{
    // Input: panah atas = naik, panah bawah = turun
    if (slGetKey(SL_KEY_UP)) speedY = 200;
    else if (slGetKey(SL_KEY_DOWN)) speedY = -200;
    else speedY = 0;

    y += speedY * dt;

    // Batas layar
    if (y < 0) y = 0;
    if (y > 600) y = 600;

    // Animasi naga (frame berganti tiap 0.1 detik)
    animTime += dt;
    if (animTime >= 0.1)
    {
        frame = (frame + 1) % 9;
        animTime = 0;
    }
}

void Dragon::render()
{
    slSprite(tex[frame], x, y, 100, 100);
}

void Dragon::reset()
{
    x = 200;
    y = 300;
    speedY = 0;
    frame = 0;
}
