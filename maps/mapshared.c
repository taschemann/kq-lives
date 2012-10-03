/***************************************************************************\
 * This map editor is for the KQ game, supported by the kqlives community. *
 * Comments and suggestions about the editor are welcome.  Please join our *
 * mailing list: kqlives-main@lists.sourceforge.net                        *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/index.php             *
 *                                                                         *
 * Mapshared.c contains anything that will need to be shared across more   *
 * than one instance of main(), including disk I/O, etc.                   *
\***************************************************************************/

#include "mapdraw.h"
#include "../include/disk.h"

/* Selectable tiles on the right-hand menu */
BITMAP *icons[MAX_TILES];
BITMAP *double_buffer, *pcx_buffer, *eframes[MAX_EPICS][12];
BITMAP *font6, *mesh1[MAX_OBSTACLES], *shadow[MAX_SHADOWS];
BITMAP *marker_image, *marker_image_active;

// This line turns off other/indent.pro indentation settings:
// *INDENT-OFF*
PALETTE pal = {
   {0, 0, 0, 0}, {0, 0, 0, 0}, {8, 8, 8, 0}, {12, 12, 12, 0},
   {16, 16, 16, 0}, {20, 20, 20, 0}, {24, 24, 24, 0}, {28, 28, 28, 0},
   {33, 33, 33, 0}, {38, 38, 38, 0}, {43, 43, 43, 0}, {47, 47, 47, 0},
   {51, 51, 51, 0}, {55, 55, 55, 0}, {59, 59, 59, 0}, {63, 63, 63, 0},

   {7, 0, 0, 0}, {15, 0, 0, 0}, {23, 0, 0, 0}, {31, 0, 0, 0},
   {39, 0, 0, 0}, {47, 0, 0, 0}, {55, 0, 0, 0}, {63, 0, 0, 0},
   {63, 7, 7, 0}, {63, 14, 14, 0}, {63, 21, 21, 0}, {63, 29, 29, 0},
   {63, 36, 36, 0}, {63, 44, 44, 0}, {63, 51, 51, 0}, {63, 59, 59, 0},

   {0, 7, 0, 0}, {0, 11, 0, 0}, {0, 15, 0, 0}, {0, 19, 0, 0},
   {0, 23, 0, 0}, {0, 27, 0, 0}, {0, 31, 0, 0}, {0, 39, 0, 0},
   {0, 47, 0, 0}, {0, 55, 0, 0}, {0, 63, 0, 0}, {22, 63, 22, 0},
   {37, 63, 37, 0}, {47, 63, 47, 0}, {53, 63, 53, 0}, {58, 63, 58, 0},

   {0, 0, 7, 0}, {0, 0, 15, 0}, {0, 0, 23, 0}, {0, 0, 31, 0},
   {0, 0, 39, 0}, {0, 0, 47, 0}, {0, 0, 55, 0}, {0, 0, 63, 0},
   {7, 7, 63, 0}, {15, 15, 63, 0}, {22, 22, 63, 0}, {30, 30, 63, 0},
   {37, 37, 63, 0}, {45, 45, 63, 0}, {52, 52, 63, 0}, {58, 58, 63, 0},

   {7, 0, 7, 0}, {15, 0, 15, 0}, {23, 0, 23, 0}, {31, 0, 31, 0},
   {39, 0, 39, 0}, {47, 0, 47, 0}, {55, 0, 55, 0}, {63, 0, 63, 0},
   {63, 7, 63, 0}, {63, 15, 63, 0}, {63, 22, 63, 0}, {63, 30, 63, 0},
   {63, 37, 63, 0}, {63, 45, 63, 0}, {63, 52, 63, 0}, {63, 58, 63, 0},

   {7, 1, 0, 0}, {15, 2, 0, 0}, {23, 4, 0, 0}, {31, 7, 0, 0},
   {39, 9, 0, 0}, {46, 12, 0, 0}, {54, 15, 0, 0}, {63, 18, 0, 0},
   {63, 21, 4, 0}, {63, 25, 8, 0}, {63, 29, 12, 0}, {63, 34, 16, 0},
   {63, 38, 22, 0}, {63, 44, 28, 0}, {63, 49, 34, 0}, {63, 55, 41, 200},

   {7, 7, 0, 200}, {11, 11, 0, 0}, {15, 15, 0, 0}, {19, 19, 0, 200},
   {23, 23, 0, 0}, {27, 27, 0, 0}, {31, 31, 0, 0}, {39, 39, 0, 0},
   {47, 47, 0, 0}, {54, 54, 0, 0}, {63, 63, 0, 0}, {63, 63, 23, 0},
   {63, 63, 35, 0}, {63, 63, 47, 0}, {63, 63, 53, 0}, {63, 63, 58, 0},

   {0, 7, 7, 0}, {0, 11, 11, 0}, {0, 15, 15, 0}, {0, 19, 19, 0},
   {0, 23, 23, 0}, {0, 27, 27, 0}, {0, 31, 31, 0}, {0, 39, 39, 0},
   {0, 47, 47, 0}, {0, 55, 55, 0}, {0, 63, 63, 0}, {16, 63, 63, 0},
   {32, 63, 63, 0}, {47, 63, 63, 0}, {51, 63, 63, 0}, {58, 63, 63, 0},

   {3, 1, 0, 0}, {7, 3, 0, 0}, {11, 5, 0, 0}, {15, 8, 0, 0},
   {19, 11, 0, 0}, {23, 14, 0, 0}, {27, 17, 0, 0}, {31, 20, 0, 105},
   {35, 24, 0, 93}, {39, 27, 0, 0}, {43, 32, 0, 0}, {47, 35, 0, 0},
   {51, 38, 0, 0}, {55, 42, 0, 0}, {59, 46, 0, 0}, {63, 55, 22, 0},

   {6, 3, 3, 0}, {12, 6, 6, 0}, {18, 9, 9, 0}, {24, 12, 12, 0},
   {30, 15, 15, 0}, {36, 18, 18, 0}, {42, 21, 21, 0}, {48, 24, 24, 0},
   {54, 27, 27, 0}, {60, 30, 30, 0}, {63, 33, 33, 0}, {63, 36, 36, 0},
   {63, 39, 39, 0}, {63, 43, 43, 0}, {63, 47, 47, 0}, {63, 51, 51, 0},

   {3, 6, 3, 0}, {6, 12, 6, 0}, {9, 18, 9, 0}, {12, 24, 12, 0},
   {15, 30, 15, 0}, {18, 36, 18, 0}, {21, 42, 21, 0}, {24, 48, 24, 0},
   {27, 54, 27, 0}, {30, 60, 30, 0}, {33, 63, 33, 0}, {37, 63, 37, 0},
   {41, 63, 41, 0}, {45, 63, 45, 0}, {49, 63, 49, 0}, {53, 63, 53, 0},

   {4, 2, 1, 0}, {8, 4, 2, 0}, {12, 6, 3, 0}, {16, 8, 4, 0},
   {20, 10, 5, 0}, {24, 16, 7, 0}, {28, 20, 10, 0}, {32, 24, 16, 0},
   {36, 28, 20, 0}, {40, 32, 25, 0}, {44, 36, 28, 0}, {48, 40, 32, 0},
   {52, 44, 36, 0}, {56, 48, 40, 0}, {60, 52, 44, 0}, {63, 56, 48, 0},

   {6, 3, 6, 0}, {12, 6, 12, 0}, {18, 9, 18, 0}, {24, 12, 24, 0},
   {30, 15, 30, 0}, {36, 18, 36, 0}, {42, 21, 42, 0}, {48, 24, 48, 0},
   {54, 27, 54, 0}, {60, 30, 60, 0}, {63, 34, 63, 0}, {63, 38, 63, 0},
   {63, 42, 63, 0}, {63, 46, 63, 0}, {63, 50, 63, 0}, {63, 54, 63, 0},

   {6, 3, 2, 79}, {12, 6, 3, 0}, {18, 9, 4, 0}, {24, 12, 5, 0},
   {30, 15, 6, 0}, {36, 18, 7, 0}, {42, 21, 8, 0}, {48, 24, 9, 0},
   {54, 27, 11, 0}, {60, 30, 13, 0}, {63, 33, 15, 0}, {63, 36, 17, 0},
   {63, 39, 19, 80}, {63, 42, 21, 71}, {63, 46, 24, 0}, {63, 50, 27, 0},

   {6, 6, 3, 0}, {12, 12, 6, 87}, {18, 18, 9, 0}, {24, 24, 12, 0},
   {30, 30, 15, 0}, {36, 36, 18, 0}, {42, 42, 21, 0}, {48, 48, 24, 0},
   {54, 54, 27, 0}, {60, 60, 30, 0}, {63, 63, 34, 0}, {63, 63, 38, 0},
   {63, 63, 42, 0}, {63, 63, 46, 0}, {63, 63, 50, 0}, {63, 63, 54, 0},

   {3, 6, 6, 0}, {6, 12, 12, 0}, {9, 18, 18, 0}, {12, 24, 24, 0},
   {15, 30, 30, 0}, {18, 36, 36, 0}, {21, 42, 42, 0}, {24, 48, 48, 0},
   {27, 54, 54, 0}, {30, 60, 60, 0}, {34, 63, 63, 0}, {38, 63, 63, 0},
   {42, 63, 63, 0}, {46, 63, 63, 0}, {50, 63, 63, 0}, {63, 63, 63, 0}
};
// This line turns back on other/indent.pro indentation settings:
// *INDENT-ON*


const char *icon_files[NUM_TILESETS] = {
   "land.pcx", "newtown.pcx", "castle.pcx", "Incave.pcx", "village.pcx",
   "mount.pcx", "shrine.pcx", "fortress.pcx"
};

char map_fname[40], *strbuf;
char map_path[MAX_PATH] = "./";

/* Used for the icons */
short active_bound = 0;
short icon_set = 0;
short max_sets = 51;
short num_markers = 0;
int number_of_ents = 0;
short num_bound_boxes = 0;

s_map gmap;
s_entity gent[50];
s_show showing;

unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
unsigned char *z_map, *sh_map, *o_map, *cz_map, *csh_map, *co_map;
unsigned char *search_map;

s_bound bound_box[MAX_BOUNDS]; // WK - not redundant. Used on line 266

/*! \brief Blit to screen
 *
 * Since this is called repeatedly, make a function; if we ever have to make
 * changes to the way this blits to the screen, we'll only have to update it
 * once.
 */
void blit2screen (void)
{
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
}


void load_iconsets (PALETTE pal)
{
   int x, y;

   set_pcx (&pcx_buffer, icon_files[gmap.tileset], pal, 1);
   max_sets = (pcx_buffer->h / 16);

   for (y = 0; y < max_sets; y++)
      for (x = 0; x < ICONSET_SIZE; x++)
         blit (pcx_buffer, icons[y * ICONSET_SIZE + x], x * 16, y * 16, 0, 0,
               16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);
}


/*! \brief Load a map
 *
 * \param   filename File to load
 */
void load_map (const char *path)
{
   int p, q, i;
   PACKFILE *pf;
   char load_fname[PATH_MAX];

   ASSERT (load_fname);
   strcpy (load_fname, path);

   if (!exists (load_fname)) {
      replace_extension (load_fname, load_fname, "map", sizeof (load_fname));
      if (!exists (load_fname)) {
         error_load (load_fname);
         return;
      }
   }

   pf = pack_fopen (load_fname, F_READ_PACKED);
   if (!pf) {
      error_load (load_fname);
      return;
   }

   strcpy (map_path, load_fname);
   strcpy (map_fname, get_filename(load_fname));

   load_s_map (&gmap, pf);

   /* Recount the number of entities on the map */
   number_of_ents = 0;
   for (i = 0; i < 50; i++) {
      load_s_entity (gent + i, pf);
      if (gent[i].active == 1)
         number_of_ents = i + 1;
   }

   bufferize ();

   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         map[q * gmap.xsize + p] = pack_igetw (pf);
      }
   }
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         b_map[q * gmap.xsize + p] = pack_igetw (pf);
      }
   }
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         f_map[q * gmap.xsize + p] = pack_igetw (pf);
      }
   }

   pack_fread (z_map, (gmap.xsize * gmap.ysize), pf);
   pack_fread (sh_map, (gmap.xsize * gmap.ysize), pf);
   pack_fread (o_map, (gmap.xsize * gmap.ysize), pf);
   pack_fclose (pf);

   set_pcx (&pcx_buffer, icon_files[gmap.tileset], pal, 1);
   max_sets = (pcx_buffer->h / 16);

   load_iconsets (pal);

   /* Check for bogus map squares */
   for (p = 0; p < gmap.xsize * gmap.ysize; ++p) {
      /* Mid layer */
      if (map[p] > MAX_TILES)
         map[p] = 0;

      /* Background layer */
      if (b_map[p] > MAX_TILES)
         b_map[p] = 0;

      /* Foreground layer */
      if (f_map[p] > MAX_TILES)
         f_map[p] = 0;

      /* Obstacles layer */
      if (o_map[p] > MAX_OBSTACLES)
         o_map[p] = 0;

      /* Shadow layer */
      if (sh_map[p] >= MAX_SHADOWS)
         sh_map[p] = 0;
   }

   for (i = 0; i < MAX_TILES; i++)
      tilex[i] = i;
   for (i = 0; i < MAX_ANIM; i++)
      adata[i] = tanim[gmap.tileset][i];

   /* Note: try to use gmap.num_markers, rather than num_markers. It is bad
   programming practice to store the same variable in multiple places.
   mapdraw2 uses gmap.num_markers exclusively. */
   num_markers = gmap.num_markers;

   num_bound_boxes = gmap.num_bound_boxes;
   memcpy (bound_box, gmap.bound_box, gmap.num_bound_boxes * sizeof (s_bound));
}                               /* load_map () */


/*! \brief Check to see if a requested PCX file is available
 *
 * Another very useful function
 *
 * /param   pcx_buf Buffer to save the PCX image back into
 * /param   pcx_file PCX file to load
 * /param   pcx_pal Palette to set the image to
 * /param   critical If file cannot be found, exit the program with an error
 */
void set_pcx (BITMAP ** pcx_buf, const char *pcx_file, PALETTE pcx_pal,
              const int critical)
{
	char filename[PATH_MAX];
	sprintf(filename, "%s/data/%s", KQ_DATA, pcx_file);

   if (exists (filename))
      *pcx_buf = load_pcx (filename, pcx_pal);
   else {
      if (exists (pcx_file))
      	*pcx_buf = load_pcx (pcx_file, pcx_pal);
      else {
         sprintf (strbuf, "Could not find file: %s.\n", pcx_file);

         allegro_message ("%s", strbuf);
         if (critical) {
            /* This means that this file is critical to the program, so we need to
             * exit the program completely, as this cannot be recovered from.
             */
            cleanup ();
            exit (EXIT_FAILURE);
         }
      }
   }
}                               /* set_pcx () */


/*! \brief Cleanup scripts used in mapedit.c and mapdump.c */
void shared_cleanup (void)
{
   int i;

   destroy_bitmap (font6);
   destroy_bitmap (marker_image);
   destroy_bitmap (marker_image_active);
   for (i = 0; i < MAX_TILES; i++)
      destroy_bitmap (icons[i]);
   for (i = 0; i < MAX_SHADOWS; i++)
      destroy_bitmap (shadow[i]);
   for (i = 0; i < MAX_OBSTACLES; i++)
      destroy_bitmap (mesh1[i]);
}


/*! \brief Startup scripts used in mapedit.c and mapdump.c */
void shared_startup (void)
{
   int i, x, y;

   set_palette (pal);

	/* Give several variables sane defaults */

	map = b_map = f_map = c_map = cf_map = cb_map = 0;
	z_map = sh_map = o_map = cz_map = csh_map = co_map = 0;
	search_map = 0;
	gmap.markers = NULL;
	gmap.num_markers = num_markers = 0;


   /* Used for icons */
   for (i = 0; i < MAX_TILES; i++) {
      icons[i] = create_bitmap (16, 16);
      clear (icons[i]);
   }

   /* Used for Obstacles */
   /* Block all directions */
   mesh1[0] = create_bitmap (16, 16);
   clear (mesh1[0]);
   for (y = 0; y < 16; y += 2) {
      for (x = 0; x < 16; x += 2)
         putpixel (mesh1[0], x, y, 255);
      for (x = 1; x < 16; x += 2)
         putpixel (mesh1[0], x, y + 1, 255);
   }

   /* Block up */
   mesh1[1] = create_bitmap (16, 16);
   clear (mesh1[1]);
   hline (mesh1[1], 0, 0, 15, 255);
   vline (mesh1[1], 8, 0, 15, 255);

   /* Block right */
   mesh1[2] = create_bitmap (16, 16);
   clear (mesh1[2]);
   hline (mesh1[2], 0, 8, 15, 255);
   vline (mesh1[2], 15, 0, 15, 255);

   /* Block down */
   mesh1[3] = create_bitmap (16, 16);
   clear (mesh1[3]);
   hline (mesh1[3], 0, 15, 15, 255);
   vline (mesh1[3], 8, 0, 15, 255);

   /* Block left */
   mesh1[4] = create_bitmap (16, 16);
   clear (mesh1[4]);
   hline (mesh1[4], 0, 8, 15, 255);
   vline (mesh1[4], 0, 0, 15, 255);

   /* Used for Shadows */
   /* Shadows */
   set_pcx (&pcx_buffer, "Misc.pcx", pal, 1);
   for (i = 0; i < MAX_SHADOWS; i++) {
      shadow[i] = create_bitmap (16, 16);
      blit (pcx_buffer, shadow[i], i * 16, 160, 0, 0, 16, 16);
   }
   destroy_bitmap (pcx_buffer);

   /* Entity images */
   set_pcx (&pcx_buffer, "entities.pcx", pal, 1);
   for (x = 0; x < MAX_EPICS; x++) {
      for (i = 0; i < 12; i++) {
         eframes[x][i] = create_bitmap (16, 16);
         blit (pcx_buffer, eframes[x][i], i * 16, x * 16, 0, 0, 16, 16);
      }
   }
   destroy_bitmap (pcx_buffer);

   /* Create the marker image */
   marker_image = create_bitmap (16, 16);
   clear_bitmap (marker_image);
   vline (marker_image, 0, 0, 16, makecol (255, 255, 255));
   vline (marker_image, 1, 0, 16, makecol (192, 192, 192));
   rectfill (marker_image, 2, 0, 10, 8, makecol (255, 0, 0));

   /* Create the active marker image */
   marker_image_active = create_bitmap (16, 16);
   clear_bitmap (marker_image_active);
   vline (marker_image_active, 0, 0, 16, makecol (255, 255, 255));
   vline (marker_image_active, 1, 0, 16, makecol (192, 192, 192));
   rectfill (marker_image_active, 2, 0, 10, 8, makecol (0, 0, 255));
}


/*! \brief Save the whole map as a pcx
 *
 * Make one giant bitmap and draw all the layers on it, so you can get an
 * overview of what's going on.  Doesn't work well with parallax (any ideas
 * how to handle parallax?).
 * \author PH
 * \date 20030412
 * \updated 20040730 by TT
 *
 * \param   save_fname File to save the map to
 */
void visual_map (s_show showing, const char *save_fname)
{
   int i, j, w;
   BITMAP *bmp;

   /* Create a bitmap the same size as the map */
   bmp = create_bitmap (gmap.xsize * 16, gmap.ysize * 16);
   clear (bmp);

   for (j = 0; j < gmap.ysize; j++) {
      for (i = 0; i < gmap.xsize; i++) {
         /* Which tile is currently being evaluated */
         w = gmap.xsize * j + i;

         if (showing.layer[0])
            blit (icons[map[w]], bmp, 0, 0, i * 16, j * 16, 16, 16);
         if (showing.layer[1])
            draw_sprite (bmp, icons[b_map[w]], i * 16, j * 16);
         if (showing.layer[2])
            draw_sprite (bmp, icons[f_map[w]], i * 16, j * 16);
         if (showing.shadows)
            draw_trans_sprite (bmp, shadow[sh_map[w]], i * 16, j * 16);
         if (showing.obstacles && o_map[w] > 0)
            draw_sprite (bmp, mesh1[o_map[w] - 1], i * 16, j * 16);

         if ((showing.zones) && (z_map[w] > 0)) {
/* This check is here because of the differing versions of the Allegro library */
#ifdef HAVE_TEXT_EX
            if (z_map[w] < 10) {
               /* The zone's number is single-digit, center vert+horiz */
               textprintf_ex (bmp, font, i * 16 + 4, j * 16 + 4,
                              makecol (255, 255, 255), 0, "%d", z_map[w]);
            } else if (z_map[w] < 100) {
               /* The zone's number is double-digit, center only vert */
               textprintf_ex (bmp, font, i * 16, j * 16 + 4,
                              makecol (255, 255, 255), 0, "%d", z_map[w]);
            } else {
               /* The zone's number is triple-digit.  Print the 100's digit in
                * top-center of the square; the 10's and 1's digits on bottom
                * of the square
                */
               textprintf_ex (bmp, font, i * 16 + 4, j * 16,
                              makecol (255, 255, 255), 0, "%d",
                              (int) (z_map[w] / 100));
               textprintf_ex (bmp, font, i * 16, j * 16 + 8,
                              makecol (255, 255, 255), 0, "%02d",
                              (int) (z_map[w] % 100));
            }
#else
            if (z_map[w] < 10) {
               /* The zone's number is single-digit, center vert+horiz */
               textprintf (bmp, font, i * 16 + 4, j * 16 + 4,
                           makecol (255, 255, 255), "%d", z_map[w]);
            } else if (z_map[w] < 100) {
               /* The zone's number is double-digit, center only vert */
               textprintf (bmp, font, i * 16, j * 16 + 4,
                           makecol (255, 255, 255), "%d", z_map[w]);
            } else {
               /* The zone's number is triple-digit.  Print the 100's digit in
                * top-center of the square; the 10's and 1's digits on bottom
                * of the square
                */
               textprintf (bmp, font, i * 16 + 4, j * 16,
                           makecol (255, 255, 255), "%d",
                           (int) (z_map[w] / 100));
               textprintf (bmp, font, i * 16, j * 16 + 8,
                           makecol (255, 255, 255), "%02d",
                           (int) (z_map[w] % 100));
            }
#endif
         }

      }
   }

   /* Show entities */
   if (showing.entities) {
      for (i = 0; i < 50; i++) {
         if (gent[i].active) {
            if (gent[i].transl == 0) {
               draw_sprite (bmp, eframes[gent[i].chrx][gent[i].facing * 3],
                            gent[i].tilex * 16, gent[i].tiley * 16);
            } else {
               draw_trans_sprite (bmp,
                                  eframes[gent[i].chrx][gent[i].facing * 3],
                                  gent[i].tilex * 16, gent[i].tiley * 16);
            }                   // if..else ()
         }
      }
   }

   /* Show marker flags */
   if (showing.markers == 1 && gmap.num_markers > 0) {
      num_markers = gmap.num_markers;
      for (i = 0; i < num_markers; ++i) {
         draw_sprite (bmp, marker_image, gmap.markers[i].x * 16 + 8,
                      gmap.markers[i].y * 16 - 8);
      }
   }

   /* Show boundary boxes */
   if (showing.boundaries == 1) {
      for (i = 0; i < gmap.num_bound_boxes; i++) {
         rect (bmp, bound_box[i].x1 * 16, bound_box[i].y1 * 16,
               bound_box[i].x2 * 16 + 15, bound_box[i].y2 * 16 + 15, 24);
      }
   }

   get_palette (pal);
   save_bitmap (save_fname, bmp, pal);
   destroy_bitmap (bmp);
}                               /* visual_map () */
