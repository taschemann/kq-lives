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
 * \brief Bounding areas
 * \author TT
 * \date 20060720
 */

#include <stdio.h>
#include <ctype.h>

#include "kq.h"


s_bound bound_box[MAX_BOUNDS];


/*! \brief See if this bounding area overlaps (or is contained inside of) any
 * other bounding area, or vice versa.
 *
 * \param   which - Bounding area to check
 * \param   num_bound_boxes Number of elements in the \sa bound_box array
 * \return  1 if 'which' coords found anywhere withing any other boxes
 */
int bound_in_bound (s_bound *which, int num_bound_boxes)
{
   /* Check if any part of box1 is inside box2 (or box2 in box1) */

   int i, j, k;
   int true1, true2; // See if an entire query is true
   int was_found;

   /* We can use an array to speed this up so we don't have to duplicate
    * identical code
    */
   int x1[2] = {which->x1, which->x2};
   int y1[2] = {which->y1, which->y2};
   int x2[2];  // Defined inside for..loop for all bounding areas
   int y2[2];  // Defined inside for..loop for all bounding areas

   /* This will contain a small hack. Assign the results of the queries to
    * variables. We can break out with a "nothing found" in these cases:
    *
    * 1) NEITHER x-coords is true
    * 2) NEITHER y-coords is true
    *
    * Else, something was overlapping, and we'll return '1'
    */
   was_found = 0;
   for (i = 0; i < num_bound_boxes; i++) {
      x2[0] = bound_box[i].x1;
      x2[1] = bound_box[i].x2;
      y2[0] = bound_box[i].y1;
      y2[1] = bound_box[i].y2;

      for (j = 0; j < 2; j++) {
         true1 = (x1[j] >= x2[j] && x1[j] <= x2[1 - j]);
         true2 = (x2[j] >= x1[j] && x2[j] <= x1[1 - j]);
         if (!true1 && !true2) {
            continue;
         }

         for (k = 0; k < 2; k++) {
            true1 = (y1[k] >= y2[k] && y1[k] <= y2[1 - k]);
            true2 = (y2[k] >= y1[k] && y2[k] <= y1[1 - k]);
            if (!true1 && !true2) {
               continue;
            } else {
               was_found = 1;
            }
         }
      }
      if (was_found) {
         break;
      }
   }

   return was_found;
}

/*! \brief See if this bounding area overlaps (or is contained inside of) any
 * bounding area in the given array, or vice versa. Note that this function is
 * identical to the one above, except that we pass a pointer to the bound_box
 * array, rather than just assuming the one we want to check against.
 *
 * \param   which - Bounding area to check
 * \param   bound_box - pointer to the bounding boxes we are checking against
 * \param   num_bound_boxes Number of elements in the \sa bound_box array
 * \return  1 if 'which' coords found anywhere withing any other boxes
 */
int bound_in_bound2 (s_bound *which, s_bound * bound_box, int num_bound_boxes)
{
   /* Check if any part of box1 is inside box2 (or box2 in box1) */

   int i, j, k;
   int true1, true2; // See if an entire query is true
   int was_found;

   /* We can use an array to speed this up so we don't have to duplicate
    * identical code
    */
   int x1[2] = {which->x1, which->x2};
   int y1[2] = {which->y1, which->y2};
   int x2[2];  // Defined inside for..loop for all bounding areas
   int y2[2];  // Defined inside for..loop for all bounding areas

   /* This will contain a small hack. Assign the results of the queries to
    * variables. We can break out with a "nothing found" in these cases:
    *
    * 1) NEITHER x-coords is true
    * 2) NEITHER y-coords is true
    *
    * Else, something was overlapping, and we'll return '1'
    */
   was_found = 0;
   for (i = 0; i < num_bound_boxes; i++) {
      x2[0] = bound_box[i].x1;
      x2[1] = bound_box[i].x2;
      y2[0] = bound_box[i].y1;
      y2[1] = bound_box[i].y2;

      for (j = 0; j < 2; j++) {
         true1 = (x1[j] >= x2[j] && x1[j] <= x2[1 - j]);
         true2 = (x2[j] >= x1[j] && x2[j] <= x1[1 - j]);
         if (!true1 && !true2) {
            continue;
         }

         for (k = 0; k < 2; k++) {
            true1 = (y1[k] >= y2[k] && y1[k] <= y2[1 - k]);
            true2 = (y2[k] >= y1[k] && y2[k] <= y1[1 - k]);
            if (!true1 && !true2) {
               continue;
            } else {
               was_found = 1;
            }
         }
      }
      if (was_found) {
         break;
      }
   }

   return was_found;
}




/* Check whether these coordinates are within the given bounding area
 * \param   b - The specified bounding area we are to check
 * \param   x - Coordinates that we want to see if they are contained within
 * \param   y - the specified bounding area
 */
int is_contained_bound (s_bound b, int x, int y)
{
   if (x >= b.x1 && x <= b.x2 && y >= b.y1 && y <= b.y2)
      return 1;
   else
      return 0;
}



/* Assign the given bounding area with the x and y coords.
 * Check whether x1 < x2 or y1 < y2, and swap accordingly.
 *
 * \param   which_bound - Where we will store the correct x/y coords
 * \param   x1, y1, x2, y2 - All the coords
 */
void set_bounds (s_bound *which_bound, int x1, int y1, int x2, int y2)
{
   int tempx, tempy; // Use these incase a swap is needed

   if (x2 < x1) {
      tempx = x1;
      x1 = x2;
      x2 = tempx;
   }
   if (y2 < y1) {
      tempy = y1;
      y1 = y2;
      y2 = tempy;
   }
   which_bound->x1 = x1;
   which_bound->y1 = y1;
   which_bound->x2 = x2;
   which_bound->y2 = y2;
}
