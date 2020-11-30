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

#include "Level.hpp"


/* Functions. */

/*
 * constructor - create the Level data
 */

Level::Level( uint8_t p_level )
{
  /* Save the level number. */
  level = p_level;

  /*
   * In a normal world, we'd load a level file based on level number. But as
   * we're embedded, level data is compiled in so we just access it.
   */
  switch( p_level )
  {
    case 1:
      init( a_level_01_dat, a_level_01_dat_length );
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
  memset( bricks, 0, BOARD_HEIGHT * BOARD_WIDTH );

  /* Now we work through all the data we have. */
  for( uint32_t i = 0; i < p_datalength; i++ )
  {
    bricks[i / BOARD_WIDTH][i % BOARD_WIDTH] = p_data[i];
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
 * get_brick_count - return an absolute count of remaining breakable bricks.
 */

uint16_t Level::get_brick_count( void )
{
  uint16_t l_total = 0;

  /* So, work through the bricks counting as we go. */
  for( uint8_t row = 0; row < BOARD_HEIGHT; row++ )
  {
    for( uint8_t col = 0; col < BOARD_WIDTH; col++ )
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


/* End of Level.cpp */
