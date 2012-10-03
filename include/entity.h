/*
   KQ is Copyright (C) 2002 - Josh Bolduc

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

#ifndef __ENTITY_H
#define __ENTITY_H

/*! \file 
 * \brief Stuff related to entities on the map
 *
 * An entity is a hero, or an NPC.
 * \author JB
 * \date ??????
 */

#define MAX_ENT     50
#define ID_ENEMY    254



void process_entities (void);   /*  kq.c  */
int entityat (int, int, int);   /*  kq.c  */
void set_script (int, const char *);  /*  intrface.c  */
void place_ent (int, int, int); /*  intrface.c, kq.c  */
void count_entities (void);     /*  kq.c  */



#if 0
int entity_near (int, int, int);        /*  staticed  */
void speed_adjust (int);        /*  staticed  */
void process_entity (int);      /*  staticed  */
void lastm_check (int);         /*  staticed  */
void follow (void);             /*  staticed  */
void wander (int);              /*  staticed  */
void player_move (void);        /*  staticed  */
void moveup (int);              /*  staticed  */
void movedown (int);            /*  staticed  */
void moveright (int);           /*  staticed  */
void moveleft (int);            /*  staticed  */
int obstruction (int, int, int, int);   /*  staticed  */
void parsems (int);             /*  staticed  */
void getcommand (int);          /*  staticed  */
void entscript (int);           /*  staticed  */
#endif /*  0  */

/* move modes */
#define MM_STAND  0
#define MM_WANDER 1
#define MM_SCRIPT 2
#define MM_CHASE  3
#define MM_TARGET 4


#endif /*  __ENTITY_H  */
