/*
 * SplashState.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * This State class manages the Splash state of the game; here we just show
 * a nicely animated title screen and invite the user to press A to start.
 */

/* System headers. */

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"

#include "SplashState.hpp"


/* Functions. */

/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t SplashState::update( uint32_t p_time )
{
  /* All done, remain in our current state */
  return STATE_SPLASH;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void SplashState::render( uint32_t p_time )
{

  blit::screen.clear();

  // draw some text at the top of the screen
  blit::screen.alpha = 255;
  blit::screen.mask = nullptr;
  blit::screen.pen = blit::Pen(255, 255, 255);
  blit::screen.rectangle(blit::Rect(0, 0, 320, 14));
  blit::screen.pen = blit::Pen(0, 0, 0);
  blit::screen.text("Hello 32blit!", blit::minimal_font, blit::Point(5, 4));


  /* All done. */
  return;
}


/* End of SplashState.cpp */
