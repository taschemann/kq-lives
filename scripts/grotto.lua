-- grotto - "Small forest grotto north of Ekla"

-- /*
-- {
-- P_FELLINPIT: Set when you fall down the pit
--   (0) Haven't fallen down the pit
--   (1) Fell into the pit, Derig helped you out
--   (2) Can't fall down the pit since Derig isn't there to help you back out
--   (3) You may fall down the pit again
--
-- P_TALKDERIG: Set when you make contact with Derig
--   (0) If you've never entered the grotto (this will be set to 1 immediately when you enter)
--   (1) Set as soon as you've been to the grotto at least once
--   (2) Fell into the pit before, DIDN'T meet Derig (but he helped you out)
--   (3) Fell into the pit before, DID meet Derig (and he helped you out)
--   (4) Derig is taking you back to Ekla to meet Jen, his granddaughter (you change_map() immediately after this is set to '4')
--   (5) Derig is still in Ekla
--   (6) Derig is back, you have sealed the portal
--
-- P_TALK_TSORIN: If you've spoken to Tsorin in Andra (and got his seal)
--   (0) You haven't spoken to him yet
--   (1) Tsorin gave you a note to give to Derig
--   (2) Derig gave you a note to return to Tsorin
--   (3) Tsorin gave you his seal to get through the fort
--   (4) You've shown the seal to the guards at the fort
--   (5) You are free pass through the fort anytime (no contention in goblin lands)
--
-- P_UCOIN: Spoke to Jen, the granddaughter in Ekla
--   (0) Have not yet spoken with Jen
--   (1) Spoke to Jen
--   (2) Jen gave you the Unadium Coin
--   (3) Returned the Unadium Coin to Jen
-- }
-- */


function autoexec()
  local x, y
  -- You have fallen in the pit at least once; the hole is showing on the map
  if (get_progress(P_FELLINPIT) > 0) then
    x, y = marker("by_pit")
    set_btile(x - 1, y, 153)
  end

  -- // You've entered the grotto at least once
  if (get_progress(P_TALKDERIG) == 0) then
    set_progress(P_TALKDERIG, 1)
  end

  -- Determine if the fire should be lit or not
  if (get_progress(P_TALKDERIG) < 2) then
    x, y = marker("by_fire")
    set_mtile(x + 1, y, 154)
    set_zone(x + 1, y, 0)
  end

  -- Allows you to fall down the pit once Derig is back in the Grotto
  if (get_progress(P_TALKDERIG) == 6) then
    set_progress(P_FELLINPIT, 3)
  end

  -- // There will be some times when the player shouldn't go down in the pit
  if (get_progress(P_FELLINPIT) == 2) then
    x, y = marker("by_pit")
    set_obs(x - 1, y, 1)
  end

  -- Determine if Derig should be showing or not
  if (get_progress(P_TALKDERIG) == 3 or
      get_progress(P_TALKDERIG) == 6) then
    -- Derig WILL be in the Grotto in these two instances
  else
    -- // He won't be here unless Tsorin told you to find him here
    if (get_progress(P_TALK_TSORIN) == 0 or
        get_progress(P_TALK_TSORIN) > 2) then
      set_ent_active(0, 0)
    else
      -- Light the fire if Derig is next to it
      x, y = marker("by_fire")
      set_mtile(x + 1, y, 154)
      set_zone(x + 1, y, 0)
    end
  end

  refresh()
end


function entity_handler(en)
  if (en == 0) then
    -- We've never spoken to Tsorin, or we've finished his quest already
    if (get_progress(P_TALK_TSORIN) == 0 or
        get_progress(P_TALK_TSORIN) > 2) then
      -- TALKDERIG will always == 3 the first time you talk to him
      if (get_progress(P_TALKDERIG) == 3) then
        if (get_progress(P_TALK_TSORIN) > 2) then
          -- You have spoken to Derig because of Tsorin
          bubble(en, _"Thank you for your assistance, $0. I have another request for you.")
        else
          -- You have never spoken to him before
          bubble(en, _"Hello, I'm Derig. I presume that my granddaughter sent you here?")
        end
        bubble(en, _"Let's go back to town.")
        set_progress(P_FELLINPIT, 2)
        set_progress(P_TALKDERIG, 4)
        change_map("town1", "by_derig")
      elseif (get_progress(P_TALKDERIG) == 6) then
        -- // Derig would only be here after you've finished with the Rod of Cancellation
        bubble(en, _"Good job with the portal. I have returned the Rod of Cancellation.")
      end
    elseif (get_progress(P_TALK_TSORIN) == 1) then
      bubble(en, _"That note you're carrying... I recognize the seal on there; let me see it!")
      msg(_"You show Derig the note.", 255, 0)
      bubble(en, _"$0, Tsorin says that a special treasure of the goblins, the Oracle Statue, has been stolen.")
      bubble(en, _"Apparently, the goblins are so upset that he's sealed off any entrance to the goblin lands until this is resolved.")
      if (get_numchrs() == 1) then
        bubble(HERO1, _"So what am I supposed to do?")
      else
        bubble(HERO1, _"So what are we supposed to do?")
      end
      bubble(en, _"I agree that you must get through there. Here, deliver this note to Tsorin so he'll let you proceed.")
      msg(_"Derig hands you a sealed envelope.", 18, 0)
      set_progress(P_TALK_TSORIN, 2)
      remove_special_item(SI_NOTE_TSORIN)
      add_special_item(SI_NOTE_DERIG)
      set_progress(P_TALKDERIG, 3)
    elseif (get_progress(P_TALK_TSORIN) == 2) then
      bubble(en, _"You must deliver that note to Tsorin so he'll let you through the fort.")
    else
      bubble(en, _"Good luck.")
    end
  end
end


function postexec()
  return
end


function refresh()
  -- Treasure on NE corner, in trees
  if (get_treasure(15) == 1) then
    set_zone("treasure1", 0)
    set_obs("treasure1", 0)
  end

  -- Treasure under flowers on E corner
  if (get_treasure(80) == 1) then
    set_zone("treasure2", 0)
    set_obs("treasure2", 0)
  end

end


function zone_handler(zn)
  -- Grotto entrance/exit
  if (zn == 1) then
    change_map("main", "grotto")

  -- Campfire
  elseif (zn == 2) then
    if (get_progress(P_TALKDERIG) == 2) then
      bubble(HERO1, _"That's strange. I wonder who lit this fire?")
    else
      touch_fire(party[0])
    end

  -- Pit
  elseif (zn == 3) then
    if (get_progress(P_FELLINPIT) == 0) then
      set_btile(16, 14, 153)
      bubble(HERO1, _"Uh oh!")
    elseif (get_progress(P_FELLINPIT) == 2) then
    -- if (get_progress(P_TALKDERIG) == 4) or (get_progress(P_TALKDERIG) == 5) then
      bubble(HERO1, _"I'd rather not go down there right now.")
      return
    end
    change_map("cave2", "entrance")

  -- Treasure under flowers on E corner
  elseif (zn == 4) then
    chest(15, I_ERUNE, 1)
    refresh()

  -- Rune
  elseif (zn == 5) then
    if (get_progress(P_UCOIN) == 0) then
      bubble(HERO1, _"Hmm... this seems like it should do something. But what?")
    elseif (get_progress(P_UCOIN) == 1) then
      bubble(HERO1, _"Wow. The rune hummed for a second but now it's quiet.")
    elseif (get_progress(P_UCOIN) == 2) then
        change_map("grotto2", "entrance")
    else
      bubble(HERO1, _"I can't go through here anymore since I no longer have the Unadium Coin.")
    end

  -- Treasure on NE corner, in trees
  elseif (zn == 6) then
    chest(80, I_MACE1, 1)
    refresh()

  end
end
