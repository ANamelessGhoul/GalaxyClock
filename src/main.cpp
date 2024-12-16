#include "raylib.h"
#include "raymath.h"
#include "local_time.h"
#include "rlgl.h"
#include "helpers.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(void);

#define DEBUG_MODE 0

#if DEBUG_MODE
static const int screenWidth = 240 * 2;
static const int screenHeight = 240 * 2;
#else
static const int screenWidth = 240;
static const int screenHeight = 240;
#endif

constexpr float close_button_size = 20;
constexpr float close_button_padding = 5;
constexpr Vector2 close_button_position = {
    screenWidth / 2 - close_button_size / 2 - close_button_padding,
    -screenHeight / 2 + close_button_size / 2 + close_button_padding
};

constexpr float maximize_button_size = 15;
constexpr float maximize_button_padding = 7;
constexpr Rectangle maximize_button_rect = {
    screenWidth / 2 - maximize_button_size - close_button_padding - close_button_size - maximize_button_padding,
    -screenHeight / 2 + maximize_button_padding,
    maximize_button_size,
    maximize_button_size
};

constexpr float minimize_button_size = 15;
constexpr float minimize_button_padding = 7;
constexpr Rectangle minimize_button_rect = {
    maximize_button_rect.x - minimize_button_size - minimize_button_padding - 3,
    -screenHeight / 2 + minimize_button_padding,
    minimize_button_size,
    minimize_button_size
};



Vector2 GetArmPosition(float time_ratio, float radius)
{
    return {cosf((PI * 2) * time_ratio - PI / 2) * radius, sinf((PI * 2) * time_ratio - PI / 2) * radius};
}

void DrawFourStar(Vector2 position, float size, Color color = RAYWHITE);
void DrawCross(Rectangle rect, Color color);

const Color background_color = GetColor(0x181818FF);
constexpr size_t star_count = 100;

constexpr int target_fps = 120;

Vector2 star_positions[star_count];
float star_sizes[star_count];

Vector2 mouse_monitor_position{0};
Vector2 window_position{0};

bool should_close{0};

bool is_dragging{0};

int main(void)
{
    // Initialization
    //---------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Galaxy Clock");

    NGrandom::seed();
    for (size_t i = 0; i < star_count; i++)
    {
        star_positions[i] = {NGrandom::signed_float() * screenWidth / 2.f, NGrandom::signed_float() * screenHeight / 2.f};
        star_sizes[i] = {5 + NGrandom::unsigned_float() * 5};
    }
    
    //--------------------------------------------------------------------------------------

    InitTimer();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(target_fps);

    // Main game loop
    while (!WindowShouldClose() && !should_close)    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------

    CloseWindow();          // Close window and OpenGL context
    
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdateDrawFrame()
{
    Time time = GetLocalTime();

    float seconds_ratio = (time.seconds + time.milliseconds / 1000.f) / 60.f;
    float minutes_ratio = (time.minutes + seconds_ratio) / 60.f;
    float hours_ratio = (time.hours + minutes_ratio) / 12.f;

    Vector2 screen_center = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    float screen_scale = math::min(GetScreenWidth(), GetScreenHeight()) / static_cast<float>(screenWidth);


#if !defined(PLATFORM_WEB)
    Matrix matScale = MatrixScale(screen_scale, screen_scale, 1);
    Matrix matOrigin = MatrixTranslate(screen_center.x, screen_center.y, 0);
    Matrix cameraMatrix = MatrixMultiply(matScale, matOrigin);
    Matrix invCameraMatrix = MatrixInvert(cameraMatrix);
    Vector2 world_mouse_position = Vector2Transform(GetMousePosition(), invCameraMatrix);

    bool hovering_close_button = CheckCollisionPointCircle(world_mouse_position, close_button_position, close_button_size / 2);
    bool hovering_maximize_button = CheckCollisionPointRec(world_mouse_position, maximize_button_rect);
    bool hovering_minimize_button = CheckCollisionPointRec(world_mouse_position, minimize_button_rect);

    // Window Drag
    if (!IsWindowMaximized() && !IsWindowFullscreen())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            mouse_monitor_position = Vector2Add(GetWindowPosition(), GetMousePosition());
            window_position = GetWindowPosition();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            is_dragging = true;
            SetTargetFPS(60);
            Vector2 new_mouse_position = Vector2Add(GetWindowPosition(), GetMousePosition());
            Vector2 mouse_delta = Vector2Subtract(new_mouse_position, mouse_monitor_position);
            SetWindowPosition(window_position.x + mouse_delta.x, window_position.y + mouse_delta.y);
        }
        else
        {
            SetTargetFPS(target_fps);
            
            is_dragging = false;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (hovering_close_button)
        {
            should_close = true;
            return;
        }

        if (hovering_maximize_button)
        {
            hovering_maximize_button = false;

            if (IsWindowMaximized())
            {
                RestoreWindow();
            }
            else
            {
                MaximizeWindow();
            }
        }

        if (hovering_minimize_button)
        {
            hovering_minimize_button = false;
            MinimizeWindow();
        }
    }

#endif

    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));

    rlPushMatrix();
    rlTranslatef(screen_center.x, screen_center.y, 0);
    rlScalef(screen_scale, screen_scale, 1);
    for (size_t i = 0; i < star_count; i++)
    {
        DrawFourStar(star_positions[i], star_sizes[i], DARKGRAY);
    }

    DrawCircle(0, 0, 25, YELLOW);

    DrawCircleLines(0, 0, 100, RAYWHITE);

    DrawFourStar(GetArmPosition(12 / 12.f, 90), 20);
    DrawFourStar(GetArmPosition(3 / 12.f, 90), 20);
    DrawFourStar(GetArmPosition(6 / 12.f, 90), 20);
    DrawFourStar(GetArmPosition(9 / 12.f, 90), 20);

    DrawFourStar(GetArmPosition(1 / 12.f, 95), 10, GRAY);
    DrawFourStar(GetArmPosition(2 / 12.f, 95), 10, GRAY);
    DrawFourStar(GetArmPosition(4 / 12.f, 95), 10, GRAY);
    DrawFourStar(GetArmPosition(5 / 12.f, 95), 10, GRAY);
    DrawFourStar(GetArmPosition(7 / 12.f, 95), 10, GRAY);
    DrawFourStar(GetArmPosition(8 / 12.f, 95), 10, GRAY);
    DrawFourStar(GetArmPosition(10 / 12.f, 95), 10, GRAY);
    DrawFourStar(GetArmPosition(11 / 12.f, 95), 10, GRAY);

    DrawCircleLinesV(GetArmPosition(hours_ratio, 100), 15, RED);
    DrawCircleV(GetArmPosition(hours_ratio, 100), 15 / 2.f, RED);

    DrawCircleLinesV(GetArmPosition(minutes_ratio, 100), 10, BLUE);
    DrawCircleV(GetArmPosition(minutes_ratio, 100), 10 / 2.f, BLUE);

    DrawCircleLinesV(GetArmPosition(seconds_ratio, 100), 5, GREEN);
    DrawCircleV(GetArmPosition(seconds_ratio, 100), 5 / 2.f, GREEN);


    
#if !defined(PLATFORM_WEB)
    // UI
    DrawRectangleRec(maximize_button_rect, background_color);
    DrawRectangleLinesEx(maximize_button_rect, 2, hovering_maximize_button ? GRAY : DARKGRAY);

    DrawRectangleRec(minimize_button_rect, background_color);
    Rectangle drawn_rec = minimize_button_rect;
    drawn_rec.y = minimize_button_rect.y + minimize_button_rect.height - 2;
    drawn_rec.height = 2;
    DrawRectangleLinesEx(drawn_rec, 2, hovering_minimize_button ? GRAY : DARKGRAY);

    rlPushMatrix();
    rlTranslatef(close_button_position.x, close_button_position.y, 0);
    rlRotatef(45, 0, 0, 1);
    DrawCircle(0, 0, close_button_size / 2, background_color);
    DrawFourStar({0, 0}, close_button_size, hovering_close_button ? GRAY : DARKGRAY);
    rlPopMatrix();
#endif

    rlPopMatrix();

#if DEBUG_MODE
    int line = 0;
    DrawText(TextFormat("Hours: %d", time.hours), 0, 20 * line++, 20, RAYWHITE);
    DrawText(TextFormat("Minutes: %d", time.minutes), 0, 20 * line++, 20, RAYWHITE);
    DrawText(TextFormat("Seconds: %d", time.seconds), 0, 20 * line++, 20, RAYWHITE);
    DrawText(TextFormat("Millis: %d", time.milliseconds), 0, 20 * line++, 20, RAYWHITE);
#endif
    EndDrawing();
}

void DrawFourStar(Vector2 position, float size, Color color)
{
    float half_size = size * 0.5f;
    float tenth_size = size * 0.1f;

    rlColor4ub(color.r, color.g, color.b, color.a);
    rlBegin(RL_TRIANGLES);
        rlVertex2f(position.x, position.y - half_size);
        rlVertex2f(position.x - tenth_size, position.y);
        rlVertex2f(position.x + tenth_size, position.y);

        rlVertex2f(position.x + half_size, position.y);
        rlVertex2f(position.x, position.y - tenth_size);
        rlVertex2f(position.x, position.y + tenth_size);

        rlVertex2f(position.x, position.y + half_size);
        rlVertex2f(position.x + tenth_size, position.y);
        rlVertex2f(position.x - tenth_size, position.y);

        rlVertex2f(position.x - half_size, position.y);
        rlVertex2f(position.x, position.y + tenth_size);
        rlVertex2f(position.x, position.y - tenth_size);
    rlEnd();

}

void DrawCross(Rectangle rect, Color color)
{
    rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex2f(rect.x, rect.y);
        rlVertex2f(rect.x + rect.width, rect.y + rect.height);
    rlEnd();

    rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex2f(rect.x + rect.width, rect.y);
        rlVertex2f(rect.x, rect.y + rect.height);
    rlEnd();
}
