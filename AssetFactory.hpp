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

/* Key prompts depend on whether we're on the actual Blit or not. */
#ifdef    TARGET_32BLIT_HW
#define   STR_B_TO_LAUNCH     "Press 'B' To Launch"
#define   STR_B_TO_SAVE       "PRESS 'B' TO SAVE"
#define   STR_A_TO_START      "PRESS 'A' TO START"
#define   STR_MENU_TO_RETURN  "PRESS <MENU> TO RETURN"
#else  /* TARGET_32BLIT_HW */
#define   STR_B_TO_LAUNCH     "Press 'X' To Launch"
#define   STR_B_TO_SAVE       "PRESS 'X' TO SAVE"
#define   STR_A_TO_START      "PRESS 'Z' TO START"
#define   STR_MENU_TO_RETURN  "PRESS '2' TO RETURN"
#endif /* TARGET_32BLIT_HW */

class AssetFactory
{
private:
                        AssetFactory( void );
public:
  static AssetFactory  &get_instance( void );

  blit::Surface        *surface_logo;
  blit::Surface        *surface_long_logo;
  blit::Surface        *spritesheet_game;
  const blit::Font      number_font = blit::Font( a_font_number );
  const blit::Font      message_font = blit::Font( a_font_message );
  const blit::Font      splash_font = blit::Font( a_font_splash );
};


#endif /* _ASSETFACTORY_HPP_ */

/* End of AssetFactory.hpp */
