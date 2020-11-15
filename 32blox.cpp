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
#include "GameState.hpp"
#include "DeathState.hpp"


/* Module variables. */

static gamestate_t          m_state = STATE_SPLASH;
static GameStateInterface  *m_handlers[STATE_MAX];
static uint32_t             m_current_tick;


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
  for( uint8_t i = 0; i < STATE_MAX; i++ )
  {
    m_handlers[i] = nullptr;
  }
  m_handlers[STATE_SPLASH] = new SplashState();
  m_handlers[STATE_GAME] = new GameState();
  m_handlers[STATE_DEATH] = new DeathState();

  /* We track what tick we're in, to avoid infinite recursion. */
  m_current_tick = 0;

  /* And we set the starting state to something sensible. */
  m_state = STATE_SPLASH;
  m_handlers[m_state]->init( nullptr );

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
  if ( nullptr == m_handlers[m_state] )
  {
    m_state = STATE_SPLASH;
  }

  /* So we can safely just call the handler update. */
  l_newstate = m_handlers[m_state]->update( p_time );

  /* If the state has changed, re-call the update for it. */
  if ( ( l_newstate != m_state ) && ( nullptr != m_handlers[l_newstate] ) )
  {
    /* Initialise the new state. */
    m_handlers[l_newstate]->init( m_handlers[m_state] );

    /* Switch to the new state. */
printf( "Switching states: %d -> %d\n", m_state, l_newstate );    
    m_state = l_newstate;

    /* Only call the update if we haven't been here before. */
    if ( m_current_tick != p_time )
    {
      m_current_tick = p_time;
      update( p_time );
    }
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
  if ( nullptr == m_handlers[m_state] )
  {
    m_state = STATE_SPLASH;
  }

  /* So we can safely just call the handler render. */
  m_handlers[m_state]->render( p_time );

  /* All done. */
  return;
}


/* End of 32blox.cpp */
