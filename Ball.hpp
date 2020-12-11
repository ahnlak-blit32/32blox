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
  BALL_MAX
} ball_type_t;

class Ball
{
private:
  blit::Vec2    location;
  blit::Vec2    vector;
  ball_type_t   ball_type;
  blit::Rect    bat_position;
  const uint8_t ball_size[BALL_MAX] = { 8, 6 };

public:
                Ball( blit::Vec2, ball_type_t = BALL_NORMAL );
  blit::Point   get_render_location( void );
  blit::Rect    get_bounds( void );
  ball_type_t   get_type( void );
  void          update( void );
  void          launch( void );
  void          bounce( bool );
  void          offset( blit::Vec2 );
  void          move_bat( blit::Rect, float );

  bool          stuck;
  bool          sticky;
};

#endif /* _BALL_HPP_ */

/* End of Ball.hpp */
