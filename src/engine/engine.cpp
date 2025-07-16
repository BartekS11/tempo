#include "../../include/raylib.h"
#include "../../include/utils.h"

typedef struct {
    const char* title;
    bs_int      screenWidth;
    bs_int      screenHeight;
} WindowSettings;

WindowSettings WindowSettingsDefault = { .title = "DoomCaster DEV",
    .screenWidth                                = SCREENWIDTH,
    .screenHeight                               = SCREENHEIGHT };

static void Init(void)
{
    InitWindow(WindowSettingsDefault.screenWidth,
    WindowSettingsDefault.screenHeight, WindowSettingsDefault.title);

    // SetTargetFPS(TARGETFPS);
}

void UpdatePlayerPosition2d() {

};

void Update(void)
{
    if(IsWindowResized()) {
        WindowSettingsDefault.screenHeight = GetScreenHeight();
        WindowSettingsDefault.screenWidth  = GetScreenWidth();
    }

    UpdatePlayerPosition2d();
}

static void Shutdown(void)
{
    CloseWindow();
}
