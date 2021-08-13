/*
 * Level.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Level object contains the details of a game level.
 */

#ifndef   _LEVEL_HPP_
#define   _LEVEL_HPP_

#include "AssetFactory.hpp"

#define   MAX_BOARD_HEIGHT  15
#define   MAX_BOARD_WIDTH   10

#define   LEVEL_MAX         10

class Level
{
private:
  uint8_t     level;
  uint8_t     bricks[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH];
  uint8_t     width = MAX_BOARD_WIDTH;
  uint8_t     height = MAX_BOARD_HEIGHT;
  uint8_t     margin = 0;

  void        init( const uint8_t *, uint32_t );

public:
              Level( uint8_t, target_type_t );
  uint8_t     get_level( void );
  uint8_t     get_width( void );
  uint8_t     get_height( void );
  uint8_t     get_margin( void );
  uint16_t    get_brick_count( void );
  uint8_t     get_brick( uint8_t, uint8_t );
  uint8_t     get_brick( blit::Point );
  uint8_t     hit_brick( blit::Point );
  float       get_ball_speed( void );
};

#endif /* _LEVEL_HPP_ */

/* End of Level.hpp */
