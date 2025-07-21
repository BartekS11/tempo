#include "../../include/g_lib.h"
#include "../../include/raylib.h"
#include "../../include/utils.h"

typedef struct {
    const char* title;
    bs_int      screenWidth;
    bs_int      screenHeight;
} WindowSettings;

static WindowSettings windowSettingsDefault = { .title = "DoomCaster DEV",
    .screenWidth                                       = SCREENWIDTH,
    .screenHeight                                      = SCREENHEIGHT };

static void Init(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(windowSettingsDefault.screenWidth,
    windowSettingsDefault.screenHeight, windowSettingsDefault.title);

    // SetTargetFPS(TARGETFPS);
}

void UpdatePlayerPosition2d() {

};

void Update(void)
{
    // if(IsWindowResized()) {
    //     WindowSettingsDefault.screenHeight = GetScreenHeight();
    //     WindowSettingsDefault.screenWidth  = GetScreenWidth();
    // }

    // UpdatePlayerPosition2d();
}

static void Shutdown(void)
{
    CloseWindow();
}

WindowSettings* GetWindowSettings(void)
{
    if(IsWindowResized()) {
        // Update the window settings if the window has been resized
        windowSettingsDefault.screenWidth  = GetScreenWidth();
        windowSettingsDefault.screenHeight = GetScreenHeight();
    }

    BS_ASSERT((windowSettingsDefault.screenWidth > 0 && windowSettingsDefault.screenHeight > 0),
    "Invalid screen dimensions: %d x %d", windowSettingsDefault.screenWidth,
    windowSettingsDefault.screenHeight);

    return &windowSettingsDefault;
}