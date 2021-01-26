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
  location.x = p_origin.x;
  location.y = p_origin.y;
  ball_type = p_type;

  /* And set some defaults, for now. */
  vector = blit::Vec2( 0, 0 );
  bat_position = blit::Rect( 0, 0, 0, 0 );
  stuck = false;

  /* All done. */
  return;
}


/*
 * compute_bat_angle - works out the (radian) angle from vertical to rotate
 *                     a bat bounce from; the central zone is a straight bounce,
 *                     but close to the edge we rotate downwards a little
 */

float Ball::compute_bat_angle( void )
{
  /* So, work out the centre of the bat. */
  uint16_t l_bat_centre = bat_position.x + bat_position.w / 2;

  /* And the ratio away from the centre we are. */
  float l_offset = ( location.x - l_bat_centre ) / (float)bat_position.w;

  /* So, this is the angle to twist at... */
  return l_offset;
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
    location - blit::Vec2( ball_size[ball_type] / 2 - 1, ball_size[ball_type] / 2 - 1 ),
    location + blit::Vec2( ball_size[ball_type] / 2 - 1, ball_size[ball_type] / 2 - 1 )
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
 * moving_up and _left; boolean flags to show the balls current direction of travel
 */

bool Ball::moving_up( void )
{
  return vector.y < 0.0f;
}
bool Ball::moving_left( void )
{
  return vector.x < 0.0f;
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
 * render - draw the ball in it's current position / type
 */

void Ball::render( void )
{
  /* We know all we need to know about ourselves! */
  blit::screen.sprite(
    blit::Rect( ball_type, SPRITE_ROW_BALL, 1, 1 ),
    get_render_location()
  );
}


/*
 * launch - releases a stuck ball from the bat; a random vector is picked,
 *          but it's (partially) influenced by how close to the centre of the
 *          bat it is...
 */

void Ball::launch( void )
{
  /* First off, start with a launch-speed vertical vector. */
  vector.x = 0;
  vector.y = -1.5;

  /* And apply a launch angle to that. */
  vector.rotate( compute_bat_angle() );

  /* This means we're unstuck. */
  stuck = false;

  /* All done. */
  return;
}


/*
 * randomise - sets the ball vector to a random (upwards) direction
 */

void Ball::randomise( void )
{
  /* First off, start with a launch-speed vertical vector. */
  vector.x = 0;
  vector.y = -1.5;

  /* And apply a random angle to it. */
  vector.rotate( (float)( blit::random() % 180 ) / 100.0f - 0.9f );

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
    vector.x *= -1;
  }
  else
  {
    vector.y *= -1;
  }

  /* Sanity check, we should never end up *too* horizontal (<30 degrees) */
  float l_current_angle = vector.angle( blit::Vec2( 1, 0 ) );
  if ( fabs( l_current_angle ) > 2.6f )
  {
    l_current_angle = vector.angle( blit::Vec2( -1, 0 ) );
  }
  if ( fabsf( l_current_angle ) < 0.5f )
  {
    /* Rotate a bit further toward vertical then. */
    if ( l_current_angle < 0.0f )
    {
      vector.rotate( 0.5f + l_current_angle );
    }
    else
    {
      vector.rotate( l_current_angle - 0.5f );
    }
  }

  /* All done. */
  return;
}


/*
 * bat_bounce - a special kind of bounce to handle the bat being involved.
 *              Called whenever the ball is in proximity to the bat.
 * uint16_t - the height of the bat, needed to know if we've just hit. 
 * bool     - a flag to indicate if the bat is sticky.
 *
 * Returns a bool flag to indicate whether this was, indeed, a bounce
 */

bool Ball::bat_bounce( uint16_t p_bat_height, bool p_sticky )
{
  /* Sanity check; nothing to do if the ball is already stuck to the bat. */
  if ( stuck )
  {
    return false;
  }

  /* Only consider this bounce if (a) we weren't on the bat before, and (b) */
  /* we are now.                                                            */
  blit::Rect l_bounds = get_bounds();
  if ( l_bounds.br().y < p_bat_height || ( l_bounds.br().y - vector.y ) >= p_bat_height )
  {
    return false;
  }

  /* So it's a hit, respond appropriately. */
  if ( p_sticky )
  {
    /* Just set a flag to say we're stuck, and zero the vector. */
    stuck = true;
    vector.x = vector.y = 0;
    location.y -= l_bounds.bl().y - bat_position.y;
  }
  else
  {
    /* So... do a vertical bounce first. */
    bounce( false );

    /* And apply a suitable rotation, too. */
    vector.rotate( compute_bat_angle() );
  }

  /* All done. */
  return true;
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
 * bool     - whether the bat is sticky
 */

void Ball::move_bat( blit::Rect p_bat, float p_offset, bool p_sticky )
{
  /* Remeber the position of the bat, we'll often need it. */
  bat_position = p_bat;

  /* So, this is only significant if the bat is sticky. */
  if ( !stuck && !p_sticky )
  {
    return;
  }

  /* And if there's no movement, there's nothing to do. */
  if ( p_offset == 0.0f )
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
      /* Apply the bat offset. */
      location.x += p_offset;

      /* But make sure we're still on the screen. */
      if ( location.x < ball_size[ball_type] / 2 )
      {
        location.x = ball_size[ball_type] / 2;
      }
      if ( location.x > blit::screen.bounds.w - ball_size[ball_type] / 2 )
      {
        location.x = blit::screen.bounds.w - ball_size[ball_type] / 2;
      }
    }

    /* Otherwise, remember we're stuck and have done with it. */
    stuck = true;

  }

  /* All done. */
  return;
}


/* End of Ball.cpp */
