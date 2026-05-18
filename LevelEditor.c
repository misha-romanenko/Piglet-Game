#include <stdio.h>
#include "raylib.h"

#define MAP_WIDTH 200
#define MAP_HEIGHT 200


int map[MAP_HEIGHT][MAP_WIDTH] = {0};

void SaveMap(void)
{
    FILE *file = fopen("leveldata.txt", "w");

    if (file == NULL)
    {
        printf("Failed to save map\n");
        return;
    }

    for (int row = 0; row < MAP_HEIGHT; row++)
    {
        for (int col = 0; col < MAP_WIDTH; col++)
        {
            fprintf(file, "%d", map[row][col]);
        }

        fprintf(file, "\n");
    }

    fclose(file);

    printf("Map saved!\n");
}
    

int main(void)
{
    const int width = 1920;
    const int height = 1080;
    int gridSize = 16;
    InitWindow(width, height, "Level Editor");

    
    int selectedTile = 1;

    SetTargetFPS(60);

    int camera_speed = 200;

    float camera_zoom = 1.0f;

    Camera2D camera = {0};

    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){ width / 2.0f, height / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = camera_zoom;

    float x = 128;
    float y = 128;

    Texture2D texture0 = LoadTexture("sky.png");
    Texture2D texture1 = LoadTexture("ground.png");
    Texture2D texture2 = LoadTexture("brick.png");
    Texture2D texture3 = LoadTexture("spike.png");
    SetTextureFilter(texture0, TEXTURE_FILTER_POINT);
    SetTextureFilter(texture1, TEXTURE_FILTER_POINT);
    SetTextureFilter(texture2, TEXTURE_FILTER_POINT);
    SetTextureFilter(texture3, TEXTURE_FILTER_POINT);

    Texture2D tiles[4];

    tiles[0] = texture0;
    tiles[1] = texture1;
    tiles[2] = texture2;
    tiles[3] = texture3;
    
    
    

    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        Vector2 mousePosition = GetMousePosition();

        Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), camera);

        int TileX = worldMouse.x / gridSize;
        int TileY = worldMouse.y / gridSize;    

        

        if (IsKeyDown(KEY_LEFT))
        {
            camera.target.x -= camera_speed * delta_time;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            camera.target.x += camera_speed * delta_time;
        }
        if (IsKeyDown(KEY_UP))
        {
            camera.target.y -= camera_speed * delta_time;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            camera.target.y += camera_speed * delta_time;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mousePosition,
                (Rectangle){24, 32, 16, 16}))
            {
                selectedTile = 0;
            }

            if (CheckCollisionPointRec(mousePosition,
                (Rectangle){40, 32, 16, 16}))
            {
                selectedTile = 1;
            }
            if (CheckCollisionPointRec(mousePosition,
                (Rectangle){56, 32, 16, 16}))
            {
                selectedTile = 2;
            }
            if (CheckCollisionPointRec(mousePosition,
                (Rectangle){72, 32, 16, 16}))
            {
                selectedTile = 3;
            }
            
        }

        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            if (TileX >= 0 &&
                TileX < MAP_WIDTH &&
                TileY >= 0 &&
                TileY < MAP_HEIGHT)
                    {
                        map[TileY][TileX] = selectedTile;
                    }
        }


        

        

        for (int row = 0; row < MAP_HEIGHT; row++)
        {
            for (int col = 0; col < MAP_WIDTH; col++)
            {
                int tile = map[row][col];

                Texture2D tex;

                DrawTexture(
                    tiles[tile],
                    col * gridSize,
                    row * gridSize,
                    WHITE
                );
                
                                
            }
        }
        




        for (int row = 0; row < MAP_HEIGHT; row++)
        {
            for (int col = 0; col < MAP_WIDTH; col++)
            {
                DrawRectangleLines(
                    col * gridSize,
                    row * gridSize,
                    gridSize,
                    gridSize,
                    BLACK
                );
            }
        }
        //for (int i = 0; i < width; i += gridSize)
        //{
        //    for (int j = 0; j < height; j += gridSize)
        //    {
        //        DrawRectangleLines(x + (i), y + j, gridSize, gridSize, BLACK);
        //    }
        //}

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            map[TileY][TileX] = 0;
        }

        float wheel = GetMouseWheelMove();
        if(!IsKeyDown(KEY_LEFT_SHIFT)){
            if (wheel > 0)
            {
                camera_speed += 50;
            } 
            else if (wheel < 0) 
            {
                camera_speed -= 50;
            }


        }
        

        if(IsKeyDown(KEY_LEFT_SHIFT)){
            if(wheel > 0){camera_zoom += 0.2;}
            else if(wheel < 0){camera_zoom -= 0.2;}
        }
        if (camera_zoom < 0.2f)
        {
            camera_zoom = 0.2f;
        }
        camera.zoom = camera_zoom;

        EndMode2D();

        if (IsKeyPressed(KEY_S))
        {
            SaveMap();
        }

        
        
        
        
        DrawTexture(texture0, 24, 32, WHITE);
        DrawTexture(texture1, 40, 32, WHITE);
        DrawTexture(texture2, 56, 32, WHITE);
        DrawTexture(texture3, 72, 32, WHITE);

        DrawText("You've selected: ", 200, 10, 20, DARKGRAY);
        if(selectedTile == 0){DrawText("texture0 or sky", 400, 10, 20, DARKGRAY);}
        else if(selectedTile == 1){DrawText("texture1 or ground", 400, 10, 20, DARKGRAY);}
        else if(selectedTile == 2){DrawText("texture2 or brick", 400, 10, 20, DARKGRAY);}
        else if(selectedTile == 3){DrawText("texture3 or spike", 400, 10, 20, DARKGRAY);}
        

        DrawText("Level Editor", 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Camera speed %d ", camera_speed), 24, 256, 20, DARKGRAY);

        DrawText(TextFormat("FPS: %d", GetFPS()), 24, 128, 24, GRAY);

        //DrawText(TextFormat("World mouse cordinates: %d, %d", worldMouse.x, worldMouse.y), 24, 128, 24, GRAY);

        EndDrawing();
    }


    




    

    UnloadTexture(texture0);
    UnloadTexture(texture1);
    UnloadTexture(texture2);
    UnloadTexture(texture3);
    CloseWindow();
}