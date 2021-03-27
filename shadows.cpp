#include "raylib.h"
#include <math.h>

const int screenWidth = 500;
const int screenHeight = 500;
bool moving = false;

float distance(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

class Light{
    public:
        float x, y, radius;
        Color color;
        bool selfMoving = false;
    
    void draw(){
        DrawCircle(x, y, radius, color);
    }
    
    void move(){
        // Mouse pressed
        if (!moving && IsMouseButtonPressed(0) && distance(GetMouseX(), GetMouseY(), x, y) <= 20){
            moving = true;
            selfMoving = true;
        }
        // Mouse released
        if (selfMoving && IsMouseButtonReleased(0)){
            selfMoving = false;
            moving = false;
        }
        // While pressed
        if (selfMoving){
            x = GetMouseX();
            y = GetMouseY();
        }
    }
};

class Blocker{
    public:
        float x, y, width, height;
        Color color;
        bool selfMoving = false;
        
    void draw(){
        DrawRectangle(x, y, width, height, color);
    }
    
    void move(){
        // Mouse pressed
        if (!moving && IsMouseButtonPressed(0) && distance(GetMouseX(), GetMouseY(), x + width/2, y + height/2) <= 40){
            moving = true;
            selfMoving = true;
        }
        // Mouse released
        if (selfMoving && IsMouseButtonReleased(0)){
            selfMoving = false;
            moving = false;
        }
        // While Moving
        if (selfMoving){
            x = GetMouseX() - width/2;
            y = GetMouseY() - height/2;
        }
    }
};

void drawPixels(float blockerX, float blockerY, float lightX, float lightY, float blockerWidth, float blockerHeight){
    Vector2 zero = Vector2{0, 0};
    Vector2 * zeroPtr = &zero;
    Vector2 pixelPos;
    Vector2 blockerPosTL = Vector2{blockerX, blockerY};
    Vector2 blockerPosBR = Vector2{blockerX + blockerWidth, blockerY + blockerHeight};
    Vector2 blockerPosBL = Vector2{blockerX, blockerY + blockerHeight};
    Vector2 blockerPosTR = Vector2{blockerX + blockerWidth, blockerY};
    Vector2 lightPos = Vector2{lightX, lightY};
    for (float x = 0; x < screenWidth; x++){
        for (float y = 0; y < screenHeight; y++){
            pixelPos.x = x;
            pixelPos.y = y;
            if (CheckCollisionLines(pixelPos, lightPos, blockerPosTL, blockerPosTR, zeroPtr)){ // Top side
                DrawPixel(x, y, BLACK);
            }
            if (CheckCollisionLines(pixelPos, lightPos, blockerPosBL, blockerPosBR, zeroPtr)){ // Bottom side
                DrawPixel(x, y, BLACK);
            }
            if (CheckCollisionLines(pixelPos, lightPos, blockerPosTL, blockerPosBL, zeroPtr)){ // Left side
                DrawPixel(x, y, BLACK);
            }
            if (CheckCollisionLines(pixelPos, lightPos, blockerPosTR, blockerPosBR, zeroPtr)){ // right side
                DrawPixel(x, y, BLACK);
            }
        }
    }
}

int main(void)
{   
    // Init Objects
    // Light Object
    Light light;
    light.x = light.y = 0;
    light.radius = 20;
    light.color = WHITE;
    // Blocker object
    Blocker blocker;
    blocker.x = blocker.y = 250;
    blocker.width = 100;
    blocker.height = 50;
    blocker.color = DARKGRAY;

    InitWindow(screenWidth, screenHeight, "Shadows");

    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(GRAY);
        
        drawPixels(blocker.x, blocker.y, light.x, light.y, blocker.width, blocker.height);
        
        light.draw();
        blocker.draw();
        
        // light.move(); This can be turned on, but when the light is moved the shadows aren't always accurate
        blocker.move();

        EndDrawing();
    }

    CloseWindow();
}