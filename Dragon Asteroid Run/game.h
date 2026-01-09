#pragma once
#include "dragon.h"
#include "asteroid.h"
#include <vector>

class Game
{
public:
    void load();
    void update();
    void render();

private:
    // Background
    int bgTex;
    int gameState = 0;      // 0: Menu, 1: Main, 2: Game Over
    int menuBgTex;          // Aset gambar menu
    int gameOverBgTex;      // Aset gambar game over
    int startBtnTex, retryBtnTex, exitBtnTex;  // Tambahkan variabel tekstur tombol
    bool isClicked(float x, float y, float w, float h);  // Fungsi bantuan untuk cek apakah mouse mengklik area tertentu
    bool isPaused = false;
    int pauseInfoTex;
    float bgX1 = 400; // posisi background pertama (tengah)
    float bgX2 = 1200; // posisi background kedua
    float bgSpeed = 100.0f; // kecepatan gerak background (pixel/detik)
    float difficultyMultiplier = 1.0f; // Mulai dari kecepatan normal (1x)
    float difficultyRate = 0.01f;     // Seberapa cepat naiknya (0.01 per detik)
    int score = 0;           // Variabel skor
    int highScore = 0; // Tambahkan ini
    int scoreLabelTex; // Untuk gambar "SCORE:"
    int highLabelTex;  // Untuk gambar "HIGH SCORE:"
    int numTex[10];          // Array untuk tekstur angka 0-9  
    void drawScore(float x, float y, int value); // Fungsi pembantu untuk menampilkan skor
    int lives = 5;          // Jumlah nyawa awal
    int heartTex;           // Tekstur untuk gambar hati
    int bgm;        // ID untuk musik latar
    int hitSfx;     // ID untuk efek suara saat menabrak

    Dragon dragon;

    // Multiple asteroids in waves
    std::vector<Asteroid> asteroids;
    int waveSize = 6;
    float waveSpacing = 120.0f;
    float spawnX = 900.0f;
    float baseAsteroidSpeed = 200.0f;
    float speedVariation = 60.0f;

    void spawnWave();
};
