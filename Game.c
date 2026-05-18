#include <raylib.h>
#include <stdio.h>

#define TILE_SKY 0
#define TILE_GROUND 1
#define TILE_BRICK 2
#define TILE_SPIKE 3

//#define TEXTURE_SKY texture0
//#define TEXTURE_GROUND texture1
//#define TEXTURE_BRICK texture2
//#define TEXTURE_SPIKE texture3

#define MAP_WIDTH 200
#define MAP_HEIGHT 200

int map[MAP_HEIGHT][MAP_WIDTH];

void LoadMap(void) {

    FILE *file = fopen("leveldata.txt", "r");

    if (file == NULL)
    {
        printf("Failed to load map\n");
        return;
    }

    char line[MAP_WIDTH + 2];

    for (int row = 0; row < MAP_HEIGHT; row++)
    {
        fgets(line, sizeof(line), file);

        for(int col = 0; col < MAP_WIDTH; col++)
        {
            map[row][col] = line[col] - '0';

        }

    }

    fclose(file);

    printf("all good bru \n");

}







int main(void)
{
    
    int width = 640;
    int height = 480;

    InitWindow(width, height, "EZ GAYM!");
    SetTargetFPS(60);

    float pig_x = width / 2 - 64;
    float pig_y = height / 2 - 48;

    const int PIG_WIDTH = 128 / 4;
    const int PIG_HEIGHT = 96 / 4;

    float pig_speed = 200.0f;

    float gravity = 900.0f;

    float jump_force = -400.0f;

    float pig_velocity_y = 0.0f;

    float move_x = 0;

    bool pig_facing_right = true;
    bool is_on_ground = true;

    Texture2D pig_texture = LoadTexture("Piggie323.png");
    Texture2D texture0 = LoadTexture("sky.png");
    Texture2D texture1 = LoadTexture("ground.png");
    Texture2D texture2 = LoadTexture("brick.png");
    Texture2D texture3 = LoadTexture("spike.png");
    SetTextureFilter(texture0, TEXTURE_FILTER_POINT);
    SetTextureFilter(texture1, TEXTURE_FILTER_POINT);
    SetTextureFilter(texture2, TEXTURE_FILTER_POINT);
    SetTextureFilter(texture3, TEXTURE_FILTER_POINT);   
    
    
    Texture2D tiles[4];
    
    tiles[TILE_SKY] = texture0;
    tiles[TILE_GROUND] = texture1;
    tiles[TILE_BRICK] = texture2;
    tiles[TILE_SPIKE] = texture3;
   
    if (pig_texture.id == 0)
    {
        printf("failed to load texture\n");
    } else {
        printf(" pig textures loaded successfully\n");
    }

    
    float camera_zoom = 1.0f;

    Camera2D camera = {0};

    camera.target = (Vector2){pig_x, pig_y};
    camera.offset = (Vector2){ width / 2.0f, height / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = camera_zoom;




    // Tilemap
    const int TILE_SIZE = 16;
    



    


    
    SetTextureFilter(pig_texture, TEXTURE_FILTER_POINT);

    LoadMap();
    
    while (!WindowShouldClose())
    {
        
        
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        

        

        float delta_time = GetFrameTime();
        move_x = 0;

        
        
        


        
    
        /* WALKIN/ INPUT*/

        float wheel = GetMouseWheelMove();
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            if (wheel > 0)
            {
                camera_zoom += 0.2;
            } 
            else if (wheel < 0) 
            {
                camera_zoom -= 0.2;
            }


        }
        camera.zoom = camera_zoom;
        
        //jumping
        
        // walking
        if (IsKeyDown(KEY_RIGHT))
        {
            move_x += pig_speed * delta_time;
            pig_facing_right = true;
        }
            if (IsKeyDown(KEY_LEFT))
        {
            move_x -= pig_speed * delta_time;
            pig_facing_right = false;
        }

        if (IsKeyPressed(KEY_UP) && is_on_ground)
        {
            pig_velocity_y = jump_force;
        }

                // MOVE HORIZONTALLY
        pig_x += move_x;

        // Pig collision rectangle

        Rectangle pig_rect = {
            pig_x,
            pig_y,
            PIG_WIDTH,
            PIG_HEIGHT
        };

        // HORIZONTAL COLLISION
        for (int row = 0; row < MAP_HEIGHT; row++)
        {
            for (int col = 0; col < MAP_WIDTH; col++)
            {
                if (map[row][col] == TILE_GROUND ||
                    map[row][col] == TILE_BRICK ||
                    map[row][col] == TILE_SPIKE)
                {
                    Rectangle tile_rect = {
                        col * TILE_SIZE,
                        row * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE
                    };

                    if (CheckCollisionRecs(pig_rect, tile_rect))
                    {
                        // moving right
                        if (move_x > 0)
                        {
                            pig_x = tile_rect.x - PIG_WIDTH;
                        }

                        // moving left
                        if (move_x < 0)
                        {
                            pig_x = tile_rect.x + tile_rect.width;
                        }

                        pig_rect.x = pig_x;
                    }
                }
            }
        }

        // APPLY GRAVITY
        pig_velocity_y += gravity * delta_time;

        // MOVE VERTICALLY
        pig_y += pig_velocity_y * delta_time;

        pig_rect.y = pig_y;

        is_on_ground = false;

        // VERTICAL COLLISION
        for (int row = 0; row < MAP_HEIGHT; row++)
        {
            for (int col = 0; col < MAP_WIDTH; col++)
            {
                if (map[row][col] == TILE_GROUND ||
                    map[row][col] == TILE_BRICK ||
                    map[row][col] == TILE_SPIKE)
                {
                    Rectangle tile_rect = {
                        col * TILE_SIZE,
                        row * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE
                    };

                    if (CheckCollisionRecs(pig_rect, tile_rect))
                    {
                        // FALLING DOWN
                        if (pig_velocity_y > 0)
                        {
                            pig_y = tile_rect.y - PIG_HEIGHT;
                            pig_velocity_y = 0;
                            is_on_ground = true;
                        }

                        // HITTING CEILING
                        if (pig_velocity_y < 0)
                        {
                            pig_y = tile_rect.y + tile_rect.height;
                            pig_velocity_y = 0;
                        }

                        pig_rect.y = pig_y;
                    }
                }
            }
        }

        
        

        
        
        for (int row = 0; row < MAP_HEIGHT; row++)
        {
            for (int col = 0; col < MAP_WIDTH; col++)
            {
                if (map[row][col] == TILE_GROUND || map[row][col] == TILE_BRICK || map[row][col] == TILE_SPIKE)
                {
                    Rectangle tile_rect = {
                        col * TILE_SIZE,
                        row * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE
                    };

                    

                    if (CheckCollisionRecs(pig_rect, tile_rect))
                    {
                        // Landing on top
                        if (pig_velocity_y > 0)
                        {
                            pig_y = tile_rect.y - PIG_HEIGHT;
                            pig_velocity_y = 0;
                            is_on_ground = true;

                            pig_rect.y = pig_y;
                        }
                    }
                }
            }
        }
        Rectangle source = {
            0,
            0,
            pig_texture.width,
            pig_texture.height
        };
        //Rectangle pig_rect = {
        //    pig_x,
         //   pig_y,
        //    PIG_WIDTH,
        //    PIG_HEIGHT
        //};
        

        Texture2D *tileTexture = NULL;

        for (int row = 0; row < MAP_HEIGHT; row++)
        {
            for (int col = 0; col < MAP_WIDTH; col++)
            {
                //if (map[row][col] == TILE_GROUND || map[row][col] == TILE_BRICK || map[row][col] == TILE_SPIKE || map[row][col] == TILE_SKY)
                //{
                    switch (map[row][col])
                    {
                        case TILE_SKY:
                            tileTexture = &texture0;
                            break;

                        case TILE_GROUND:
                            tileTexture = &texture1;
                            break;

                        case TILE_BRICK:
                            tileTexture = &texture2;
                            break;

                        case TILE_SPIKE:
                            tileTexture = &texture3;
                            break;
                    }
                    if (tileTexture != NULL)
                    {
                        Rectangle source = {
                            0,
                            0,
                            tileTexture->width,
                            tileTexture->height
                        };

                        Rectangle dest = {
                            col * TILE_SIZE,
                            row * TILE_SIZE,
                            TILE_SIZE,
                            TILE_SIZE
                        };

                        DrawTexturePro(
                            *tileTexture,
                            source,
                            dest,
                            (Vector2){0,0},
                            0.0f,
                            WHITE
                        );
                    }
                //}
            }
        }


        if(pig_facing_right)
        {
            source.width = pig_texture.width;
        } 
        else 
        {
            source.width = -pig_texture.width;
        }

        camera.target = (Vector2){ pig_x, pig_y };
        
        
        
                
        Rectangle dest = {
            pig_x,
            pig_y,
            PIG_WIDTH,
            PIG_HEIGHT
        };

        

        
                
        DrawTexturePro(
            pig_texture,
            source,
            dest,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        EndMode2D();
        DrawText(
            TextFormat("FPS: %i", GetFPS()),
            10,
            10,
            20,
            BLACK
        );
        
        EndDrawing();
    }
    
    UnloadTexture(pig_texture);
    CloseWindow();

    return 0;
}