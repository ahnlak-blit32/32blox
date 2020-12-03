/*
 * Ball.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Ball object is a simple encapsulation of, well, a ball.
 */

#ifndef   _BALL_HPP_
#define   _BALL_HPP_

typedef enum
{
  BALL_NORMAL,
  BALL_SMALL,
  BALL_LARGE,
  BALL_MAX
} ball_type_t;

class Ball
{
private:
  blit::Point location;
  blit::Vec2  vector;
  ball_type_t ball_type;

public:
              Ball( blit::Point, ball_type_t = BALL_NORMAL );
  blit::Point get_location( void );
  void        update( void );
  void        bounce( bool );
};

#endif /* _BALL_HPP_ */

/* End of Ball.hpp */
