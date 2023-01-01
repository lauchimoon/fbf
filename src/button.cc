#include "button.h"
#include "raygui.h"

void Button::draw() { GuiButton(bounds, text); }

bool Button::pressed()
{
    if (CheckCollisionPointRec(GetMousePosition(), bounds))
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    return false;   
}

Button button_new(Rectangle bounds, const char *text)
{
    return Button{ bounds, text };
}
