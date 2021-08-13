/*
 * Level.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Level object contains the details of a game level. It maintains a map
 * of which bricks are where, along with quick access functions to that data.
 */


/* System headers. */

#include <string.h>


/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "assets_levels.hpp"
#include "assets_pico_levels.hpp"

#include "Level.hpp"


/* Functions. */

/*
 * constructor - create the Level data
 */

Level::Level( uint8_t p_level, target_type_t p_target )
{
  bool l_pico = false;

  /* Save the level number. */
  level = p_level;

  /*
   * The level dimensions are determined by platform, because not all screens
   * are equal in size, or even aspect ratio.
   */
  switch( p_target )
  {
    case TARGET_PICOSYSTEM: /* Need to shrink things. */
    /* case TARGET_SDL:        /* (for testing) */
      width = 7;
      height = 8;
      margin = 8;
      l_pico = true;
      break;
    default:                /* In all other cases, use maximum size. */
      width = MAX_BOARD_WIDTH;
      height = MAX_BOARD_HEIGHT;
      margin = 0;
      break;
  }

  /*
   * In a normal world, we'd load a level file based on level number. But as
   * we're embedded, level data is compiled in so we just access it.
   *
   * We have a seperate set of levels for pico systems.
   */
  switch( ( p_level % LEVEL_MAX ) )
  {
    case 1:
      l_pico ? init( a_pico_level_01, a_pico_level_01_length ) : init( a_level_01, a_level_01_length );
      break;
    case 2:
      l_pico ? init( a_pico_level_02, a_pico_level_02_length ) : init( a_level_02, a_level_02_length );
      break;
    case 3:
      l_pico ? init( a_pico_level_03, a_pico_level_03_length ) : init( a_level_03, a_level_03_length );
      break;
    case 4:
      l_pico ? init( a_pico_level_04, a_pico_level_04_length ) : init( a_level_04, a_level_04_length );
      break;
    case 5:
      l_pico ? init( a_pico_level_05, a_pico_level_05_length ) : init( a_level_05, a_level_05_length );
      break;
    case 6:
      l_pico ? init( a_pico_level_06, a_pico_level_06_length ) : init( a_level_06, a_level_06_length );
      break;
    case 7:
      l_pico ? init( a_pico_level_07, a_pico_level_07_length ) : init( a_level_07, a_level_07_length );
      break;
    case 8:
      l_pico ? init( a_pico_level_08, a_pico_level_08_length ) : init( a_level_08, a_level_08_length );
      break;
    case 9:
      l_pico ? init( a_pico_level_09, a_pico_level_09_length ) : init( a_level_09, a_level_09_length );
      break;
    case 0:
      l_pico ? init( a_pico_level_10, a_pico_level_10_length ) : init( a_level_10, a_level_10_length );
      break;
    default:
      init( nullptr, 0 );
      break;
  }
  return;
}


/*
 * init - reads the provided data, and loads it into our internal brick matrix
 *
 * uint8_t *, a single dimension array which contains an arbitrary number of rows.
 * uint32_t, the number of elements in the data.
 */

void Level::init( const uint8_t *p_data, uint32_t p_datalength )
{
  /* Firstly, we make sure the brick matrix is empty. */
  memset( bricks, 0, MAX_BOARD_HEIGHT * MAX_BOARD_WIDTH );

  /* Now we work through all the data we have. */
  for( uint32_t i = 0; i < p_datalength; i++ )
  {
    bricks[i / width][i % width] = p_data[i];
  }

  /* That's it, that's all we have to do. */
  return;
}


/*
 * get_level - return the level number we represent
 */

uint8_t Level::get_level( void )
{
  return level;
}


/*
 * get_width - return the width of the level, in bricks.
 */

uint8_t Level::get_width( void )
{
  return width;
}


/*
 * get_height - return the height of the level, in bricks.
 */

uint8_t Level::get_height( void )
{
  return height;
}


/*
 * get_margin - returns the width of the side margin, when the board doesn't
 *              fill the whole screen.
 */

uint8_t Level::get_margin( void )
{
  return margin;
}


/*
 * get_brick_count - return an absolute count of remaining breakable bricks.
 */

uint16_t Level::get_brick_count( void )
{
  uint16_t l_total = 0;

  /* So, work through the bricks counting as we go. */
  for( uint8_t row = 0; row < MAX_BOARD_HEIGHT; row++ )
  {
    for( uint8_t col = 0; col < MAX_BOARD_WIDTH; col++ )
    {
      /* We only count breakable bricks - greater than 0, less than 8. */
      if ( bricks[row][col] > 0 && bricks[row][col] < 8 )
      {
        l_total++;
      }
    }
  }

  /* Return the count we found. */
  return l_total;
}


/*
 * get_brick - returns the brick value at the given co-ordinate, zero if none.
 *
 * uint8_t * 2 - the row and column in the level being queried.
 */

uint8_t Level::get_brick( uint8_t p_row, uint8_t p_column )
{
  return bricks[p_row][p_column];
}

uint8_t Level::get_brick( blit::Point p_point )
{
  return get_brick( p_point.y, p_point.x );
}


/*
 * hit_brick - updates the brick at the given co-ordinate, reflecting that it's
 *             been hit by a ball.
 *
 * blit::Point - the location of the brick.
 *
 * Returns uint8_t, the score generated by this hit
 */

uint8_t Level::hit_brick( blit::Point p_point )
{
  /* If there's nothing there, there is nothing to do. */
  if ( bricks[p_point.y][p_point.x] == 0 )
  {
    return 0;
  }

  /* Also, if it's an unbreakable brick, we skip it too. */
  if ( bricks[p_point.y][p_point.x] == 8 )
  {
    return 0;
  }

  /* So, decrement the brick number. */
  bricks[p_point.y][p_point.x]--;

  /* And grant the player a score for each brick level destroyed. */
  return 10;
}


/*
 * get_ball_speed - works out how fast a standard ball should be moving, which
 *                  is based on the level we're currently on. It's a cynical
 *                  trick to increase difficulty without building load of new
 *                  levels :-)
 *
 * Returns float, the speed of the ball
 */

float Level::get_ball_speed( void )
{
  float l_base_speed = 1.5f;

  /* Levels are cyclic, and we have a speed bump for each complete cycle. */
  l_base_speed += ( ( level - 1 ) / LEVEL_MAX ) / 2.0f;

  /* All done, return it. */
  return l_base_speed;
}
 
/* End of Level.cpp */
