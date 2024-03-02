#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//-lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32

using namespace std;


class Player{
    public:
        float x;
        float y;

        Player(float _x, float _y){
            x = _x;
            y = _y;
        }
};

GLFWwindow * initWindow(){
    if (!glfwInit()) {
        cout << "GLFW Initialise ERROR\n";
        return NULL;
    }

    // Create a windowed mode window and its OpenGL context
    //use glfwGetPrimaryMonitor() after title (first NULL (4)) to make window full screened
    GLFWwindow* window = glfwCreateWindow(800, 600, "i Win", NULL, NULL);
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



void renderScreen(GLfloat aspectRatio, int ** world, GLfloat textures[6][4], float player_x, float player_y){
    //add checks to avoid rending if out of scope of array

    int offset_y = 10;
    int offset_x = (int) ((2.0f / (0.1f * aspectRatio)) / -2.0f);

    for(GLfloat y = 1.0f; y > -1.0f; y -= 0.1f){
        for(GLfloat x = -1.0f; x < 1.0f; x += (0.1 * aspectRatio)){
            //textures based on blocktype

            //check if not air
            if(world[(int) player_y + offset_y][(int) player_x + offset_x] != 0){
                int block_y = (int) player_y + offset_y;
                int block_x = (int) player_x + offset_x;

                drawRectangle(x, y, x + (0.1 * aspectRatio), y - 0.1, textures[world[block_y][block_x]][0], textures[world[block_y][block_x]][1], textures[world[block_y][block_x]][2]);
            }
            offset_x++;
        }
        offset_y--;
        offset_x = (int) ((2.0f / (0.1f * aspectRatio)) / -2.0f);
    }
   
    return;
}



int main() {

    int height = 128;
    int length = 512;

    // world 
    int ** world = new int*[height];
    for(int i = 0; i < height; i++){
        world[i] = new int[length];
    }
    
    //0 - air
    //1 - stone
    //2 - dirt
    //3 - grass
    //4 - logs
    //5 - leaves

    GLfloat textures[6][4] = {{0.0f, 0.0f, 0.0f, 0.0f}, {139.0f/256.0f, 136.0f/256.0f, 140.0f/256.0f, 0.0f}, {173.0f/256.0f, 139.0f/256.0f, 90.0f/256.0f, 0.0f}, {99.0f/256.0f, 153.0f/256.0f, 58.0f/256.0f, 0.0f}, {97.0f/256.0f, 70.0f/256.0f, 32.0f/256.0f, 0.0f}, {56.0f/256.0f, 207.0f/256.0f, 91.0f/256.0f, 0.0f}};
    
    //terrain generation
    //insert stone
    for(int i = 0; i < 61; i++){
        for(int j = 0; j < length; j++){
            world[i][j] = 1;
        }
    }
    //insert dirt
    for(int i = 61; i < 64; i++){
        for(int j = 0; j < length; j++){
            world[i][j] = 2;
        }
    }
    //insert grass
    for(int j = 0; j < length; j++){
        world[64][j] = 3;
    }


    //greate player
    Player player = Player(256, 66);

    



    GLFWwindow * window = initWindow();

    // Set the clear color to light blue
    glClearColor(0.49f, 0.71f, 0.99f, 1.0f);


    while (!glfwWindowShouldClose(window)) {
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        //multiply width values by aspect Ratio to get a square
        GLfloat aspectRatio = (float)windowHeight / (float)windowWidth;


        // Input handling (if needed)


        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //draw squares
        renderScreen(aspectRatio, world, textures, player.x, player.y);


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        
    }

    // Terminate GLFW before exiting the program
    glfwTerminate();
    return 0;
}
