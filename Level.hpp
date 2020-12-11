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

#define   BOARD_HEIGHT  15
#define   BOARD_WIDTH   10

class Level
{
private:
  uint8_t     level;
  uint8_t     bricks[BOARD_HEIGHT][BOARD_WIDTH];

  void        init( const uint8_t *, uint32_t );

public:
              Level( uint8_t );
  uint8_t     get_level( void );
  uint16_t    get_brick_count( void );
  uint8_t     get_brick( uint8_t, uint8_t );
  blit::Point get_brick_location( uint8_t, uint8_t );
};

#endif /* _LEVEL_HPP_ */

/* End of Level.hpp */
