#include <Engine/OGLGame.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>

#include "GameLogic.h"

/*
 * Core game to engine logic. Primarily used to capture key inputs and render output text.
 */

class HauntedHouseGame : public ASGE::OGLGame
{
public:
    HauntedHouseGame() = default;
    ~HauntedHouseGame() = default;

    bool init() override;
    void keyHandler(const ASGE::SharedEventData data);
    void update(const ASGE::GameTime& us) override;
    void render(const ASGE::GameTime& us) override;

private:
    GameLogic logic;
    GlobalDynamicText screen_text;

    int key_callback_id = -1;
    string user_input;

    float text_colour[3] = {0.815F, 0.996F, 0.854F};
    float background_colour[3] = {0.172F, 0.172F, 0.172F};
};
