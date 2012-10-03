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

/*! \file
 * \brief Hero combat header file
 */

#ifndef __HEROC_H
#define __HEROC_H

/*! \name Hero identifiers */
#define SENSAR            0
#define SARINA            1
#define CORIN             2
#define AJATHAR           3
#define CASANDRA          4
#define TEMMIN            5
#define AYLA              6
#define NOSLOM            7

extern int can_use_item;        /* intrface.c */

void hero_init (void);          /*  combat.c  */
void hero_choose_action (int);  /*  combat.c  */
void auto_herochooseact (int);  /*  combat.c  */
int available_spells (int);     /*  hskill.c  */
int combat_spell_menu (int);    /*  hskill.c  */


#endif /*  __HEROC_H  */
