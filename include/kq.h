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
 * \brief Main include file for KQ
 * \author JB
 * \date ??????
 */

#ifndef __KQ_H
#define __KQ_H

#include <allegro.h>

#ifdef HAVE_CONFIG_H
/* Have to undef some stuff because Allegro defines it - thanks guys
*/
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_BUGREPORT
#include "config.h"
#endif

#ifdef __GNUC__
#define NORETURN __attribute__((noreturn))
#else
#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif /* MSVC */
#endif /* GNUC */


#include "gettext.h"
#define _(s) gettext(s)

#include "entity.h"
#include "structs.h"
#include "bounds.h"
#include "shopmenu.h"

#define MAX_TILES        1024
#define MAXE               41
#define MAXCHRS             8
#define PSIZE               2
#define MAXFRAMES          12
#define MAXEFRAMES         12
/* PH: MAXCFRAMES never seems to be used properly -
   all frames are identical for enemies?
 */
#define MAXCFRAMES          8
#define MAX_TILESETS        8
#define NUM_FIGHTERS        7
#define NUM_STATS          13
#define MISS             9999
#define NODISPLAY        9998
#define SEL_ALL_ALLIES   9997
#define SEL_ALL_ENEMIES  9996
#define SCANALL          9995
#define NO_STS_CHECK     9994
#define CURE_CHECK       9993
#define MAX_SHADOWS        12
#define MAX_INV            64
/* Maximum number of special items allowed in the world */
#define MAX_SPECIAL_ITEMS  50
#define MAX_PLAYER_SPECIAL_ITEMS 20
#define STEPS_NEEDED       15
#define SIZE_PROGRESS    1750
#define SIZE_SAVE_RESERVE1 150
#define SIZE_SAVE_SPELL    50
#define SIZE_TREASURE    1000


/* Get the directory for application data (music, gfx, etc.)
   or user data (screenshots, prefs, saved games)
 */
const char *kqres (int dir_id, const char *file);
#define DATA_DIR       0
#define MAP_DIR        1
#define SAVE_DIR       2
#define MUSIC_DIR      3
#define SCRIPT_DIR     4
#define SETTINGS_DIR   5

#define PCX_DATAFILE   kqres(DATA_DIR, "mpcx.dat")
#define SPELL_DATAFILE kqres(DATA_DIR, "ssprites.dat")
#define SOUND_DATAFILE kqres(DATA_DIR, "kqsnd.dat")


/*! \name Map layer setters */
/*\{*/
#define set_btile(tx,ty,t) map_seg[ty*g_map.xsize+tx] = t
#define set_mtile(tx,ty,t) b_seg[ty*g_map.xsize+tx] = t
#define set_ftile(tx,ty,t) f_seg[ty*g_map.xsize+tx] = t
#define set_zone(tx,ty,t) z_seg[ty*g_map.xsize+tx] = t
#define set_obs(tx,ty,t) o_seg[ty*g_map.xsize+tx] = t
#define set_shadow(tx,ty,t) s_seg[ty*g_map.xsize+tx] = t
/*\}*/

/*! \name Attributes of characters */
/*  These are the stats when you check your
 *  characters stats (on the left)
 */
/*\{*/
#define A_STR 0                 // Strength
#define A_AGI 1                 // Agility
#define A_VIT 2                 // Vitality
#define A_INT 3                 // Intellect
#define A_SAG 4                 // Sagacity
#define A_SPD 5                 // Speed
#define A_AUR 6                 // Aura
#define A_SPI 7                 // Spirit
#define A_ATT 8                 // Attack
#define A_HIT 9                 // Hit
#define A_DEF 10                // Defense
#define A_EVD 11                // Evade
#define A_MAG 12                // Mag.Def
/*\}*/

/*! \name Spells */
/*\{*/
#define S_POISON   0
#define S_BLIND    1
#define S_CHARM    2
#define S_STOP     3
#define S_STONE    4
#define S_MUTE     5
#define S_SLEEP    6
#define S_DEAD     7
#define S_MALISON  8
#define S_RESIST   9
#define S_TIME     10
#define S_SHIELD   11
#define S_BLESS    12
#define S_STRENGTH 13
#define S_ETHER    14
#define S_TRUESHOT 15
#define S_REGEN    16
#define S_INFUSE   17
/*\}*/

/*! \name Special combat skills */
/*\{*/
#define C_ATTACK   1
#define C_COMBO    2
#define C_SPELL    3
#define C_SKILL    4
#define C_DEFEND   5
#define C_INVOKE   6
#define C_ITEM     7
#define C_RUN      8
/*\}*/

/*! \name Runes/Resistances */
/*  These are what your strengths and weaknesses to certain
 *  elements and elemental attacks.  This can be a negative
 *  value (lots of damage), 0 (neutral), or a positive value
 *  (very little damage).
 */
/*\{*/
#define R_EARTH     0
#define R_BLACK     1
#define R_FIRE      2
#define R_THUNDER   3
#define R_AIR       4
#define R_WHITE     5
#define R_WATER     6
#define R_ICE       7
#define R_POISON    8
#define R_BLIND     9
#define R_CHARM     10
#define R_PARALYZE  11
#define R_PETRIFY   12
#define R_SILENCE   13
#define R_SLEEP     14
#define R_TIME      15
#define R_NONE      16
/*\}*/

/*! \name Weapons */
/*\{*/
#define W_BASH         0
#define W_MACE         1
#define W_HAMMER       2
#define W_SWORD        3
#define W_AXE          4
#define W_KNIFE        5
#define W_SPEAR        6
#define W_ROD          7
#define W_STAFF        8
#define W_SBOOK        27
#define W_ABOOK        28
#define W_CHENDIGAL    29
/*\}*/

/*!\name Use modes
 * Specify how an item can be used.
 */
/*\{*/
#define USE_NOT          0
#define USE_ANY_ONCE     1
#define USE_ANY_INF      2
#define USE_CAMP_ONCE    3
#define USE_CAMP_INF     4
#define USE_COMBAT_ONCE  5
#define USE_COMBAT_INF   6
#define USE_ATTACK       7
#define USE_IMBUED       8
/*\}*/

/*! \name Weapon/Spell targeting modes */
/*\{*/
#define TGT_CASTER        -1
#define TGT_NONE          0
#define TGT_ALLY_ONE      1
#define TGT_ALLY_ONEALL   2
#define TGT_ALLY_ALL      3
#define TGT_ENEMY_ONE     4
#define TGT_ENEMY_ONEALL  5
#define TGT_ENEMY_ALL     6
/*\}*/

/*! \name Facing directions */
/*\{*/
#define FACE_DOWN    0
#define FACE_UP      1
#define FACE_LEFT    2
#define FACE_RIGHT   3
/*\}*/

/*! \name Map identifiers */
/* I removed these. The less extra information that has to be maintained,
 * the better. --Winter Knight */

/*! \brief Player */
typedef struct
{
   char name[9];                /*!< Entity name */
   int xp;                      /*!< Entity experience */
   int next;                    /*!< Experience needed for level-up */
   int lvl;                     /*!< Entity's level */
   int mrp;                     /*!< Magic use rate (0-100) */
   int hp;                      /*!< Hit points */
   int mhp;                     /*!< Maximum hit points */
   int mp;                      /*!< Magic points */
   int mmp;                     /*!< Maximum magic points */
   int stats[NUM_STATS];
   char res[16];
   unsigned char sts[24];
   unsigned char eqp[6];        /*!< Weapons, armor, etc. equipped */
   unsigned char spells[60];    /*!< Known spells */
}
s_player;

/*! \brief Hero information
 *
 * This holds static or constant information about a hero. PH: It's not fully used yet
 * the intention is to cut down on some of those globals.
 */
typedef struct
{
   s_player plr;                /*!< all other statistics */
   BITMAP *portrait;            /*!< The hero's portrait for the stats screen */
   BITMAP *frames[MAXFRAMES];   /*!< Frames for movement */
   BITMAP *cframes[MAXCFRAMES]; /*!< Frames for combat */
   int xpi, bxp, hpi, mpi;      /*!< for level_up() */
   int stat_mult[NUM_STATS];    /*!<stats multipliers for level calculations (see player2fighter() ) */
}
s_heroinfo;

/*! \brief Fighter
 *
 * s_player is transformed into a s_fighter during combat.
 * See enemy_init() for more information on the fields.
 */
typedef struct
{
   char name[25];               /*!<\brief Name */
   int xp;                      /*!<\brief eXperience Points */
   int gp;                      /*!<\brief Gold Points */
   int lvl;                     /*!<\brief LeVeL */
   int cx;                      /*!<\brief x-coord of image in datafile */
   int cy;                      /*!<\brief y-coord of image in datafile */
   int cw;                      /*!<\brief width in datafile */
   int cl;                      /*!<\brief height in datafile */
   int hp;                      /*!<\brief Hit Points */
   int mhp;                     /*!<\brief Max Hit Points */
   int mp;                      /*!<\brief Magic Points */
   int mmp;                     /*!<\brief Max Magic Points */
   int dip;                     /*!<\brief Defeat Item Probability
                                 * Probability in % that the enemy will yield an item when defeated.
                                 */
   int defeat_item_common;      /*!<\brief Defeat Item Common
                                 * If the enemy yields an item, you will get this item 95% of the time.
                                 */
   int defeat_item_rare;        /*!<\brief Defeat Item Rare
                                 * If the enemy yields an item, you will get this item 5% of the time.
                                 */
   int steal_item_common;       /*!<\brief Steal Item Common
                                 * If Ayla steals something, she will get this item 95% of the time.
                                 */
   int steal_item_rare;         /*!<\brief Steal Item Rare
                                 * If Ayla steals something, she will get this item 5% of the time.
                                 */
   int stats[NUM_STATS];        /*!<\brief See A_* constants in kq.h */
   char res[16];                /*!<\brief See R_* constants in kq.h */
   unsigned char facing;        /*!<\brief Direction character's sprite faces */
   unsigned char aframe;        /*!<\brief Battle sprite to display (standing, casting, attacking) */
   unsigned char crit;
   unsigned char sts[24];
   unsigned char defend;
   unsigned char ai[8];
   unsigned char aip[8];
   unsigned char atrack[8];
   int csmem;
   int ctmem;
   int cwt;                     /*!< \brief Current Weapon Type
                                 * The shape of the currently held weapon (sword, dagger, axe etc) \sa hero_init()
                                 */
   int welem;                   /*!<\brief Which Element type (sick, fire, water, etc.) */
   int unl;                     /*!<\brief UNLiving (undead), like zombies, skeletons, etc. */
   int aux;
   int bonus;
   int bstat;
   int mrp;
   int imb_s;
   int imb_a;
   int imb[2];
   BITMAP *img;
}
s_fighter;


/*! \brief Special Items
 *
 * Contains a list of the special items in the player's party (Opal Armor et al)
 */
typedef struct
{
	char name[38];
	char description[40];
	short icon;
} s_special_item;


void change_map (const char *, int, int, int, int);   /*  intrface.c, magic.c  */
void change_mapm (const char *, const char *, int, int);      /*  intrface.c */
void readcontrols (void);       /*  everywhere ;)  */
void calc_viewport (int);       /*  entity.c, intrface.c  */
void zone_check (void);         /*  entity.c  */
void warp (int, int, int);      /*  only in intrface.c  */
void check_animation (void);    /*  draw.c, intrface.c  */
void activate (void);           /*  only in entity.c  */
void unpress (void);            /*  everywhere ;)  */
void wait_enter (void);         /*  everywhere ;)  */
void klog (const char *);             /*  draw.c, intrface.c, magic.c, setup.c  */
void init_players (void);       /*  sgame.c  */
void kwait (int);               /*  intrface.c  */
NORETURN void program_death (const char *);    /*  everywhere ;)  */
int in_party (int);             /*  combat.c, intrface.c  */
void wait_for_entity (int, int);        /*  intrface.c  */
char *get_timer_event (void);   /*  entity.c, kq.c  */
int add_timer_event (const char *, int);        /*  intrface.c  */
void reset_timer_events (void); /*  intrface.c  */
void reset_world(void);         /*  sgame.c  */



extern char curmap[16];         /*  sgame.c, draw.c, magic.c */

extern int right;               /*  intrface.c, (eqp|item|mas|shop)menu.c,  */
                        /*  entity.c, menu.c, selector.c, setup.c,  */
                        /*  heroc.c                                 */

extern int left;                /*  intrface.c, (eqp|item|mas|shop)menu.c,  */
                        /*  entity.c, menu.c, selector.c, setup.c,  */
                        /*  heroc.c                                 */

extern int up;                  /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  heroc.c, intrface.c, menu.c, entity.c,  */
                        /*  setup.c, sgame.c, draw.c                */

extern int down;                /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  heroc.c, intrface.c, menu.c, entity.c,  */
                        /*  setup.c, sgame.c, draw.c                */

extern int besc;                /*  intrface.c, setup.c, sgame.c            */

extern int benter;              /*  entity.c, intrface.c, setup.c, sgame.c  */

extern int balt;                /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  heroc.c, intrface.c, menu.c, setup.c,   */
                        /*  draw.c, sgame.c                         */

extern int bctrl;               /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  sgame.c, heroc.c, intrface.c, setup.c,  */
                        /*  menu.c                                  */

extern int bhelp;
extern int kright;              /*  setup.c, sgame.c  */
extern int kleft;               /*  setup.c, sgame.c  */
extern int kup;                 /*  setup.c, sgame.c  */
extern int kdown;               /*  setup.c, sgame.c  */
extern int kesc;                /*  setup.c, sgame.c  */
extern int kenter;              /*  setup.c, sgame.c  */
extern int kalt;                /*  setup.c, sgame.c  */
extern int kctrl;               /*  setup.c, sgame.c, entity.c  */
extern int jbalt;               /*  setup.c, sgame.c  */
extern int jbctrl;              /*  setup.c, sgame.c  */
extern int jbenter;             /*  setup.c, sgame.c  */
extern int jbesc;               /*  setup.c, sgame.c  */
extern int vx, vy, mx, my, steps, lastm[PSIZE];

extern BITMAP *double_buffer, *fx_buffer;
extern BITMAP *map_icons[MAX_TILES];


extern BITMAP *back, *tc, *tc2, *bub[8], *b_shield, *b_shell, *b_repulse,
   *b_mp;
extern BITMAP *cframes[NUM_FIGHTERS][MAXCFRAMES],
   *tcframes[NUM_FIGHTERS][MAXCFRAMES], *frames[MAXCHRS][MAXFRAMES];
extern BITMAP *eframes[MAXE][MAXEFRAMES], *pgb[9], *sfonts[5], *bord[8];
extern BITMAP *menuptr, *mptr, *sptr, *stspics, *sicons, *bptr, *missbmp,
   *noway, *upptr, *dnptr;
extern BITMAP *shadow[MAX_SHADOWS];     /*  draw.c  */
extern unsigned short *map_seg;
extern unsigned short *b_seg, *f_seg;
extern unsigned char *z_seg, *s_seg, *o_seg;
extern unsigned char progress[SIZE_PROGRESS];
extern unsigned char treasure[SIZE_TREASURE];
extern unsigned short shop_time[NUMSHOPS];
extern unsigned char save_spells[SIZE_SAVE_SPELL];
extern BITMAP *kfonts, *portrait[MAXCHRS];
extern s_map g_map;
extern s_entity g_ent[MAX_ENT + PSIZE];
extern s_anim tanim[MAX_TILESETS][MAX_ANIM];
extern s_anim adata[MAX_ANIM];
extern int noe, pidx[MAXCHRS], numchrs, gp, xofs, yofs, gsvol, gmvol;
extern unsigned char autoparty, alldead, is_sound, deadeffect, vfollow,
   use_sstone, sound_avail;
extern const unsigned char kq_version;
extern unsigned char hold_fade, cansave, skip_intro, wait_retrace,
   windowed, stretch_view, cpu_usage;
extern unsigned short tilex[MAX_TILES], adelay[MAX_ANIM];
extern char *strbuf, *savedir;
extern s_player party[MAXCHRS];
extern s_heroinfo players[MAXCHRS];
extern s_fighter fighter[NUM_FIGHTERS];
extern s_fighter tempa, tempd;
extern int noi, shin[12], dct;
extern char sname[39], ctext[39];
extern volatile int timer, ksec, kmin, khr, animation_count, timer_count;
extern unsigned short lup[MAXCHRS][20];
extern COLOR_MAP cmap;
extern unsigned char can_run, display_desc;
extern unsigned char draw_background, draw_middle, draw_foreground,
   draw_shadow;
extern unsigned short g_inv[MAX_INV][2];
extern s_special_item special_items[MAX_SPECIAL_ITEMS];
extern short player_special_items[MAX_SPECIAL_ITEMS];
extern int view_x1, view_y1, view_x2, view_y2, view_on, in_combat;
extern int show_frate, use_joy;

extern int cheat;
extern int no_random_encounters;
extern int every_hit_999;
extern int no_monsters;
extern int warx, wary;

/*! Yield processor to other tasks */
void kq_yield (void);

#ifdef DEBUGMODE
extern BITMAP *obj_mesh;
#endif


#endif /*  __KQ_H  */
