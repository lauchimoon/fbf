#include "button.h"
#include "raygui.h"

void Button::draw()
{
    if (!enabled) {
        GuiDisable();
        GuiButton(bounds, text);
        GuiEnable();
    } else {
        GuiButton(bounds, text);
    }
}

bool Button::pressed()
{
    if (CheckCollisionPointRec(GetMousePosition(), bounds) && enabled)
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    return false;   
}

Button button_new(Rectangle bounds, const char *text)
{
    return Button{ bounds, text, true };
}
