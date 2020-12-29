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
  spritesheet_game = blit::SpriteSheet::load( a_game_sprites_img );

  /* And then any individual image assets. */
  surface_logo = blit::Surface::load( a_logo_img );
  surface_long_logo = blit::Surface::load( a_long_logo_img );

  /* And the fonts. */
  //number_font = blit::Font( a_number_font );
  //message_font = blit::Font( a_message_font );

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


/* End of AssetFactory.cpp */
