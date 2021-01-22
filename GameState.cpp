/*
 * GameState.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * This state contains the core gameplay logic; by definition, it is by far
 * the largest and most complex state.
 */

/* System headers. */

#include <iterator>


/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "assets_images.hpp"

#include "GameState.hpp"
#include "HighScore.hpp"
#include "Level.hpp"
#include "PowerUp.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

GameState::GameState( void )
{
  /* The bat height will always be the same, but is bound by the screen size */
  /* so can't quite be hard coded.                                           */
  bat_height = blit::screen.bounds.h - 10;

  /* The font pen will be simpler. */
  font_pen = blit::Pen( 255, 255, 0 );
  number_pen = blit::Pen( 255, 255, 0 );
  font_tween.init( blit::tween_sine, 255.0f, 100.0f, 500 );

  /* And we'll need access to the high score table. */
  high_score = new HighScore();

  /* Prepare the tween for splashing messages. */
  splash_tween.init( blit::tween_linear, 255.0f, 0.0f, 1750, 1 );

  /* All done. */
  return;
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void GameState::init( GameStateInterface *p_previous )
{
  /* Select the game spritesheet into the screen. */
  blit::screen.sprites = assets.spritesheet_game;

  /* Fetch the current top score. */
  const hiscore_t *l_top_entry = high_score->get_entry( 0 );
  if ( l_top_entry == nullptr )
  {
    hiscore = 0;
  }
  else
  {
    hiscore = l_top_entry->score;
  }

  /* Load the first level. */
  load_level( 1 );

  /* Set the tweens running. */
  font_tween.start();

  /* Reset the lives count and score. */
  lives = 3;
  score = 0;

  /* All done. */
  return;
}


/*
 * fini - called whenever the game engine turns off this state.
 * 
 * GameStateInterface * - the state we were switching to
 */

void GameState::fini( GameStateInterface *p_next )
{
  /* Turn oiff the tweens. */
  font_tween.stop();
  splash_tween.stop();

  /* All done. */
  return;
}

/*
 * move_bat - updates the bat position, taking into account the bat size and
 *            the edges of the screen.
 * float - the movement amount wanted; this may get clipped if the edge is hit
 */
void GameState::move_bat( float p_movement )
{
  /* First up, let's apply the full movement. */
  float l_last_pos = bat_position;
  bat_position += p_movement;

  /* And then clamp it, top and bottom. */
  if ( bat_position < ( bat_width[bat_type] / 2 ) )
  {
    bat_position = bat_width[bat_type] / 2;
  }
  if ( bat_position > blit::screen.bounds.w - ( bat_width[bat_type] / 2 ) )
  {
    bat_position = blit::screen.bounds.w - ( bat_width[bat_type] / 2 );
  }

  /* Now, check to see if any of our balls are sticky. */
  for ( auto l_ball : balls )
  {
    /* Then we can just ask the ball to move itself, if it wants to. */
    l_ball->move_bat( blit::Rect( bat_position - ( bat_width[bat_type] / 2 ), 
                                  bat_height, bat_width[bat_type], 1 ), 
                      bat_position - l_last_pos );
  }

  /* All done! */
  return;
}


/*
 * brick_to_screen - returns a Rect of the position of the designated brick,
 *                   to make rendering and collision detection nice and consistent.
 * uint8_t * 2 - the row and column in the brick being queried.
 */

blit::Rect GameState::brick_to_screen( uint8_t p_row, uint8_t p_column )
{
  return blit::Rect( p_column * 32, p_row * 16 + 10, 32, 16 );
}


/*
 * screen_to_brick - returns the x/y co-ordinates of the brick at the given
 *                   screen location.
 * blit::Point - the screen location being queried.
 */

blit::Point GameState::screen_to_brick( blit::Point p_location )
{
  return blit::Point( p_location.x / 32, ( p_location.y - 10 ) / 16 );
}


/*
 * spawn_ball - creates a new ball on the bat; either at the start of the level,
 *              or after a ball is lost 
 */

void GameState::spawn_ball( void )
{
  /* The ball starts in the middle of the bat. */
  blit::Vec2 l_ballpos( bat_position, bat_height - 3 );

  /* But then we offset it a little one side or the other... */
  switch( blit::random() % 4 )
  {
    case 0:
      l_ballpos.x -= 4;
      break;
    case 1:
      l_ballpos.x -= 2;
      break;
    case 2:
      l_ballpos.x += 2;
      break;
    case 3:
      l_ballpos.x += 4;
      break;
  }

  /* create a new ball. */
  Ball *l_ball = new Ball( l_ballpos );

  /* Stick it to the bat. */
  l_ball->stuck = true;
  l_ball->move_bat( blit::Rect( bat_position - ( bat_width[bat_type] / 2 ), 
                                bat_height, bat_width[bat_type], 1 ), 0.0f );

  /* And add it to the internal ball list. */
  balls.push_front( l_ball );

  /* All done. */
  return;
}


/*
 * load_level - loads the required level data, resets the balls, the bats and
 *              everything else for the start of a whole new level
 */

void GameState::load_level( uint8_t p_level )
{
  /* Load up the level data. */
  level = new Level( p_level );

  /* Centre the bat, and set it to a default type. */
  bat_position = blit::screen.bounds.w / 2;
  bat_speed = 1.0f;
  bat_type = BAT_NORMAL;

  /* Clear out the list of balls, and spawn one on the bat. */
  balls.clear();
  spawn_ball();

  /* Clear out the list of powerups, too. */
  powerups.clear();

  /* Let the user know what level they're on. */
  snprintf( splash_message, 30, "Level\n%02d", level->get_level() );
  splash_tween.start();

  /* All done. */
  return;
}


/*
 * get_score - exposes the current score for the current game
 */

uint16_t GameState::get_score( void )
{
  return score;
}


/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t GameState::update( uint32_t p_time )
{
  /* Calculate any bat movement that's required. */
  float l_movement = 0.0f;

  /* Handle the joystick, which is slightly more gradiated. */
  if ( blit::joystick.x < -0.66f )
  {
    l_movement = bat_speed * -1;
  }
  else if ( blit::joystick.x > 0.66f )
  {
    l_movement = bat_speed;
  }
  else
  {
    l_movement = bat_speed * 1.5f * blit::joystick.x;
  }

  /* But if the player has moved the dpad, then use that instead. */
  if ( blit::buttons.state & blit::Button::DPAD_LEFT )
  {
    l_movement = bat_speed * -1;
  }
  if ( blit::buttons.state & blit::Button::DPAD_RIGHT )
  {
    l_movement = bat_speed;
  }

  /* And lastly, apply that movement. */
  if ( l_movement != 0.0f )
  {
    move_bat( l_movement );
  }

  /* Next, if the user presses B and there's a ball on the bat, fire it. */
  /* And yes, if we've collected multiple balls, we launch them all!     */
  if ( blit::buttons.pressed & blit::Button::B && lives > 0 )
  {
    /* Work through all our balls then. */
    for ( auto l_ball : balls )
    {
      /* Only interested in one that's stuck. */
      if ( l_ball->stuck )
      {
        /* Balls know how to launch themselves, happily. */
        l_ball->launch();
      }
    }
  }

  /* Next up, we work our way through all the balls we have, and update their */
  /* positions. We'll deal with any collisions in a little while...           */
  for ( auto l_ball : balls )
  {
    bool l_bounce_vertical = false;
    bool l_bounce_horizontal = false;
    bool l_brick_destroyed = false;
    blit::Point l_brick_location;

    /* Fetch the bounds of the ball's current position. */
    blit::Rect l_old_bounds = l_ball->get_bounds();

    /* Update the balls position. */
    l_ball->update();

    /* And fetch the bounds of the ball in it's new location. */
    blit::Rect l_new_bounds = l_ball->get_bounds();

    /* Collision detect on the top of the screen. */
    if ( l_new_bounds.y <= 0 )
    {
      output.trigger_haptic( 0.25f, 50 );
      output.play_effect_bounce( FREQ_BOUNDS );
      l_ball->bounce( false );
    }

    /* And the edges of the screen, which gives some points too! */
    if ( l_new_bounds.x <= 0 || ( l_new_bounds.x + l_new_bounds.w ) >= blit::screen.bounds.w )
    {
      score++;
      output.trigger_haptic( 0.25f, 50 );
      output.play_effect_bounce( FREQ_BOUNDS );
      l_ball->bounce( true );
    }

    /* Now, check to see if our bounds have crossed into a new brick area. */
    if ( l_ball->moving_up() )
    {
      /* Check if the brick row has changed for the leading (top) edge. */
      blit::Point l_old_tl = screen_to_brick( l_old_bounds.tl() );
      blit::Point l_new_tl = screen_to_brick( l_new_bounds.tl() );
      blit::Point l_new_tr = screen_to_brick( l_new_bounds.tr() );
      if ( l_old_tl.y != l_new_tl.y )
      {
        /* We've crossed a line. See if it's occupied! */
        if ( level->get_brick( l_new_tl ) > 0 )
        {
          /* Bounce and increment the score. */
          score += level->hit_brick( l_new_tl );
          l_bounce_vertical = true;

          /* Check to see if the brick was destroyed. */
          if ( level->get_brick( l_new_tl ) == 0 )
          {
            l_brick_destroyed = true;
            l_brick_location = l_new_tl;
          }
        }

        /* Also consider the top right corner, in case we bounced on a boundary. */
        if ( ( l_new_tl.x != l_new_tr.x ) || ( l_new_tl.y != l_new_tr.y ) )
        {
          if ( level->get_brick( l_new_tr ) > 0 )
          {
            /* Bounce, and increment the score. */
            score += level->hit_brick( l_new_tr );
            l_bounce_vertical = true;

            /* Check to see if the brick was destroyed. */
            if ( level->get_brick( l_new_tr ) == 0 )
            {
              l_brick_destroyed = true;
              l_brick_location = l_new_tr;
            }
          }
        }
      }
    }
    else
    {
      /* Moving down, check if the brick row has changed on the bottom edge. */
      blit::Point l_old_bl = screen_to_brick( l_old_bounds.bl() );
      blit::Point l_new_bl = screen_to_brick( l_new_bounds.bl() );
      blit::Point l_new_br = screen_to_brick( l_new_bounds.br() );
      if ( l_old_bl.y != l_new_bl.y )
      {
        /* We've crossed a line. See if it's occupied! */
        if ( level->get_brick( l_new_bl ) > 0 )
        {
          /* Bounce, and increment the score. */
          score += level->hit_brick( l_new_bl );
          l_bounce_vertical = true;

          /* Check to see if the brick was destroyed. */
          if ( level->get_brick( l_new_bl ) == 0 )
          {
            l_brick_destroyed = true;
            l_brick_location = l_new_bl;
          }
        }

        /* Also consider the top right corner, in case we bounced on a boundary. */
        if ( ( l_new_bl.x != l_new_br.x ) || ( l_new_bl.y != l_new_br.y ) )
        {
          if ( level->get_brick( l_new_br ) > 0 )
          {
            /* Bounce, and increment the score. */
            score += level->hit_brick( l_new_br );
            l_bounce_vertical = true;

            /* Check to see if the brick was destroyed. */
            if ( level->get_brick( l_new_br ) == 0 )
            {
              l_brick_destroyed = true;
              l_brick_location = l_new_br;
            }
          }
        }
      }
    }
    if ( l_ball->moving_left() )
    {
      /* Check if the brick row has changed for the leading (left) edge. */
      blit::Point l_old_tl = screen_to_brick( l_old_bounds.tl() );
      blit::Point l_new_tl = screen_to_brick( l_new_bounds.tl() );
      blit::Point l_new_bl = screen_to_brick( l_new_bounds.bl() );
      if ( l_old_tl.x != l_new_tl.x )
      {
        /* We've crossed a line. See if it's occupied! */
        if ( level->get_brick( l_new_tl ) > 0 )
        {
          /* Bounce, and increment the score. */
          score += level->hit_brick( l_new_tl );
          l_bounce_horizontal = true;

          /* Check to see if the brick was destroyed. */
          if ( level->get_brick( l_new_tl ) == 0 )
          {
            l_brick_destroyed = true;
            l_brick_location = l_new_tl;
          }
        }

        /* Also consider the top right corner, in case we bounced on a boundary. */
        if ( ( l_new_tl.x != l_new_bl.x ) || ( l_new_tl.y != l_new_bl.y ) )
        {
          if ( level->get_brick( l_new_bl ) > 0 )
          {
            /* Bounce, and increment the score. */
            score += level->hit_brick( l_new_bl );
            l_bounce_horizontal = true;

            /* Check to see if the brick was destroyed. */
            if ( level->get_brick( l_new_bl ) == 0 )
            {
              l_brick_destroyed = true;
              l_brick_location = l_new_bl;
            }
          }
        }
      }
    }
    else
    {
      /* Moving down, check if the brick row has changed on the bottom edge. */
      blit::Point l_old_tr = screen_to_brick( l_old_bounds.tr() );
      blit::Point l_new_tr = screen_to_brick( l_new_bounds.tr() );
      blit::Point l_new_br = screen_to_brick( l_new_bounds.br() );
      if ( l_old_tr.x != l_new_tr.x )
      {
        /* We've crossed a line. See if it's occupied! */
        if ( level->get_brick( l_new_tr ) > 0 )
        {
          /* Bounce, and increment the score. */
          score += level->hit_brick( l_new_tr );
          l_bounce_horizontal = true;

          /* Check to see if the brick was destroyed. */
          if ( level->get_brick( l_new_tr ) == 0 )
          {
            l_brick_destroyed = true;
            l_brick_location = l_new_tr;
          }
        }

        /* Also consider the top right corner, in case we bounced on a boundary. */
        if ( ( l_new_tr.x != l_new_br.x ) || ( l_new_tr.y != l_new_br.y ) )
        {
          if ( level->get_brick( l_new_br ) > 0 )
          {
            /* Bounce, and increment the score. */
            score += level->hit_brick( l_new_br );
            l_bounce_horizontal = true;

            /* Check to see if the brick was destroyed. */
            if ( level->get_brick( l_new_br ) == 0 )
            {
              l_brick_destroyed = true;
              l_brick_location = l_new_br;
            }
          }
        }
      }
    }

    /* Apply the required ball bounces. */
    if ( l_bounce_vertical )
    {
      output.trigger_haptic( 0.25f, 50 );
      output.play_effect_bounce( FREQ_BRICK );
      l_ball->bounce( false );
    }
    if ( l_bounce_horizontal )
    {
      output.trigger_haptic( 0.25f, 50 );
      output.play_effect_bounce( FREQ_BRICK );
      l_ball->bounce( true );
    }

    /* If a brick was fully destroyed, maybe spawn a powerup. */
    if ( l_brick_destroyed )
    {
      /* Work out the screen location of the brick. */
      blit::Rect l_brick = brick_to_screen( l_brick_location.y, l_brick_location.x );
      powerups.push_front( new PowerUp( blit::Point( l_brick.x + l_brick.w / 2, l_brick.y + l_brick.h / 2 ) ) );
    }

    /* And lastly, the bat itself. */
    if ( ( l_new_bounds.x <= ( bat_position + bat_width[bat_type] / 2 ) ) &&
         ( ( l_new_bounds.x + l_new_bounds.w ) >= ( bat_position - bat_width[bat_type] / 2 ) ) &&
         ( ( l_new_bounds.y + l_new_bounds.h ) >= bat_height ) )
    {
      if ( l_ball->bat_bounce( bat_height ) )
      {
        output.trigger_haptic( 0.25f, 50 );
        output.play_effect_bounce( FREQ_BOUNDS );
      }
    }
  }

  /* Clean up any balls that drop off the bottom of the screen. */
  balls.remove_if( []( auto l_ball) { return l_ball->get_bounds().y > blit::screen.bounds.h; } );

  /* Update the powerup positions. */
  for ( auto l_powerup : powerups )
  {
    l_powerup->update();
  }  

  /* If there are no more balls in play, then we lose a life. */
  if ( std::distance( balls.begin(), balls.end() ) == 0 )
  {
    lives--;
    spawn_ball();
    if ( lives == 0 )
    {
      snprintf( splash_message, 30, "Game\nOver" );
    }
    else
    {
      snprintf( splash_message, 30, "Ball\nLost" );
    }
    splash_tween.start();
  }

  /* If after all that we have no more lives, it's game over. */
  if ( lives == 0 && splash_tween.is_finished() ) 
  {
    return STATE_DEATH;
  }

  /* Lastly, check the level - if we've cleared all the clearable bricks, */
  /* then it's time to move onto the next one!                            */
  if ( level->get_brick_count() == 0 )
  {
    load_level( level->get_level() + 1 );
  }

  /* The font pen we use will pulse more subtlely. */
  font_pen.g = font_tween.value;

  /* All done, remain in our current state */
  return STATE_GAME;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void GameState::render( uint32_t p_time )
{
  uint8_t l_brick;
  bool    l_stuck_ball = false;
  char    l_buffer[32];

  /* Clear the screen down. */
  blit::screen.clear();

  /* Draw a smooth gradient backdrop. */
  for ( uint16_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = blit::Pen( 10, 10, ( ( blit::screen.bounds.h - i ) / 2 ) );
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }

  /* Draw in the score line. */
  snprintf( l_buffer, 30, "SCORE: %05d", score );
  blit::screen.pen = number_pen;
  blit::screen.text(
    l_buffer,
    assets.number_font,
    blit::Point( 1, 1 ),
    true,
    blit::TextAlign::top_left
  );
  snprintf( l_buffer, 30, "HI: %05d", hiscore );
  blit::screen.text(
    l_buffer,
    assets.number_font,
    blit::Point( blit::screen.bounds.w - 1, 1 ),
    true,
    blit::TextAlign::top_right
  );

  /* And a display of the remaining lives. */
  blit::screen.sprite(
    blit::Rect( 0, SPRITE_ROW_BAT, 1, 1 ),
    blit::Point( blit::screen.bounds.w / 2 - 24, 1 )
  );
  blit::screen.sprite(
    blit::Rect( 2, SPRITE_ROW_BAT, 1, 1 ),
    blit::Point( blit::screen.bounds.w / 2 - 16, 1 )
  );
  snprintf( l_buffer, 30, "x%d", lives );
  blit::screen.text( 
    l_buffer, 
    assets.number_font, 
    blit::Point( blit::screen.bounds.w / 2 - 4, 1 ),
    true, blit::TextAlign::top_left
  );

  /* Now we work through the level one brick at a time... */
  for ( uint8_t l_row = 0; l_row < BOARD_HEIGHT; l_row++ )
  {
    for ( uint8_t l_column = 0; l_column < BOARD_WIDTH; l_column++ )
    {
      /* Fetch the brick for this location. */
      l_brick = level->get_brick( l_row, l_column );

      /* Move on if it's empty. */
      if ( l_brick == 0 )
      {
        continue;
      }

      /* Then draw the appropriate brick from the spritesheet. */
      blit::screen.sprite( 
        blit::Rect( ( l_brick - 1 ) * 4, SPRITE_ROW_BRICK, 4, 2 ),
        blit::Point( l_column * 32, l_row * 16 + 10 )
      );
    }
  }

  /* Add in the bat; the position is the centre location. */
  switch( bat_type )
  {
    case BAT_NORMAL:  /* Simple bat, two sprites wide. */
      blit::screen.sprite(
        blit::Rect( 0, SPRITE_ROW_BAT, 3, 1 ),
        blit::Point( bat_position - ( bat_width[bat_type] / 2 ), bat_height )
      );
      break;
  }

  /* Render the powerups, too - these go behind (before) the balls. */
  for ( auto l_powerup : powerups )
  {
    l_powerup->render();
  }

  /* Balls next; we could have a number of them, in a handy container. */
  for ( auto l_ball : balls )
  {
    /* Render the ball. */
    l_ball->render();

    /* And remember if it's stuck to the bat... */
    if ( l_ball->stuck )
    {
      l_stuck_ball = true;
    }
  }

  /* So, if we have a sticky ball, explain what the user needs to do... */
  if ( l_stuck_ball && lives > 0 )
  {
    blit::screen.pen = font_pen;
    blit::screen.text(
      "Press 'B' To Launch",
      assets.message_font,
      blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 45 ),
      true,
      blit::TextAlign::center_center
    );
  }

  /* If there's a splash tween running, we have a message to display. */
  if ( splash_tween.is_running() )
  {
    blit::screen.pen = font_pen;
    blit::screen.pen.a = splash_tween.value;
    blit::screen.text(
      splash_message,
      assets.splash_font,
      blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h / 2 ),
      false,
      blit::TextAlign::center_center
    );
  }

  /* All done. */
  return;
}

/* End of GameState.cpp */
