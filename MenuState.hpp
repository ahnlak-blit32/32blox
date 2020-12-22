/*
 * MenuState.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The MenuState handles the in-game menu, which opens an out-of-state menu
 * to allow for global things, like quitting and adjust sound options.
 */

#ifndef   _MENUSTATE_HPP_
#define   _MENUSTATE_HPP_

#include "AssetFactory.hpp"
#include "OutputManager.hpp"

#define MENUSTATE_GRADIENT_HEIGHT 160

class MenuState : public GameStateInterface
{
private:
  AssetFactory   &assets = AssetFactory::get_instance();
  OutputManager  &output = OutputManager::get_instance();
  blit::Pen       font_pen;
  blit::Pen       plain_pen;
  blit::Tween     font_tween;
  blit::Pen       gradient_pen[MENUSTATE_GRADIENT_HEIGHT];
  uint8_t         gradient_offset;
  blit::Size      menu_size;
  uint8_t         cursor;

public:
                  MenuState( void );
  void            init( GameStateInterface * );
  void            fini( GameStateInterface * );
  gamestate_t     update( uint32_t );
  void            render( uint32_t );
};

#endif /* _MENUSTATE_HPP_ */

/* End of MenuState.hpp */
