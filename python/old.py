from tkinter import *
import random
from time import sleep, time
import math
import keyboard

from perlin_noise import PerlinNoise

BLOCK_SIZE = 80
WORLD_LENGTH = 1000
WORLD_HEIGHT = 256
SPEED = 0.25

class Block():
    def __init__(ob, name, col, solid):
        ob.name = name
        ob.col = col
        ob.solid = solid
        
    def draw(se, mx, my):
        se.x = mx
        se.y = my
        c.create_rectangle((se.x - (BLOCK_SIZE / 2)), (se.y - (BLOCK_SIZE / 2)), (se.x + (BLOCK_SIZE / 2)), (se.y + (BLOCK_SIZE / 2)), fill=se.col, outline=se.col)
        
#solid - 0 gas, 1 solid, 2 block like water, ladders, can go up or down
plank = Block("plank", '#B89B49', 1)
dirt = Block("dirt", '#E0A440', 1)
grass = Block("grass", '#70C72D', 1)
stone = Block("stone", '#868287', 1)
log = Block("log", '#855817', 1)
leaves = Block("leaves", '#068F43', 1)
water = Block("water", 'blue', 2)
air = Block("air", 'light blue', 0)

selector = Block('selector', 'white', 0)

blockArray = [plank, dirt, grass, stone, log, leaves, water]

class Player():
    speed = 1
    jumping = False
    startJump = 0
    jumpFrames = 0
    falling = False
    selected_x = 0
    selected_y = 0
    mouse_x = 0
    mouse_y = 0
    selectedBlock = 0
    pause_screen = False
    def __init__(ob, x, y):
        ob.x = x
        ob.y = y
        ob.block_x = x
        ob.block_y = y
    def draw(self, mx, my):
        self.mx = mx
        self.my = my
        c.create_rectangle((self.mx - BLOCK_SIZE / 2), (self.my + BLOCK_SIZE * 2), (self.mx + BLOCK_SIZE / 2), (self.my), fill='#F5DD86', outline='#F5DD86')
    


chunk = []
treeArray = []

def drawTree(x, y):
    if x > 1 and (x + 2) < WORLD_LENGTH:
        chunk[y][x] = log
        chunk[y + 1][x] = log
        chunk[y + 2][x] = leaves
        chunk[y + 3][x] = leaves
        chunk[y + 4][x] = leaves
        chunk[y + 2][x - 1] = leaves
        chunk[y + 2][x + 1] = leaves
        chunk[y + 3][x - 1] = leaves
        chunk[y + 3][x + 1] = leaves

for y in range(WORLD_HEIGHT):
    temp = []
    for x in range(WORLD_LENGTH):
        temp.append(0)
        
    chunk.append(temp)


noise = PerlinNoise(octaves=2, seed=1)
xpix = 40
pic = [noise([j/xpix, j/xpix]) for j in range(WORLD_LENGTH + 1)]



#find difference
dif = []
for x in range(1, len(pic)):
    dif.append(int((pic[x] - pic[x -1]) * 100))


for x in range(WORLD_LENGTH):
    y = 0
    for m in range(64 + (dif[x])):
        chunk[y][x] = stone
        y += 1
        
    for m in range(3):
        chunk[y][x] = dirt
        y += 1

    useGs = True
    wt = True
    while wt:
        if y < 64:
            useGs = False
            chunk[y][x] = water
            y += 1
        else:
            wt = False 

    if useGs:
        for m in range(1):
            chunk[y][x] = grass
            y += 1
            
            if random.randint(0, 5) == 1:
                if len(treeArray) > 0:
                    if treeArray[len(treeArray) - 1][0] < (x - 3):
                        treeArray.append((x, y))
                else:
                   treeArray.append((x, y)) 
    
        
    for m in range(y, WORLD_HEIGHT):
        chunk[y][x] = air    
        y += 1



for i in range(len(treeArray)):
    drawTree(treeArray[i][0], treeArray[i][1])
        

#tdinter stuff
window = Tk()
window.title("Terreria")
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()

#center of screen variables
center_x = int(screen_width / 2)
center_y = int(screen_height / 2)
quarter_x = int(center_x / 2)
quarter_y = int(center_y / 2)

window.configure(width=screen_width, height=screen_height, bg='white')

#define canvas
c = Canvas(window, width=screen_width, height=screen_height, bg='light blue')
c.pack()


#player - move player/center co-ordinates to middle of screen
midx = int(len(chunk[1]) / 4)
midy = 40
#calc first air block
for m in range(40, 100, 1):
    if chunk[m][midx].name == "air":
        midy = m
        break




player = Player(midx, midy)


def calcPlayerX(x):
    return center_x + ((x - player.block_x) * BLOCK_SIZE)
    

def calcPlayerY(y):
    return (center_y - BLOCK_SIZE) - ((y - player.block_y) * BLOCK_SIZE)

def moveLeft(spd, amt):
    if calcPlayerX(player.x) <= quarter_x:
        player.block_x -= amt
        player.x -= amt
    else:
        player.x -= spd

def moveRight(spd, amt):
    if calcPlayerX(player.x) >= (screen_width - quarter_x):
        player.block_x += amt
        player.x += amt
    else:
        player.x += spd

def moveUp(spd, amt):
    if calcPlayerY(player.y) <= quarter_y:
        player.block_y += amt
        player.y += amt
    else:
        player.y += spd

def moveDown(spd, amt):
    if calcPlayerY(player.y) >= (screen_height - center_y):
        player.block_y -= amt
        player.y -= amt
    else:
        player.y -= spd


def checkBelow(x, y):
    if x == math.floor(x):
        if y == math.floor(y):
            if chunk[math.floor(y) - 1][math.floor(x)].solid == 1:
                return False
            else:
                return True
        else:
            if chunk[math.floor(y)][math.floor(x)].solid == 1:
                return False
            else:
                return True

    else:
        if y == math.floor(y):
            if chunk[math.floor(y) - 1][math.floor(x)].solid == 1 and chunk[math.floor(y) - 1][math.floor(x) + 1].solid == 1:
                return False
            else:
                return True
        else:
            if chunk[math.floor(y)][math.floor(x)].solid == 1:
                return False
            else:
                return True


def checkLeft(x, y):
    dx = math.floor(x)
    dy = math.floor(y)

    if x == math.floor(x):
        if chunk[dy][dx - 1].solid == 1 or chunk[dy + 1][dx - 1].solid == 1:
            return False
        return True
    else:
        return True

def checkRight(x, y):
    dy = math.floor(y)
    dx = math.floor(x)
    if x == math.floor(x):
        #on single block
        if chunk[dy][dx + 1].solid == 1 or chunk[dy + 1][dx + 1].solid == 1:
            return False
        return True
    else:
        #between blocks, check not inside block
        if chunk[dy][dx].solid == 1 or chunk[dy + 1][dx].solid == 1:
            return False
        return True
            

def doGravity(x, y):
    #work out x to work out highest block

    if player.jumping:
        return False
    
    #check if x cord is on whole block
    if x == math.floor(x):
        #check if y cord is more than whole number
        if y == math.floor(y):
            if chunk[math.floor(y) - 1][math.floor(x)].solid == 0:
                return True
            else:
                return False
        else:
            if chunk[math.floor(y)][math.floor(x)].solid == 0:
                return True
            else:
                return False
            
        
    else:
        if y == math.floor(y):
            #will be across 2 - check which is higher
            if chunk[math.floor(y) - 1][math.floor(x)].solid == 0 and chunk[math.floor(y) - 1][math.floor(x) + 1].solid == 0:
                return True
            else:
                return False
        else:
            #will be across 2 - check which is higher
            if chunk[math.floor(y)][math.floor(x)].solid == 0 and chunk[math.floor(y)][math.floor(x) + 1].solid == 0:
                return True
            else:
                return False


def jump():
    #if jump is complete
    if (time() - player.startJump) >= 0.1:
        #fall back down before jump again
        player.jumping = False
        player.falling = True


    if keyboard.is_pressed('Right'):
        if checkRight(player.x, player.y):
            moveRight(SPEED, (math.floor(SPEED) + 1))

    if keyboard.is_pressed('Left'):
        if checkLeft(player.x, player.y):
            moveLeft(SPEED, (math.floor(SPEED) + 1))
        
    #if 0.1 seconds have passed since last jump movement
    if (time() - player.startJump) >= (0.01 * player.jumpFrames):
        if calcPlayerY(player.y) <= quarter_y:
            player.block_y += 1
            player.y += SPEED
        else:
            player.y += SPEED
            
        player.jumpFrames += 1
           
        

    


def move(event):
    if event.keysym == 'Up':
        if player.jumping == False and player.falling == False:
           player.jumping = True
           player.startJump = time()
           player.jumpFrames = 0
        
           
    if event.keysym == 'Down':
        #only if solid of block below is 2
        if checkBelow(player.x, player.y):
            moveDown(SPEED, (math.floor(SPEED) + 1))
        
            
    if event.keysym == 'Left':
        if checkLeft(player.x, player.y):
            moveLeft(SPEED, (math.floor(SPEED) + 1))
        

    if event.keysym == 'Right':
        if checkRight(player.x, player.y):
            moveRight(SPEED, (math.floor(SPEED) + 1))

    if event.keysym == 'Escape':
        if player.pause_screen:
            player.pause_screen = False
        else:
            player.pause_screen = True


c.bind_all('<Key>', move)



def readMouseMotion(e):
    player.mouse_x = e.x
    player.mouse_y = e.y



c.bind('<Motion>', readMouseMotion)


def drawSelector(x, y):
    #x, y are for mouse co-ordinates
    x_block = math.floor(((x - center_x) / BLOCK_SIZE) + player.block_x + 0.5)
    y_block = math.floor((((center_y - BLOCK_SIZE) - y) / BLOCK_SIZE) + player.block_y + 1.5)


    dif_x = abs(x_block - player.x)
    dif_y = abs(y_block - player.y)

    if dif_x < 5 and dif_y < 5:
        #x and y cord for mouse in canvas
        dx = center_x + ((x_block - player.block_x) * BLOCK_SIZE)
        dy = (center_y) - ((y_block - player.block_y) * BLOCK_SIZE)

        player.selected_x = x_block
        player.selected_y = y_block

        selector.draw(dx, dy)

    else:
        player.selected_x = -1
        player.selected_y = -1
    


def removeBlock(event):
    if player.selected_x != -1 and player.selected_y != -1:
        chunk[player.selected_y][player.selected_x] = air

def addBlock(event):
    if player.selected_x != -1 and player.selected_y != -1:
        chunk[player.selected_y][player.selected_x] = blockArray[player.selectedBlock]

def changeBlock(event):
    if event.delta > 0:
        select_block(1)
    else:
        select_block(-1)
        

def select_block(x):
        player.selectedBlock += x
        if player.selectedBlock >= len(blockArray):
            player.selectedBlock = 0
        if player.selectedBlock < 0:
            player.selectedBlock = len(blockArray) - 1



    

c.bind("<Button-1>", removeBlock)
c.bind("<Button-3>", addBlock)
c.bind("<MouseWheel>", changeBlock)
    




#function for drawing world
def render_blocks(x, y):
    #work out range of blocks that will be onscreen
    xamt = int(screen_width / BLOCK_SIZE)
    xdraw = ((int(xamt / 2))) + 1 #amount to draw from center
    yamt = int(screen_width / BLOCK_SIZE)
    ydraw = ((int(yamt / 2))) + 1 #amount to draw from center

    #y - iy represent index in chunk
    for iy in range((0 - ydraw), ydraw):
        #x ix represent index in chunk
        for ix in range((0 - xdraw), xdraw):
            if (iy + y) < len(chunk) and (iy + y) >= 0:
                if (ix + x) < len(chunk[int(iy + y)]) and (ix + x) >= 0:
                    xcord = center_x + (ix * BLOCK_SIZE)
                    ycord = center_y + (-iy * BLOCK_SIZE)
                    if chunk[int(iy + y)][int(ix + x)].name != "air":
                        chunk[int(iy + y)][int(ix + x)].draw(int(xcord), int(ycord))

def render_player(x, y):
    dx = center_x + ((x - player.block_x) * BLOCK_SIZE)
    dy = (center_y - BLOCK_SIZE) - ((y - player.block_y) * BLOCK_SIZE) - (BLOCK_SIZE * 0.5)
    player.draw(dx, dy)


def draw_pause_screen():
    c.create_text(center_x, (center_y - 200), text="Game Paused", fill="black", font=('Helvetica 30 bold'))
    resume = Button(window, text='Resume', width=15)
    resume.place(x=(center_x - 30), y=(center_y - 50))


    

c.create_text(30, 20, text='FPS: ', fill = 'white')
fps_text = c.create_text(30, 40, fill='white')
c.create_text(30, 50, text='Cord: ', fill = 'white')
cord_text = c.create_text(50, 50, fill='white')
c.create_text(90, 20, text='Block: ', fill = 'white')
block_text = c.create_text(130, 20, fill='white')
block_icon = c.create_rectangle(150, 10, 170, 30, fill=blockArray[player.selectedBlock].col, outline=blockArray[player.selectedBlock].col)

def write_fps(fps, cord):
    c.itemconfig(fps_text, text=str(fps))
    c.itemconfig(cord_text, text=str(cord))


fps_lastTime = 0
fps_lastReading = 0
fps_counter = 0




#main loop
while True:
    c.delete("all")

    if player.pause_screen:
        draw_pause_screen()
        sleep(0.1)

    else:
        #gravity / jumping
        if doGravity(player.x, player.y):
            player.falling = True
            moveDown(SPEED, 1)
        else:
            player.falling = False
        

        if player.jumping:
            jump()              
        
    
        render_blocks(player.block_x, player.block_y)
        render_player(player.x, player.y)
        drawSelector(player.mouse_x, player.mouse_y)

        c.create_text(30, 20, text='FPS: ', fill = 'white')
        fps_text = c.create_text(50, 20, fill='white')
        c.create_text(30, 50, text='Cord: ', fill = 'white')
        cord_text = c.create_text(80, 50, fill='white')
        c.create_text(90, 20, text='Block: ', fill = 'white')
        block_text = c.create_text(130, 20, fill='white')
        block_icon = c.create_rectangle(150, 10, 170, 30, fill=blockArray[player.selectedBlock].col, outline=blockArray[player.selectedBlock].col)
        c.itemconfig(block_text, text=str(blockArray[player.selectedBlock].name))



        sleep(0.01)
 

        if fps_counter >= 5:
            fps_lastReading = math.floor(fps_counter / (time() - fps_lastTime))
            write_fps(fps_lastReading, (player.x, player.y))
            fps_lastTime = time()
            fps_counter = 0
        else:
            write_fps(fps_lastReading, (player.x, player.y))
            fps_counter += 1

    
        
    
    window.update()

    
   













