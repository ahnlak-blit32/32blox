/*
 * HighScore.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The HighScore class manages the high score table, which will be saved onto
 * the SD card (if we can)
 */

/* System headers. */

#include <string.h>


/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"

#include "HighScore.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

HighScore::HighScore( void )
{
  char    l_buffer[256];

  /* First off, check to see if the game data space exists and, if not, make it. */
  for( uint8_t i = 0; i < 255 && i < strlen( GAME_DATAFILE_HISCORE ); i++ )
  {
    /* If we find a path divider, check that the current path exists and, */
    /* if not, create it.                                                 */
    if ( '/' == GAME_DATAFILE_HISCORE[i] )
    {
      /* Pull out this bit of the path. */
      strncpy( l_buffer, GAME_DATAFILE_HISCORE, i );
      l_buffer[i] = '\0';

      /* Does it exist? */
      if ( !blit::directory_exists( l_buffer ) )
      {
        printf( "Creating '%s'\n", l_buffer);
        blit::create_directory( l_buffer );
      }
    }
  }

  /* Try and open the file then! */
  load();

  /* All done. */
  return;
}


/*
 * rank_score - determines where in the table the provided score should go
 * 
 * uint16_t - the score being checked
 *
 * Returns the table position, or MAX_SCORES if it's below all scores. 
 */

uint8_t HighScore::rank( uint16_t p_score )
{
  /* Scan through the high score table, looking to see where this score goes. */
  for( uint8_t i = 0; i < MAX_SCORES; i++ )
  {
    /* If the score equals or exceeds the current entry, this is it! */
    if ( p_score >= scores[i].score )
    {
      return i;
    }
  }

  /* If we drop out here, we found nothing matching. */
  return MAX_SCORES;
}


/*
 * load - loads the table from the file, if we can.
 */

void HighScore::load( void )
{
  /* Try to open the data file. */
  blit::File *l_fptr = new blit::File( GAME_DATAFILE_HISCORE );
  if ( l_fptr )
  {
    /* Read it in, in one big lump. */
    l_fptr->read( 0, sizeof( hiscore_t ) * MAX_SCORES, (char *)scores );
    l_fptr->close();
  }
  else
  {
    /* Initialise the table to empty, then. */
    for( uint8_t i = 0; i < MAX_SCORES; i++ )
    {
      scores[i].name[0] = scores[i].name[1] = scores[i].name[2] = 'A';
      scores[i].score = 0;
    }
  }

  /* All done. */
  return;
}


/*
 * save - adds the score to the table, and saves it to persistent storage
 *
 * uint16_t - the score being saved,
 * const char * - the name being saved with the score
 */

void HighScore::save( uint16_t p_score, const char *p_name )
{
  /* Work out where we want to add ourselves. */
  uint8_t l_position = rank( p_score );

  /* Sanity check; this had better be a valid position! */
  if ( l_position >= MAX_SCORES )
  {
    return;
  }

  /* Shuffle entries below down, to make room. */
  for( uint8_t i = MAX_SCORES-1; i > l_position; i-- )
  {
    scores[i].name[0] = scores[i-1].name[0];
    scores[i].name[1] = scores[i-1].name[1];
    scores[i].name[2] = scores[i-1].name[2];
    scores[i].score = scores[i-1].score;
  }

  /* Fill in the new data. */
  scores[l_position].name[0] = p_name[0];
  scores[l_position].name[1] = p_name[1];
  scores[l_position].name[2] = p_name[2];
  scores[l_position].score = p_score;

  /* And lastly, write this out to the high score storage. */
  blit::File *l_fptr = new blit::File( GAME_DATAFILE_HISCORE, blit::OpenMode::write );
  if ( l_fptr )
  {
    /* Read it in, in one big lump. */
    l_fptr->write( 0, sizeof( hiscore_t ) * MAX_SCORES, (const char *)scores );
    l_fptr->close();
  }

  /* All done. */
  return;
}


/*
 * get_entry - fetches the high score details for the given position.
 *
 * uint8_t - the position in the table
 */

const hiscore_t *HighScore::get_entry( uint8_t p_position )
{
  /* Sanity check the position requested. */
  if ( p_position >= MAX_SCORES )
  {
    return nullptr;
  }

  /* Then simply return the appropriate entry. */
  return &scores[p_position];
}


/* End of HighScore.cpp */
