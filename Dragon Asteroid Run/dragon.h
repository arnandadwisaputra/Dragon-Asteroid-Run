#pragma once

class Dragon
{
public:
    void load();
    void update(double dt);
    void render();
    void reset();
    float getX() const { return x; }
    float getY() const { return y; }

private:
    int tex[9]; // 9 frame animasi naga
    int frame = 0;
    double animTime = 0;
    float x = 200;
    float y = 300;
    float speedY = 0;
};
