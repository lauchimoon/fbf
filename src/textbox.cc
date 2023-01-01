#include "config.h"
#include "textbox.h"
#include "raygui.h"

bool Textbox::draw() { return GuiTextBox(bounds, text, TEXTSIZE, edit); }

bool Textbox::draw(int *value) { return GuiValueBox(bounds, text, value, MIN_FPS_VALUE, MAX_FPS_VALUE, edit); }

Textbox textbox_new(Rectangle bounds, char *text)
{
    return Textbox{ bounds, text, false };
}
