#ifndef GAME_H
#define GAME_H

//implement objects
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <chrono>
#include <ctime>  
#include <cmath>


using namespace std;


class Player{
    public:
        float x, y;
        float render_x, render_y;
        float walk_speed, run_speed;
        bool running = false;
        bool falling = false;
        int jumping = 0;    //nothing, 1 rising, 2 falling (gravity)
        int jumping_frames = 0;
        bool holdLeft, holdRight = false;
        int selectedX, selectedY = 0;

        Player(float _x, float _y, float _render_x, float _render_y, float _walk_speed, float _run_speed){
            x = _x;
            y = _y;
            render_x = _render_x;
            render_y = _render_y;
            walk_speed = _walk_speed;
            run_speed = _run_speed;
        }

        void move_x(float a){
            if(a > 0){
                if((x + a) < 512) x += a;
            } else {
                if((x + a) >= 0) x += a;
            }
            return;
        }

        void move_y(float a){
            if(a > 0){
                if((y + a) < 128) y += a;
            } else {
                if((y + a) >= 0) y += a;
            }
            return;
        }
        
};

class Block{
    public:
        bool liquid;
        float breakTime;

        Block(bool _liquid, float _breakTime){
            liquid = _liquid;
            breakTime = _breakTime;
        }
};


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

  

    Game(int _width, int _height, const char * name, const char * icon_directory, int _world_height, int _world_length){
        world_height = _world_height;
        world_length = _world_length;
        screen_height = _height;
        screen_width = _width;

        // create world data arrays
        world = new int*[world_height];
        for(int i = 0; i < world_height; i++) world[i] = new int[world_length];

        //generate terrain
        generateTerrain();

        window = initWindow(name, icon_directory);
       
        
        // Set the clear color to light blue
        glClearColor(0.49f, 0.71f, 0.99f, 1.0f);

        return;
        
    }


    void run(){
        //time
        //for fps calc
        auto time = chrono::system_clock::now();

        while (!glfwWindowShouldClose(window)) {
            double now = glfwGetTime();

            // This if-statement only executes once every 60th of a second
            if ((now - lastFrameTime) >= fpsLimit){
                // Scale to window size
                GLint windowWidth, windowHeight;
                glfwGetWindowSize(window, &windowWidth, &windowHeight);
                glViewport(0, 0, windowWidth, windowHeight);

                //multiply width values by aspect Ratio to get a square
                GLfloat aspectRatio = (float)windowHeight / (float)windowWidth;

                //gravity before other movements
                processGravity();

                // Poll for and process events
                glfwPollEvents();

                // Check the state of a specific key (e.g., GLFW_KEY_A)
                process_key();                


                //handle jumping
                processJump();

                //cursor handling
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                cursorBlock(xpos, ypos, windowWidth, windowHeight, aspectRatio, &player.selectedX, &player.selectedY);

                process_mouse();

                // Render here
                glClear(GL_COLOR_BUFFER_BIT);

                //see if coordinates of render needs to be change
                checkXYRender(aspectRatio, world_height, world_length);

                //draw squares on screen
                renderScreen(aspectRatio, world, textures, world_height, world_length);
                renderPlayer(aspectRatio, player.x, player.y);


                // Swap front and back buffers
                glfwSwapBuffers(window);



                //calculate actual fps
                //https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
                auto current_time = chrono::system_clock::now();
                chrono::duration<double> elapsed_seconds = current_time - time;
                //cout << 1 / elapsed_seconds.count() << " fps " << "aspect ratio: " << aspectRatio << "\r";
                time = current_time;


                // only set lastFrameTime when you actually draw something
                lastFrameTime = now;
            } 

        }

        // Terminate GLFW before exiting the program
        glfwTerminate();

    }


    ~Game(){
        //delete allocated memory
        for(int i = 0; i < world_height; i++) delete[] world[i];
        delete[] world;
    }


    

};

#endif
