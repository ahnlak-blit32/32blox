/*
 * Ball.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Ball object wraps up balls within the game. This is where the physics
 * is worked out, so the GameState is kept as simple as possible
 */


/* System headers. */


/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"

#include "Ball.hpp"


/* Functions. */

/*
 * constructor - Spawns a ball at the specified location, of the specfied type
 */

Ball::Ball( blit::Point p_origin, ball_type_t p_type )
{
  /* Save the origin and type. */
  location = p_origin;
  ball_type = p_type;

  /* And set some defaults, for now. */
  vector = blit::Vec2( 0, 0 );

  /* All done. */
  return;
}


/*
 * get_location - accessor for the balls current location
 */

blit::Point Ball::get_location( void )
{
  return location;
}


/*
 * get_type - accessor for the ball type
 */

ball_type_t Ball::get_type( void )
{
  return ball_type;
}


/*
 * update - moves the ball along it's defined vectore
 */

void Ball::update( void )
{
  /* This is relatively painless, actually. */
  location += vector;

  /* All done. */
  return;
}


/*
 * bounce - bounces the ball off a horizontal or vertical surface; this is
 *          always done as a straight bounce
 * bool - a flag to indicate a horizontal (true) or vertical (false) bounce
 */

void Ball::bounce( bool p_horizontal )
{
  /* Also fairly easy, thanks to vectors. */
  if ( p_horizontal )
  {
    vector.y *= -1;
  }
  else
  {
    vector.x *= -1;
  }

  /* All done. */
  return;
}


/* End of Ball.cpp */
