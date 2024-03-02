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


int main() {

    GLFWwindow * window = initWindow();
    


    while (!glfwWindowShouldClose(window)) {
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);


        // Input handling (if needed)

        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //draw triangle
        drawTriangle(-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f);
        drawTriangle(-0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f);


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        
    }

    // Terminate GLFW before exiting the program
    glfwTerminate();
    return 0;
}
