/*
 * 32blox.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * This is the main entry point of the game; it provides the init/update/render
 * functions expected by the 32blit firmware.
 */

/* System headers. */

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "SplashState.hpp"


/* Module variables. */

static gamestate_t m_state = STATE_SPLASH;
static GameStateInterface *m_handlers[STATE_MAX];


/* Functions. */

/*
 * init - called once on startup, to initialise the game. 
 */

void init( void )
{
  /* Switch the screen into low res (160x120) mode. */
  blit::set_screen_mode( blit::ScreenMode::lores );

  /* Black then screen to a nice dark blue. */
  blit::screen.pen = blit::Pen( 100, 0, 0 );
  blit::screen.clear();

  /* Create the game state handlers. */
  for( int i = 0; i < STATE_MAX; i++ )
  {
    m_handlers[i] = NULL;
  }
  m_handlers[STATE_SPLASH] = new SplashState();

  /* All done. */
  return;
}


/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void update( uint32_t p_time )
{
  gamestate_t l_newstate;

  /* Sanity check that the current state has a valid handler. */
  if ( NULL == m_handlers[m_state] )
  {
    m_state = STATE_SPLASH;
  }

  /* So we can safely just call the handler update. */
  l_newstate = m_handlers[m_state]->update( p_time );

  /* If the state has changed, re-call the update for it. */
  if ( l_newstate != m_state )
  {
    m_state = l_newstate;
    update( p_time );
  }

  /* All done. */
  return;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void render( uint32_t p_time )
{
  /* Sanity check that the current state has a valid handler. */
  if ( NULL == m_handlers[m_state] )
  {
    m_state = STATE_SPLASH;
  }

  /* So we can safely just call the handler render. */
  m_handlers[m_state]->render( p_time );

  /* All done. */
  return;
}


/* End of 32blox.cpp */
