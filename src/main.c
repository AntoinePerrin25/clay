#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "clay/clay_renderer_raylib.c"
#include "main.h"

const uint32_t FONT_ID_BODY_24 = 0;
const uint32_t FONT_ID_BODY_16 = 1;
#define COLOR_ORANGE (Clay_Color) {225, 138, 50, 255}
#define COLOR_BLUE (Clay_Color) {111, 173, 162, 255}

Texture2D profilePicture;
#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) (Clay_Vector2) { .x = vector.x, .y = vector.y }

Clay_String profileText = CLAY_STRING_CONST("Profile Page one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen");
Clay_TextElementConfig headerTextConfig = { .fontId = 1, .fontSize = 16, .textColor = {0,0,0,255} };

void HandleHeaderButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        // Do some click handling
    }
}

Clay_LayoutConfig dropdownTextItemLayout = { .padding = {8, 8, 4, 4} };
Clay_TextElementConfig dropdownTextElementConfig = { .fontSize = 24, .textColor = {255,255,255,255} };

void RenderDropdownTextItem(int index) {
    CLAY({ .layout = dropdownTextItemLayout, .backgroundColor = {180, 180, 180, 255} }) {
        CLAY_TEXT(CLAY_STRING("I'm a text field in a scroll container."), &dropdownTextElementConfig);
    }
}

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))



void HeaderLayout(float resY, float resX)
{
    CLAY({.id = CLAY_ID("Header"),
          .layout = {.sizing = {.width = CLAY_SIZING_GROW(0),
                                .height = CLAY_SIZING_FIXED(resY * 0.1)},
                     .padding = {resX * 0.02, resX * 0.02, resY * 0.02, resY * 0.02},
                     .childGap = resX * 0.02},
          .backgroundColor = {0, 0, 0, 255}})
    {
        CLAY({.id = CLAY_ID("HeaderButton_Fichier"),
              .layout = {.sizing = {.width = CLAY_SIZING_FIXED(resX * 0.1),
                                    .height = CLAY_SIZING_FIXED(resY * 0.06)},
                         .padding = {8, 8, 8, 8},
                         .childGap = 8,
                         .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
              .backgroundColor = Clay_PointerOver(Clay__HashString(CLAY_STRING("HeaderButton_Fichier"), 0, 0)) ? COLOR_ORANGE : COLOR_BLUE})
        {
            CLAY_TEXT(CLAY_STRING("Fichier"), CLAY_TEXT_CONFIG({.fontSize = MIN(resX * 0.08, resY * 0.035), .textColor = {255, 255, 255, 255}}));
        }
        CLAY({.id = CLAY_ID("HeaderButton_Affichage"),
              .layout = {.sizing = {.width = CLAY_SIZING_FIXED(resX * 0.1),
                                    .height = CLAY_SIZING_FIXED(resY * 0.06)},
                         .padding = {8, 8, 8, 8},
                         .childGap = 8,
                         .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
              .backgroundColor = Clay_PointerOver(Clay__HashString(CLAY_STRING("HeaderButton_Affichage"), 0, 0)) ? COLOR_ORANGE : COLOR_BLUE})
        {
            CLAY_TEXT(CLAY_STRING("Affichage"), CLAY_TEXT_CONFIG({.fontSize = MIN(resX * 0.08, resY * 0.035), .textColor = {255, 255, 255, 255}}));
        }
        CLAY({.id = CLAY_ID("HeaderButton_Options"),
              .layout = {.sizing = {.width = CLAY_SIZING_FIXED(resX * 0.1),
                                    .height = CLAY_SIZING_FIXED(resY * 0.06)},
                         .padding = {8, 8, 8, 8},
                         .childGap = 8,
                         .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
              .backgroundColor = Clay_PointerOver(Clay__HashString(CLAY_STRING("HeaderButton_Options"), 0, 0)) ? COLOR_ORANGE : COLOR_BLUE})
        {
            CLAY_TEXT(CLAY_STRING("Options"), CLAY_TEXT_CONFIG({.fontSize = MIN(resX * 0.08, resY * 0.035), .textColor = {255, 255, 255, 255}}));
        }
    }
}


Clay_RenderCommandArray CreateLayout(void) {
    // Header on 10% height from top
    // Sidebar on 20% width from left and under header
    // Main content on 80% width from left and under header
    float resX = (float)GetScreenWidth(); 
    float resY = (float)GetScreenHeight();
    Clay_BeginLayout();
    CLAY({ .id = CLAY_ID("OuterContainer"),
           .layout = {  .sizing = {.width = CLAY_SIZING_GROW(0),
                                   .height = CLAY_SIZING_GROW(0) },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    },
            .backgroundColor = {200, 200, 200, 255} })
                    
            {
                HeaderLayout(resY, resX);
            }
        
    return Clay_EndLayout();
}


typedef struct
{
    Clay_Vector2 clickOrigin;
    Clay_Vector2 positionOrigin;
    bool mouseDown;
} ScrollbarData;

ScrollbarData scrollbarData = {0};

bool debugEnabled = false;

void UpdateDrawFrame(Font* fonts)
{
    Vector2 mouseWheelDelta = GetMouseWheelMoveV();
    float mouseWheelX = mouseWheelDelta.x;
    float mouseWheelY = mouseWheelDelta.y;

    if (IsKeyPressed(KEY_D)) {
        debugEnabled = !debugEnabled;
        Clay_SetDebugModeEnabled(debugEnabled);
    }
    //----------------------------------------------------------------------------------
    // Handle scroll containers
    Clay_Vector2 mousePosition = RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition());
    Clay_SetPointerState(mousePosition, IsMouseButtonDown(0) && !scrollbarData.mouseDown);
    Clay_SetLayoutDimensions((Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() });

    // Generate the auto layout for rendering
    double time1 = GetTime();
    Clay_RenderCommandArray renderCommands = CreateLayout();
    // RENDERING ---------------------------------
    double time2 = GetTime();
    BeginDrawing();
    ClearBackground(BLACK);
    Clay_Raylib_Render(renderCommands, fonts);
    EndDrawing();
    double time3 = GetTime();
    printf("Layout|Render : %2.1f|%f us\n", (time2 - time1) * 1000 * 1000, (time3 - time2) * 1000 * 1000);

    //----------------------------------------------------------------------------------
}

bool reinitializeClay = false;

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
    if (errorData.errorType == CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED) {
        reinitializeClay = true;
        Clay_SetMaxElementCount(Clay_GetMaxElementCount() * 2);
    } else if (errorData.errorType == CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED) {
        reinitializeClay = true;
        Clay_SetMaxMeasureTextCacheWordCount(Clay_GetMaxMeasureTextCacheWordCount() * 2);
    }
}

int main(void) {
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(clayMemory, (Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() }, (Clay_ErrorHandler) { HandleClayErrors, 0 });
    Clay_Raylib_Initialize(1024, 768, "Clay - Raylib Renderer Example", FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    profilePicture = LoadTexture("resources/profile-picture.png");

    Font fonts[2];
    fonts[FONT_ID_BODY_24] = LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400);
	SetTextureFilter(fonts[FONT_ID_BODY_24].texture, TEXTURE_FILTER_BILINEAR);
    fonts[FONT_ID_BODY_16] = LoadFontEx("resources/Roboto-Regular.ttf", 32, 0, 400);
    SetTextureFilter(fonts[FONT_ID_BODY_16].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (reinitializeClay) {
            Clay_SetMaxElementCount(8192);
            totalMemorySize = Clay_MinMemorySize();
            clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
            Clay_Initialize(clayMemory, (Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() }, (Clay_ErrorHandler) { HandleClayErrors, 0 });
            reinitializeClay = false;
        }
        UpdateDrawFrame(fonts);
    }
    Clay_Raylib_Close();
    return 0;
}