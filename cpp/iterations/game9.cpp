#include "game9.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif


#ifndef STB_PERLIN_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION
#include <stb/stb_perlin.h>
#endif


GLFWwindow* Game::initWindow(const char * name, const char * icon_dir){
    if (!glfwInit()) {
        cout << "GLFW Initialise ERROR\n";
        return NULL;
    }

    // Create a windowed mode window and its OpenGL context
    //use glfwGetPrimaryMonitor() after title (first NULL (4)) to make window full screened
    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        // Handle window creation failure
        cout << "GLFW create window ERROR\n";
        return NULL;
    }

    // Maximize the window
    glfwMaximizeWindow(window);


    //glew init stuff
    glfwMakeContextCurrent(window);


    //load image file
    int img_width, img_height;
    int channels;
    unsigned char* pixels = stbi_load(icon_dir, &img_width, &img_height, &channels, 4);
    if (!pixels) {
        std::cout << "Failed to load icon: " << icon_dir << std::endl;
    } else {
        //window icon
        GLFWimage images[1];
        images[0].width = img_width;
        images[0].height = img_height;
        images[0].pixels = pixels;

        glfwSetWindowIcon(window, 1, images);
    }    
  

    if (glewInit() != GLEW_OK) {
        // Handle GLEW initialization failure
        cout << "Glew Init Fail\n";
        return NULL;
    }
   


    //chat gpt stuff
    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    return window;

}

void Game::drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat red, GLfloat green, GLfloat blue){
    // Draw the colored triangle
    glBegin(GL_TRIANGLES);
    glColor3f(red, green, blue); // colour
    glVertex3f(x1, y1, 0.0f);   //vertex 1
    glVertex3f(x2, y2, 0.0f);   //vertex 2
    glVertex3f(x3, y3, 0.0f);   //vertex 3
    glEnd();

    return;
}

void Game::drawRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat red, GLfloat green, GLfloat blue){
    drawTriangle(x1, y1, x1, y2, x2, y1, red, green, blue);
    drawTriangle(x2, y2, x1, y2, x2, y1, red, green, blue);
}

void Game::generateTerrain(){
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

void Game::checkXYRender(GLfloat aspectRatio, int world_height, int world_length){
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
    if(player.y - player.render_y > 6 && player.render_y < world_height - 1) {
        if(player.running){
            player.render_y += player.run_speed;
        } else {
            player.render_y += player.walk_speed;
        }
    }  

    //check y down
    if(player.render_y - player.y > 6 && player.render_y > 1) {
        if(player.running){
            player.render_y -= player.run_speed;
        } else {
            player.render_y -= player.walk_speed;
        }
    } 

    return;
}

void Game::renderScreen(GLfloat aspectRatio, int ** world, GLfloat textures[7][4], int world_height, int world_width){

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
                //block coordinates
                int block_y = (int) player_y + offset_y;
                int block_x = (int) player_x + offset_x;
                
                //check if block is selected by player cursor
                if(block_x == player.selectedX && block_y == player.selectedY){
                    drawRectangle(x, y, x + (0.1 * aspectRatio), y - 0.1, 1.0f, 1.0f, 1.0f);
                } else {
                    //check if not air
                    if(world[block_y][block_x] != 0){
                        drawRectangle(x, y, x + (0.1 * aspectRatio), y - 0.1, textures[world[block_y][block_x]][0], textures[world[block_y][block_x]][1], textures[world[block_y][block_x]][2]); 
                    }
                }
                
            }
            offset_x++;
        }
        offset_y--;
        offset_x = (int) ((2.0f / (0.1f * aspectRatio)) / -2.0f);
    }

    return;
}

void Game::renderPlayer(GLfloat aspectRatio, float player_x, float player_y){
    int multiplyer = (int) (1.0f / (0.1 * aspectRatio));
    GLfloat x = -1.0f + (0.1 * aspectRatio) * (multiplyer + (player.x - player.render_x));
    GLfloat y = (player.y - player.render_y) * 0.1;

    drawRectangle(x, y + 0.2f, x + (0.1f * aspectRatio), y, 238.0/256.0, 221.0/256.0, 181.0/256.0);
}

void Game::cursorBlock(double xpos, double ypos, GLint windowWidth, GLint windowHeight, GLfloat aspectRatio, int * x, int * y){
    //make sure cursor is within the window
    if(xpos < 0 || xpos > windowWidth || ypos < 0 || ypos > windowHeight){
        *x = -1;
        *y = -1;
        return;
    }

    // find y
    int y_block_size = (int) round(windowHeight / 20);
    int y_block_offset = (int) (ypos / y_block_size);
    *y = player.render_y + (9 - y_block_offset);

    //find x
    int x_block_size = (int) round(windowWidth / (20 / aspectRatio));
    int decimal_offset = 0;

    //need to concider if render is on a decimal
    if(player.render_x != (int) player.render_x){
        decimal_offset = (player.render_x - (int) player.render_x) * x_block_size;
    }
    
    //number of blocks from left screen
    int x_block_offset = (int) ((xpos + decimal_offset) / x_block_size);

    //number of blocks to middle of screen
    int x_mid = round((20 / aspectRatio) / 2); 

    *x = player.render_x + (x_block_offset - x_mid);
    
    return;

}

bool Game::collisions(int direction, float distance){
    int x, y;
    switch(direction){
        case 0:
            //right
            //final position - round up for checking
            x = ceil(player.x + distance);
            y = (int) player.y;
            break;
        case 1:
            //left
            //final position - round down
            x = floor(player.x - distance);
            y = (int) player.y;
            break;
        case 2:
            //up
            //final position - round up
            x = (int) player.x;
            y = ceil(player.y + distance);
            break;
        case 3:
            //down
            x = (int) player.x;
            y = floor(player.y - distance);
            break;
        default:
        break;
    }

    if(world[y][x] != 0 && world[y][x] != 6) return false;
    if(world[y + 1][x] != 0 && world[y + 1][x] !=  6) return false;
    return true;
}

void Game::processGravity(){
    //check if block directly below

    //make sure player isn't jumping
    

    if(player.jumping != 1 && world[(int) ceil(player.y - 1)][(int) floor(player.x)] == 0 && world[(int) ceil(player.y - 1)][(int) ceil(player.x)] == 0){
        //do gravity
        player.falling = true;  //for other later calculations
        player.move_y(-0.125);
    } else {
        player.falling = false;   

        //stop jumping
        if(player.jumping == 2){
            player.jumping = 0;
        }
    }
    
}

void Game::processJump(){
    if(player.jumping){
        //complete jump in 10 frames
        //will jump approx 1.25 blocks
        
        if(player.jumping_frames < 15){
            //check if can jump
            if(world[(int) floor(player.y + 2)][(int) floor(player.x)] == 0 && world[(int) floor(player.y + 2)][(int) ceil(player.x)] == 0){
                player.move_y(0.125);
            } else {
                player.jumping_frames = 15;
            }
        
        } else {
            //stop jumping
            player.jumping = 2;

            //check if holding keys
            if(player.holdLeft && collisions(0, player.walk_speed)){
                player.move_x(-1.0 * player.walk_speed);
                player.holdLeft = false;
            } 
            if(player.holdRight && collisions(1, player.walk_speed)){
                player.move_x(player.walk_speed);
                player.holdRight = false;
            }
            return;
        }

        
        player.jumping_frames++;
    }
}

void Game::process_key(){
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if(collisions(0, player.walk_speed)){
            player.move_x(player.walk_speed); 
            player.running = false;
        }
        if(player.jumping > 0) player.holdRight = true;
    } 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_REPEAT) {
        if(collisions(0, player.run_speed)){
            player.move_x(player.run_speed);
            player.running = true;
        } else {
            if(collisions(0, player.walk_speed)){
                player.move_x(player.walk_speed); 
                player.running = false;
            }
        }
        if(player.jumping > 0) player.holdRight = true;
    } 




    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if(collisions(1, player.walk_speed)){
            player.move_x(-1.0 * player.walk_speed);
            player.running = false;
        }
        if(player.jumping > 0) player.holdLeft = true;
    } 
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_REPEAT){
        if(collisions(1, player.run_speed)){
            player.move_x(-1.0 * player.run_speed);
            player.running = true;
        } else {
            if(collisions(1, player.walk_speed)){
                player.move_x(-1.0 * player.walk_speed);
                player.running = false;
            }
        }
        if(player.jumping > 0) player.holdLeft = true;
    } 



    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if(world[(int) player.y][(int) player.x] == 6){
            if(collisions(2, player.walk_speed)){
                player.move_y(player.walk_speed);
                player.running = false;
            }
        } else {
            //jump - if player isn't already jumping
            if(player.jumping == 0){
                player.jumping = true;
                player.jumping_frames = 0;
            }
        }
        
    } 
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_REPEAT) {
        //check if water - go up
        if(world[(int) player.y][(int) player.x] == 6){
            //go up 
            if(collisions(2, player.run_speed)){
                player.move_y(player.run_speed);
                player.running = true;
            } else {
                if(collisions(2, player.walk_speed)){
                    player.move_y(player.walk_speed);
                    player.running = false;
                }
            }
        } else {
            //jump - if player isn't already jumping
            if(!player.jumping){
                player.jumping = true;
                player.jumping_frames = 0;
            }
        }
        
    } 





    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if(collisions(3, player.walk_speed)){
            player.move_y(-1.0 * player.walk_speed);
            player.running = false;
        }
    } 
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_REPEAT) {
        if(collisions(3, player.walk_speed)){
            player.move_y(-1.0 * player.run_speed);
            player.running = true;
        } else {
            if(collisions(3, player.run_speed)){
                player.move_y(-1.0 * player.walk_speed);
                player.running = false;
            }
        }
    } 
}
    
void Game::process_mouse(){
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        //add block
        world[player.selectedY][player.selectedX] = 1;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        //delete block
        world[player.selectedY][player.selectedX] = 0;
    }
}





