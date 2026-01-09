#include <sl.h>
#include "game.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
using namespace std;

void Game::spawnWave()
{
    asteroids.clear();
    asteroids.resize(waveSize);

    for (int i = 0; i < waveSize; ++i)
    {
        asteroids[i].load(); // textures load once inside
        float startX = spawnX + i * waveSpacing; // spaced horizontally so they come as a wave
        // Slight random speed per asteroid for variety
        float speed = baseAsteroidSpeed + (rand() % (int)(speedVariation * 2 + 1) - speedVariation);
        asteroids[i].reset(startX, speed);
    }
}

void Game::load()
{
    bgTex = slLoadTexture("assets/background/nebula.png");
    menuBgTex = slLoadTexture("assets/ui/menu_bg.jpg");     // Aset gambar menu
    gameOverBgTex = slLoadTexture("assets/ui/game_over.jpg"); // Aset gambar game over
    startBtnTex = slLoadTexture("assets/ui/start_btn.png");
    retryBtnTex = slLoadTexture("assets/ui/retry_btn.png");
    exitBtnTex = slLoadTexture("assets/ui/exit_btn.png");
    pauseInfoTex = slLoadTexture("assets/ui/informasi.png");
    scoreLabelTex = slLoadTexture("assets/ui/score_label.png");
    highLabelTex = slLoadTexture("assets/ui/high_label.png");
    heartTex = slLoadTexture("assets/ui/heart.png");
    // Load Musik dan SFX
    bgm = slLoadWAV("assets/audio/bgm.wav");
    hitSfx = slLoadWAV("assets/audio/hit.wav");

    slSoundLoop(bgm);
 
    for (int i = 0; i < 10; i++) {
        // Mengasumsikan file ada di folder assets/numbers/
        string path = "assets/number/" + to_string(i) + ".png";
        numTex[i] = slLoadTexture(path.c_str());
    }

    dragon.load();

    // Seed RNG once
    srand((unsigned)time(NULL));

    // Spawn the first wave
    spawnWave();
}

bool Game::isClicked(float x, float y, float w, float h) {
    if (slGetMouseButton(SL_MOUSE_BUTTON_LEFT)) {
        float mx = slGetMouseX();
        float my = slGetMouseY();
        // Cek apakah posisi mouse (mx, my) ada di dalam rentang tombol
        return (mx > x - w / 2 && mx < x + w / 2 && my > y - h / 2 && my < y + h / 2);
    }
    return false;
}

void Game::update()
{
    // LOGIKA MENU
    if (gameState == 0) {
        if (isClicked(400, 200, 200, 60) || slGetKey(SL_KEY_ENTER)) 
            gameState = 1;
        return;
    }

    // LOGIKA GAME OVER
    if (gameState == 2) {
        if (isClicked(300, 150, 180, 50) || slGetKey(' ')) { // Spasi untuk restart 
            difficultyMultiplier = 1.0f;
            score = 0;
            lives = 5;
            dragon.reset();
            spawnWave();
            gameState = 1;
        }
        if (isClicked(500, 150, 180, 50)) {
            gameState = 0; // Kembali ke Menu Utama
            score = 0;
            lives = 5;
            dragon.reset();
            spawnWave();
        }
        return;
    }

    if (slGetKey(SL_KEY_ESCAPE)) {
        isPaused = true;
    }

    // 2. Jika sedang Pause, cek tombol kontrol khusus
    if (isPaused) {
        if (slGetKey(SL_KEY_ENTER)) {
            isPaused = false; // Lanjut game
        }
        if (slGetKey(' ')) {
            // Minimize program (khusus Windows)
            HWND hwnd = GetActiveWindow();
            ShowWindow(hwnd, SW_MINIMIZE);
        }
        if (slGetKey('Q')) {
            exit(0); // Menutup program
        }

        return; // Berhenti di sini agar pergerakan di bawah tidak jalan
    }

    // LOGIKA SAAT BERMAIN (Hanya jalan jika gameState == 1)
    float dt = (float)slGetDeltaTime();

    difficultyMultiplier += difficultyRate * (float)dt;

    // Update Background & Objek
    bgX1 -= bgSpeed * dt;
    bgX2 -= bgSpeed * dt;
    if (bgX1 <= -400) bgX1 = bgX2 + 800;
    if (bgX2 <= -400) bgX2 = bgX1 + 800;

    bool hitThisFrame = false;

    dragon.update(dt);
    for (auto& a : asteroids) a.update(dt, difficultyMultiplier);

    // Cek Tabrakan
    for (auto& a : asteroids) {
        if (a.checkCollision(dragon.getX(), dragon.getY())) {
            hitThisFrame = true;
            a.reset(); // Pindahkan asteroid yang menabrak agar tidak kena berkali-kali
            break;
        }
    }

    if (hitThisFrame) {
        slSoundPlay(hitSfx);
        lives--;
        if (lives <= 0) {
            // SIMPAN HIGH SCORE SEBELUM PINDAH STATE
            if (score > highScore) highScore = score;
            gameState = 2;
        }
    }

    static float scoreTimer = 0;
    scoreTimer += slGetDeltaTime();
    if (scoreTimer >= 2.0f) {
        score += 10; // Tambah 10 poin setiap detik
        scoreTimer = 0;
    }

}

void Game::drawScore(float x, float y, int value) {
    // Ubah angka menjadi string, misal 125 jadi "125"
    string scoreStr = to_string(value);

    float spacing = 25.0f; // Jarak antar angka (sesuaikan dengan lebar asetmu)
    float currentX = x;

    for (char const& c : scoreStr) {
        // Ambil index angka (konversi char ke int)
        int digit = c - '0';

        // Gambar angka tersebut
        // Ukuran 20x30 adalah contoh, sesuaikan dengan keinginanmu
        slSprite(numTex[digit], currentX, y, 20, 30);

        // Geser posisi X untuk angka berikutnya agar berderet ke kanan
        currentX += spacing;
    }
}

void Game::render()
{
    // Pastikan warna putih terang dan tidak transparan sebelum menggambar apapun
    slSetForeColor(1, 1, 1, 1);

    if (gameState == 0) // MODE MENU UTAMA
    {
        // Gambar background naga kamu (Menu)
        slSprite(menuBgTex, 400, 300, 800, 600);
        slSprite(startBtnTex, 400, 200, 200, 60);

    }
    else if (gameState == 1) // MODE GAMEPLAY
    {
        // Gambar background nebula (Running)
        slSprite(bgTex, bgX1, 300, 800, 600);
        slSprite(bgTex, bgX2, 300, 800, 600);

        // Gambar Naga dan Asteroid
        dragon.render();
        for (auto& a : asteroids) a.render();

        // Tampilkan teks "SCORE:" di (40, 550)
        slSprite(scoreLabelTex, 40, 550, 80, 30);
        // Tampilkan angkanya di sebelah kanan teks (geser X ke 100)
        drawScore(100, 550, score);
        for (int i = 0; i < lives; i++) {
            slSprite(heartTex, 600 + (i * 35), 560, 30, 30);
        }
    }
    else if (gameState == 2) // MODE GAME OVER
    {
        // Gambar background kalah (Game Over)
        slSprite(gameOverBgTex, 400, 300, 800, 600);
        // Gambar tombol Retry (kiri) dan Exit (kanan)
        slSprite(retryBtnTex, 300, 150, 180, 50);
        slSprite(exitBtnTex, 500, 150, 180, 50);
        // Tampilkan Score saat ini di bawahnya
        slSprite(scoreLabelTex, 350, 240, 80, 30);
        drawScore(410, 240, score);
        // Tampilkan High Score di atas Score saat ini
        slSprite(highLabelTex, 350, 280, 120, 35);
        drawScore(430, 280, highScore);
    }

    if (isPaused) {
        slSprite(pauseInfoTex, 400, 300, 500, 300); // tampilkan info
    }
}

