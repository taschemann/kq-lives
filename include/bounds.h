/*! \page License
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
 * \brief Contains functions specific to bounded areas
 * \author TT
 * \date 20060720
 */

#ifndef __BOUNDS_H
#define __BOUNDS_H

#define MAX_BOUNDS    256


/*! \brief Bounding area box
 *
 * A boundary is a viewable area on the map.
 * These are set up in the map editor to remove the need to call set_view
 * in the scripts. If the player is inside a bounding box' coordinate, then
 * the map will only redraw those tiles, else it will redraw everything on the
 * map.
 * \author TT
 * \date 20060710
 */
typedef struct
{
   short x1;   /*!< The top-left coordinates of the bounding box */
   short y1;   /*!< The top-left coordinates of the bounding box */
   short x2;   /*!< The bottom-right coordinates of the bounding box */
   short y2;   /*!< The bottom-right coordinates of the bounding box */
   short btile; /*!< Index of the tile to draw everywhere BUT here */
} s_bound;


int bound_in_bound (s_bound *, int);
int bound_in_bound2 (s_bound *, s_bound *, int);
int is_contained_bound (s_bound, int, int);
void set_bounds (s_bound *, int, int, int, int);

extern short num_bound_boxes;
extern s_bound bound_box[MAX_BOUNDS];

#endif /*  __BOUNDS_H  */
