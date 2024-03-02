#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

//-lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32

int main() {
    if (!glfwInit()) {
        // Handle initialization failure
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "My Win", NULL, NULL);
    if (!window) {
        glfwTerminate();
        // Handle window creation failure
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        // Handle GLEW initialization failure
        return -1;
    }

    // Set up viewport
    glViewport(0, 0, 800, 600);

    while (!glfwWindowShouldClose(window)) {
        // Input handling (if needed)

        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the colored triangle
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex3f(-0.5f, -0.5f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex3f(0.5f, -0.5f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex3f(0.0f, 0.5f, 0.0f);
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW before exiting the program
    glfwTerminate();
    return 0;
}
