#include <sl.h>
#include "game.h"

int main()
{
    // Membuka window dengan resolusi 800x600
    slWindow(800, 600, "Dragon Asteroid Run", 1);

    Game game;
    game.load();

    // Game loop utama
    while (!slShouldClose())
    {
        
        if (slGetKey('Q'))
            break;

        game.update();
        game.render();
        slRender();
    }

    slClose();
    return 0;
}
