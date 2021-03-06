/*
 * GameState.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The GameState is the core of the game, active whenever the game is actually
 * in play.
 */

#ifndef   _GAMESTATE_HPP_
#define   _GAMESTATE_HPP_

#include <forward_list>
#include "AssetFactory.hpp"
#include "Ball.hpp"
#include "HighScore.hpp"
#include "Level.hpp"
#include "OutputManager.hpp"
#include "PowerUp.hpp"


typedef enum
{
  BAT_NORMAL,
  BAT_NARROW,
  BAT_WIDE,
  BAT_STICKY,
  BAT_MAX
} bat_type_t;

#define FREQ_BOUNDS 96
#define FREQ_BRICK  640


class GameState : public GameStateInterface
{
private:
  AssetFactory               &assets = AssetFactory::get_instance();
  OutputManager              &output = OutputManager::get_instance();
  HighScore                  *high_score;
  Level                      *level;
  uint8_t                     lives;
  blit::Pen                   font_pen;
  blit::Pen                   number_pen;
  blit::Tween                 font_tween;
  blit::Tween                 splash_tween;
  char                        splash_message[32];
  float                       bat_position;
  float                       bat_speed;
  uint16_t                    bat_height;
  bat_type_t                  bat_type;
  uint16_t                    score;
  uint16_t                    hiscore;
  std::forward_list<Ball*>    balls;
  std::forward_list<PowerUp*> powerups;
  const uint8_t               bat_width[BAT_MAX] = { 24, 16, 32, 24 };

  void                        init( void );
  void                        move_bat( float );
  blit::Rect                  brick_to_screen( uint8_t, uint8_t );
  blit::Point                 screen_to_brick( blit::Point );
  blit::Rect                  bat_bounds( void );
  void                        spawn_ball( bool );
  void                        load_level( uint8_t );

public:
                              GameState( void );
  void                        init( GameStateInterface * );
  void                        fini( GameStateInterface * );
  uint16_t                    get_score( void );
  gamestate_t                 update( uint32_t );
  void                        render( uint32_t );
};

#endif /* _GAMESTATE_HPP_ */

/* End of GameState.hpp */
