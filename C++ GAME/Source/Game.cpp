#include "Game.h"

/* Gsme initialisation */
bool HauntedHouseGame::init()
{
    //Setup game resolution
    game_width = 650;
    game_height = 402;

    //Initialise game engine
    if (!initAPI())
        return false;

    //Basic configurations
    renderer->setWindowTitle("Text Adventure");
    renderer->setClearColour(background_colour);

    //Input listener
    inputs->use_threads = false;
    key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &HauntedHouseGame::keyHandler, this);

    //Game font
    auto font = ASGE::FILEIO::File();
    font.open("/data/apple2.ttf");
    auto buffer = font.read();
    renderer->setFont(renderer->loadFontFromMem("TextAdventureFont", buffer.as_unsigned_char(), buffer.length, 16));

    return true;
}

/* Triggered on key input event */
void HauntedHouseGame::keyHandler(const ASGE::SharedEventData data)
{
    //Current key data
    auto key = static_cast<const ASGE::KeyEvent*>(data.get());

    if (key->action == ASGE::KEYS::KEY_RELEASED && key->key == ASGE::KEYS::KEY_BACKSPACE)
    {
        //Backspace - delete last character
        user_input = user_input.substr(0, user_input.size() - 1);
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED && key->key == ASGE::KEYS::KEY_ENTER)
    {
        //Pass input off to game logic and clear
        logic.handleInput(user_input);
        user_input = "";
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
        //Key pressed, add to input and capitalise
        user_input += toupper((char)key->key);
    }
}

/* Currently unused game update tick logic */
void HauntedHouseGame::update(const ASGE::GameTime &us) {}

/* Render output */
void HauntedHouseGame::render(const ASGE::GameTime &us)
{
    //Render output text and user input
    renderer->renderText(screen_text.getScreenText()+user_input, 20, 20, 1, text_colour);
}
