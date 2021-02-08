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
  float         speed;
  ball_type_t   ball_type;
  blit::Rect    bat_position;
  const uint8_t ball_size[BALL_MAX] = { 8, 6 };
  float         compute_bat_angle( void );
  blit::Point   get_render_location( void );

public:
                Ball( blit::Point, float = 1.5, ball_type_t = BALL_NORMAL );
  blit::Rect    get_bounds( void );
  ball_type_t   get_type( void );
  bool          moving_up( void );
  bool          moving_left( void );
  void          update( void );
  void          render( void );
  void          launch( void );
  void          randomise( void );
  void          bounce( bool );
  bool          bat_bounce( uint16_t, bool );
  void          offset( blit::Vec2 );
  void          move_bat( blit::Rect, float, bool );

  bool          stuck;
};

#endif /* _BALL_HPP_ */

/* End of Ball.hpp */
