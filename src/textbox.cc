#include "config.h"
#include "textbox.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>
#include <stdlib.h>

// Stolen from raygui.h:2082
// Allows numerical restriction without using GuiValueBox
bool Textbox::draw(bool numerical)
{
    GuiState state = guiState;
    Rectangle textBounds = GetTextBounds(TEXTBOX, bounds);

    bool editMode = edit;
    int textSize = TEXTSIZE;

    bool pressed = false;
    int textWidth = GetTextWidth(text);

    Rectangle cursor = {
        bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + textWidth + 2,
        bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE),
        4,
        (float)GuiGetStyle(DEFAULT, TEXT_SIZE)*2
    };

    if (cursor.height >= bounds.height) cursor.height = bounds.height - GuiGetStyle(TEXTBOX, BORDER_WIDTH)*2;
    if (cursor.y < (bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH))) cursor.y = bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH);

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (editMode)
        {
            state = STATE_PRESSED;

            int key = GetCharPressed();      // Returns codepoint as Unicode
            int keyCount = (int)strlen(text);
            int byteSize = 0;
            const char *textUTF8 = CodepointToUTF8(key, &byteSize);

            // Only allow keys in range [32..125]
            if ((keyCount + byteSize) < textSize)
            {
                //float maxWidth = (bounds.width - (GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING)*2));

                bool cond = numerical? (key >= '0' && key <= '9') : (key >= 32);
                if (cond)
                {
                    for (int i = 0; i < byteSize; i++)
                    {
                        text[keyCount] = textUTF8[i];
                        keyCount++;
                    }

                    text[keyCount] = '\0';
                }
            }

            // Delete text
            if (keyCount > 0)
            {
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    while ((keyCount > 0) && ((text[--keyCount] & 0xc0) == 0x80));
                    text[keyCount] = '\0';
                }
            }

            if (IsKeyPressed(KEY_ENTER) || (!CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) pressed = true;
        }
        else
        {
            if (CheckCollisionPointRec(mousePoint, bounds))
            {
                state = STATE_FOCUSED;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pressed = true;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_PRESSED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), Fade(GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)), guiAlpha));
    }
    else if (state == STATE_DISABLED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), Fade(GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)), guiAlpha));
    }
    else GuiDrawRectangle(bounds, 1, Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), BLANK);

    if (editMode)
    {
        // In case we edit and text does not fit in the textbox,
        // we move text pointer to a position it fits inside the text box
        while ((textWidth >= textBounds.width) && (text[0] != '\0'))
        {
            int codepointSize = 0;
            GetCodepointNext(text, &codepointSize);
            text += codepointSize;
            textWidth = GetTextWidth(text);
            cursor.x = textBounds.x + textWidth + 2;
        }
    }

    GuiDrawText(text, textBounds, GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(TEXTBOX, TEXT + (state*3))), guiAlpha));

    // Draw cursor
    if (editMode) GuiDrawRectangle(cursor, 0, BLANK, Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)), guiAlpha));
    else if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //--------------------------------------------------------------------

    return pressed;

}

void Textbox::end(void)
{
    free(text);
}

Textbox textbox_new(Rectangle bounds)
{
    Textbox t;
    t.bounds = bounds;
    t.text = (char *)malloc(TEXTSIZE + 1);
    memset(t.text, 0, TEXTSIZE);
    t.edit = false;
    return t;
}
