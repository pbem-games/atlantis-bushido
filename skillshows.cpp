// START A3HEADER
//
// This source file is part of the Atlantis PBM game program.
// Copyright (C) 1995-1999 Geoff Dunbar
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program, in the file license.txt. If not, write
// to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// See the Atlantis Project web page for details:
// http://www.prankster.com/project
//
// END A3HEADER
// MODIFICATIONS
// Date        Person            Comments
// ----        ------            --------
// 2001/Feb/23 Joseph Traub      Made skill texts runtime generated
//
#include "skills.h"
#include "items.h"
#include "object.h"
#include "gamedata.h"
#include "astring.h"

#define ITEM_ENABLED(X) (!(ItemDefs[(X)].flags & ItemType::DISABLED))
#define ITEM_DISABLED(X) (ItemDefs[(X)].flags & ItemType::DISABLED)
#define SKILL_ENABLED(X) (!(SkillDefs[(X)].flags & SkillType::DISABLED))
#define SKILL_DISABLED(X) (SkillDefs[(X)].flags & SkillType::DISABLED)
#define OBJECT_ENABLED(X) (!(ObjectDefs[(X)].flags & ObjectType::DISABLED))
#define OBJECT_DISABLED(X) (ObjectDefs[(X)].flags & ObjectType::DISABLED)


AString *ShowSkill::Report(Faction *f)
{
  if(SkillDefs[skill].flags & SkillType::DISABLED) return NULL;
  
  AString *str = new AString;
  RangeType *range = NULL;

  //BUSHIDO MODS
  //check skill type
  if (SkillDefs[skill].flags & SkillType::S_BUSHIDO_SAMURAI) {
    *str += "This is a samurai only ability and study of this skill"
      " outside of a RYU is at half rate. ";
  } else if (SkillDefs[skill].flags & SkillType::S_BUSHIDO_NINJA) {
    *str += "This is a ninja only ability and study of this skill"
      " outside of a NINJA RYU is at half rate. ";
  } else if (SkillDefs[skill].flags & SkillType::S_BUSHIDO_MERCHANT) {
    *str += "This is a merchant only ability and study of this skill"
      " outside of a YAKUZA HOUSE is at half rate. ";
  } else if (SkillDefs[skill].flags & SkillType::S_BUSHIDO_SORCEROR) {
    *str += "This is a sorceror only ability and study of this skill"
      " outside of a TOWER is at half rate. ";
  }
  //end BUSHIDO MODS

  // Here we pick apart the skill
  switch (skill) {
  case S_FARMING:
    if(level > 1) break;
    *str += "This skill deals with all aspects of grain production.";
    break;
  case S_RANCHING:
    if(level > 1) break;
    *str += "This skill deals with all aspects of livestock "
      "production.";
    break;
  case S_MINING:
    if(level > 1) break;
    *str += "This skill deals with all aspects of extracting raw "
      "metals and gems from the earth. Metals and gems tend to be "
      "found more often in mountainous regions, but may be found "
      "elsewhere as well.";
    break;
  case S_LUMBERJACK:
    if(level > 1) break;
    *str += "This skill deals with all aspects of various wood "
      "production. Woods are more often found in forests, but "
      "may also be found elsewhere.";
    break;
    //case S_QUARTERMASTER:
  case S_CARAVANMASTER:
    if (level > 1) break;
    if (!(Globals->TRANSPORT & GameDefs::ALLOW_TRANSPORT))
      break;
    *str += "This skill deals with transporting and "
      "distributing goods between non-local units "
      "and transport structures.";
    if (Globals->SHIPPING_COST > 0) {
      *str += " The cost of shipping one weight unit from one "
	"transport structure to another transport structure is ";
      if (Globals->TRANSPORT & GameDefs::QM_AFFECT_COST)
	*str += AString("4-((level+1)/2) * ");
      *str += AString(Globals->SHIPPING_COST) + " silver.";
      if (Globals->FRACTIONAL_WEIGHT) {
	*str += " Items with a normal weight of 0 are "
	  "treated as if ";
	*str += Globals->FRACTIONAL_WEIGHT;
	*str += " of the item in question weigh one weight unit.";
      }
    }
    
    if (Globals->NONLOCAL_TRANSPORT > 0) {
      *str += " Items may be shipped between two transport "
	"structures which are up to ";
      *str += Globals->NONLOCAL_TRANSPORT;
      if (Globals->TRANSPORT & GameDefs::QM_AFFECT_DIST)
	*str += " plus (level+1)/3 ";
      *str += (Globals->NONLOCAL_TRANSPORT != 1) ? "hexes" : "hex";
      *str += " distant from each other.";
    } else if (Globals->NONLOCAL_TRANSPORT == 0) {
      *str += " Items may be instantaneously "
	"shipped between any two transport "
	"structures.";
    }
    if (Globals->LOCAL_TRANSPORT > 0) {
      *str += " Items may be distributed from a transport "
	"structure to any unit or transported to a transport "
	"structure by any unit located within ";
      *str += Globals->LOCAL_TRANSPORT;
      *str += (Globals->LOCAL_TRANSPORT != 1) ? " hexes" : " hex";
      *str += " of the transport structure.";
    }
    break;
  case S_QUARRYING:
    if(level > 1) break;
    *str += "This skill deals with all aspects of various stone "
      "production. Mountains are the main producers of stone, but "
      "it may be found in other regions as well.";
    break;
  case S_HUNTING:
    if(level > 1) break;
    *str += "This skill deals with all aspects of animal hide "
      "production.";
    break;
  case S_FISHING:
    if(level > 1) break;
    *str += "This skill deals with all aspects of fish production.";
    break;
  case S_HERBLORE:
    if(level > 1) break;
    *str += "This skill deals with all aspects of herb production.";
    break;
  case S_HORSETRAINING:
    if(level > 1) break;
    *str += "This skill deals with all aspects of horse production.";
    break;
  case S_WEAPONSMITH:
    if(level > 1) break;
    *str += "This skill deals with all aspects of weapon "
      "construction and production."
      " Trade factions with a merchant specialist may learn more advanced skills using this one as a foundation.";
    break;
  case S_ARMORER:
    if(level > 1) break;
    *str += "This skill deals with all aspects of armor construction "
      "and production."
      " Trade factions with a merchant specialist may learn more advanced skills using this one as a foundation.";
    break;
  case S_CARPENTER:
    if(level > 1) break;
    *str += "This skill deals with all aspects of wood based item "
      "production other than for use as weapons."
      " Trade factions with a merchant specialist may learn more advanced skills using this one as a foundation.";
    break;
  case S_BUILDING:
    if(level > 1) break;
    *str += "This skill deals with the construction of "
      "fortifications, roads and other buildings, including "
      "most trade structures."
      " Trade factions with a merchant specialist may learn more advanced skills using this one as a foundation.";
    break;
  case S_SHIPBUILDING:
    if(level > 1) break;
    *str += "This skill deals with the constructions of all types "
      "of ships."
      " Trade factions with a merchant specialist may learn more advanced skills using this one as a foundation.";
    break;
  case S_ENTERTAINMENT:
    if(level > 1) break;
    *str += "A unit with this skill may use the ENTERTAIN order "
      "to generate funds. The amount of silver gained will "
      "be 20 per man, times the level of the entertainers. "
      "This amount is limited by the region that the unit is in.";
    break;
  case S_TACTICS:
    if(level > 1) break;
    *str += "Tactics allows the unit, and all allies, to gain a "
      "free round of attacks during battle. The army with the "
      "highest level tactician in a battle will receive this free "
      "round; if the highest levels are equal, no free round is "
      "awarded. Only one free round total will be awarded for any "
      "reason. Only samurai specialists can learn to level 5.";
    break;
  case S_COMBAT:
    if(level > 1) break;
    *str += "This skill gives the unit a bonus in hand to hand "
      "combat. Also, a unit with this skill may TAX or PILLAGE or RAZE a "
      "region.";
    break;
  case S_RIDING:
    if(level == 1) {
      *str += "A unit with this skill, if possessing a mount, may "
	"gain a bonus in combat, if the battle is in a location "
	"where that mount may be utilized and if the skill of the "
	"rider is sufficient to control that mount. The bonus "
	"gained can vary with the mount, the riders skill, and the "
	"terrain. At this level, a unit may ride a horse.";
    } else if (level == 2) {
      *str += "At this skill level, a unit can ride a war horse into battle.";
    } else if (level == 3) {
      *str += "At this skill level, a unit can ride a winged horse, hou-ou or dragon into battle.";
    }
    break;

  case S_CROSSBOW:
    if(level > 1) break;
    *str += "A unit with this skill may use a crossbow or other bow "
      "derived from one, either in battle, or to TAX or PILLAGE or RAZE a "
      "region.";
    break;
  case S_LONGBOW:
    if(level > 1) break;
    *str += "A unit with this skill may use a longbow or other bow "
      "derived from one, either in battle, or to TAX or PILLAGE or RAZE a "
      "region.";
    break;
  case S_STEALTH:
    if(level > 1) break;
    *str += "A unit with this skill is concealed from being seen";
    if(SKILL_ENABLED(S_OBSERVATION)) {
      *str += ", except by units with an Observation skill greater "
	"than or equal to the stealthy unit's Stealth level";
    }
    *str += ". Only ninja specialists can learn to level 5.";
    break;
  case S_OBSERVATION:
    if(level > 1) break;
    *str += "A unit with this skill can see stealthy units or "
      "monsters whose stealth rating is less than or equal to "
      "the observing unit's Observation level. The unit can "
      "also determine the faction owning a unit, provided its "
      "Observation level is higher than the other unit's Stealth "
      "level. Only ninja specialists can learn to level 5.";
    break;
  case S_HEALING:
    if(level > 1) break;
    *str += "A unit with this skill is able to heal units hurt in "
      "battle.";
    break;
  case S_SAILING:
    if(level > 1) break;
    *str += "A unit with this skill may use the SAIL order to sail "
      "ships.";
    break;
    //BUSHIDO foundation skills
  case S_WAY_SAMURAI:
    if (level > 1) break;
    *str += "A unit with this skill gains the unique samurai combat "
      "abilities. This is a leader unit only skill and a unit with this "
      "skill counts toward the faction war point specialist limit.";
    break;
  case S_WAY_NINJA:
    if (level > 1) break;
    *str += "A unit with this skill gains the unique ninja stealth "
      "abilities, which includes assassination. This is a leader unit "
      "only skill and a unit with this "
      "skill counts toward the faction stealth point specialist limit.";
    break;
  case S_WAY_MERCHANT:
    if (level > 1) break;
    *str += "A unit with this skill gains the unique merchant trade "
      "abilities. This is a leader unit only skill and a unit with this "
      "skill counts toward the faction trade point specialist limit.";
    break;
  case S_WAY_SORCEROR:
    if (level > 1) break;
    *str += "A unit with this skill gains the unique sorceror magic "
      "abilities. This is a leader unit only skill and a unit with this "
      "skill counts toward the faction magic point specialist limit.";
    break;

    //BUSHIDO skills
  case S_ENDURANCE:
    if (level > 1) break;
    *str += "A unit with this skill gains one extra hit in battle per "
      "skill level. This is a leader unit only skill.";
    break;
  case S_FORCED_MARCH:
    if (level > 1) break;
    *str += "A unit with this skill is gains an extra 2 movement "
      "points per turn. There is a 20% chance per level of "
      "this occuring. This is a leader unit only skill.";
    break;
  case S_JEWELLER:
    if(level > 1) break;
    *str += "This skill enables a unit to fashion higher quality "
      "gems from lower quality ones and create items of jewellery.";
    break;    
     
    //samurai skills
  case S_CREATE_FEAR:
    if(level > 1) break;
    *str += "A unit with this skill gains the ability to intimidate "
      "in battle at a level equal to the skill level of this ability.";
    break;
  case S_RESIST_FEAR:
    if (level > 1) break;
    *str += "A unit with this skill gains the ability to resist fear "
      "at a level equal to the skill level of this ability.";
    break;    
  case S_MULTIPLE_ATTACKS:
    if (level > 1) break;
    *str += "A unit with this skill gains extra attacks with melee wepaons in battle. "
      "The number of extra attacks conferred in battle is equal to the "
      "level of this ability. This skill stacks with the attacks provided "
      "by the NITO skill.";
    break;
  case S_TWO_WEAPON_COMBO:
    if (level > 1) break;
    *str += "A unit with this skill can use the daisho (katana and wakizashi) in battle. "
      "The number of attacks conferred in battle is equal to the "
      "level of this ability.";
    break;
  case S_DEFLECT_ARROWS:
    if (level > 1) break;
    *str += "A unit with this skill can deflect arrows in battle, "
      "with the defence against ranged attacks equal to the level "
      "of this ability.";
    break;
  case S_HAWKING:
    if(level == 1) {
      *str += "A unit with Hawking may control the birds of the "
	"sky. At skill level 1, the unit can control small "
	"birds, sending them to an adjacent region to obtain a "
	"report on that region. (This skill only works on the "
	"surface of the world, as there are no birds elsewhere)."
	" To use this skill, CAST Hawking DIRECTION <dir>, "
	"where <dir> is the direction the unit wishes the birds "
	"to report on.";
    } else if (level == 3) {
      if(ITEM_DISABLED(I_EAGLE)) break;
      *str += "A unit with Hawking 3 can summon eagles to join "
	"him, who will aid him in combat, and provide for flying "
	"transportation. A unit may summon a number of eagles "
	"equal to his skill level, squared; the eagles "
	"will appear in his inventory. To summon an eagle, issue "
	"the order CAST Hawking EAGLE.";
    } else if (level == 5) {
      if(ITEM_DISABLED(I_HOU_OU)) break;
      *str += "A unit with Hawking 5 can summon Hou-Ou to join "
	"him, who will aid him in combat, and provide for flying "
	"transportation. A unit may summon a number of hou-ou "
	"equal to his skill level; the hou-ou "
	"will appear in his inventory. To summon an hou-ou, issue "
	"the order CAST Hawking HOU_OU.";
    }
    break;

    //ninja skills
  case S_WATER_WALKING:
    if (level > 1) break;
    *str += "A unit with this skill gains the ability to walk on water. "
      "The chance of success is equal to 20% times the skill level of "
      "this ability.";
    break;    

  case S_WILDERNESS_MOVEMENT:
    if (level > 1) break;
    *str += "A unit with this skill gains the ability to treat all terrain "
      "as though the unit was able to fly. The chance of success is "
      "equal to 20% times the skill level of this ability.";
    break;    

  case S_THROWING_STAR:
    if (level > 1) break;
    *str += "A unit with this skill gains the ability use throwing stars or "
      "shuriken in assassination attempt or combat.";
    break;    

  case S_POISON_WEAPONS:
    if (level > 1) break;
    *str += "A unit with this skill gains the ability to create poisoned "
      "weapons from existing ninja weapons.";
    break;

  case S_RESIST_ELEMENTS:
    if (level > 1) break;
    *str += "A unit with this skill gains the ability to resist the elements. "
      "This skill provides a bonus to defence against energy and weather "
      "attacks at a level equal to the skill level of this ability.";
    break;

  case S_CAMOUFLAGE:
    if (level > 1) break;
   *str += "The Camouflage skill allows a unit to render themselves nearly "
     "invisible to other factions, giving them a bonus to Stealth equal to "
     "the skill level of this ability divided by 2, rounded up. This "
     "camouflage will last until the next Magic round. To cast this spell, "
     "use the order CAST Camouflage.";
    break;

  case S_KEEN_EYES:
    if (level > 1) break;
    *str += "The Keen Eyes skill gives a unit a bonus to Observation equal to "
     "the skill level of this ability divided by 2, rounded up. This "
     "ability will last until the next Magic round. To cast this spell, "
     "use the order CAST Keen_Eyes.";
    break;

    //merchant
  case S_COMMERCE:
      if (level > 1) break;      
    *str += "The Commerce skill allows a merchant to buy goods for less and "
      "sell goods for more. For each skill level, the merchant gains a 5% "
      "difference in the buying and selling price, in favour of the merchant.";
    break;

  case S_GAMBLING:
      if (level > 1) break;
      *str += "The Gambling skill allows a merchant to set up a gambling den in "
	  " a region, with each region provides up to 1 silver per population" 
	  " a month. For each skill level the merchant has in this skill, they"
	  " gain up to 1000 silver per month as proceeds from gambling in the"
	  " region."
      " To do this, the merchant must be the owner of a gambling den and"
      " issue the GAMBLE instruction. Gambling is a monthly activity.";
    break;

  case S_TEA_CEREMONY:
    if(level > 1) break;
    *str += "A unit with this skill may obtain region reports on "
      "distant regions, similar to the Farsight skill, through gossip "
      "obtained in a Geisha House. "
      "The report will be as if the unit was in "
      "the distant region himself. "
      "The caster must be the owner of a Geisha House to use this ability.";
    range = FindRange(SkillDefs[skill].range);
    if(range) {
      if(range->flags & RangeType::RNG_SURFACE_ONLY) {
	*str += " This skill only works on the surface of the "
	  "world.";
      }
      *str += " The target region must be within ";
      *str += range->rangeMult;
      switch(range->rangeClass) {
      case RangeType::RNG_LEVEL:
	*str += " times the caster's skill level ";
	break;
      case RangeType::RNG_LEVEL2:
	*str += " times the caster's skill level squared ";
	break;
      case RangeType::RNG_LEVEL3:
	*str += " times the caster's skill level cubed ";
	break;
      default:
      case RangeType::RNG_ABSOLUTE:
	break;
      }
      *str += "regions of the caster. ";
      if(range->flags & RangeType::RNG_CROSS_LEVELS) {
	*str += "Coordinates of locations not on the surface are "
	  "scaled to the surface coordinates for this "
	  "calculation. Attempting to view across different "
	  "levels increases the distance by ";
	*str += range->crossLevelPenalty;
	*str += " per level difference. ";
	*str += "To use this skill, CAST Tea_Ceremony REGION <x> <y> "
	  "<z> where <x>, <y>, and <z> are the coordinates of "
	  "the region that you wish to view. If you omit the "
	  "<z> coordinate, the <z> coordinate of the caster's "
	  "current region will be used.";
	if(Globals->UNDERWORLD_LEVELS +
	   Globals->UNDERDEEP_LEVELS == 1) {
	  *str += " The <z> coordinate for the surface is '1' "
	    "and the <z>-coordinate for the underworld is "
	    "'2'.";
	}
	*str += " Note that Tea Ceremony cannot be used either into "
	  "or out of the Nexus.";
      } else {
	*str += "To use this skill, CAST Tea Ceremony REGION <x> "
	  "<y>, where <x> and <y> are the coordinates of the "
	  "region that you wish to view.";
      }
    }
    if(Globals->IMPROVED_FARSIGHT) {
      *str += " Any other skills which the unit has which give "
	"region information will be used when farsight is used.";
    } else {
      *str += " Note that Tea Ceremony does not work in conjunction "
	"with other skills or spells; the unit can only rely on "
	"his normal facilities while casting Tea Ceremony.";
    }
    break;


    //sorceror
  case S_HELLFIRE:
  case S_FIRE_LORE:
    break;
  case S_EARTHQUAKE:
    if(level > 1) break;
    break;
  case S_FORCE_SHIELD:
    if(level > 1) break;
    break;
  case S_ENERGY_SHIELD:
    if(level > 1) break;
    break;
  case S_SPIRIT_SHIELD:
    if(level > 1) break;
    break;
  case S_MAGICAL_HEALING:
    /* XXX -- This should be cleaner somehow. */
    if(level == 1) {
      *str += "This skill enables the mage to magically heal units "
	"after battle. A mage at this level can heal up to 10 "
	"casualties, with a 50 percent rate of success. No order "
	"is necessary to use this spell, it will be used "
	"automatically when the mage is involved in battle.";
    } else if(level == 2) {
      *str += "A mage at this level of skill can work greater "
	"wonders of healing with his new found powers; he may "
	"heal up to 25 casualties, with a success rate of 75 "
	"percent.";
    } else if(level == 3) {
      *str += "A mage at this level of skill can work greater "
	"wonders of healing with his new found powers; he may "
	"heal up to 25 casualties, with a success rate of 75 "
	"percent.";
    } else if(level == 4) {
      *str += "A mage at this level of skill can work greater "
	"wonders of healing with his new found powers; he may "
	"heal up to 25 casualties, with a success rate of 75 "
	"percent.";
    } else if(level == 5) {
      *str += "A mage at this skill level can bring soldiers back "
	"from near death; he may heal up to 100 casualties, with "
	"a 90 percent rate of success.";
    }
    break;
  case S_GATE_LORE:
    /* XXX -- This should be cleaner somehow. */
    if(level == 1) {
      *str += "Gate Lore is the art of detecting and using magical "
	"Gates, which are spread through the world. The Gates are "
	"numbered in order, but spread out randomly, so there is "
	"no correlation between the Gate number and the Gate's "
	"location. A sorceror with skill 1 in Gate Lore can see a "
	"Gate if one exists in the same region as the sorceror. This "
	"detection is automatic; the Gate will appear in the "
	"region report. A sorceror with skill 1 in Gate Lore may "
	"also jump through a Gate into another region on the same "
	"level containing a gate, selected at random. To use "
	"Gate Lore in this manner, use the syntax CAST Gate_Lore "
	"RANDOM UNITS <unit> ... UNITS is followed by a list "
	"of units to follow the sorceror through the Gate (the sorceror "
	"always jumps through the Gate). At level 1, the sorceror "
	"may carry 15 weight units through the Gate (including "
	"the weight of the sorceror).";
    } else if (level == 2) {
      *str += "A sorceror with Gate Lore skill 2 can detect Gates in "
	"adjacent regions. The sorceror should use the syntax CAST "
	"Gate_Lore DETECT in order to detect these nearby Gates. "
	"Also, at level 2 Gate Lore, the sorceror may carry 100 "
	"weight units through a Gate when doing a random jump.";
    } else if(level == 3) {
      *str += "A sorceror with Gate Lore skill 3 and higher can step "
	"through a Gate into another region containing a specific "
	"Gate. To use this spell, use the syntax CAST Gate_Lore "
	"GATE <number> UNITS <unit> ... <number> specifies the "
	"Gate that the sorceror will jump to. UNITS is followed by a "
	"list of units to follow the sorceror through the gate (the "
	"sorceror always jumps through the gate). At level 3, the "
	"sorceror may carry 15 weight units through the Gate "
	"(including the sorceror). Also, a level 3 or higher sorceror "
	"doing a random gate jump may carry 1000 weight units "
	"through the Gate.";
    } else if(level == 4) {
      *str += "A sorceror with Gate Lore skill 4 may carry 100 weight "
	"units through a Gate.";
    } else if(level == 5) {
      *str += "A sorceror with Gate Lore skill 5 may carry 1000 weight "
	"units through a Gate.";
    }
    break;
  case S_PORTAL_LORE:
    if(level > 1) break;
    /* XXX -- This should be cleaner somehow. */
    if(ITEM_DISABLED(I_PORTAL)) break;
    *str += "A sorceror with the Portal Lore skill may, with the aid of "
      "another sorceror, make a temporary Gate between two regions, and "
      "send units from one region to another. In order to do this, "
      "both sorcerors (the caster, and the target sorceror) must have "
      "Portals, and the caster must be trained in Portal Lore. The "
      "caster may teleport units weighing up to 50 weight units "
      "times his skill level, to the target sorceror's region. ";
    range = FindRange(SkillDefs[skill].range);
    if(range) {
      *str += " The target region must be within ";
      *str += range->rangeMult;
      switch(range->rangeClass) {
      case RangeType::RNG_LEVEL:
	*str += " times the caster's skill level ";
	break;
      case RangeType::RNG_LEVEL2:
	*str += " times the caster's skill level squared ";
	break;
      case RangeType::RNG_LEVEL3:
	*str += " times the caster's skill level cubed ";
	break;
      default:
      case RangeType::RNG_ABSOLUTE:
	break;
      }
      *str += "regions of the caster. ";
    }
    *str += "To use this skill, CAST Portal_Lore <target> UNITS "
      "<unit> ..., where <target> is the unit number of the "
      "target sorceror, and <unit> is a list of units to be "
      "teleported (the casting sorceror may teleport himself, if "
      "he so desires).";
    break;
  case S_FARSIGHT:
    if(level > 1) break;
    *str += "A unit with this skill may obtain region reports on "
      "distant regions. The report will be as if the unit was in "
      "the distant region himself.";
    range = FindRange(SkillDefs[skill].range);
    if(range) {
      if(range->flags & RangeType::RNG_SURFACE_ONLY) {
	*str += " This skill only works on the surface of the "
	  "world.";
      }
      *str += " The target region must be within ";
      *str += range->rangeMult;
      switch(range->rangeClass) {
      case RangeType::RNG_LEVEL:
	*str += " times the caster's skill level ";
	break;
      case RangeType::RNG_LEVEL2:
	*str += " times the caster's skill level squared ";
	break;
      case RangeType::RNG_LEVEL3:
	*str += " times the caster's skill level cubed ";
	break;
      default:
      case RangeType::RNG_ABSOLUTE:
	break;
      }
      *str += "regions of the caster. ";
      if(range->flags & RangeType::RNG_CROSS_LEVELS) {
	*str += "Coordinates of locations not on the surface are "
	  "scaled to the surface coordinates for this "
	  "calculation. Attempting to view across different "
	  "levels increases the distance by ";
	*str += range->crossLevelPenalty;
	*str += " per level difference. ";
	*str += "To use this skill, CAST Farsight REGION <x> <y> "
	  "<z> where <x>, <y>, and <z> are the coordinates of "
	  "the region that you wish to view. If you omit the "
	  "<z> coordinate, the <z> coordinate of the caster's "
	  "current region will be used.";
	if(Globals->UNDERWORLD_LEVELS +
	   Globals->UNDERDEEP_LEVELS == 1) {
	  *str += " The <z> coordinate for the surface is '1' "
	    "and the <z>-coordinate for the underworld is "
	    "'2'.";
	}
	*str += " Note that Farsight cannot be used either into "
	  "or out of the Nexus.";
      } else {
	*str += "To use this skill, CAST Farsight REGION <x> "
	  "<y>, where <x> and <y> are the coordinates of the "
	  "region that you wish to view.";
      }
    }
    if(Globals->IMPROVED_FARSIGHT) {
      *str += " Any other skills which the unit has which give "
	"region information will be used when farsight is used.";
    } else {
      *str += " Note that Farsight does not work in conjunction "
	"with other skills or spells; the unit can only rely on "
	"his normal facilities while casting Farsight.";
    }
    break;
  case S_MIND_READING:
    /* XXX -- This should be cleaner somehow. */
    if(level == 1) {
      *str += "A unit with Mind Reading skill 1 may cast the spell "
	"and determine the faction affiliation of any unit he can "
	"see. To use the spell in this manner, CAST Mind_Reading "
	"<unit>, where <unit> is the target unit.";
    } else if (level == 3) {
      *str += "A unit with Mind Reading skill 3 will automatically "
	"determine the faction affiliation of any unit he can "
	"see. Usage of this skill is automatic, and no order is "
	"needed to use it.";
    } else if (level == 5) {
      *str += "A unit with Mind Reading skill 5 can get a full "
	"unit report on any unit he can see. To use this skill, "
	"CAST Mind_Reading <unit> where <unit> is the target "
	"unit.";
    }
    break;
  case S_TELEPORTATION:
    if(level > 1) break;
    /* XXX -- This should be cleaner somehow. */
    *str += "A sorceror with this skill may teleport himself across"
      "great distances, even without the use of a gate. The sorceror "
      "may teleport up to 15 weight units per skill level.";
    range = FindRange(SkillDefs[skill].range);
    if(range) {
      if(range->flags & RangeType::RNG_SURFACE_ONLY) {
	*str += " This skill only works on the surface of the "
	  "world.";
      }
      *str += " The target region must be within ";
      *str += range->rangeMult;
      switch(range->rangeClass) {
      case RangeType::RNG_LEVEL:
	*str += " times the caster's skill level ";
	break;
      case RangeType::RNG_LEVEL2:
	*str += " times the caster's skill level squared ";
	break;
      case RangeType::RNG_LEVEL3:
	*str += " times the caster's skill level cubed ";
	break;
      default:
      case RangeType::RNG_ABSOLUTE:
	break;
      }
      *str += "regions of the caster. ";
      if(range->flags & RangeType::RNG_CROSS_LEVELS) {
	*str += "Coordinates of locations not on the surface are "
	  "scaled to the surface coordinates for this "
	  "calculation. Attempting to view across different "
	  "levels increases the distance by ";
	*str += range->crossLevelPenalty;
	*str += " per level difference. ";
	*str += "To use this skill, CAST Teleportation REGION "
	  "<x> <y> <z> where <x>, <y>, and <z> are the "
	  "coordinates of the region that you wish to "
	  "teleport to. If you omit the <z> coordinate, the "
	  "<z> coordinate of the caster's current region will "
	  "be used.";
	if(Globals->UNDERWORLD_LEVELS +
	   Globals->UNDERDEEP_LEVELS == 1) {
	  *str += " The <z> coordinate for the surface is '1' "
	    "and the <z>-coordinate for the underworld is "
	    "'2'.";
	}
	*str += " Note that Teleportation cannot be used either "
	  "into or out of the Nexus.";
      } else {
	*str += "To use this skill, CAST Teleportation REGION "
	  "<x> <y>, where <x> and <y> are the coordinates of "
	  "the region that you wish to teleport to.";
      }
    }
    break;
  case S_WEATHER_LORE:
    if(level > 1) break;
    /* XXX -- This should be templated */
    *str += "Weather Lore is the magic of the weather; a sorceror with "
      "this skill can predict the weather in nearby regions. "
      "Weather Lore also allows further study into more powerful "
      "areas of magic. The weather may be predicted for 3 months "
      "at level 1, 6 months at level 3 and a full year at level "
      "5.";
    range = FindRange(SkillDefs[skill].range);
    if(range) {
      if(range->flags & RangeType::RNG_SURFACE_ONLY) {
	*str += " This skill only works on the surface of the "
	  "world.";
      }
      *str += " The target region must be within ";
      *str += range->rangeMult;
      switch(range->rangeClass) {
      case RangeType::RNG_LEVEL:
	*str += " times the caster's skill level ";
	break;
      case RangeType::RNG_LEVEL2:
	*str += " times the caster's skill level squared ";
	break;
      case RangeType::RNG_LEVEL3:
	*str += " times the caster's skill level cubed ";
	break;
      default:
      case RangeType::RNG_ABSOLUTE:
	break;
      }
      *str += "regions of the caster. ";
      if(range->flags & RangeType::RNG_CROSS_LEVELS) {
	*str += "Coordinates of locations not on the surface are "
	  "scaled to the surface coordinates for this "
	  "calculation. Attempting to view across different "
	  "levels increases the distance by ";
	*str += range->crossLevelPenalty;
	*str += " per level difference. ";
	*str += "To use this skill, CAST Weather_Lore REGION "
	  "<x> <y> <z> where <x>, <y>, and <z> are the "
	  "coordinates of the region where you wish to "
	  "predict the weather. If you omit the <z> "
	  "coordinate, the <z> coordinate of the caster's "
	  "current region will be used.";
	if(Globals->UNDERWORLD_LEVELS +
	   Globals->UNDERDEEP_LEVELS == 1) {
	  *str += " The <z> coordinate for the surface is '1' "
	    "and the <z>-coordinate for the underworld is "
	    "'2'.";
	}
	*str += " Note that Weather Lore cannot be used either "
	  "into or out of the Nexus.";
      } else {
	*str += "To use this skill, CAST Weather_Lore REGION "
	  "<x> <y>, where <x> and <y> are the coordinates of "
	  "the region where you wish to predict the weather.";
      }
    }
    *str += " A sorceror with Weather Lore skill will perceive the use "
      "of Weather Lore by any other sorceror in the same region.";
    break;
  case S_SUMMON_WIND:
    if(level == 1) {
      *str += "A sorceror with knowledge of Summon Wind can summon "
	"up the powers of the wind to aid him in sea or "
	"air travel. Usage of this spell is automatic. ";
					 
      /*
       *str += " If the sorceror is flying, he will receive 2 extra "
       "movement points.";
      */
    }
    if(level > 0) {
      int wind1 = (level+1) * (level+1) * 8;
      int wind0 = level * level * 8;
      int max1 = 0;
      int max2 = 0;
      int max3 = 0;
      int item1 = -1;
      int item2 = -1;
      int item3 = -1;
      for(int item=0; item<NITEMS; item++) {
	if(ItemDefs[item].type & IT_SHIP) {
	  if(ItemDefs[item].flags & ItemType::DISABLED) continue;
	  int pub = 1;
	  for(int c = 0; c < (int) sizeof(ItemDefs->pInput)/(int) sizeof(Materials); c++) {
	    int m = ItemDefs[item].pInput[c].item;
	    if(m != -1) {
	      if(ItemDefs[m].flags & ItemType::DISABLED) pub = 0;
	      if((ItemDefs[m].type & IT_ADVANCED) ||
		 (ItemDefs[m].type & IT_MAGIC)) pub = 0;
	    }
	  }
	  if(pub == 0) continue;
	  int slevel = ItemDefs[item].pLevel;
	  if(slevel > 3) continue;
	  int mass = ItemDefs[item].weight / 10;
	  if((wind1 >= mass*3) && (wind0 < mass*3)) {
	    if(mass >= max3) {
	      max3 = mass;
	      item3 = item;
	    }
	  } else if((wind1 >= mass*2) && (wind0 < mass*2)) {
	    if(mass >= max2) {
	      max2 = mass;
	      item2 = item;
	    }
	  } else if((wind1 >= mass) && (wind0 < mass)) {
	    if(mass >= max1) {
	      max1 = mass;
	      item1 = item;
	    }
	  }
	}
      }
      if((item1 != -1) || (item2 != -1) || (item3 != -1)) {
	*str += "At this level, a sorceror will effect a full bonus of ";
	switch(Globals->SHIP_SPEED / 2) {
	case '1': *str += "one ";
	  break;
	case '2': *str += "two ";
	  break;
	case '3': *str += "three ";
	  break;
	case '4': *str += "four";
	  break;
	default: *str += "two ";
	}				
	*str += "move points for ";
	int ctr = 0;
	if(item1 != -1) {
	  *str += "one ";
	  *str += ItemDefs[item1].name;
	  ctr++;
	}
	if(item2 != -1) {
	  if(ctr++ > 0) {
	    if(item3 == -1) *str += " or ";
	    else *str += ", ";
	  }
	  *str += "two ";
	  *str += ItemDefs[item2].names;
	}
	if(item3 != -1) {
	  if(ctr > 0) *str += " or ";
	  *str += "three ";
	  *str += ItemDefs[item3].names;
	}
	*str += ", for instance. ";
      }
    }
    *str += "The effects of all such sorcerors in a fleet are cumulative. ";	
    break;
  case S_SUMMON_STORM:
    if(level > 1) break;
    break;
  case S_SUMMON_TORNADO:
    if(level > 1) break;
    break;
  case S_CALL_LIGHTNING:
    if(level > 1) break;
    break;
  case S_CLEAR_SKIES:
    /* XXX -- this range stuff needs cleaning up */
    if(level > 1) break;
    if(SkillDefs[skill].flags & SkillType::CAST) {
      *str += "When cast using the CAST order, it causes the "
	"region to have good weather for the entire month; "
	"movement is at the normal rate (even if it is winter) "
	"and the economic production of the region is improved "
	"for a month (this improvement of the economy will "
	"actually take effect during the turn after the spell "
	"is cast).";
      range = FindRange(SkillDefs[skill].range);
      if(range) {
	if(range->flags & RangeType::RNG_SURFACE_ONLY) {
	  *str += " This skill only works on the surface of "
	    "the world.";
	}
	*str += " The target region must be within ";
	*str += range->rangeMult;
	switch(range->rangeClass) {
	case RangeType::RNG_LEVEL:
	  *str += " times the caster's skill level ";
	  break;
	case RangeType::RNG_LEVEL2:
	  *str += " times the caster's skill level squared ";
	  break;
	case RangeType::RNG_LEVEL3:
	  *str += " times the caster's skill level cubed ";
	  break;
	default:
	case RangeType::RNG_ABSOLUTE:
	  break;
	}
	*str += "regions of the caster. ";
	if(range->flags & RangeType::RNG_CROSS_LEVELS) {
	  *str += "Coordinates of locations not on the surface "
	    "are scaled to the surface coordinates for this "
	    "calculation. Attempting to view across "
	    "different levels increases the distance by ";
	  *str += range->crossLevelPenalty;
	  *str += " per level difference. ";
	  *str += "To use this skill, CAST Clear_Skies REGION "
	    "<x> <y> <z> where <x>, <y>, and <z> are the "
	    "coordinates of the region where you wish to "
	    "improve the weather. If you omit the <z> "
	    "coordinate, the <z> coordinate of the caster's "
	    "current region will be used.";
	  if(Globals->UNDERWORLD_LEVELS +
	     Globals->UNDERDEEP_LEVELS == 1) {
	    *str += " The <z> coordinate for the surface is "
	      "'1' and the <z>-coordinate for the "
	      "underworld is '2'.";
	  }
	  *str += " Note that Clear Skies cannot be used "
	    "either into or out of the Nexus.";
	} else {
	  *str += "To use this skill, CAST Clear_Skies REGION "
	    "<x> <y>, where <x> and <y> are the coordinates "
	    "of the region where you wish to improve the "
	    "weather.";
	}
      } else {
	*str += " To use the spell in this fashion, CAST "
	  "Clear_Skies; no arguments are necessary.";
      }
    }
    break;
  case S_EARTH_LORE:
    if(level > 1) break;
    *str += "Earth Lore is the study of nature, plants, and animals. "
      "A sorceror with knowledge of Earth Lore can use his knowledge "
      "of nature to aid local farmers, raising money for himself, "
      "and aiding the production of grain or livestock in the "
      "region. To use the spell, CAST Earth_Lore; the sorceror will "
      "receive an amount of money based on his level, and the "
      "economy of the region. Also, a sorceror with knowledge of Earth "
      "Lore will detect the use of Earth Lore by any other sorceror in "
      "the same region.";
    break;
    /*  case S_WOLF_LORE:
    if(level > 1) break;
    if(ITEM_DISABLED(I_WOLF)) break;
    *str += "A sorceror with Wolf Lore skill may summon wolves, who will "
      "fight for him in combat. A sorceror may summon a number of "
      "wolves equal to his skill level, and control a total number "
      "of his skill level squared times 4 wolves; the wolves will "
      "be placed in the sorcerors inventory. Note, however, that wolves "
      "may only be summoned in mountain and forest regions. To "
      "summon wolves, the sorceror should issue the order CAST "
      "Wolf_Lore.";
    break;*/
    /*  case S_BIRD_LORE:
    if(level == 1) {
      *str += "A sorceror with Bird Lore may control the birds of the "
	"sky. At skill level 1, the sorceror can control small "
	"birds, sending them to an adjacent region to obtain a "
	"report on that region. (This skill only works on the "
	"surface of the world, as there are no birds elsewhere)."
	" To use this skill, CAST Bird_Lore DIRECTION <dir>, "
	"where <dir> is the direction the sorceror wishes the birds "
	"to report on.";
    } else if (level == 3) {
      if(ITEM_DISABLED(I_EAGLE)) break;
      *str += "A sorceror with Bird Lore 3 can summon eagles to join "
	"him, who will aid him in combat, and provide for flying "
	"transportation. A sorceror may summon a number of eagles "
	"equal to his skill level minus 2, squared; the eagles "
	"will appear in his inventory. To summon an eagle, issue "
	"the order CAST Bird_Lore EAGLE.";
    }
    break;*/
  case S_SUMMON_DRAGON:
    if(level > 1) break;
    if(ITEM_DISABLED(I_DRAGON)) break;
    *str += "A sorceror with Summon Dragon skill can summon dragons to "
      "join him, to aid in battle, and provide flying "
      "transportation. A sorceror at level 1 has a low chance of "
      "successfully summoning a dragon, gradually increasing until "
      "at level 5 he may summon one dragon per turn; the total "
      "number of dragons that a sorceror may control at one time is "
      "equal to his skill level. To attempt to summon a dragon, "
      "CAST Summon_Dragon.";
    break;
    /*  case S_NECROMANCY:
    if(level > 1) break;
    *str += "Necromancy is the magic of death; a sorceror versed in "
      "Necromancy may raise and control the dead, and turn the "
      "powers of death to his own nefarious purposes. The "
      "Necromancy skill does not have any direct application, but "
      "is required to study the more powerful Necromantic skills. "
      "A sorceror with knowledge of Necromancy will detect the use of "
      "Necromancy by any other sorceror in the same region.";
      break;*/
    /*  case S_SUMMON_SKELETONS:
    if(level > 1) break;
    if(ITEM_DISABLED(I_SKELETON)) break;
    *str += "A sorceror with the Summon Skeletons skill may summon "
      "skeletons into his inventory, to aid him in battle. "
      "Skeletons may be given to other units, as they follow "
      "instructions mindlessly; however, they have a 10 percent "
      "chance of decaying each turn. A sorceror can summon skeletons "
      "at an average rate of 40 percent times his level squared. "
      "To use the spell, use the order CAST Summon_Skeletons, "
      "and the sorceror will summon as many skeletons as he is able.";
      break;*/
    /*  case S_RAISE_UNDEAD:
    if(level > 1) break;
    if(ITEM_DISABLED(I_UNDEAD)) break;
    *str += "A sorceror with the Raise Undead skill may summon undead "
      "into his inventory, to aid him in battle. Undead may be "
      "given to other units, as they follow instructions "
      "mindlessly; however, they have a 10 percent chance of "
      "decaying each turn. A sorceror can summon undead at an average "
      "rate of 10 percent times his level squared. To use the "
      "spell, use the order CAST Raise_Undead and the sorceror will "
      "summon as many undead as he is able.";
      break;*/
    /*case S_SUMMON_LICH:
    if(level > 1) break;
        if(ITEM_DISABLED(I_LICH)) break;
    *str += "A sorceror with the Summon Lich skill may summon a lich "
      "into his inventory, to aid him in battle. Liches may be "
      "given to other units, as they follow instructions "
      "mindlessly; however, they have a 10 percent chance of "
      "decaying each turn. A sorceror has a 2 percent times his level "
      "squared chance of summoning a lich; to summon a lich, use "
      "the order CAST Summon_Lich.";
    break;*/
    /*  case S_CREATE_AURA_OF_FEAR:
    if(level > 1) break;
    break;*/
  case S_SUMMON_BLACK_WIND:
    if(level > 1) break;
    break;
  case S_BANISH_UNDEAD:
    if(level > 1) break;
    break;
  case S_DEMON_LORE:
    if(level > 1) break;
    *str += "Demon Lore is the art of summoning and controlling "
      "demons. The Demon Lore skill does not give the sorceror any "
      "direct skills, but is required to study further into the "
      "Demonic arts.";
    break;
  case S_UNDEAD_LORE:
    if(level > 1) break;
    *str += "Undead Lore is the art of summoning and controlling "
      "undead. The Undead Lore skill does not give the sorceror any "
      "direct skills, but is required to study further into the "
      "Necromantic arts.";
    break;
  case S_SPIRIT_LORE:
    if(level > 1) break;
    *str += "Spirit Lore is the art of summoning and controlling "
      "spirits. The Spirit Lore skill does not give the sorceror any "
      "direct skills, but is required to study further into the "
      "summoning of spirits.";
    break;
  case S_DRAGON_LORE:
    if(level > 1) break;
    *str += "Dragon Lore is the art of summoning and controlling "
      "dragons. The Dragon Lore skill does not give the sorceror any "
      "direct skills, but is required to study further into the "
      "summoning of dragons.";
    break;

    /*  case S_SUMMON_IMPS:
    if(level > 1) break;
    if(ITEM_DISABLED(I_IMP)) break;
    *str += "A sorceror with the Summon Imps skill may summon imps into "
      "his inventory, to aid him in combat. A sorceror may summon one "
      "imp per skill level; however, the imps have a chance of "
      "breaking free of the sorceror's control at the end of each "
      "turn. This chance is based on the number of imps in the "
      "sorceror's control; if the sorceror has his skill level squared "
      "times 4 imps, the chance is 5 percent; this chance goes "
      "up or down quickly if the sorceror controls more or fewer imps. "
      "To use this spell, the sorceror should issue the order CAST "
      "Summon_Imps, and the sorceror will summon as many imps as he "
      "is able.";
      break;*/

  case S_SUMMON_UNDEAD:
    if(level == 1) {
      if(ITEM_DISABLED(I_SKELETON)) break;
      *str += "A unit with the Summon Undead skill may summon skeletons "
	"into his inventory, to aid him in combat. A unit may summon "
	"his skill level in skeletons each turn; however, the skeletons have a chance of "
	"breaking free of the unit's control at the end of each "
	"turn. This chance is based on the number of skeletons in the "
	"unit's control. To use this spell, the unit should issue the "
	"order CAST Summon_Undead SKELETON.";
      break;
    } else if (level == 3) {
      if(ITEM_DISABLED(I_UNDEAD)) break;
      *str += "A unit with Summon Undead 3 can summon undead to join "
	"him, who will aid him in combat. "
	"A unit may summon a number of undead "
	"equal to his skill level minus two, squared; the undead "
	"will appear in his inventory. The undead have a chance of breaking free "
	"at the end of each turn. To summon undead, issue "
	"the order CAST Summon_Undead UNDEAD.";
    } else if (level == 5) {
      if(ITEM_DISABLED(I_GAKI)) break;
      *str += "A unit with Summon Undead 5 can summon a gaki to join "
	"him, who will aid him in combat. A unit may only summon one gaki"
	"; the gski will appear in his inventory. The gaki has a chance "
	"of breaking free at the end of each turn. To summon a gaki, " 
	"issue the order CAST Summon_Undead GAKI.";
    }
    break;

  case S_SUMMON_SPIRITS:
    if(level == 1) {
      if(ITEM_DISABLED(I_MINOR_KAMI)) break;
      *str += "A unit with the Summon Spirits skill may summon minor kami "
	"into his inventory, to aid him in combat. A unit may summon "
	"his skill level in minor kami each turn; however, the minor kami have a chance of "
	"breaking free of the unit's control at the end of each "
	"turn. This chance is based on the number of minor kami in the "
	"unit's control. To use this spell, the unit should issue the "
	"order CAST Summon_Spirits MINOR_KAMI.";
      break;
    } else if (level == 3) {
      if(ITEM_DISABLED(I_KAMI)) break;
      *str += "A unit with Summon Spirits 3 can summon kami to join "
	"him, who will aid him in combat. "
	"A unit may summon a number of kami "
	"equal to his skill level minus two, squared; the kami "
	"will appear in his inventory. The kami have a chance of breaking free "
	"at the end of each turn. To summon kami, issue "
	"the order CAST Summon_Spirits KAMI.";
    } else if (level == 5) {
      if(ITEM_DISABLED(I_MAJOR_KAMI)) break;
      *str += "A unit with Summon Spirits 5 can summon a major kami to join "
	"him, who will aid him in combat. A unit may only summon one major kami"
	"; the major kami will appear in his inventory. The major kami has a chance "
	"of breaking free at the end of each turn. To summon a major kami, " 
	"issue the order CAST Summon_Spirits MAJOR_KAMI.";
    }
    break;
    
  case S_SUMMON_DEMON:
    if(level == 1) {
      if(ITEM_DISABLED(I_IMP)) break;
      *str += "A unit with the Summon Demon skill may summon imps "
	"into his inventory, to aid him in combat. A unit may summon "
	"his skill level in imps each turn; however, the imps have a chance of "
	"breaking free of the unit's control at the end of each "
	"turn. This chance is based on the number of imps in the "
	"unit's control. To use this spell, the unit should issue the "
	"order CAST Summon_Demon IMP.";
    } else if (level == 3) {
      if(ITEM_DISABLED(I_ONI)) break;
      *str += "A unit with Summon Demon 3 can summon oni to join "
	"him, who will aid him in combat. "
	"A unit may summon a number of oni "
	"equal to his skill level minus two, squared; the oni "
	"will appear in his inventory. The oni have a chance of breaking free "
	"at the end of each turn. To summon oni, issue "
	"the order CAST Summon_Demon ONI.";
    } else if (level == 5) {
      if(ITEM_DISABLED(I_DAI_ONI)) break;
      *str += "A unit with Summon Demon 5 can summon a Dai-oni to join "
	"him, who will aid him in combat. A unit may only summon one dai-oni"
	"; the dai-oni will appear in his inventory. The dai-oni has a chance "
	"of breaking free at the end of each turn. To summon a dai-oni, " 
	"issue the order CAST Summon_Demon DAI_ONI.";
    }
    break;



    /*  case S_SUMMON_BALROG:
    if(level > 1) break;
    if(ITEM_DISABLED(I_DAI_ONI)) break;
    *str += "A sorceror with the Summon Balrog skill may summon a balrog "
      "into his inventory, to aid him in combat. A sorceror has a 20 "
      "percent times his skill level chance of summoning a balrog, "
      "but may only summon a balrog if one is not already under "
      "his control. As with other demons, the balrog has a chance "
      "of breaking free of the sorceror's control at the end of each "
      "turn. This chance is equal to 1 over 4 times the sorceror's "
      "skill level to the fourth power (or, from 1 over 4 at "
      "level 1, to 1 over 2500 at level 5). To use this spell, "
      "the sorceror should issue the order CAST Summon_Balrog.";
    break;*/
  case S_BANISH_DEMONS:
    if(level > 1) break;
    break;
    /* case S_ILLUSION:
    if(level > 1) break;
    *str += "Illusion is the magic of creating isorcerors of things that "
      "do not actually exist. The Illusion skill does not have any "
      "direct applications, but is required for further study of "
      "Illusionary magic. A sorceror with knowledge of the Illusion "
      "skill will detect the use of Illusion by any other sorceror in "
      "the same region.";
      break;*/
    /*  case S_PHANTASMAL_ENTERTAINMENT:
        if(level > 1) break;
    *str += "A sorceror with the Phantasmal Entertainment skill may use "
      "his powers of Illusion to earn money by creating "
      "illusionary fireworks, puppet shows, etc. In effect, "
      "Phantasmal Entertainment grants the sorceror Entertainment "
      "skill equal to five times his Phantasmal Entertainment "
      "level. To use this skill, use the ENTERTAIN order.";
      break;*/
    /*  case S_CREATE_PHANTASMAL_BEASTS:
    if(level == 1) {
      *str += "A sorceror with Create Phantasmal Beasts may summon "
	"illusionary beasts that appear in the sorceror's inventory. "
	"These beasts will fight in combat, but do not attack, "
	"and are killed whenever they are attacked.";
      if(ITEM_ENABLED(I_IWOLF)) {
	*str += " Create Phantasmal Beasts at level 1 allows the "
	  "sorceror to summon illusionary wolves; the number the "
	  "sorceror can summon, or have in his inventory at one "
	  "time is equal to the sorceror's skill squared times 4. "
	  "To use this spell, the sorceror should CAST "
	  "Create_Phantasmal_Beasts WOLF <number>, where "
	  "<number> is the number of wolves that the sorceror "
	  "wishes to have appear in his inventory.";
      }
      *str += " Note: illusionary beasts will appear on reports as "
	"if they were normal items, except on the owner's "
	"report, where they are marked as illusionary. To "
	"reference these items in orders, you must prepend an "
	"'i' to the normal string. (For example: to reference "
	"an illusionary wolf, you would use 'iwolf').";
    } else if (level == 3) {
      if(ITEM_DISABLED(I_IEAGLE)) break;
      *str += "Create Phantasmal Beasts at level 3 allows the sorceror "
	"to summon illusionary eagles into his inventory. To "
	"summon illusionary eagles, the sorceror should CAST "
	"Create_Phantasmal_Beasts EAGLE <number>, where <number> "
	"is the number of eagles that the sorceror wishes to have "
	"appear in his inventory. The number of eagles that a "
	"sorceror may have in his inventory is equal to his skill "
	"level, minus 2, squared.";
    } else if(level == 5) {
      if(ITEM_DISABLED(I_IDRAGON)) break;
      *str += "Create Phantasmal Beasts at level 5 allows the "
	"sorceror to summon an illusionary dragon into his "
	"inventory. To summon an illusionary dragon, the sorceror "
	"should CAST Create_Phantasmal_Beasts DRAGON; the sorceror "
	"can only have one illusionary dragon in his inventory "
	"at one time.";
    }
    break;*/
    /*  case S_CREATE_PHANTASMAL_UNDEAD:
    if(level == 1) {
      *str += "A sorceror with Create Phantasmal Undead may summon "
	"illusionary undead that appear in the sorceror's inventory. "
	"These undead will fight in combat, but do not attack, "
	"and are killed whenever they are attacked.";
      if(ITEM_ENABLED(I_ISKELETON)) {
	*str += " Create Phantasmal Undead at level 1 allows the "
	  "sorceror to summon illusionary skeletons; the number "
	  "the sorceror can summon, or have in his inventory at "
	  "one time is equal to the sorceror's skill squared times "
	  "4. To use this spell, the sorceror should CAST "
	  "Create_Phantasmal_Undead SKELETON <number>, where "
	  "<number> is the number of skeletons that the sorceror "
	  "wishes to have appear in his inventory.";
      }
      *str += " Note: illusionary undead will appear on reports as "
	"if they were normal items, except on the owner's "
	"report, where they are marked as illusionary. To "
	"reference these items in orders, you must prepend an "
	"'i' to the normal string. (Example: to reference an "
	"illusionary skeleton, you would use 'iskeleton').";
    } else if(level == 3) {
      if(ITEM_DISABLED(I_IUNDEAD)) break;
      *str += "Create Phantasmal Undead at level 3 allows the sorceror "
	"to summon illusionary undead into his inventory. To "
	"summon illusionary undead, the sorceror should CAST "
	"Create_Phantasmal_Undead UNDEAD <number>, where <number> "
	"is the number of undead that the sorceror wishes to have "
	"appear in his inventory. The number of undead that a "
	"sorceror may have in his inventory is equal to his skill "
	"level, minus 2, squared.";
    } else if (level == 5) {
      if(ITEM_DISABLED(I_ILICH)) break;
      *str += "Create Phantasmal Undead at level 5 allows the sorceror "
	"to summon an illusionary lich into his inventory. To "
	"summon an illusionary lich, the sorceror should CAST "
	"Create_Phantasmal_Undead LICH; the sorceror can only have "
	"one illusionary lich in his inventory at one time.";
    }
    break;*/
    /*  case S_CREATE_PHANTASMAL_DEMONS:
    if(level == 1) {
      *str += "A sorceror with Create Phantasmal Demons may summon "
	"illusionary demons that appear in the sorceror's "
	"inventory. These demons will fight in combat, but "
	"do not attack, and are killed whenever they are "
	"attacked.";
      if(ITEM_ENABLED(I_IIMP)) {
	*str += " Create Phantasmal Demons at level 1 allows the "
	  "sorceror to summon illusionary imps; the number the "
	  "sorceror can summon, or have in his inventory at one "
	  "time is equal to the sorceror's skill squared times 4. "
	  "To use this spell, the sorceror should CAST "
	  "Create_Phantasmal_Demons IMP <number>, where "
	  "<number> is the number of imps that the sorceror wishes "
	  "to have appear in his inventory.";
      }
      *str += " Note: illusionary demons will appear on reports as "
	"if they were normal items, except on the owner's "
	"report, where they are marked as illusionary. To "
	"reference these items in orders, you must prepend an "
	"'i' to the normal string. (Example: to reference an "
	"illusionary imp, you would use 'iimp').";
    } else if (level == 3) {
      if(ITEM_DISABLED(I_IDEMON)) break;
      *str += "Create Phantasmal Demons at level 3 allows the sorceror "
	"to summon illusionary demons into his inventory. To "
	"summon illusionary demons, the sorceror should CAST "
	"Create_Phantasmal_Demons DEMON <number>, where <number> "
	"is the number of demons that the sorceror wishes to have "
	"appear in his inventory. The number of demons that a "
	"sorceror may have in his inventory is equal to his skill "
	"level, minus 2, squared.";
    } else if (level == 5) {
      if(ITEM_DISABLED(I_IBALROG)) break;
      *str += "Create Phantasmal Demons at level 5 allows the sorceror "
	"to summon an illusionary balrog into his inventory. To "
	"summon an illusionary balrog, the sorceror should CAST "
	"Create_Phantasmal_Demons BALROG; the sorceror can only have "
	"one illusionary balrog in his inventory at one time.";
    }
    break;*/
  case S_INVISIBILITY:
    /* XXX -- This should be cleaner somehow. */
    if(level > 1) break;
    *str += "The Invisibility skill allows a sorceror to render other "
      "units nearly invisible to other factions, giving them a +3 "
      "bonus to Stealth. This invisibility will last until the "
      "next Magic round. To cast this spell, use the order CAST "
      "Invisibility UNITS <unit> ..., where <unit> is a list of "
      "the units that the sorceror wishes to render invisible. A sorceror "
      "may render invisible a number of men or creatures equal to "
      "his skill level squared.";
    break;
  case S_TRUE_SEEING:
    if(level > 1) break;
    *str += "A sorceror with the True Seeing spell can see illusions "
      "for what they really are. Whether or not the sorceror can see "
      "through the illusion depends on his True Seeing skill "
      "being higher that the Illusion skill of the sorceror casting "
      "the illusion. This spell does not require any order to "
      "use; it is used automatically.";
    if(SKILL_ENABLED(S_OBSERVATION)) {
      *str += "In addition, a sorceror with the True Seeing skill "
	"receives a bonus to his Observation skill equal to his "
	"True Seeing skill divided by 2, rounded up.";
    }
    break;
    /*  case S_DISPEL_ILLUSIONS:
    if(level > 1) break;
    break;*/
    /*  case S_ARTIFACT_LORE:
    if(level > 1) break;
    *str += "Artifact Lore is one of the most advanced forms of "
      "magic; in general, creation of an artifact requires both "
      "Artifact Lore, and the appropriate skill for the item being "
      "created. A sorceror with knowledge of the Artifact Lore skill "
      "will detect the use of Artifact Lore by any other sorceror in "
      "the region.";
      break;*/
    /*  case S_CREATE_RING_OF_INVISIBILITY:
    if(ITEM_DISABLED(I_RINGOFI)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Ring of Invisibility skill may "
      "create a Ring of Invisibility, which grants a 3 bonus to a "
      "unit's effective Stealth (note that a unit must possess "
      "one ring for each man to gain this bonus).";
    if(ITEM_ENABLED(I_AMULETOFTS)) {
      *str += " A Ring of Invisibility has one limitation; a "
	"unit possessing a ring cannot assassinate, nor steal "
	"from, a unit with an Amulet of True Seeing.";
    }
    *str += " A sorceror has a 20 percent times his level chance to "
      "create a Ring of Invisibility. To use this spell, the sorceror "
      "should CAST Create_Ring_of_Invisibility.";
      break;*/
    /*  case S_CREATE_CLOAK_OF_INVULNERABILITY:
    if(ITEM_DISABLED(I_CLOAKOFI)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Cloak of Invulnerability skill "
      "may create a Cloak of Invulnerability. A sorceror has a 20 "
      "percent times his level chance to create a Cloak of "
      "Invulnerability. To use this spell, the sorceror should CAST "
      "Create_Cloak_of_Invulnerability.";
      break;*/
    /*  case S_CREATE_STAFF_OF_FIRE:
    if(ITEM_DISABLED(I_STAFFOFF)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Staff of Fire skill may create a "
      "Staff of Fire. A Staff of Fire allows any sorceror to throw "
      "fireballs in combat as if he had a Fire skill of 3. A sorceror "
      "has a 20 percent times his level chance to create a Staff "
      "of Fire. To use this spell, CAST Create_Staff_of_Fire.";
    break;*/
    /*  case S_CREATE_STAFF_OF_LIGHTNING:
    if(ITEM_DISABLED(I_STAFFOFL)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Staff of Lightning skill may "
      "create a Staff of Lightning. A Staff of Lightning allows "
      "any sorceror to call down lightning bolts as if he had Call "
      "Lightning skill of 3. A sorceror has a 20 percent times his "
      "level chance to create a Staff of Lightning. To use this "
      "spell, CAST Create_Staff_of_Lightning.";
      break;*/
    /*  case S_CREATE_AMULET_OF_TRUE_SEEING:
    if(ITEM_DISABLED(I_AMULETOFTS)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Amulet of Tree Seeing skill may "
      "create an Amulet of True Seeing. This amulet gives the "
      "possessor a bonus of 2 to his effective Observation skill.";
    if(ITEM_ENABLED(I_RINGOFI)) {
      *str += "Also, a unit with an Amulet of True Seeing cannot "
	"be assassinated by, nor have items stolen by, a unit "
	"with a Ring of Invisibility (Note that the unit must "
	"have at least one Amulet of True Seeing per man to "
	"repel a unit with a Ring of Invisibility).";
    }
    *str += "A sorceror has a 20 percent times his skill level chance to "
      "create an Amulet of True Seeing. To use this spell, CAST "
      "Create_Amulet_of_True_Seeing.";
      break;*/
    /*  case S_CREATE_AMULET_OF_PROTECTION:
    if(ITEM_DISABLED(I_AMULETOFP)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Amulet of Protection skill may "
      "create Amulets of Protection, which grants the possesor a "
      "personal Spirit Shield of 3. A sorceror may create his skill "
      "level of these amulets per turn. To use this spell, CAST "
      "Create_Amulet_of_Protection.";
      break;*/
    /*  case S_CREATE_RUNESWORD:
    if(ITEM_DISABLED(I_RUNESWORD)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Runesword skill may create a "
      "Runesword, which when wielded in combat gives the wielder "
      "a plus 4 bonus to Combat skill, and also allows the wielder "
      "to project an Aura of Fear in battle, as if he had Create "
      "Aura of Fear skill of level 3 (provided the wielder is "
      "not casting any other combat spells). A sorceror has a 20 "
      "percent times his skill level chance of creating a "
      "Runesword. To cast this spell, CAST Create_Runesword.";
      break;*/
    /*  case S_CREATE_SHIELDSTONE:
    if(ITEM_DISABLED(I_SHIELDSTONE)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Shieldstone skill may create "
      "Shieldstones, which confers upon the possessor a personal "
      "Energy Shield of 3. A sorceror may create his skill level in "
      "Shieldstones per turn. To use this spell, CAST "
      "Create_Shieldstone";
      break;*/
    /*  case S_CREATE_MAGIC_CARPET:
    if(ITEM_DISABLED(I_MCARPET)) break;
    if(level > 1) break;
    *str += "A sorceror with the Create Magic Carpet skill may create "
      "Magic Carpets, which provide for flying transportation. A "
      "Magic Carpet can carry up to 15 weight units in the air. "
      "Casting this spell allows the sorceror to create his skill "
      "level in Magic Carpets. To cast the spell, CAST "
      "Create_Magic_Carpet.";
      break;*/
  case S_ENGRAVE_RUNES_OF_WARDING:
    if(level == 1) {
      *str += "A sorceror with the Engrave Runes of Warding may "
	"engrave runes of warding on a building; these runes "
	"will give any occupants of the building a personal "
	"Energy Shield, Force Shield and Spirit Shield, both at level 3. A "
	"unit has a 20 percent chance per level of succeeding "
	"with each attempt to cast this spell. To use this "
	"spell, the unit should CAST Engrave_Runes_of_Warding, "
	"and be within the building he wishes to engrave runes "
	"upon. This spell costs 1000 silver to cast.";
      if(OBJECT_ENABLED(O_TOWER)) {
	*str += " At level 1, the sorceror may engrave runes of "
	  "warding upon a Tower.";
      }
    } else if (level == 2) {
      int comma = 0;
      if(OBJECT_DISABLED(O_FORT) && OBJECT_DISABLED(O_MTOWER))
	break;
      *str += "At this level, the sorceror may engrave runes of "
	"warding upon ";
      if(OBJECT_ENABLED(O_FORT)) {
	*str += "a Fort";
	comma = 1;
      }
      if(OBJECT_ENABLED(O_MTOWER)) {
	if(comma) *str += ", and ";
	*str += "a Magic Tower";
      }
      *str += ".";
    } else if (level == 3) {
      if(OBJECT_ENABLED(O_CASTLE)) {
	*str += "At this level, the sorceror may engrave runes of "
	  "warding upon a Castle.";
      }
    } else if (level == 4) {
      if(OBJECT_ENABLED(O_CITADEL)) {
	*str += "At this level, the sorceror may engrave runes of "
	  "warding upon a Citadel.";
      }
    } else if(level == 5) {
      if(OBJECT_ENABLED(O_MFORTRESS)) {
	*str += "At this level, the sorceror may engrave runes of "
	  "warding upon a Magical Fortress, which grants "
	  "the inhabitants an Energy Shield, Force Shield and Spirit "
	  "Shield at level 5.";
      }
    }
    break;
  case S_CONSTRUCT_GATE:
    if(level > 1) break;
    *str += "A sorceror with the Construct Gate skill may construct a "
      "Gate in a region. The sorceror has a 20 percent times his "
      "skill level chance of success, and the attempt costs 10000 "
      "silver. To use this spell, the sorceror should issue the order "
      "CAST Construct_Gate.";
    break;
    //  case S_ENCHANT_SWORDS:
    /* XXX -- This should be cleaner somehow. */
    //if(level > 1) break;
    //if(ITEM_DISABLED(I_MSWORD)) break;
    //*str += "A sorceror with the Enchant Swords skill may magically "
    //  "create mithril swords. A sorceror may create 5 times his "
    //  "skill level mithril swords per turn. The sorceror should "
    //  "issue the order CAST Enchant_Swords to cast this spell.";
    //break;
    //case S_ENCHANT_ARMOR:
    /* XXX -- This should be cleaner somehow. */
    //if(level > 1) break;
    //if(ITEM_DISABLED(I_MPLATE)) break;
    //*str += "A sorceror with the Enchant Armor skill may magically "
    //  "create mithril armor. A sorceror may create 5 times his skill "
    //  "level mithril armors per turn. The sorceror should issue the "
    //  "order CAST Enchant_Armor to cast this spell.";
    //break;
      //case S_CONSTRUCT_PORTAL:
    /* XXX -- This should be cleaner somehow. */
    //if(level > 1) break;
    //if(ITEM_DISABLED(I_MPLATE)) break;
    //*str += "A sorceror with the Construct Portal skill may construct a "
    //  "Portal";
    //if(SKILL_ENABLED(S_PORTAL_LORE)) {
    //  *str += " for use with the Portal Lore skill";
    //}
    //*str += ". The sorceror has a 20 percent times his skill level "
    //  "chance of creating a Portal, and the attempt costs 600 "
    //  "silver. To use this spell, CAST Construct_Portal.";
    //break;
    /*  case S_MANIPULATE:
    if(!Globals->APPRENTICES_EXIST) break;
    if(level > 1) break;
    *str += "A unit with this skill becomes an apprentice sorceror. "
      "While apprentices cannot cast spells directly, they can "
      "use magic items normally only usable by sorcerors. Continued "
      "study of this skill gives no further advantages.";
      break;*/
    //case S_WEAPONCRAFT:
    //if(level > 1) break;
    //*str += "The weaponcraft skill is an advanced version of the "
    //  "weaponsmith skill.";
    //break;
    //case S_ARMORCRAFT:
    //if(level > 1) break;
    //*str += "The armorcraft skill is an advanced version of the "
    //  "armorsmith skill.";
    //break;
    //case S_CAMELTRAINING:
      //if(level > 1) break;
      //*str += "This skill deals with all aspects of camel production.";
      //break;
      //case S_GEMCUTTING:
    //if(level > 1) break;
    //*str += "This skill enables a unit to fashion higher quality "
    //  "gems from lower quality ones.";
    //break;
    //case S_MONSTERTRAINING:
    //if(level > 1) break;
    //*str += "This skill deals with all aspects of training monster "
    //  "mounts.";
    //break;
    //case S_COOKING:
    //if(level > 1) break;
    //*str += "This skill deals with creating provisions from basic "
    //  "foodstuffs.  A skilled cook can feed many more people "
    //  "than a farmer alone.";
    //break;
    //case S_CREATE_FOOD:
    /* XXX -- This should be cleaner somehow. */
    //if(level > 1) break;
    //if(ITEM_DISABLED(I_FOOD)) break;
    //*str += "A sorceror with the Create Food skill may magically "
    //  "create food. A sorceror may create 5 times his skill level "
    //  "provisions per turn. The sorceror should issue the order "
    //  "CAST Create_Food to cast this spell.";
    //break;
  default:
    break;
  }

  AString temp;
  AString temp1;
  AString temp2;
  AString temp3;
  AString temp4;
  int comma = 0;
  int comma1 = 0;
  int comma2 = 0;
  int last = -1;
  int last1 = -1;
  int last2 = -1;
  unsigned int c;
  int i;
  int build = 0;

  //BUSHIDO mods
  //passive combat spell - automatic
  if(level == 1 && (SkillDefs[skill].flags & SkillType::PASSIVE_COMBAT)) {
    *str += AString(" A unit with this skill can cast ") +
      ShowSpecial(SkillDefs[skill].special, level, 0, 0);
    *str += " This is a passive combat ability that will be cast automatically.";
  }
  //end BUSHIDO mods

  // If this is a combat spell, note it.
  if(level == 1 && (SkillDefs[skill].flags & SkillType::COMBAT)) {
    *str += AString(" A unit with this skill can cast ") +
      ShowSpecial(SkillDefs[skill].special, level, 0, 0);
    *str += " In order to use this spell in combat, the unit should use "
      "the COMBAT order to set it as his combat spell.";
  }

  // production and ability to see items
  temp += "A unit with this skill is able to determine if a region "
    "contains ";
  temp1 += "A unit with this skill may PRODUCE ";
  temp2 += "A unit with this skill may create ";
  // for the new ship items
  temp3 += "A unit with this skill may BUILD ";

  SkillType *sk1, *sk2;
  sk1 = &SkillDefs[skill];
  for(i = NITEMS - 1; i >= 0; i--) {
    if(ITEM_DISABLED(i)) continue;
    sk2 = FindSkill(ItemDefs[i].mSkill);
    if(sk1 == sk2 && ItemDefs[i].mLevel==level && last2==-1) {
      int canmagic = 1;
      for(c = 0; c < sizeof(ItemDefs[i].mInput)/sizeof(Materials); c++) {
	if(ItemDefs[i].mInput[c].item == -1) continue;
	if(ITEM_DISABLED(ItemDefs[i].mInput[c].item)) {
	  canmagic = 0;
	}
      }
      if(canmagic) {
	last2 = i;
      }
    }
    sk2 = FindSkill(ItemDefs[i].pSkill);
    if(sk1 == sk2 && ItemDefs[i].pLevel == level) {
      int canmake = 1;
      int resource = 1;
      for(c = 0; c < sizeof(ItemDefs[i].pInput)/sizeof(Materials); c++) {
	if(ItemDefs[i].pInput[c].item == -1) continue;
	resource = 0;
	if(ITEM_DISABLED(ItemDefs[i].pInput[c].item)) {
	  canmake = 0;
	}
      }
      if(canmake && last1 == -1) {
	last1 = i;
      }
      if(resource && (ItemDefs[i].type & IT_ADVANCED) && last == -1) {
	last = i;
      }
    }

  }
  for(i = 0; i < NITEMS; i++) {
    if(ITEM_DISABLED(i)) continue;
    int illusion = (ItemDefs[i].type & IT_ILLUSION);
    sk2 = FindSkill(ItemDefs[i].mSkill);
    if(sk1 == sk2 && ItemDefs[i].mLevel == level) {
      int canmagic = 1;
      for(c = 0; c < sizeof(ItemDefs[i].mInput)/sizeof(Materials); c++) {
	if(ItemDefs[i].mInput[c].item == -1) continue;
	if(ITEM_DISABLED(ItemDefs[i].mInput[c].item)) {
	  canmagic = 0;
	}
      }
      if(canmagic) {
	if(comma2) {
	  if(last2 == i) {
	    if(comma2 > 1) temp2 += ",";
	    temp2 += " and ";
	  } else {
	    temp2 += ", ";
	  }
	}
	comma2++;
	temp2 += AString(illusion?"illusory ":"") + ItemDefs[i].names;
	if(f) {
	  f->DiscoverItem(i, 1, 1);
	}
      }
    }
    sk2 = FindSkill(ItemDefs[i].pSkill);
    if(sk1 == sk2 && ItemDefs[i].pLevel == level) {
      int canmake = 1;
      int resource = 1;
      for(c = 0; c < sizeof(ItemDefs[i].pInput)/sizeof(Materials); c++) {
	if(ItemDefs[i].pInput[c].item == -1) continue;
	resource = 0;
	if(ITEM_DISABLED(ItemDefs[i].pInput[c].item)) {
	  canmake = 0;
	}
      }
      if(canmake) {
	// IT_SHIP: switch to BUILD description
	if((ItemDefs[i].type & IT_SHIP) && (build == 0)) {
	  temp1 = temp3;
	  build = 1;
	}
	if(comma1) {
	  if(last1 == i) {
	    if(comma1 > 1) temp1 += ",";
	    temp1 += " and ";
	  } else {
	    temp1 += ", ";
	  }
	}
	comma1++;
	temp1 += AString(illusion?"illusory ":"") + ItemDefs[i].names;
	if(f) {
	  f->DiscoverItem(i, 1, 1);
	}
      }
      if(resource && (ItemDefs[i].type & IT_ADVANCED)) {
	if(comma) {
	  if(last == i) {
	    if(comma > 1) temp += ",";
	    temp += " and ";
	  } else {
	    temp += ", ";
	  }
	}
	comma++;
	temp += AString(illusion?"illusory ":"") + ItemDefs[i].names;
      }
    }
  }
  if(comma1) {
    if(!(*str == "")) *str += " ";
    *str += temp1 + ".";
  }
  if(comma) {
    if(!(*str == "")) *str += " ";
    *str += temp + ".";
  }
  if(comma2) {
    if(!(*str == "")) *str += " ";
    *str += temp2 + " via magic.";
  }

  // Buildings
  comma = 0;
  temp = "A unit with this skill may BUILD the following structures: ";
  for(i = 0; i < NOBJECTS; i++) {
    if(OBJECT_DISABLED(i)) continue;
    AString skname = SkillDefs[skill].abbr;
    if(skname == ObjectDefs[i].skill && ObjectDefs[i].level == level) {
      if(comma) temp += ", ";
      comma = 1;
      temp += ObjectDefs[i].name;
      if(f) {
	f->objectshows.Add(ObjectDescription(i));
      }
    }
  }
  if(comma) {
    if(!(*str == "")) *str += " ";
    *str += temp + ".";
  }

  // Required skills
  SkillType *lastpS = NULL;
  last = -1;
  if(level == 1) {
    comma = 0;
    int found = 0;
    temp = "This skill requires ";
    for(c=0; c<sizeof(SkillDefs[skill].depends)/sizeof(SkillDepend); c++) {
      SkillType *pS = FindSkill(SkillDefs[skill].depends[c].skill);
      if (!pS || (pS->flags & SkillType::DISABLED)) continue;
      found = 1;
      if(lastpS == NULL) {
	lastpS = pS;
	last = c;
	continue;
      }
      temp += SkillStrs(lastpS) + " " +
	SkillDefs[skill].depends[last].level + ", ";
      lastpS = pS;
      last = c;
      comma++;
    }
    if(comma) {
      temp += "and ";
    }
    if (found) {
      temp += SkillStrs(lastpS) + " " +
	SkillDefs[skill].depends[last].level;
    }

    if(found) {
      if(!(*str == "")) *str += " ";
      *str += temp + " to begin to study.";
    }
  }

  if(level == 1) {
    if(SkillDefs[skill].cost) {
      if(!(*str == "")) *str += " ";
      *str += AString("This skill costs ") + SkillDefs[skill].cost +
	" silver per month of study.";
    }
    if(SkillDefs[skill].flags & SkillType::SLOWSTUDY) {
      if(!(*str == "")) *str += " ";
      *str += "This skill is studied at one half the normal speed.";
    }
  }

  // Tell whether this skill can be taught, studied, or gained through exp
  if(SkillDefs[skill].flags & SkillType::NOSTUDY) {
    if(!(*str == "")) *str += " ";
    *str += "This skill cannot be studied via normal means.";
  }
  if(SkillDefs[skill].flags & SkillType::NOTEACH) {
    if(!(*str == "")) *str += " ";
    *str += "This skill cannot be taught to other units.";
  }
  if((Globals->SKILL_PRACTICE_AMOUNT > 0) && 
     (SkillDefs[skill].flags & SkillType::NOEXP)) {
    if(!(*str == "")) *str += " ";
    *str += "This skill cannot be increased through experience.";
  }

  temp1 = SkillStrs(skill) + " " + level + ": ";
  if(*str == "") {
    *str = temp1 + "No skill report.";
  } else {
    *str = temp1 + *str;
  }

  return str;
}
