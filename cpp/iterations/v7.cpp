#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <chrono>
#include <ctime>  
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_PERLIN_IMPLEMENTATION
#include <stb/stb_perlin.h>

//-lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32

using namespace std;


class Player{
    public:
        float x, y;
        float render_x, render_y;
        float walk_speed, run_speed;
        bool running = false;

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



//greate player
Player player = Player(256, 66, 256, 70, 0.25, 0.5);



GLFWwindow * initWindow(){
    if (!glfwInit()) {
        cout << "GLFW Initialise ERROR\n";
        return NULL;
    }

    // Create a windowed mode window and its OpenGL context
    //use glfwGetPrimaryMonitor() after title (first NULL (4)) to make window full screened
    GLFWwindow* window = glfwCreateWindow(800, 600, "Terreria", NULL, NULL);
    if (!window) {
        glfwTerminate();
        // Handle window creation failure
        return NULL;
    }

    // Maximize the window
    glfwMaximizeWindow(window);


    //glew init stuff
    glfwMakeContextCurrent(window);

    std::filesystem::path currentDir = std::filesystem::current_path();
    std::string currentDirStr = currentDir.string().c_str();
    std::string concatenatedDir = std::string(currentDirStr) + "/2d_icon.png";
    const char* win_dir = concatenatedDir.c_str();


    //load image file
    int width, height;
    int channels;
    unsigned char* pixels = stbi_load(win_dir, &width, &height, &channels, 4);

    //window icon
    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(window, 1, images);
    

    if (glewInit() != GLEW_OK) {
        // Handle GLEW initialization failure
        return NULL;
    }

    //chat gpt stuff
    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));


    //return init window to function
    return window;
    
}


//triangle drawing functions
//white triangle
void drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3){
    // Draw the colored triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f); // white
    glVertex3f(x1, y1, 0.0f);
    glVertex3f(x2, y2, 0.0f);
    glVertex3f(x3, y3, 0.0f);
    glEnd();

    return;
}

//coloured triangle
void drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat red, GLfloat green, GLfloat blue){
    // Draw the colored triangle
    glBegin(GL_TRIANGLES);
    glColor3f(red, green, blue); // colour
    glVertex3f(x1, y1, 0.0f);   //vertex 1
    glVertex3f(x2, y2, 0.0f);   //vertex 2
    glVertex3f(x3, y3, 0.0f);   //vertex 3
    glEnd();

    return;
}

//multicoloured triangle
void vertexColourDrawTriangle(GLfloat vertex[6], GLfloat colours[9]){
    glBegin(GL_TRIANGLES);
    glColor3f(colours[1], colours[2], colours[3]);
    glVertex3f(vertex[1], vertex[2], 0.0f);

    glColor3f(colours[4], colours[5], colours[6]);
    glVertex3f(vertex[3], vertex[4], 0.0f);

    glColor3f(colours[7], colours[8], colours[9]);
    glVertex3f(vertex[5], vertex[6], 0.0f);
    glEnd();

    return;

}

//square
void drawRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat red, GLfloat green, GLfloat blue){
    drawTriangle(x1, y1, x1, y2, x2, y1, red, green, blue);
    drawTriangle(x2, y2, x1, y2, x2, y1, red, green, blue);
}


//initial terrain generation
void generateTerrain(int ** world, GLfloat textures[7][4], int world_height, int world_length){
    //terrain generation
    //go through x axis
    float perlin_noise_counter = 0.1;
    int lastTree = 0;
    for(int x_axis = 0; x_axis < world_length; x_axis++){
        int y_axis = 0;
        //generate using perlin noise
        int y_offset = (int) (stb_perlin_noise3_seed(perlin_noise_counter, 1.0f, 1.0f, 0, 0, 0, 3.0f) * 20);
        perlin_noise_counter += 0.05;

        //stone
        for(int y_current = 0; y_current < 61 + y_offset; y_current++){
            world[y_axis][x_axis] = 1;
            y_axis++;
        }

        //dirt
        for(int y_current = 0; y_current < 2; y_current++){
            world[y_axis][x_axis] = 2;
            y_axis++;
        }

        //water or grass
        if(y_axis < 62){
            //fill with water
            for(int y_current = y_axis; y_current < 64; y_current++){
                world[y_axis][x_axis] = 6;
                y_axis++;
            }

        } else {
            //extra dirt and grass
            for(int y_current = 0; y_current < 1; y_current++){
                world[y_axis][x_axis] = 2;
                y_axis++;
            }

            //grass
            for(int y_current = 0; y_current < 1; y_current++){
                world[y_axis][x_axis] = 3;
                y_axis++;
            }
        }


        //add trees randomly
        //1 in 14 chance
        int a = rand() % 14;

        
        if(a == 0){
            //check not to close to world borders
            //make sure at least 8 blocks between trees
            if(x_axis >= 4 && x_axis < (world_length - 5) && (x_axis - lastTree) > 8){
                //check no water below, only grass
                if(world[y_axis - 1][x_axis] == 3){
                    lastTree = x_axis;
                    //spawn tree
                    //trunk
                    world[y_axis][x_axis] = 4;
                    world[y_axis + 1][x_axis] = 4;
                    

                    //leaves
                    world[y_axis + 2][x_axis] = 5;
                    world[y_axis + 3][x_axis] = 5;
                    world[y_axis + 4][x_axis] = 5;

                    world[y_axis + 2][x_axis + 1] = 5;
                    world[y_axis + 2][x_axis - 1] = 5;
                    
                    world[y_axis + 3][x_axis + 1] = 5;
                    world[y_axis + 3][x_axis - 1] = 5;

                }
            }
            
        }

    }
    return;
}

//function to move the center point for the blocks being rendered based on player position
void checkXYRender(GLfloat aspectRatio, int world_height, int world_length){
    int blocks_to_center_x = (int) (1.0f / (0.1 * aspectRatio)) + 1;

    //check x right and within bounds
    if(player.x - player.render_x > blocks_to_center_x - 4 && player.render_x < world_length - 1) {
        if(player.running){
            player.render_x += player.run_speed;
        } else {
            player.render_x += player.walk_speed;
        }
    } 
    //check x left
    if(player.render_x - player.x > blocks_to_center_x - 4 && player.render_x > 1) {
        if(player.running){
            player.render_x -= player.run_speed;
        } else {
            player.render_x -= player.walk_speed;
        }
    } 

    //check y up
    if(player.y - player.render_y > 6 && player.render_y < world_height - 1) player.render_y += player.walk_speed;

    //check y down
    if(player.render_y - player.y > 6 && player.render_y > 1) player.render_y -= player.walk_speed;

    return;
}

//render all the blocks based on render center point
void renderScreen(GLfloat aspectRatio, int ** world, GLfloat textures[7][4], int world_height, int world_width){

    int player_x = (int) player.render_x;
    int player_y = (int) player.render_y;
    float round_x_offset = (player.render_x - (float) player_x) * (0.1f * aspectRatio);
    float round_y_offset = (player.render_y - (float) player_y) * 0.1f;

    
    int offset_y = 10;
    int offset_x = (int) ((2.0f / (0.1f * aspectRatio)) / -2.0f);

    for(GLfloat y = 1.1f - round_y_offset; y > -1.0f; y -= 0.1f){
        for(GLfloat x = -1.0f - round_x_offset; x < 1.0f; x += (0.1 * aspectRatio)){
            //textures based on blocktype

            //check block is within world borders
            if((int)(player_x + offset_x) > 0 && (int)(player_x + offset_x) < world_width && (int)(player_y + offset_y) >= 0 && (int)(player_y + offset_y) < world_height){
                //check if not air
                if(world[(int) player_y + offset_y][(int) player_x + offset_x] != 0){
                    int block_y = (int) player_y + offset_y;
                    int block_x = (int) player_x + offset_x;

                    drawRectangle(x, y, x + (0.1 * aspectRatio), y - 0.1, textures[world[block_y][block_x]][0], textures[world[block_y][block_x]][1], textures[world[block_y][block_x]][2]);
                }
            }
            offset_x++;
        }
        offset_y--;
        offset_x = (int) ((2.0f / (0.1f * aspectRatio)) / -2.0f);
    }
   
    return;
}

//draw player based on x,y player coords
void renderPlayer(GLfloat aspectRatio, float player_x, float player_y){
    int multiplyer = (int) (1.0f / (0.1 * aspectRatio)) + 1;
    GLfloat x = -1.0f + (0.1 * aspectRatio) * (multiplyer + (player.x - player.render_x));
    GLfloat y = (player.y - player.render_y) * 0.1;

    drawRectangle(x, y + 0.2f, x + (0.1f * aspectRatio), y, 238.0/256.0, 221.0/256.0, 181.0/256.0);
}



//input handling
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        player.move_x(player.walk_speed); 
        player.running = false;
    } 
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT){
        player.move_x(player.run_speed);
        player.running = true;
    } 

    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        player.move_x(-1.0 * player.walk_speed);
        player.running = false;
    } 
    if(key == GLFW_KEY_LEFT && action == GLFW_REPEAT){
        player.move_x(-1.0 * player.run_speed);
        player.running = true;
    } 

    if(key == GLFW_KEY_UP && action == GLFW_PRESS) player.move_y(player.walk_speed);
    if(key == GLFW_KEY_UP && action == GLFW_REPEAT) player.move_y(player.run_speed);

    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) player.move_y(-1.0 * player.walk_speed);
    if(key == GLFW_KEY_DOWN && action == GLFW_REPEAT) player.move_y(-1.0 * player.run_speed);

}




int main() {

    int height = 128;
    int length = 512;

    // create world data arrays
    int ** world = new int*[height];
    for(int i = 0; i < height; i++) world[i] = new int[length];
    
    //0 - air
    //1 - stone
    //2 - dirt
    //3 - grass
    //4 - logs
    //5 - leaves 7. 94, 56
    //6 - water

    GLfloat textures[7][4] = {{0.0f, 0.0f, 0.0f, 0.0f}, {139.0f/256.0f, 136.0f/256.0f, 140.0f/256.0f, 0.0f}, {173.0f/256.0f, 139.0f/256.0f, 90.0f/256.0f, 0.0f}, {99.0f/256.0f, 153.0f/256.0f, 58.0f/256.0f, 0.0f}, {97.0f/256.0f, 70.0f/256.0f, 32.0f/256.0f, 0.0f}, {7.0f/256.0f, 94.0f/256.0f, 56.0f/256.0f, 0.0f}, {28.0f/256.0f, 67.0f/256.0f, 249.0f/256.0f, 0.0f}};
    
    // fill terrain array
    generateTerrain(world, textures, height, length);
    


    GLFWwindow * window = initWindow();

    // Set the clear color to light blue
    glClearColor(0.49f, 0.71f, 0.99f, 1.0f);

    //for fps calc
    auto time = chrono::system_clock::now();


    //fps limiting
    const double fpsLimit = 1.0 / 60.0;
    double lastFrameTime = 0;   // number of seconds since the last frame

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


            // Input handling 
            glfwSetKeyCallback(window, key_callback);


            // Render here
            glClear(GL_COLOR_BUFFER_BIT);

            //see if coordinates of render needs to be change
            checkXYRender(aspectRatio, height, length);

            //draw squares
            renderScreen(aspectRatio, world, textures, height, length);

            renderPlayer(aspectRatio, player.x, player.y);

            //cout << player.x << " " << player.y << endl;


            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();


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


    //delete allocated memory
    for(int i = 0; i < height; i++) delete[] world[i];
    delete[] world;

    return 0;
}
