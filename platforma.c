#include <raylib.h>
#include <stdio.h>

int main(void)
{
    int width = 640;
    int height = 480;

    InitWindow(width, height, "EZ GAYM!");
    SetTargetFPS(9999);

    float pig_x = width / 2 - 64;
    float pig_y = height / 2 - 48;

    const int PIG_WIDTH = 128;
    const int PIG_HEIGHT = 96;

    float pig_speed = 200.0f;

    float gravity = 900.0f;

    float jump_force = -400.0f;

    float pig_velocity_y = 0.0f;

    float pig_feet_y;

    float move_x = 0;

    bool pig_facing_right = true;
    bool is_on_ground = true;

    Texture2D pig_texture = LoadTexture("Piggie323.png");
    Texture2D terrain_texture = LoadTexture("Sprite-backgro.png");       
    Texture2D plank_texture = LoadTexture("plank.png");
    
      
   
    if (pig_texture.id == 0)
    {
        printf("failed to load texture\n");
    } else {
        printf(" pig textures loaded successfully\n");
    }

    if (terrain_texture.id == 0)
    {
        printf("failed to load terrain texture\n");
    } else {
        printf(" terrain textures loaded successfully\n");
    }

    Camera2D camera = {0};

    camera.target = (Vector2){ pig_x, pig_y };
    camera.offset = (Vector2){ width / 2.0f, height / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        
        
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawTexture(terrain_texture, 0, 0, WHITE);

        

        float delta_time = GetFrameTime();
        move_x = 0;

        Rectangle planks[] = {
            {100, 300, 200, 32},
            {400, 250, 200, 14},
            {700, 180, 150, 50},
            {0, 350, 150, 50},
            {-100, 430, 150, 50}
        };
        
        int plank_count = sizeof(planks) / sizeof(planks[0]);


        
    
        /* WALKIN/ INPUT*/
        
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

        pig_x += move_x;

        Rectangle pig_rect = {
            pig_x,
            pig_y,
            PIG_WIDTH,
            PIG_HEIGHT
        };

        for (int i = 0; i < plank_count; i++)
        {
            if (CheckCollisionRecs(pig_rect, planks[i]))
            {
                // moving right
                if (move_x > 0)
                {
                    pig_x = planks[i].x - PIG_WIDTH;
                }

                // moving left
                if (move_x < 0)
                {
                    pig_x = planks[i].x + planks[i].width;
                }
            }
        }

        pig_rect.x = pig_x;
        
        //gravity 
        pig_velocity_y += gravity * delta_time;
        
        pig_y += pig_velocity_y * delta_time;

        

        //simple ground collision
        float ground_y = height - PIG_HEIGHT;

        pig_rect.y = pig_y;

        is_on_ground = false;

        
        

        if (pig_y >= ground_y)
        {
            pig_y = ground_y;
            pig_velocity_y = 0;
            is_on_ground = true;
        }
        
        

        for (int i = 0; i < plank_count; i++)
        {
            if (CheckCollisionRecs(pig_rect, planks[i]))
            {
                // falling down
                if (pig_velocity_y > 0)
                {
                    pig_y = planks[i].y - PIG_HEIGHT;
                    pig_velocity_y = 0;
                    is_on_ground = true;
                }

                // hitting ceiling
                if (pig_velocity_y < 0)
                {
                    pig_y = planks[i].y + planks[i].height;
                    pig_velocity_y = 0;
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
        
        if(pig_facing_right)
        {
            source.width = pig_texture.width;
        } else {
            source.width = -pig_texture.width;
        }

        camera.target = (Vector2){ pig_x, pig_y };
        
        
        
                
        Rectangle dest = {
            pig_x,
            pig_y,
            PIG_WIDTH,
            PIG_HEIGHT
        };

        Rectangle plank_source = {
            0,
            0,
            plank_texture.width,
            plank_texture.height
        };

        for (int i = 0; i < plank_count; i++)
        {
            DrawTexturePro(
                plank_texture,
                plank_source,
                planks[i],
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
        }

        
                
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
    UnloadTexture(plank_texture);
    UnloadTexture(terrain_texture);
    UnloadTexture(pig_texture);
    CloseWindow();

    return 0;
}