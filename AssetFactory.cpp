/*
 * AssetFactory.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The AssetFactory is a singleton class to hold inflated assets, so that we
 * don't end up wasting memory on multiple copies of things like sprite sheets.
 */

/* System headers. */

#include <string.h>


/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"

#include "AssetFactory.hpp"
#include "assets_images.hpp"


/* Functions. */

/*
 * constructor - Initialises all the asset objects
 */

AssetFactory::AssetFactory( void )
{
  /* Load up the primary sprite sheet. */
  spritesheet_game = blit::Surface::load( a_img_game_sprites );

  /* And then any individual image assets. */
  surface_logo = blit::Surface::load( a_img_logo );
  surface_long_logo = blit::Surface::load( a_img_long_logo );

  /* Determine what our hardware target is. */
#ifdef TARGET_32BLIT_HW
  c_target = TARGET_32BLIT;
#else
  if ( strcmp( PICO_BOARD, "pimoroni_picosystem" ) == 0 )
  {
    c_target = TARGET_PICOSYSTEM;
  }
  else
  {
    c_target = TARGET_SDL;
  }
#endif

  /* All done. */
  return;
}


/*
 * get_instance - fetches the singleton instance of the AssetFactory
 */

AssetFactory &AssetFactory::get_instance( void )
{
  static AssetFactory myself;
  return myself;
}


/*
 * get_text - fetches the correct text to use for a given message, based both
 *            on the current language and whether we're on a physical Blit,
 *            or emulated on a desktop.
 */

const char *AssetFactory::get_text( str_message_t p_message )
{
  const char *l_text = "undefined";

  /* Choose between languages, and then messages. */
  switch( c_language )
  {
    case LANG_EN:        /* English */
      switch( p_message )
      {
        case STR_A_TO_START:
          if ( TARGET_32BLIT == c_target )
            l_text = "PRESS 'A' TO START";
          else if ( TARGET_PICOSYSTEM == c_target )
            l_text = "'A' TO START";
          else
            l_text = "PRESS 'Z' TO START";
          break;
        case STR_B_TO_LAUNCH:
          if ( TARGET_32BLIT == c_target )
            l_text = "PRESS 'B' TO LAUNCH";
          else if ( TARGET_PICOSYSTEM == c_target )
            l_text = "'B' TO LAUNCH";
          else
            l_text = "PRESS 'X' TO LAUNCH";
          break;
        case STR_B_TO_SAVE:
          if ( TARGET_32BLIT == c_target )
            l_text = "PRESS 'B' TO SAVE";
          else if ( TARGET_PICOSYSTEM == c_target )
            l_text = "'B' TO SAVE";
          else
            l_text = "PRESS 'X' TO SAVE";
          break;
        case STR_MENU_TO_EXIT:
          if ( TARGET_32BLIT == c_target )
            l_text = "PRESS <MENU> TO EXIT";
          else
            l_text = "PRESS '2' TO EXIT";
          break;

        case STR_LANG_EN:
          l_text = "English";
          break;
        case STR_NEW_HIGH_SCORE:
          l_text = "NEW HIGH SCORE!";
          break;
        case STR_LEFT_RIGHT_SELECT:
          l_text = "LEFT/RIGHT TO SELECT";
          break;
        case STR_UP_DOWN_CHANGE:
          l_text = "UP/DOWN TO CHANGE";
          break;
        case STR_LEVEL:
          l_text = "LEVEL";
          break;
        case STR_POWERUP_SPEED:
          l_text = "SPEED\nUP!";
          break;
        case STR_POWERUP_SLOW:
          l_text = "SLOW\nDOWN";
          break;
        case STR_POWERUP_STICKY:
          l_text = "STICKY\nBAT!";
          break;
        case STR_POWERUP_GROW:
          l_text = "GROW\nBAT!";
          break;
        case STR_POWERUP_SHRINK:
          l_text = "SHRINK\nBAT!";
          break;
        case STR_POWERUP_MULTI:
          l_text = "MULTI\nBALL";
          break;
        case STR_POWERUP_EXTRA:
          l_text = "EXTRA\nLIFE";
          break;
        case STR_GAME_OVER:
          l_text = "GAME\nOVER";
          break;
        case STR_BALL_LOST:
          l_text = "BALL\nLOST";
          break;
        case STR_SCORE:
          l_text = "SCORE";
          break;
        case STR_HISCORE:
          l_text = "HI";
          break;
        case STR_HIGH_SCORES:
          l_text = "HIGH SCORES";
          break;
        case STR_MENU_SOUND:
          l_text = "Sound";
          break;
        case STR_MENU_MUSIC:
          l_text = "Music";
          break;
        case STR_MENU_HAPTIC:
          l_text = "Haptic";
          break;
        case STR_MENU_ON:
          l_text = "  <ON>";
          break;
        case STR_MENU_OFF:
          l_text = " <OFF>";
          break;
        case STR_MENU_URL:
        l_text = "VISIT US AT https://blithub.co.uk";
          break;
      }
  }

  /* Return the match. */
  return l_text;
}


/*
 * get_language - returns the current language being used.
 */

str_lang_t AssetFactory::get_language( void )
{
  /* Just let them know what our language currently is. */
  return c_language;
}


/*
 * set_language - set the language to be used.
 */

void AssetFactory::set_language( str_lang_t p_language )
{
  /* Just remember the language to use. */
  c_language = p_language;

  /* All done. */
  return;
}


/*
 * get_platform - describes the platform we're playing on.
 *
 * This is kind of the wrong place for this, but the main area where platform
 * differences exist is in the strings we use.
 */

target_type_t AssetFactory::get_platform( void )
{
  return c_target;
}



/* End of AssetFactory.cpp */
