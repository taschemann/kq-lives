SUBDIRS = intl po scripts
bin_PROGRAMS = kq mapdraw mapdiff mapdump $(MAPDRAW2)
EXTRA_PROGRAMS = mapdraw2

kq_SOURCES = src/enemyc.c src/intrface.c src/sgame.c src/bounds.c \
	src/combat.c src/entity.c src/movement.c src/shopmenu.c \
	src/credits.c src/eqpmenu.c src/itemmenu.c src/music.c src/timing.c \
	src/disk.c src/eskill.c src/kq.c  src/res.c \
	src/fade.c src/magic.c src/scrnshot.c \
	src/draw.c src/heroc.c src/masmenu.c src/selector.c \
	src/effects.c src/hskill.c src/menu.c  src/setup.c src/console.c\
	include/combat.h include/fade.h include/masmenu.h include/setup.h \
	include/credits.h include/heroc.h include/menu.h include/sgame.h \
	include/disk.h include/hskill.h include/movement.h include/shopmenu.h \
	include/draw.h include/intrface.h include/mpcx.h include/skills.h \
	include/effects.h include/itemdefs.h include/music.h include/ssprites.h \
	include/enemyc.h include/itemmenu.h include/structs.h \
	include/entity.h include/kq.h include/res.h include/timing.h \
	include/eqpmenu.h include/kqsnd.h include/scrnshot.h \
	include/eskill.h include/magic.h include/selector.h \
	include/bounds.h include/gettext.h include/console.h

EXTRA_kq_SOURCES = src/unix.c src/osx.m src/beos.c src/win.c src/dos.c src/winres.rc
kq_LDADD = $(kq_extra_files) $(ALLEGRO_LIBS) $(LIBINTL)
kq_DEPENDENCIES = $(kq_extra_files)

mapdraw_SOURCES = maps/mapdata.c maps/mapdraw.h maps/mapedit.c \
	maps/mapfile.c maps/mapent.c maps/mapshared.c src/disk.c \
	maps/mapstructs.c src/bounds.c include/disk.h include/structs.h
mapdraw_LDADD = $(ALLEGRO_LIBS) $(LIBINTL)

mapdraw2_SOURCES = maps/mapdraw2.c maps/mapdraw2.h \
	maps/mainwindow.c maps/mapedit2.c maps/mapedit2.h \
	maps/mapshared.c maps/mapdraw.h src/disk.c \
        src/bounds.c include/disk.h include/structs.h
mapdraw2_LDADD = $(GTK_LIBS) $(ALLEGRO_LIBS) $(LIBINTL)
mapdraw2_CPPFLAGS = $(AM_CPPFLAGS) $(GTK_CFLAGS)

mapdiff_SOURCES = maps/mapdraw.h maps/mapdiff.c src/disk.c \
	include/disk.h include/structs.h
mapdiff_LDADD = $(ALLEGRO_LIBS) $(LIBINTL)

mapdump_SOURCES = maps/mapdump.c maps/mapshared.c maps/mapdata.c \
	src/disk.c src/bounds.c include/disk.h include/structs.h
mapdump_LDADD = $(ALLEGRO_LIBS) $(LIBINTL)

APPR=KQ.app/Contents/Resources
KQ.app: kq
	install -d $(APPR)/data $(APPR)/music $(APPR)/scripts $(APPR)/maps
	install -d KQ.app/Contents/MacOS
	install -C $(top_srcdir)/Mac/Info.plist KQ.app/Contents
	install -C kq KQ.app/Contents/MacOS
	install -C $(addprefix $(top_srcdir)/,$(dist_gfx_DATA)) $(APPR)/data
	install -C $(addprefix $(top_srcdir)/,$(dist_map_DATA)) $(APPR)/maps
	install -C $(addprefix $(top_srcdir)/,$(dist_music_DATA)) $(APPR)/music
	install -C $(top_srcdir)/Mac/KQ-icon.icns $(APPR)/KQ.icns
	$(MAKE) -C scripts $@

winres.o: winres.rc
	windres $< -o $@

dist_map_DATA = maps/starting.map maps/bridge2.map \
	maps/estate.map maps/temple1.map maps/bridge.map \
	maps/temple2.map maps/camp.map maps/fort.map maps/tower.map \
	maps/cave1.map maps/goblin.map maps/town1.map maps/cave2.map \
	maps/grotto2.map maps/town2.map maps/cave3a.map maps/grotto.map \
	maps/town3.map maps/cave3b.map maps/guild.map maps/town4.map \
	maps/cave4.map maps/main.map maps/town5.map maps/manor.map maps/town6.map \
	maps/cave5.map maps/town7.map maps/cave6a.map maps/town8.map \
	maps/cave6b.map  maps/coliseum.map maps/pass.map \
	maps/cult.map maps/dville.map maps/sunarin.map

mapdir = $(pkgdatadir)/maps

EXTRA_DIST = config.rpath Mac/Info.plist Mac/KQ-icon.icns \
        maps/mapdraw2.glade other/kq.ico other/luaxgettext

dist_gfx_DATA = data/kqsnd.dat data/mpcx.dat data/ssprites.dat \
	data/hero.kq data/tileset.kq data/allstat.mon data/resabil.mon

gfxdir = $(pkgdatadir)/data

dist_music_DATA = music/aa_arofl.xm music/exp.s3m music/oxford.s3m \
	music/walk.s3m music/comeback.mod music/hm.s3m music/rain.s3m \
	music/waterw.xm music/dag_4.xm music/infanita.mod music/rend5.s3m \
	music/enfero.xm music/into61.s3m music/toroia.s3m music/eranasp.mod \
	music/land.mod music/town.mod

musicdir = $(pkgdatadir)/music

dist_man_MANS = debian/kq.6

AM_CPPFLAGS = $(WARNING_CFLAGS) -I$(top_srcdir)/include \
	-DKQ_BIN=\"$(bindir)/kq$(EXEEXT)\" \
	-DKQ_DATA=\"$(pkgdatadir)\" \
	-DKQ_LIB=\"$(pkglibdir)\" \
	-DKQ_LOCALE=\"$(localedir)\" \
	-Iintl -I$(top_srcdir)/intl

if BUNDLE
all-local: KQ.app
endif BUNDLE

ACLOCAL_AMFLAGS = -I m4
