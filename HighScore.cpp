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
  return;
}


/*
 * rank_score - determines where in the table the provided score should go
 * 
 * uint16_t - the score being checked
 */

uint8_t HighScore::rank( uint16_t p_score )
{
  return 0;
}


/* End of HighScore.cpp */
