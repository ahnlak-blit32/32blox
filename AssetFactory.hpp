/*
 * AssetFactory.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The AssetFactory is a singleton class to hold inflated assets, so that we
 * don't end up wasting memory on multiple copies of things like sprite sheets.
 */

#ifndef   _ASSETFACTORY_HPP_
#define   _ASSETFACTORY_HPP_

#include "assets_fonts.hpp"

/* All text is now output via the Asset Factory, so we can switch for both */
/* platform and language options.                                          */
typedef enum
{
  LANG_EN
} str_lang_t;

typedef enum
{
  STR_LANG_EN = LANG_EN,
  STR_A_TO_START,
  STR_B_TO_LAUNCH,
  STR_B_TO_SAVE,
  STR_MENU_TO_EXIT,
  STR_NEW_HIGH_SCORE,
  STR_LEFT_RIGHT_SELECT,
  STR_UP_DOWN_CHANGE,
  STR_LEVEL,
  STR_POWERUP_SPEED,
  STR_POWERUP_SLOW,
  STR_POWERUP_STICKY,
  STR_POWERUP_GROW,
  STR_POWERUP_SHRINK,
  STR_POWERUP_MULTI,
  STR_POWERUP_EXTRA,
  STR_GAME_OVER,
  STR_BALL_LOST,
  STR_SCORE,
  STR_HISCORE,
  STR_HIGH_SCORES,
  STR_MENU_SOUND,
  STR_MENU_MUSIC,
  STR_MENU_HAPTIC,
  STR_MENU_ON,
  STR_MENU_OFF,
  STR_MENU_URL
} str_message_t;

typedef enum
{
  TARGET_32BLIT,
  TARGET_PICOSYSTEM,
  TARGET_SDL
} target_type_t;


class AssetFactory
{
private:

#ifndef   PICO_BOARD
#define   PICO_BOARD  "none"
#endif /* PICO_BOARD */

  target_type_t         c_target;
  str_lang_t            c_language = LANG_EN;

                        AssetFactory( void );
public:
  static AssetFactory  &get_instance( void );

  blit::Surface        *surface_logo;
  blit::Surface        *surface_long_logo;
  blit::Surface        *spritesheet_game;

  const blit::Font      number_font = blit::Font( a_font_number );
  const blit::Font      message_font = blit::Font( a_font_message );
  const blit::Font      splash_font = blit::Font( a_font_splash );
  const char *          get_text( str_message_t );
  str_lang_t            get_language( void );
  void                  set_language( str_lang_t );
  target_type_t         get_platform( void );
};


#endif /* _ASSETFACTORY_HPP_ */

/* End of AssetFactory.hpp */
