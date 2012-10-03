# Makefile to compile all .lua into .lob
# They are preprocessed first into .plua and then compiled.  This
# two-step process allows luac to give better error messages, though
# the line numbers will refer to the versions in pscripts rather than
# the original scripts

CPP=cpp -P -undef

.plua.lob:
	luac -o $@ $<

.lua.plua: $(top_srcdir)/include/itemdefs.h
	$(CPP) -include $(top_srcdir)/include/itemdefs.h $< > $@

KQ.app:
	install -C $(cscripts_DATA) ../$@/Contents/Resources/scripts

cscripts_DATA = bridge2.lob dville.lob \
	starting.lob bridge.lob estate.lob \
	temple1.lob camp.lob esteria.lob \
	temple2.lob cave1.lob fort.lob \
	tower.lob cave2.lob global.lob \
	town1.lob cave3a.lob goblin.lob \
	town2.lob cave3b.lob grotto2.lob \
	town3.lob cave4.lob grotto.lob \
	town4.lob cave5.lob guild.lob \
	town5.lob cave6a.lob main.lob \
	town6.lob cave6b.lob manor.lob \
	town7.lob town8.lob coliseum.lob \
	pass.lob tunnel.lob cult.lob \
	shrine.lob sunarin.lob init.lob
MOSTLYCLEANFILES = $(cscripts_DATA)
cscriptsdir = $(pkglibdir)/scripts

EXTRA_DIST = bridge2.lua dville.lua \
	starting.lua bridge.lua estate.lua \
	temple1.lua camp.lua esteria.lua \
	temple2.lua cave1.lua fort.lua \
	tower.lua cave2.lua global.lua \
	town1.lua cave3a.lua goblin.lua \
	town2.lua cave3b.lua grotto2.lua \
	town3.lua cave4.lua grotto.lua \
	town4.lua cave5.lua guild.lua \
	town5.lua cave6a.lua main.lua \
	town6.lua cave6b.lua manor.lua \
	town7.lua town8.lua coliseum.lua \
	pass.lua tunnel.lua cult.lua \
	shrine.lua sunarin.lua init.lua 
