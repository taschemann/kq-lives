-- Global functions available to all scripts

--! \name Progress indicators
-- PLEASE ADD TO THIS LIST IN ORDER!!!

P_START           = 0
P_ODDWALL         = 1
P_DARKIMPBOSS     = 2
P_DYINGDUDE       = 3
P_BUYCURE         = 4
P_GETPARTNER      = 5
P_PARTNER1        = 6
P_PARTNER2        = 7
P_SHOWBRIDGE      = 8
P_TALKDERIG       = 9
P_FIGHTONBRIDGE   = 10
P_FELLINPIT       = 11
P_EKLAWELCOME     = 12
P_LOSERONBRIDGE   = 13
P_ASLEEPONBRIDGE  = 14
P_ALTARSWITCH     = 15
P_KILLBLORD       = 16
P_GOBLINITEM      = 17
P_ORACLE          = 18
P_FTOTAL          = 19
P_FLOOR1          = 20
P_FLOOR2          = 21
P_FLOOR3          = 22
P_FLOOR4          = 23
--P_WSTONES         = 24
--P_BSTONES         = 25
P_WALL1           = 26
P_WALL2           = 27
P_WALL3           = 28
P_WALL4           = 29
P_DOOROPEN        = 30
P_DOOROPEN2       = 31
P_TOWEROPEN       = 32
P_DRAGONDOWN      = 33
P_TREASUREROOM    = 34
P_UNDEADJEWEL     = 35
P_UCOIN           = 36
P_CANCELROD       = 37
P_PORTALGONE      = 38
P_WARPEDTOT4      = 39
P_OLDPARTNER      = 40
P_BOUGHTHOUSE     = 41
P_TALKGELIK       = 42
P_OPALHELMET      = 43
P_FOUNDMAYOR      = 44
P_TALK_TEMMIN     = 45
P_EMBERSKEY       = 46
P_FOUGHTGUILD     = 47
P_GUILDSECRET     = 48
P_SEECOLISEUM     = 49
P_OPALSHIELD      = 50
P_STONE1          = 51
P_STONE2          = 52
P_STONE3          = 53
P_STONE4          = 54
P_DENORIAN        = 55
P_C4DOORSOPEN     = 56
P_DEMNASDEAD      = 57
P_FIRSTTIME       = 58
P_ROUNDNUM        = 59
P_BATTLESTATUS    = 60
P_USEITEMINCOMBAT = 61
P_FINALPARTNER    = 62
P_TALKGRAMPA      = 63
P_SAVEBREANNE     = 64
P_PASSGUARDS      = 65
P_IRONKEY         = 66
P_AVATARDEAD      = 67
P_GIANTDEAD       = 68
P_OPALBAND        = 69
P_BRONZEKEY       = 70
P_CAVEKEY         = 71
P_TOWN6INN        = 72
P_WARPSTONE       = 73
P_DOINTRO         = 74
P_GOTOFORT        = 75
P_GOTOESTATE      = 76
P_TALKBUTLER      = 77
P_PASSDOOR1       = 78
P_PASSDOOR2       = 79
P_PASSDOOR3       = 80
P_BOMB1           = 81
P_BOMB2           = 82
P_BOMB3           = 83
P_BOMB4           = 84
P_BOMB5           = 85
P_DYNAMITE        = 86
P_TALKRUFUS       = 87
P_EARLYPROGRESS   = 88
P_OPALDRAGONOUT   = 89
P_OPALARMOUR      = 90


-- /* These are to store who's waiting in the manor */
P_MANORPARTY      = 91
P_MANORPARTY1     = 92
P_MANORPARTY2     = 93
P_MANORPARTY3     = 94
P_MANORPARTY4     = 95
P_MANORPARTY5     = 96
P_MANORPARTY6     = 97
P_MANORPARTY7     = 98
P_MANOR           = 99
P_PLAYERS         = 100
P_TALK_AJATHAR    = 101
P_BLADE           = 102
P_AYLA_QUEST      = 103

P_BANGTHUMB       = 104
P_WALKING         = 105
P_MAYORGUARD1     = 106
P_MAYORGUARD2     = 107

P_TALK_TSORIN     = 108
P_TALK_CORIN      = 109
P_TALKOLDMAN      = 110
P_ORACLEMONSTERS  = 111
P_TRAVELPOINT     = 112

-- /* side quests */
P_SIDEQUEST1      = 113
P_SIDEQUEST2      = 114
P_SIDEQUEST3      = 115
P_SIDEQUEST4      = 116
P_SIDEQUEST5      = 117
P_SIDEQUEST6      = 118
P_SIDEQUEST7      = 119

-- Item identifiers (empty for now. Defined in itemdefs.h)

-- Special Item definitions

SI_UCOIN = 0
SI_CANCELROD = 1
SI_JADEPENDANT = 2
SI_UNDEADJEWEL = 3
SI_WHITESTONE = 4
SI_BLACKSTONE = 5
SI_EMBERSKEY = 6
SI_BRONZEKEY = 7
SI_DENORIANSTATUE = 8
SI_OPALHELMET = 9
SI_OPALSHIELD = 10
SI_IRONKEY = 11
SI_OPALBAND = 12
SI_OPALARMOUR = 13
SI_CAVEKEY = 14
SI_NOTE_TSORIN = 15
SI_NOTE_DERIG = 16
SI_RUSTYKEY = 17


-- Facing directions, HERO1 and HERO2, and the Hero identifiers are all found
-- in code. These are duplicates. Avoid changing them.

-- Facing directions
FACE_DOWN    = 0
FACE_UP      = 1
FACE_LEFT    = 2
FACE_RIGHT   = 3

-- Special identifiers for bubble()
HERO1             = 200
HERO2             = 201


-- Hero identifiers
SENSAR            = 0
SARINA            = 1
CORIN             = 2
AJATHAR           = 3
CASANDRA          = 4
TEMMIN            = 5
AYLA              = 6
NOSLOM            = 7


-- gettext alias
_ = gettext


-- Add this hero to the manor if not already there
-- hero can be a single value or a table
-- returns the number of heroes that were actually added
function add_to_manor(hero)
  local total, i

  if (not hero) then
    return 0
  end

  if (istable(hero)) then
    total = 0
    i = 1
    while (hero[i]) do
      total = total + add_to_manor(hero[i])
      i = i + 1
    end
    return total
  else
    if (hero < 0 or hero > 7) then
      return 0
    end
    for i = 0, 7 do
      if (get_progress(i + P_MANORPARTY) == (hero + 1)) then
        return 0
      end
    end
    for i = 0, 7 do
      if (get_progress(i + P_MANORPARTY) == 0) then
        set_progress(i + P_MANORPARTY, hero + 1)
        return 1
      end
    end
  end
end


-- Display bubble text; just concatenate all the args and call the _ex function
-- Args ent  Entity number
--      ...  Variable number of arguments - text to show
function bubble(ent, ...)
  s = ""
  for i = 1, arg.n do
    if (i ~= 1) then
      s = s.."\n"
    end
    s = s..arg[i]
  end
  bubble_ex(ent, s)
end


-- See function bubble()
function thought(ent, ...)
  s = ""
  for i = 1, arg.n do
    if (i ~= 1) then
      s = s.."\n"..arg[i]
    else
      s = s..arg[i]
    end
  end
  thought_ex(ent, s)
end


function get_quest_info()
   if LOC_add_quest_item then
      LOC_add_quest_item()
   end

   add_quest_item("About...", "This doesn't do much yet")
   add_quest_item("Test1", "Some test info")
   add_quest_item("Sensar", "He rages!")
end

-- backward compat
change_mapm = change_map


-- Checks if this ent is in the party, or in the manor,
-- or has never been recruited.
-- who: hero id
-- returns "manor" if in manor, "party" if in party, nil otherwise
function LOC_manor_or_party(who)
  local a
  if (get_pidx(0) == who) then
    return "party"
  elseif (get_numchrs() > 1 and get_pidx(1) == who) then
    return "party"
  end

  for a = P_MANORPARTY, P_MANORPARTY7 do
    if (get_progress(a) - 1 == who) then
      return "manor"
    end
  end
  return nil
end


-- Pick one of the args
-- If arg is a table it can have a pr field which gives
-- its probability of being picked
-- e.g. print(pick(1,2,3))
--      pick({pr = 5, name = "Rare"}, {pr = 95, name = "Common"}).name
function pick(...)
  cumprob = 0

  for i = 1, arg.n do
    if (istable(arg[i]) ) then
      prob = arg[i].pr or 1
    else
      prob = 1
    end

    cumprob = cumprob + prob
  end

  cumprob = krnd(cumprob)

  for i = 1, arg.n do
    if (istable(arg[i]) ) then
      prob = arg[i].pr or 1
    else
      prob = 1
    end

    cumprob = cumprob - prob

    if (cumprob < 0) then
      return arg[i]
    end
  end
end


-- Select from heroes in the manor
-- The available list is stored in eight consecutive P_ constants
-- as 0 for nobody and 1..8 for characters 0..7
function select_manor()
  -- Get the current list
  heroes = {}
  for i = 1, 8 do
    v = get_progress(i + P_MANORPARTY - 1)
    if (v ~= 0) then
      heroes[i] = v - 1
    end
  end
  -- Do the selecting
  heroes = select_team(heroes)
  -- Put back in the list
  for i = 1, 8 do
    if (heroes[i]) then
      v = heroes[i] + 1
    else
      v = 0
    end
    set_progress(i + P_MANORPARTY - 1, v)
  end
end


--  Response for reading a book.
function book_talk(ent)
  if (party[0] == Sensar) then
    bubble(HERO1, pick(_"Reading makes me sleepy...",
           _"So many books...",
           _"Reading is for wimps."))
  
  elseif (party[0] == Temmin) then
  bubble(HERO1, pick(_"If only I had more time...",
         _"So many books...",
         _"Some of these are pretty old."))
  
  elseif (party[0] == Sarina) then
  bubble(HERO1, pick(_"Ugh... this would take me forever to read.",
         _"I never liked reading.",
         _"Who wrote this trash?"))
  
  elseif (party[0] == Noslom) then
    bubble(HERO1, pick(_"Fascinating.",
           _"I have this one.",
           _"Romance novels... gack!"))

  elseif (party[0] == Ajathar) then
    bubble(HERO1, pick(_"Hmmm... I don't approve of that.",
           _"I'm too busy to read now.",
           _"How many books can you write that start with 'The Joy of...'?"))

  elseif (party[0] == Ayla) then
    bubble(HERO1, pick(_"I don't have time for this.",
          _"What language is this written in?",
          _"The pages are stuck together!?"))
	
  elseif (party[0] == Casandra) then
    bubble(HERO1, pick(_"Boring.",
           _"Somebody should burn these.",
           _"Terrible... just terrible."))
	
  elseif (party[0] == Corin) then
	 bubble(HERO1, pick(_"Doesn't anybody leave spellbooks lying around?",
           _"Why would I read this?",
           _"Can't talk... reading."))
  else
    msg("Script Error. global.lua:book_talk()", 255, 0)
  end

end


-- This function can be called whenever the hero touches a fire
function touch_fire(ent)
  local x
  
  if (party[0] == Sensar) then
    bubble(HERO1, pick(_"What th..? Ouch! That's hot!",
           _"There's no way I'm sticking my hand in that fire!",
           _"This feels pretty nice."))
           
  elseif (party[0] == Temmin) then
    bubble(HERO1, pick(_"Ah, the age-old fire.",
           _"This needs more coal.",
           _"This would be great to read a book next to."))

  elseif (party[0] == Sarina) then
    bubble(HERO1, pick(_"Mmm, wood smoke.",
           _"Smells like burnt hair. Hey wait... that's MY hair!",
           _"Ooh, cozy."))

  elseif (party[0] == Noslom) then
    bubble(HERO1, pick(_"I prefer torches.",
           _"I love the crackle of a good fire.",
           _"I wonder if a spell would make this burn brighter?"))

  elseif (party[0] == Ajathar) then
    bubble(HERO1, pick(_"Hmm... I want marshmallows.",
           _"You call this a fire?!",
           _"Ah, relaxing."))

  elseif (party[0] == Ayla) then
    bubble(HERO1, pick(_"I wonder how hot this is?",
           _"Someone should clean all this soot out of here.",
           _"Well, my face is warm now, but my butt is still freezing!"))
  
  elseif (party[0] == Casandra) then
    bubble(HERO1, pick(_"Something's burning. I hope it's one of those stupid books!",
           _"The fire is getting low.",
           _"Yessir, this is a fire."))

  elseif (party[0] == Corin) then
    bubble(HERO1, pick(_"I sure like fire.",
           _"Watching this is relaxing.",
           _"This is making me sleepy."))
           
  else
    msg("Script Error. global.lua:touch_fire()", 255, 0)
  end
  
end


