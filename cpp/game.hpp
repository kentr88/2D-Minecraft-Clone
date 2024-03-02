//implement objects
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <chrono>
#include <ctime>  
#include <cmath>

#include "player.hpp"
#include "block.hpp"


using namespace std;

#pragma once

class Game {
private:
    GLFWwindow * window;
    int screen_height;
    int screen_width;
    int world_height;
    int world_length;
    int ** world;

    //create player
    Player player = Player(250, 70, 256, 70, 0.25, 0.5);

    //fps limiting
    const double fpsLimit = 1.0 / 60.0;
    double lastFrameTime = 0;

    //0 - air
    //1 - stone
    //2 - dirt
    //3 - grass
    //4 - logs
    //5 - leaves 7. 94, 56
    //6 - water

    GLfloat textures[7][4] = {{0.0f, 0.0f, 0.0f, 0.0f}, {139.0f/256.0f, 136.0f/256.0f, 140.0f/256.0f, 0.0f}, {173.0f/256.0f, 139.0f/256.0f, 90.0f/256.0f, 0.0f}, {99.0f/256.0f, 153.0f/256.0f, 58.0f/256.0f, 0.0f}, {97.0f/256.0f, 70.0f/256.0f, 32.0f/256.0f, 0.0f}, {7.0f/256.0f, 94.0f/256.0f, 56.0f/256.0f, 0.0f}, {28.0f/256.0f, 67.0f/256.0f, 249.0f/256.0f, 0.0f}};
    
    GLFWwindow* initWindow(const char * name, const char * icon_dir);

    //coloured square
    void drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat red, GLfloat green, GLfloat blue);
    void drawRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat red, GLfloat green, GLfloat blue);

    void generateTerrain();


    //function to move the center point for the blocks being rendered based on player position
    void checkXYRender(GLfloat aspectRatio, int world_height, int world_length);
    
    //render all the blocks based on render center point
    void renderScreen(GLfloat aspectRatio, int ** world, GLfloat textures[7][4], int world_height, int world_width);
    

    //draw player based on x,y player coords
    void renderPlayer(GLfloat aspectRatio, float player_x, float player_y);
    

    //find current block of cursor
    void cursorBlock(double xpos, double ypos, GLint windowWidth, GLint windowHeight, GLfloat aspectRatio, int * x, int * y);

    bool collisions(int direction, float distance);
    
    void processGravity();

    void processJump();


public:
    void process_key();
    void process_mouse();
    Game(int _width, int _height, const char * name, const char * icon_directory, int _world_height, int _world_length);
    void run();
    ~Game();
};