/*
 * HighScore.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The HighScore class manages the high score table, which will be saved onto
 * the SD card (if we can)
 */

#ifndef   _HIGHSCORE_HPP_
#define   _HIGHSCORE_HPP_

#define MAX_SCORES 10

typedef struct
{
  char      name[3];
  uint16_t  score;
} hiscore_t;

class HighScore
{
private:
  hiscore_t         scores[MAX_SCORES];
public:
                    HighScore( void );
  uint8_t           rank( uint16_t );
  void              save( uint16_t, const char * );
  const hiscore_t  *get_entry( uint8_t );
};


#endif /* _HIGHSCORE_HPP_ */

/* End of HighScore.hpp */
