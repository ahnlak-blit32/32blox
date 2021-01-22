/*
 * PowerUp.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The PowerUp object is a randomly-dropped power up thingie.
 */

#ifndef   _POWERUP_HPP_
#define   _POWERUP_HPP_

typedef enum
{
  POWERUP_SPEED,
  POWERUP_SLOW,
  POWERUP_STICKY,
  POWERUP_SHRINK,
  POWERUP_GROW,
  POWERUP_MULTI,
  POWERUP_MAX
} powerup_type_t;

class PowerUp
{
private:
  blit::Vec2      location;
  blit::Vec2      vector;
  powerup_type_t  powerup_type;
  blit::Point     get_render_location( void );

public:
                  PowerUp( blit::Point );
  blit::Rect      get_bounds( void );
  powerup_type_t  get_type( void );
  void            update( void );
  void            render( void );
  void            remove( void );
};

#endif /* _POWERUP_HPP_ */

/* End of PowerUp.hpp */
