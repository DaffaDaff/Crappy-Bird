/*
Crappy Bird
by Sulthan Daffa Arif Mahmudi

https://github.com/DaffaDaff
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define canvasWidth 100
#define canvasHeight 25

#define gravity 1 //points per tick

#define obstacleDelay 25 //ticks

void playerSetup();
void playerUpdate();
void updatePlayerPosition();
void clearPlayer();
void playerDeath();
void playerMovement();

void obstacleUpdate();
void updateObstaclePosition();
void setObstacle();
void drawObstacles();

void menu();
void game();
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
    menu();

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
int jumpAcceleration; // points per tick
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

int obstacleCount;

int obstacleTimer;

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

void menu()
{
    clearCanvas();
    draw();
    
    printf("\nCRAPPY BIRD\n");
    printf("          \n");
    printf("\nScore: %d\n\n", score);
    printf("Press Space To Jump \n");
    printf("Press X To Exit\n");
    while(1)
    {
        if(kbhit())
        {
            switch(getch())
            {
                case ' ': 
                    game();
                    break;
                case 'x': 
                    exit(0);
                    break;
            }
        }
    }
}

void game()
{
    start();
    
    while(1)
    {
        update();
        
        if(isExit) break;
    }
}

void start()
{
    isGameover = 0;

    jumpAcceleration = 0;

    obstacleCount = 0;
    obstacleTimer = 0;

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

    printf("\nCRAPPY BIRD\n");
    printf("          \n");
    printf("\nScore: %d\n\n", score);
    printf("Press Space To Jump \n");
    printf("Press X To Exit\n");
    //printf("\nTick : %d\n", ticksCount);
    ticksCount++;
}

void GameOver()
{
    printf("\nCRAPPY BIRD\n");
    printf("GAMEOVER!!\n");
    printf("\nScore: %d\n\n", score);
    printf("Press Space To Start\n");
    printf("Press X To Exit\n");
    while(1)
    {
        if(kbhit())
        {
            switch(getch())
            {
                case ' ': 
                    game();
                    break;
                case 'x': 
                    exit(0);
                    break;
            }
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
    printf("\033[H");
    
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
            case 'x': 
                exit(0);
                break;
        }
    }
}

int rnd(int min, int max)
{
    return ( rand() % (max - min + 1) ) + min;
}

void Exit()
{
    isExit = 1;
}

// Sulthan Daffa Arif Mahmudi - 2021
