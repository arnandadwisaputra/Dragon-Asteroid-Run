#pragma once

class Asteroid
{
public:
    static bool texturesLoaded;
    static int tex[5]; // 5 variasi asteroid (shared)

    void load();
    void update(double dt, float speedMultiplier);
    void render();
    void reset(); // original behavior
    void reset(float startX, float newSpeed); // initialize with custom X and speed (for waves)
    bool checkCollision(float x, float y);
    float getX() const { return posX; }
    bool passed = false;

private:
    int currentTex = 0;
    float posX = 900;
    float posY = 300;
    float speed = 200;
};
