#include "game9.h"

//-lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32




int main(void){
    const char * win_name = "MC";
    const char * win_dir = "C:/Users/kentr/Documents/coding/projects/vs-c/2dblock/2d_icon.png";
    Game game(800, 600, win_name, win_dir, 128, 512);
    game.run();


    return 0;
}