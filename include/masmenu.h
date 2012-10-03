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

#ifndef __MASMENU_H
#define __MASMENU_H



int learn_new_spells (int);     /*  kq.c, menu.c, combat.c  */
void camp_spell_menu (int);     /*  menu.c  */

extern int close_menu;          /*  menu.c  */



#if 0
int need_spell (int, int, int); /*  staticed  */
void camp_draw_spell_menu (int, int, int);      /*  staticed  */
void camp_spell_targeting (int, int);   /*  staticed  */
int camp_castable (int, int);   /*  staticed  */
#endif /*  0  */



#endif /*  __MASMENU_H  */
