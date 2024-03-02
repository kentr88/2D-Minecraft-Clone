#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

//-lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32

using namespace std;

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



void renderScreen(GLfloat aspectRatio){
    //go from top right to bottom left
    for(float yBlock = 1.0f; yBlock > -1.0f; yBlock -= 0.1f){
        for(float xBlock = -1.0f; xBlock < 1.0f; xBlock += 0.1f){
            drawRectangle((xBlock), yBlock, ((xBlock + 0.1f)), (yBlock - 0.1f), 0.0f, abs(yBlock), abs(xBlock));
        }
    }
    return;
}



int main() {

    GLFWwindow * window = initWindow();

    // Set the clear color to light blue (R=0.7, G=0.7, B=1.0)
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
        renderScreen(aspectRatio);


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        
    }

    // Terminate GLFW before exiting the program
    glfwTerminate();
    return 0;
}
