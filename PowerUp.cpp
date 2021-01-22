/*
 * PowerUp.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The PowerUp object defines the pills that randomly drop from broken bricks,
 * that provide (usually!) beneficial powerups to the player.
 */


/* System headers. */


/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"

#include "PowerUp.hpp"


/* Functions. */

/*
 * constructor - Spawns a power up at the specified location. 
 */

PowerUp::PowerUp( blit::Point p_origin )
{
  /* Save the origin and pick a type. */
  location = blit::Vec2( p_origin.x, p_origin.y );
  powerup_type = (powerup_type_t)( blit::random() % POWERUP_MAX );

  /* And set a default gravity. */
  vector = blit::Vec2( 0, 0.75f );

  /* All done. */
  return;
}


/*
 * get_render_location - returns the render location of the powerup, taking into
 *                       account the dimensions of the thing
 */

blit::Point PowerUp::get_render_location( void )
{
  /* The inner location represents the middle of the powerup; powerups will */
  /* always be two sprites wide, so similar to the bat logic.               */
  return location - blit::Vec2( 8, 4 );
}


/*
 * get_bounds - returns a rectangle describing the bounding box around the 
 *              powerup; simpler than the ball, in that powerups are always 
 *              a full two sprites big.
 */

blit::Rect PowerUp::get_bounds( void )
{
  return blit::Rect( 
    location - blit::Vec2( 8, 4 ),
    location + blit::Vec2( 8, 4 )
  );
}


/*
 * get_type - accessor for the powerup type
 */

powerup_type_t PowerUp::get_type( void )
{
  return powerup_type;
}


/*
 * update - updates the location of the powerup.
 */

void PowerUp::update( void )
{
  /* This is relatively painless, actually. */
  location += vector;

  /* All done. */
  return;
}


/*
 * render - draws the powerup on the screem. 
 */

void PowerUp::render( void )
{
  /* This is a relatively simple sprite blit. */
  blit::screen.sprite(
    blit::Rect( powerup_type * 2, SPRITE_ROW_POWERUP, 2, 1 ),
    get_render_location()
  );

  /* All done. */
  return;
}


/* End of PowerUp.cpp */
