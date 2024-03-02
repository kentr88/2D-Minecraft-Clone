#include "game.hpp"
#include <filesystem>

//-lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32


int main(void){
    const char * win_name = "2D Minecraft Clone";
    std::filesystem::path currentDir = std::filesystem::current_path();
    std::string currentDirStr = currentDir.string().c_str();
    std::string concatenatedDir = std::string(currentDirStr) + "/2d_icon.png";
    const char* win_dir = concatenatedDir.c_str();

    Game game(800, 600, win_name, win_dir, 128, 512);
    game.run();


    return 0;
}