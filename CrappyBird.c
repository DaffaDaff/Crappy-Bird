/*
Crappy Bird
by Sulthan Daffa Arif Mahmudi

https://github.com/DaffaDaff
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <time.h>

#define tickDelay 250 // milliseconds

#define canvasWidth 100
#define canvasHeight 25

#define gravity 1 //points per tick

#define obstacleDelay 25 //ticks

void playerSetup();
void playerUpdate();
void updatePlayerPosition();
void clearPlayer();
void playerDeath();

void obstacleUpdate();
void updateObstaclePosition();
void setObstacle();
void drawObstacles();

void playerMovement();
void start();
void update();
void GameOver();
void clearCanvas();
void drawStart();
void draw();
void Input();
void delay(int milli_seconds);
int rnd(int min, int max);
void Exit();

int isExit = 0;
int isGameover = 0;
int ticksCount = 0;
int score = 0;

char canvas[canvasHeight][canvasWidth];

int main()
{
    start();
    
    while(1)
    {
        update();
        
        if(isExit) break;
    }

    return 0;
}


#pragma region Objects

typedef struct Position{
    int x;
    int y;
} position;

typedef struct Object
{
    position position;

    int width;
    int height;
    char string[canvasHeight][canvasWidth];
} object;

object * insertObject(object objects[], object obj, int len, int index)
{

    for(int i = len - 1; i >= index; i--)
    {
        objects[i + 1] = objects[i];
    }
    objects[index] = obj;

    return objects;
}

object * removeObject(object objects[], int len, int index)
{
    for(int i = index; i < len; i++)
        objects[i] = objects[i + 1];

    return objects;
}

#pragma endregion Objects


#pragma region Player

object player;

int playerVelocity = gravity; // y velocity | points per tick
int jumpAcceleration = 0; // points per tick
int jumpPower = -3;

void playerSetup()
{
    player.position.x = 4;
    player.position.y = canvasHeight / 2 - 2;



    /*
       birb

       # # #    
     # # # O #
     # # # # # >
       # # # 
    */

    player.width = 6;
    player.height = 4;

    char sprite[4][6] =
    {
        {' ','#','#','#',' ',' '},
        {'#','#','#','O','#',' '},
        {'#','#','#','#','#','>'},
        {' ','#','#','#',' ',' '}
    };
    
    for(int i = 0; i < 4; i++)
        for (int j = 0; j < 6; j++)
            player.string[i][j] = sprite[i][j];
}

void playerUpdate()
{
    playerMovement();
    updatePlayerPosition();

    if(isGameover) playerDeath();
}

void updatePlayerPosition()
{
    for(int i = 0; i < player.height; i++)
        for (int j = 0; j < player.width; j++)
        {
            if(player.string[i][j] == ' ') continue;

            if(canvas[player.position.y + i][player.position.x + j] != ' ') isGameover = 1;
            canvas[player.position.y + i][player.position.x + j] = player.string[i][j];
        }
}

void clearPlayer()
{
    for(int i = 0; i < player.height; i++)
        for (int j = 0; j < player.width; j++)
        canvas[player.position.y + i][player.position.x + j] = ' ';
} 

void playerMovement()
{
    playerVelocity = gravity + jumpAcceleration;
    if(jumpAcceleration < 0) jumpAcceleration += gravity;

    player.position.y += playerVelocity;
    if(player.position.y < 0) player.position.y = 0;

    if(player.position.y >= canvasHeight) isGameover = 1;
}

void playerDeath()
{
    /*
     ded birb

       # # #    
     # # # X #
     # # # # # >
       # # # 
    */

    char sprite[4][6] =
    {
        {' ','#','#','#',' ',' '},
        {'#','#','#','X','#',' '},
        {'#','#','#','#','#','>'},
        {' ','#','#','#',' ',' '}
    };
    
    for(int i = 0; i < 4; i++)
        for (int j = 0; j < 6; j++)
            player.string[i][j] = sprite[i][j];
    
    updatePlayerPosition();
}

#pragma endregion Player


#pragma region Obstacles

object obstacles[canvasWidth / obstacleDelay + 1];

int obstacleCount = 0;

int obstacleTimer = 0;

void obstacleUpdate()
{
    obstacleTimer--;
    
    updateObstaclePosition();
    drawObstacles();
    
    if(obstacleTimer <= 0)
    {
        setObstacle();
        obstacleTimer = obstacleDelay;
    }
}

void updateObstaclePosition()
{
    for(int i = 0; i < obstacleCount; i++)
    {
        for(int j = 0; j < canvasHeight; j++)
        {
            int x_pos = obstacles[i].position.x;

            for(int k = -2; k <= 2; k++)
                if(x_pos + k >= 0 && x_pos + k < canvasWidth)
                    canvas[j][x_pos + k] = ' ';
        }

        obstacles[i].position.x--;
    }
}

void setObstacle()
{
    object obstacle;

    obstacle.position.x = canvasWidth;
    obstacle.position.y = rnd(10, canvasHeight - 10);

    insertObject(obstacles, obstacle, obstacleCount, obstacleCount);
    obstacleCount++;
}

void drawObstacles()
{
    for(int i = 0; i < obstacleCount; i++)
    {
        int x_pos = obstacles[i].position.x;
        int y_pos = obstacles[i].position.y;

        if(x_pos + 5 == 0)
        {
            removeObject(obstacles, obstacleCount, 0);
            obstacleCount--;
            continue;
        }

        if(x_pos + 5 == player.position.x) score++;

        for(int j = 0; j < canvasHeight; j++)
        {
            if(j > y_pos - 5 && j < y_pos + 5) continue;
            
            for(int k = -2; k <= 2; k++)
                if(x_pos + k >= 0 && x_pos + k < canvasWidth)
                    canvas[j][x_pos + k] = '*';
        }
        
    }
}

#pragma endregion Obstacles

void start()
{
    clearCanvas();
    draw();
    

    printf("\nCRAPPY BIRD\n");
    printf("Press Space To Start");
    while(1)
    {
        if(getch() == ' ') break; 
    }

    srand(time(0));
    clearCanvas();

    playerSetup();
}

void update()
{
    clearPlayer();

    obstacleUpdate();
    playerUpdate();

    draw();
    Input();

    if(isGameover) GameOver();

    printf("\nScore: %d", score);
    //printf("\nTick : %d\n", ticksCount);
    ticksCount++;
    delay(tickDelay);
}

void GameOver()
{
    printf("\n\n!!GAMEOVER!!\n\n");
    printf("Press Space To Exit");
    while(1)
    {
        if(getch() == ' ') 
        {
            Exit();
            break;
        } 
    }
}

void clearCanvas()
{
    for (int i = 0; i < canvasHeight; i++)
    {
        for (int j = 0; j < canvasWidth; j++)
        {
            canvas[i][j] = ' ';
        }
    }
}

void draw()
{
    system("cls");
    
    for(int i = -1; i <= canvasHeight; i++){
        for(int j = -1; j <= canvasWidth; j++){
            if(i == -1 || j == -1 || i == canvasHeight || j == canvasWidth) printf("# ");
            else printf("%c ", canvas[i][j]);
        }
        printf("\n");
    }
}

void Input()
{
    if(kbhit())
    {
        switch(getch())
        {
            case ' ': 
                jumpAcceleration = jumpPower;
                break;
        }
    }
}

int rnd(int min, int max)
{
    return ( rand() % (max - min + 1) ) + min;
}

void delay(int milli_seconds)
{
    clock_t start_time = clock();
  
    while (clock() < start_time + milli_seconds);
}

void Exit()
{
    isExit = 1;
}

// Sulthan Daffa Arif Mahmudi - 2021