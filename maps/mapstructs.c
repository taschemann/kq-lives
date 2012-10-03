/***************************************************************************\
 * This map editor is for the KQ game, supported by the kqlives community. *
 * Comments and suggestions about the editor are welcome.  Please join our *
 * mailing list: kqlives-main@lists.sourceforge.net                        *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/index.php             *
 *                                                                         *
 * Mapstructs.c contains markers, bounding areas, etc..                    *
\***************************************************************************/


#include "mapdraw.h"


/*! \brief Add or remove a bounding area
 *
 * \param   x - Coordinate of bounding area
 * \param   y - same
 * \param   mouse_b - Which mouse button (left/right) was pressed
 * \param   current - Number of bounding areas currently defined
 */
void add_change_bounding (int x, int y, int mouse_b, int *current)
{
   s_bound *b;
   s_bound *found = NULL;

   /* Does a bounding box exist at this coordinate? */
   for (b = bound_box; b < bound_box + num_bound_boxes; ++b) {
      if (is_contained_bound (*b, x, y)) {
         found = b;
      }
   }

   if (mouse_b == 2) {
      /* We will unset the active_bound variable if a right-click is ever
       * encountered, regardless if the bounding box in question is found
       * or not.
       */
      active_bound = 0;
   }

   if (found) {
      /* There is a bounding box here */
      if (mouse_b == 1) {
         /* The user clicked on a bounded area; allow user to select the
          * background tile.  This tile is used everywhere that the bounded
          * box is NOT located.  It is most commonly '0' (black) for the
          * side-rooms, but can be '1' or '3' for other areas (for trees or
          * grass, etc.).
          */
         rename_bound_tile (found);
         return;
      } else if (mouse_b == 2) {
         /* Delete it */

         /* Move the selector to the previous bounding box if this was the
          * last one on the map
          */
         num_bound_boxes--;
         if (*current >= num_bound_boxes)
            *current = num_bound_boxes - 1;

         memcpy (found, found + 1,
                 (&bound_box[num_bound_boxes] - found) * sizeof (s_bound));
         bound_box[num_bound_boxes].btile = 0;
         return;
      }
   } else {
      /* There is no bounding box here */
      if (mouse_b == 1 && num_bound_boxes < MAX_BOUNDS) {
         /* Add a bounding box */
         *current = num_bound_boxes;

         b = &bound_box[num_bound_boxes];

         if (!active_bound) {
            /* We are not already tracking a boundary, so create new */
            b->x1 = x;
            b->y1 = y;
            active_bound = 1;
         } else {
            /* We have started tracking a boundary already, meaning we
             * already have the top-left coords. Now we just need the
             * bottom-right ones.
             */
            if (b->x1 == x || b->y1 == y) {
               /* Our bounding rectangle will never be only 1-tile high or
                * wide, so do not allow that small of an area to be selected.
                */
               return;
            }

            /* Used only to check if boundaries are okay */
            s_bound temp;
            set_bounds (&temp, b->x1, b->y1, x, y);
            /* Don't allow the user to begin (or end) any portion of a bounding
             * area if it has any points contained in another bounding area's
             * region.
             */
            if (!bound_in_bound (&temp, num_bound_boxes)) {
               set_bounds (b, temp.x1, temp.y1, temp.x2, temp.y2);
               num_bound_boxes++;
               active_bound = 0;
            }
         }
         return;
      } else if (mouse_b == 2) {
         /* Nothing to do */
         return;
      }
   }
}



/*! \brief Add or remove a marker
 *
 * \param   x - Coordinate to place marker
 * \param   y - same
 * \param   mouse_b - Whether to add or remove marker (1 to add, 2 to remove)
 * \param   current - Number of markers currently defined
 */
void add_change_marker (int x, int y, int mouse_b, int *current)
{
   s_marker *m;
   s_marker *found = NULL;

   /* Does a marker exist here? */
   for (m = gmap.markers; m < gmap.markers + num_markers; ++m) {
      if (m->x == x && m->y == y) {
         found = m;
         break;
      }
   }

   if (found) {
      /* There is a marker here */
      if (mouse_b == 1) {
         /* Rename it */
         rename_marker (found);
      } else if (mouse_b == 2) {
         /* Delete it */

         /* Move the selector to the previous marker if this was the last
          * marker on the map
          */
         num_markers--;
         if (*current == num_markers)
            *current = num_markers - 1;
         memcpy (found, found + 1,
                 (&gmap.markers[num_markers] - found) * sizeof (s_marker));

         /* Wait for mouse button to be released */
      }
   } else {
      /* There is no marker here */
      if (mouse_b == 1) {
         /* Add a marker with default name */
         if (num_markers < MAX_MARKERS) {
            *current = num_markers;
            gmap.markers = realloc (gmap.markers, (num_markers + 1) * sizeof (s_marker));
            m = &gmap.markers[num_markers];
            m->x = x;
            m->y = y;
            sprintf (m->name, "Marker_%d", num_markers);
            num_markers++;
         }
      }
   }
}



/*! \brief Draw rectangle around bounding area
 *
 * \param   where - Location to draw rectangle
 * \param   b - Bounding box's coordinates
 * \param   color - color of the rectangle
 */
void bound_rect (BITMAP *where, s_bound b, int color)
{
   s_bound rectb;

   rectb.x1 = (b.x1 - window_x) * 16;
   rectb.y1 = (b.y1 - window_y) * 16;

   if (b.x1 < window_x + htiles) {
      rectb.x2 = (b.x2 - window_x) * 16 + 15;
   } else {
      rectb.x2 = htiles * 16;
   }

   if (b.y1 < window_y + htiles) {
      rectb.y2 = (b.y2 - window_y) * 16 + 15;
   } else {
      rectb.y2 = vtiles * 16;
   }

   rect (where, rectb.x1, rectb.y1, rectb.x2, rectb.y2, color);
}



/*! \brief Locate the bounded area specified by the given direction
 *
 *                    - -3 means first box
 *                    - -1 means previous box
 * \param   direction - 0 means current box
 *                    - 1 means next box
 *                    - 3 means last box
 * \param   current - Number of bounded areas currently defined
 */
int find_bound (int direction, int *current)
{
   // No bounding areas; nothing to do, so return 'not found'
   if (num_bound_boxes < 1) {
      return 0;
   }

   /* Which bounding box to move to */
   if (direction == -3) {
      /* First box */
      *current = 0;
   } else if (direction == -1) {
      /* Previous box */
      if (--*current < 0) {
         *current = num_bound_boxes - 1;
      }
   } else if (direction == 0) {
      /* Center map on current Box, if it's not too big */

      int x, y;
      x = (bound_box[*current].x2 - bound_box[*current].x1) / 2 +
         bound_box[*current].x1;
      y = (bound_box[*current].y2 - bound_box[*current].y1) / 2 +
         bound_box[*current].y1 + 1;

      center_window (x, y);
   } else if (direction == 1) {
      /* Next Box */
      if (++*current >= num_bound_boxes) {
         *current = 0;
      }
   } else if (direction == 3) {
      /* Last Box */
      *current = num_bound_boxes - 1;
   }
   return 1;
}



/*! \brief Locate the marker specified by the given direction
 *
 *                    - -3 means first marker
 *                    - -1 means previous marker
 * \param   direction - 0 means current marker
 *                    - 1 means next marker
 *                    - 3 means last marker
 * \param   current - Number of markers currently defined
 */
int find_marker (int direction, int *current)
{
   // No markers; nothing to do, so return 'not found'
   if (num_markers < 1) {
      return 0;
   }

   /* Which marker to move to */
   if (direction == -3) {
      /* First marker */
      *current = 0;
   } else if (direction == -1) {
      /* Previous Marker */
      if (--*current < 0) {
         *current = num_markers - 1;
      }
   } else if (direction == 0) {
      /* Center map on current Marker */
      center_window (gmap.markers[*current].x, gmap.markers[*current].y);
   } else if (direction == 1) {
      /* Next Marker */
      if (++*current >= num_markers) {
         *current = 0;
      }
   } else if (direction == 3) {
      /* Last Marker */
      *current = num_markers - 1;
   }
   return 1;
}



/* Check whether this marker is at these coordinates
 * \param   m - The specified marker we are to check
 * \param   x - Given x coordinate on the map
 * \param   y - Given y coordinate on the map
 */
int is_contained_marker (s_marker m, int x, int y)
{
   if (m.x == x && m.y == y)
      return 1;
   else
      return 0;
}



/*! \brief Move the window so we can see the currenly-selected box
 *
 * \param   current - Number of bounding boxes currently defined
 * \modify  global coords "window_[xy]" will be updated
 */
void orient_bounds (int current)
{
   int width, height;

   width = bound_box[current].x2 - bound_box[current].x1;
   height = bound_box[current].y2 - bound_box[current].y1;

   if (width > htiles - 1) {
      /* Boundary is larger than window's width */

      /* Move window's left edge to the box's left edge */
      window_x = bound_box[current].x1;
   } else {
      /* Boundary fits inside viewable window */

      /* Move window just enough to see the entire box */
      if (bound_box[current].x1 < window_x)
         window_x = bound_box[current].x1;
      else if (bound_box[current].x2 > window_x + htiles - 1)
         window_x = bound_box[current].x2 - htiles + 1;
   }

   if (height > vtiles - 1) {
      /* Boundary is larger than window's height */

      /* Move window's top edge to the box's top edge */
      window_y = bound_box[current].y1;
   } else {
      /* Boundary fits inside viewable window */

      /* Move window just enough to see the entire box */
      if (bound_box[current].y1 < window_y)
         window_y = bound_box[current].y1;
      else if (bound_box[current].y2 > window_y + vtiles - 1)
         window_y = bound_box[current].y2 - vtiles + 1;
   }
}



/*! \brief Move the window so we can see the currenly-selected marker
 *
 * \param   current - Number of markers currently defined
 * \modify  global coords "window_[xy]" will be updated
 */
void orient_markers (int current)
{
   /* Move view-window enough to show marker */
   if (gmap.markers[current].x < window_x)
      window_x = gmap.markers[current].x;
   else if (gmap.markers[current].x > window_x + htiles - 1)
      window_x = gmap.markers[current].x - htiles + 1;

   if (gmap.markers[current].y < window_y)
      window_y = gmap.markers[current].y;
   else if (gmap.markers[current].y > window_y + vtiles - 1)
      window_y = gmap.markers[current].y - vtiles + 1;
}



/*! \brief Give the bounded area a new background tile number
 *
 * \param   box - Which box to assign the value to
 */
void rename_bound_tile (s_bound *box)
{
   int response, done;
   int selected_tile = 0;

   make_rect (double_buffer, 2, 32);
   sprintf (strbuf, "Tile: %d", box->btile);
   print_sfont (6, 6, strbuf, double_buffer);
   print_sfont (6, 12, ">", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (12, 12, strbuf, 4);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         selected_tile = atoi (strbuf);

         /* Make sure the value is valid */
         if (selected_tile < 0 || selected_tile >= ICONSET_SIZE * max_sets) {
            sprintf (strbuf, "Invalid tile: %d", selected_tile);
            cmessage (strbuf);
            wait_enter ();
         } else {
            done = 1;
         }
      }
   }
   box->btile = selected_tile;
}                               /* rename_bound_tile () */



/*! \brief Give the marker a new name if the name hasn't been taken already
 *
 * \param   found - Which marker to rename
 */
void rename_marker (s_marker *found)
{
   int response, done;
   s_marker *m;

   make_rect (double_buffer, 2, 32);
   print_sfont (6, 6, found->name, double_buffer);
   print_sfont (6, 12, ">", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (12, 12, strbuf, 31);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) == 0) {
         cmessage ("Do you want to clear the name of this marker? (y/n)");
         if (yninput ())
            done = 1;
      } else {
         done = 1;
      }

      /* Make sure no other markers have the same name */
      for (m = gmap.markers; m < gmap.markers + num_markers; ++m) {
         if (!strcmp (strbuf, m->name) && m != found) {
            cmessage ("Another marker has that name. Use another name.");
            yninput ();
            done = 0;
            break;
         }
      }
   }
   strcpy (found->name, strbuf);
}                               /* rename_marker () */
