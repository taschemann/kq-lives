/*
   KQ is Copyright (C) 2002 by Josh Bolduc

   This file is part of KQ... a freeware RPG.

   KQ is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   KQ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with KQ; see the file COPYING.  If not, write to
   the Free Software Foundation,
       675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*! \file
 * \brief Entity functions
 *
 * \author JB
 * \date ??????
 */

#include <stdio.h>
#include <string.h>

#include "kq.h"
#include "combat.h"
#include "entity.h"
#include "intrface.h"
#include "itemdefs.h"
#include "menu.h"
#include "setup.h"



/*  internal functions  */
static void chase (int);
static int entity_near (int, int, int);
static void entscript (int);
static void follow (int tx, int ty);
static void getcommand (int);
static int move (int, int, int);
static int obstruction (int, int, int, int, int);
static void parsems (int);
static void player_move (void);
static void process_entity (int);
static void speed_adjust (int);
static void target (int);
static void wander (int);



/*! \brief Chase player
 *
 * Chase after the main player #0, if he/she is near.
 * Speed up until at maximum. If the player goes out
 * of range, wander for a bit.
 *
 * \param   target_entity Index of entity
 */
static void chase (int target_entity)
{
   int emoved = 0;
   if (g_ent[target_entity].chasing == 0) {
      if (entity_near (target_entity, 0, 3) == 1
          && rand () % 100 <= g_ent[target_entity].extra) {
         g_ent[target_entity].chasing = 1;
         if (g_ent[target_entity].speed < 7)
            g_ent[target_entity].speed++;
         g_ent[target_entity].delay = 0;
      } else
         wander (target_entity);
   }
   if (g_ent[target_entity].chasing == 1) {
      if (entity_near (target_entity, 0, 4) == 1) {
         if (g_ent[0].tilex > g_ent[target_entity].tilex)
            emoved = move (target_entity, 1, 0);
         if (g_ent[0].tilex < g_ent[target_entity].tilex && !emoved)
            emoved = move (target_entity, -1, 0);
         if (g_ent[0].tiley > g_ent[target_entity].tiley && !emoved)
            emoved = move (target_entity, 0, 1);
         if (g_ent[0].tiley < g_ent[target_entity].tiley && !emoved)
            emoved = move (target_entity, 0, -1);
         if (!emoved)
            wander (target_entity);
      } else {
         g_ent[target_entity].chasing = 0;
         if (g_ent[target_entity].speed > 1)
            g_ent[target_entity].speed--;
         g_ent[target_entity].delay = 25 + rand () % 25;
         wander (target_entity);
      }
   }
}



/*! \brief Count active entities
 *
 * Force calculation of the 'noe' variable.
 */
void count_entities (void)
{
   int p;

   noe = 0;
   for (p = 0; p < MAX_ENT; p++)
      if (g_ent[p].active == 1)
         noe = p + 1;
}



/*! \brief Check proximity
 *
 * Check to see if the target is within "rad" squares.
 * Test area is a square box rather than a circle
 * target entity needs to be within the view area
 * to be visible
 * (PH) this implementation is really odd :?
 *
 * \param   eno Entity under consideration
 * \param   tgt Entity to test
 * \param   rad Radius to test within
 * \returns 1 if near, 0 otherwise
 */
static int entity_near (int eno, int tgt, int rad)
{
   int ax, ay, ex, ey, b;

   b = 0 - rad;
   ex = g_ent[eno].tilex;
   ey = g_ent[eno].tiley;
   for (ay = b; ay <= rad; ay++)
      for (ax = b; ax <= rad; ax++)
         if (ex + ax >= view_x1 && ax + ax <= view_x2 && ey + ay >= view_y1
             && ey + ay <= view_y2)
            if (ex + ax == g_ent[tgt].tilex && ey + ay == g_ent[tgt].tiley)
               return 1;
   return 0;
}



/*! \brief Check entites at location
 *
 * Check for any entities in the specified co-ordinates.
 * Runs combat routines if a character and an enemy meet,
 * and de-activate the enemy if it was defeated.
 *
 * \sa combat_check()
 * \param   ox x-coord to check
 * \param   oy y-coord to check
 * \param   who Id of entity doing the checking
 * \returns index of entity found+1 or 0 if none found
 */
int entityat (int ox, int oy, int who)
{
   int i;

   for (i = 0; i < MAX_ENT; i++) {
      if (g_ent[i].active && ox == g_ent[i].tilex && oy == g_ent[i].tiley) {
         if (who >= PSIZE) {
            if (g_ent[who].eid == ID_ENEMY && i < PSIZE) {
               if (combat(0) == 1)
                  g_ent[who].active = 0;
               return 0;
            }
            return i + 1;
         } else {
            if (g_ent[i].eid == ID_ENEMY) {
               if (combat(0) == 1)
                  g_ent[i].active = 0;
               return 0;
            }
            if (i >= PSIZE)
               return i + 1;
         }
      }

   }
   return 0;
}



/*! \brief Run script
 *
 * This executes script commands.  This is from Verge1.
 *
 * \param   target_entity Entity to process
 */
static void entscript (int target_entity)
{
   if (g_ent[target_entity].active == 0)
      return;
   if (g_ent[target_entity].cmd == 0)
      getcommand (target_entity);
   switch (g_ent[target_entity].cmd) {
   case 1:
      if (move (target_entity, 0, -1))
         g_ent[target_entity].cmdnum--;
      break;
   case 2:
      if (move (target_entity, 0, 1))
         g_ent[target_entity].cmdnum--;
      break;
   case 3:
      if (move (target_entity, -1, 0))
         g_ent[target_entity].cmdnum--;
      break;
   case 4:
      if (move (target_entity, 1, 0))
         g_ent[target_entity].cmdnum--;
      break;
   case 5:
      g_ent[target_entity].cmdnum--;
      break;
   case 6:
      return;
   case 7:
      g_ent[target_entity].sidx = 0;
      g_ent[target_entity].cmdnum = 0;
      break;
   case 8:
      if (g_ent[target_entity].tilex < g_ent[target_entity].cmdnum)
         move (target_entity, 1, 0);
      if (g_ent[target_entity].tilex > g_ent[target_entity].cmdnum)
         move (target_entity, -1, 0);
      if (g_ent[target_entity].tilex == g_ent[target_entity].cmdnum)
         g_ent[target_entity].cmdnum = 0;
      break;
   case 9:
      if (g_ent[target_entity].tiley < g_ent[target_entity].cmdnum)
         move (target_entity, 0, 1);
      if (g_ent[target_entity].tiley > g_ent[target_entity].cmdnum)
         move (target_entity, 0, -1);
      if (g_ent[target_entity].tiley == g_ent[target_entity].cmdnum)
         g_ent[target_entity].cmdnum = 0;
      break;
   case 10:
      g_ent[target_entity].facing = g_ent[target_entity].cmdnum;
      g_ent[target_entity].cmdnum = 0;
      break;
   }
   if (g_ent[target_entity].cmdnum == 0)
      g_ent[target_entity].cmd = 0;
}



/*! \brief Party following leader
 *
 * This makes any characters (after the first) follow the leader.
 */
static void follow (int tx, int ty)
{
   int i;

   if (numchrs == 1)
      return;
   for (i = numchrs - 1; i > 0; --i) {
      if (i == 1)
         move (i, tx - g_ent[i].tilex, ty - g_ent[i].tiley);
      else
         move (i, g_ent[i - 1].tilex - g_ent[i].tilex,
               g_ent[i - 1].tiley - g_ent[i].tiley);
   }
}



/*! \brief Read a command and parameter from a script
 *
 * This processes entity commands from the movement script.
 * This is from Verge1.
 *
 * Script commands are:
 * - U,R,D,L + param:  move up, right, down, left by param spaces
 * - W+param: wait param frames
 * - B: start script again
 * - X+param: move to x-coord param
 * - Y+param: move to y-coord param
 * - F+param: face direction param (0=S, 1=N, 2=W, 3=E)
 * - K: kill (remove) entity
 *
 * \param   target_entity Entity to process
 */
static void getcommand (int target_entity)
{
   char s;
   /* PH FIXME: prevented from running off end of string */
   if (g_ent[target_entity].sidx < sizeof (g_ent[target_entity].script))
      s = g_ent[target_entity].script[g_ent[target_entity].sidx++];
   else
      s = '\0';
   switch (s) {
   case 'u':
   case 'U':
      g_ent[target_entity].cmd = 1;
      parsems (target_entity);
      break;
   case 'd':
   case 'D':
      g_ent[target_entity].cmd = 2;
      parsems (target_entity);
      break;
   case 'l':
   case 'L':
      g_ent[target_entity].cmd = 3;
      parsems (target_entity);
      break;
   case 'r':
   case 'R':
      g_ent[target_entity].cmd = 4;
      parsems (target_entity);
      break;
   case 'w':
   case 'W':
      g_ent[target_entity].cmd = 5;
      parsems (target_entity);
      break;
   case '\0':
      g_ent[target_entity].cmd = 6;
      g_ent[target_entity].movemode = MM_STAND;
      g_ent[target_entity].cmdnum = 0;
      g_ent[target_entity].sidx = 0;
      break;
   case 'b':
   case 'B':
      g_ent[target_entity].cmd = 7;
      break;
   case 'x':
   case 'X':
      g_ent[target_entity].cmd = 8;
      parsems (target_entity);
      break;
   case 'y':
   case 'Y':
      g_ent[target_entity].cmd = 9;
      parsems (target_entity);
      break;
   case 'f':
   case 'F':
      g_ent[target_entity].cmd = 10;
      parsems (target_entity);
      break;
   case 'k':
   case 'K':
      /* PH add: command K makes the ent disappear */
      g_ent[target_entity].active = 0;
      break;
   default:
#ifdef DEBUGMODE
      if (debugging > 0) {
         sprintf (strbuf,
                  _("Invalid entity command (%c) at position %d for ent %d"), s,
                  g_ent[target_entity].sidx, target_entity);
         program_death (strbuf);
      }
#endif
      break;
   }
}



/*! \brief Generic movement
 *
 * Set up the entity vars to move in the given direction
 *
 * \param   target_entity Index of entity to move
 * \param   dx tiles to move in x direction
 * \param   dy tiles to move in y direction
 */
static int move (int target_entity, int dx, int dy)
{
   int tx, ty, source_tile, oldfacing;
   s_entity *ent = &g_ent[target_entity];

   if (dx == 0 && dy == 0)      // Speed optimization.
      return 0;

   tx = ent->x / 16;
   ty = ent->y / 16;
   oldfacing = ent->facing;
   if (dx < 0)
      ent->facing = FACE_LEFT;
   else if (dx > 0)
      ent->facing = FACE_RIGHT;
   else if (dy > 0)
      ent->facing = FACE_DOWN;
   else if (dy < 0)
      ent->facing = FACE_UP;
   if (tx + dx == -1 || tx + dx == g_map.xsize ||
       ty + dy == -1 || ty + dy == g_map.ysize)
      return 0;
   if (ent->obsmode == 1) {
      // Try to automatically walk/run around obstacle.
      if (dx && obstruction (tx, ty, dx, 0, FALSE)) {
         if (dy != -1 && oldfacing == ent->facing
             && !obstruction (tx, ty + 1, dx, 0, TRUE)
             && !obstruction (tx, ty, 0, 1, TRUE))
            dy = 1;
         else if (dy != 1 && oldfacing == ent->facing
                  && !obstruction (tx, ty - 1, dx, 0, TRUE)
                  && !obstruction (tx, ty, 0, -1, TRUE))
            dy = -1;
         else
            dx = 0;
      }
      if (dy && obstruction (tx, ty, 0, dy, FALSE)) {
         if (dx != -1 && oldfacing == ent->facing
             && !obstruction (tx + 1, ty, 0, dy, TRUE)
             && !obstruction (tx, ty, 1, 0, TRUE))
            dx = 1;
         else if (dx != 1 && oldfacing == ent->facing
                  && !obstruction (tx - 1, ty, 0, dy, TRUE)
                  && !obstruction (tx, ty, -1, 0, TRUE))
            dx = -1;
         else
            dy = 0;
      }
      if ((dx || dy) && obstruction (tx, ty, dx, dy, FALSE)) {
         dx = dy = 0;
      }
   }
   if (!dx && !dy && oldfacing == ent->facing)
      return 0;
   if (ent->obsmode == 1 && entityat (tx + dx, ty + dy, target_entity))
      return 0;

   // Make sure that the player can't avoid special zones by moving diagonally.
   if (dx && dy)
   {
      source_tile = ty * g_map.xsize + tx;
      if (z_seg[source_tile] != z_seg[source_tile + dx]
         || z_seg[source_tile] != z_seg[source_tile + dy * g_map.xsize]) {
         if (ent->facing == FACE_LEFT || ent->facing == FACE_RIGHT) {
            if (!obstruction(tx, ty, dx, 0, TRUE))
               dy = 0;
            else
               dx = 0;
         } else {   // They are facing up or down.
            if (!obstruction(tx, ty, 0, dy, TRUE))
               dx = 0;
            else
               dy = 0;
         }
      }
   }

   // Make sure player can't walk diagonally between active entities.
   if (dx && dy)
      if (obstruction(tx, ty, dx, 0, TRUE) && obstruction(tx, ty, 0, dy, TRUE))
         return 0;

   ent->tilex = tx + dx;
   ent->tiley = ty + dy;
   ent->y += dy;
   ent->x += dx;
   ent->moving = 1;
   ent->movcnt = 15;
   return 1;
}



/*! \brief Check for obstruction
 *
 * Check for any map-based obstructions in the specified co-ordinates.
 *
 * \param   ox Original x-coord position
 * \param   oy Original y-coord position
 * \param   mx Amount to move -1..+1
 * \param   my Amount to move -1..+1
 * \param   check_entity Whether to return 1 if an entity is at the target
 * \returns 1 if path is obstructed, 0 otherwise
 */
static int obstruction (int ox, int oy, int mx, int my, int check_entity)
{
   int son, sto, tox, toy, i;

   tox = ox + mx;
   if (tox < 0 || tox > g_map.xsize - 1)
      return 1;
   toy = oy + my;
   if (toy < 0 || toy > g_map.ysize - 1)
      return 1;
   son = o_seg[(oy * g_map.xsize) + ox];
   sto = o_seg[(toy * g_map.xsize) + tox];
   if (sto == 1)
      return 1;
   if (my == -1) {
      if (son == 2 || sto == 4)
         return 1;
   }
   if (my == 1) {
      if (son == 4 || sto == 2)
         return 1;
   }
   if (mx == -1) {
      if (son == 5 || sto == 3)
         return 1;
   }
   if (mx == 1) {
      if (son == 3 || sto == 5)
         return 1;
   }

   if (check_entity) {
      for (i = 0; i < MAX_ENT; i++) {
         if (g_ent[i].active && tox == g_ent[i].tilex && toy == g_ent[i].tiley) {
            return 1;
         }
      }
   }
   return 0;
}



/*! \brief Read an int from a script
 *
 * This parses the movement script for a value that relates
 * to a command.  This is from Verge1.
 *
 * \param   target_entity Entity to process
 */
static void parsems (int target_entity)
{
   int p = 0;
   char tok[10];
   char s;

   s = g_ent[target_entity].script[g_ent[target_entity].sidx];
   while (s >= 48 && s <= 57) {
      tok[p] = s;
      g_ent[target_entity].sidx++;
      s = g_ent[target_entity].script[g_ent[target_entity].sidx];
      p++;
   }
   tok[p] = 0;
   g_ent[target_entity].cmdnum = atoi (tok);
}



/*! \brief Set position
 *
 * Position an entity manually.
 *
 * \param   en Entity to position
 * \param   ex x-coord
 * \param   ey y-coord
 */
void place_ent (int en, int ex, int ey)
{
   g_ent[en].tilex = ex;
   g_ent[en].tiley = ey;
   g_ent[en].x = g_ent[en].tilex * 16;
   g_ent[en].y = g_ent[en].tiley * 16;
}



/*! \brief Process movement for player
 *
 * This is the replacement for process_controls that used to be in kq.c
 * I realized that all the work in process_controls was already being
 * done in process_entity... I just had to make this exception for the
 * player-controlled dude.
 */
static void player_move (void)
{
   int oldx = g_ent[0].tilex;
   int oldy = g_ent[0].tiley;
   readcontrols ();

   if (balt)
      activate ();
   if (benter)
      menu ();
#ifdef KQ_CHEATS
   if (key[KEY_F10]) {
      unpress ();
      do_luacheat ();
   }
#endif
   move (0, right ? 1 : left ? -1 : 0, down ? 1 : up ? -1 : 0);
   if (g_ent[0].moving) {
      follow (oldx, oldy);
   }
}



/*! \brief Main entity routine
 *
 * The main routine that loops through the entity list and processes each
 * one.
 */
void process_entities (void)
{
   int i;
   const char *t_evt;
   for (i = 0; i < MAX_ENT; i++)
      if (g_ent[i].active == 1)
         speed_adjust (i);
   /* Do timers */
   t_evt = get_timer_event ();
   if (t_evt)
      do_timefunc (t_evt);
}



/*! \brief Actions for one entity
 * \date    20040310 PH added TARGET movemode, broke out chase into separate function
 *
 * Process an individual active entity.  If the entity in question
 * is #0 (main character) and the party is not automated, then allow
 * for player input.
 *
 * \param   target_entity Index of entity
 * \date    20040310 PH added TARGET movemode, broke out chase into separate function
 */
static void process_entity (int target_entity)
{
   s_entity *ent = &g_ent[target_entity];
   s_player *player = 0;
   ent->scount = 0;

   if (!ent->active)
      return;

   if (!ent->moving) {
      if (target_entity == 0 && !autoparty) {
         player_move ();
         if (ent->moving && display_desc == 1)
            display_desc = 0;
         return;
      }
      switch (ent->movemode) {
      case MM_STAND:
         return;
      case MM_WANDER:
         wander (target_entity);
         break;
      case MM_SCRIPT:
         entscript (target_entity);
         break;
      case MM_CHASE:
         chase (target_entity);
         break;
      case MM_TARGET:
         target (target_entity);
         break;
      }
   } else {                  /* if (.moving==0) */
      if (ent->tilex * 16 > ent->x)
         ++ent->x;
      if (ent->tilex * 16 < ent->x)
         --ent->x;
      if (ent->tiley * 16 > ent->y)
         ++ent->y;
      if (ent->tiley * 16 < ent->y)
         --ent->y;
      ent->movcnt--;
      ent->framectr++;

      if (ent->framectr > 20)
         ent->framectr = 0;

      if (ent->movcnt == 0) {
         ent->moving = 0;
         if (target_entity < PSIZE) {
            player = &party[pidx[target_entity]];
            steps++;
            if (steps > STEPS_NEEDED)
               steps = STEPS_NEEDED;
            if (player->sts[S_POISON] > 0) {
               player->hp--;
               if (player->hp < 1)
                  player->hp = 1;
               play_effect (21, 128);
            }
            if (player->eqp[5] == I_REGENERATOR) {
               player->hp++;
               if (player->hp > player->mhp)
                  player->hp = player->mhp;
            }
         }
         if (target_entity == 0)
            zone_check ();
      }

      if (target_entity == 0 && vfollow == 1)
         calc_viewport (0);
   }
}



/*! \brief Initialise script
 *
 * This is used to set up an entity with a movement script so that
 * it can be automatically controlled.
 *
 * \param   target_entity Entity to process
 * \param   movestring The script
 */
void set_script (int target_entity, const char *movestring)
{
   g_ent[target_entity].moving = 0;
   g_ent[target_entity].movcnt = 0;
   g_ent[target_entity].cmd = 0;
   g_ent[target_entity].sidx = 0;
   g_ent[target_entity].cmdnum = 0;
   g_ent[target_entity].movemode = MM_SCRIPT;
   strncpy (g_ent[target_entity].script, movestring,
            sizeof (g_ent[target_entity].script));
}



/*! \brief Adjust movement speed
 *
 * This has to adjust for each entity's speed.
 * 'Normal' speed appears to be 4.
 *
 * \param   target_entity Index of entity
 */
static void speed_adjust (int target_entity)
{
   if (g_ent[target_entity].speed < 4) {
      switch (g_ent[target_entity].speed) {
      case 1:
         if (g_ent[target_entity].scount < 3) {
            g_ent[target_entity].scount++;
            return;
         }
         break;
      case 2:
         if (g_ent[target_entity].scount < 2) {
            g_ent[target_entity].scount++;
            return;
         }
         break;
      case 3:
         if (g_ent[target_entity].scount < 1) {
            g_ent[target_entity].scount++;
            return;
         }
         break;
      }
   }
   if (g_ent[target_entity].speed < 5)
      process_entity (target_entity);
   switch (g_ent[target_entity].speed) {
   case 5:
      process_entity (target_entity);
      process_entity (target_entity);
      break;
   case 6:
      process_entity (target_entity);
      process_entity (target_entity);
      process_entity (target_entity);
      break;
   case 7:
      process_entity (target_entity);
      process_entity (target_entity);
      process_entity (target_entity);
      process_entity (target_entity);
      break;
   }
   /* TT: This is to see if the player is "running" */
   if (key[kctrl] && target_entity < PSIZE)
      process_entity (target_entity);
}



/*! \brief Move entity towards target
 * \author PH
 * \date 20040310
 *
 * When entity is in target mode (MM_TARGET) move towards the goal.  This is
 * fairly simple; it doesn't do clever obstacle avoidance.  It simply moves
 * either horizontally or vertically, preferring the _closer_ one. In other
 * words, it will try to get on a vertical or horizontal line with its target.
 *
 * \param   target_entity Index of entity
 */
static void target (int target_entity)
{
   int dx, dy, ax, ay, emoved = 0;
   s_entity *ent = &g_ent[target_entity];
   ax = dx = ent->target_x - ent->tilex;
   ay = dy = ent->target_y - ent->tiley;
   if (ax < 0)
      ax = -ax;
   if (ay < 0)
      ay = -ay;
   if (ax < ay) {
      /* Try to move horizontally */
      if (dx < 0)
         emoved = move (target_entity, -1, 0);
      if (dx > 0)
         emoved = move (target_entity, 1, 0);
      /* Didn't move so try vertically */
      if (!emoved) {
         if (dy < 0)
            move (target_entity, 0, -1);
         if (dy > 0)
            move (target_entity, 0, 1);
      }
   } else {
      /* Try to move vertically */
      if (dy < 0)
         emoved = move (target_entity, 0, -1);
      if (dy > 0)
         emoved = move (target_entity, 0, 1);
      /* Didn't move so try horizontally */
      if (!emoved) {
         if (dx < 0)
            move (target_entity, -1, 0);
         if (dx > 0)
            move (target_entity, 1, 0);
      }
   }
   if (dx == 0 && dy == 0) {
      /* Got there */
      ent->movemode = MM_STAND;
   }
}



/*! \brief Move randomly
 *
 * Choose a random direction for the entity to walk in and set up the
 * vars to do so.
 *
 * \param   target_entity Index of entity to move
 */
static void wander (int target_entity)
{
   if (g_ent[target_entity].delayctr < g_ent[target_entity].delay) {
      g_ent[target_entity].delayctr++;
      return;
   }
   g_ent[target_entity].delayctr = 0;
   switch (rand () % 8) {
   case 0:
      move (target_entity, 0, -1);
      break;
   case 1:
      move (target_entity, 0, 1);
      break;
   case 2:
      move (target_entity, -1, 0);
      break;
   case 3:
      move (target_entity, 1, 0);
      break;
   }
}
