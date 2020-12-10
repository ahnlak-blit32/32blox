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

Ball::Ball( blit::Vec2 p_origin, ball_type_t p_type )
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
 * get_render_location - returns the render location of the ball, taking into
 *                       account the ball time and offsets and suchlike.
 */

blit::Point Ball::get_render_location( void )
{
  /* The inner location represents the middle of the ball; every type of ball */
  /* (currently) fits into one sprite, so just move half a sprite up/left.    */
  return location - blit::Vec2( 4, 4 );
}


/*
 * get_bounds - returns a rectangle describing the bounding box around the 
 *              ball, taking into account it's size. This is for quick and
 *              dirty collision detection (the best kind).
 */

blit::Rect Ball::get_bounds( void )
{
  return blit::Rect( 
    location - blit::Vec2( ball_size[ball_type] / 2, ball_size[ball_type] / 2 ),
    location + blit::Vec2( ball_size[ball_type] / 2, ball_size[ball_type] / 2 )
  );
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


/*
 * offset - adds a (signed) offset to the current location, to allow for
 *          moving the ball outside of it's normal vectoring.
 */

void Ball::offset( blit::Vec2 p_offset )
{
  /* Nice and simple. */
  location += p_offset;

  /* All done. */
  return;
}


/*
 * move_bat - handles the bat moving, if we happen to be stuck to it
 * Rect     - the location of the current bat
 * float    - the offset by which the bat has moved
 */

void Ball::move_bat( blit::Rect p_bat, float p_offset )
{
  /* So, this is only significant if the ball is sticky. */
  if ( !sticky )
  {
    return;
  }

  /* If we're on the bat, then we should probably stick to it. */
  blit::Rect l_bounds = get_bounds();

  if ( ( l_bounds.bl().y == p_bat.y ) && 
       ( l_bounds.br().x >= p_bat.x ) && ( l_bounds.bl().x < ( p_bat.x + p_bat.w ) ) )
  {
    /* If we're already stuck, just follow the bat. */
    if ( stuck )
    {
      location.x += p_offset;
    }

    /* Otherwise, remember we're stuck and have done with it. */
    stuck = true;

  }
  /* All done. */
  return;
}


/* End of Ball.cpp */
