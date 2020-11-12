/*
 * 32blox.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * This file declares common structures and constants across the project.
 */

#ifndef   _32BLOX_HPP_
#define   _32BLOX_HPP_

/* Constants. */


/* Enums. */

typedef enum 
{
  STATE_SPLASH,
  STATE_GAME,
  STATE_DEATH,
  STATE_HISCORE,
  STATE_MAX
} gamestate_t;


/* Interfaces. */

class GameStateInterface
{
public:
  virtual gamestate_t update( uint32_t ) = 0;
  virtual void        render( uint32_t ) = 0;
};


#endif /* _32BLOX_HPP_ */

/* End of 32blox.hpp */
