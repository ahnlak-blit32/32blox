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
  /* File stuff now handled by the API, we just ask it nicely to load. */
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
  /* We can just let the API handle this now. */
  if ( !blit::read_save( scores, SAVE_SLOT_HISCORE ) )
  {
    /* Failure means the file was empty. */
    for( uint8_t i = 0; i < MAX_SCORES; i++ )
    {
      strcpy( scores[i].name, "ahnlak" );
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
    strcpy( scores[i].name, scores[i-1].name );
    scores[i].score = scores[i-1].score;
  }

  /* Fill in the new data. */
  strcpy( scores[l_position].name, p_name );
  scores[l_position].score = p_score;

  /* And lastly, ask the API to save all this. */
  blit::write_save( scores, SAVE_SLOT_HISCORE );

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
