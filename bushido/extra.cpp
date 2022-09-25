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
//
// This file contains extra game-specific functions
//
#include "game.h"
#include "gamedata.h"
#include "rules.h"


int Game::SetupFaction( Faction *pFac )
{
	pFac->unclaimed = Globals->START_MONEY + TurnNumber() * 500;

	if(pFac->noStartLeader)
		return 1;

	//
	// Set up first unit.
	//
	Unit *temp21 = GetNewUnit( pFac );
	Unit *temp22 = GetNewUnit( pFac );
	Unit *temp23 = GetNewUnit( pFac );
	Unit *temp24 = GetNewUnit( pFac );
	Unit *temp25 = GetNewUnit( pFac );
	Unit *temp26 = GetNewUnit( pFac );
	Unit *temp27 = GetNewUnit( pFac );
	Unit *temp28 = GetNewUnit( pFac );
	AString *name;

	name = new AString("Samurai");
	temp21->SetMen( I_SAMURAI, 1 );
	temp21->reveal = REVEAL_FACTION;
	temp21->type = U_BUSHIDO_SAMURAI;
	temp21->SetFlag(FLAG_BEHIND,1);
	temp21->SetSkill(S_WAY_SAMURAI,3);
	temp21->SetSkill(S_COMBAT,1);
	temp21->SetSkill(S_TACTICS,1);
	temp21->items.SetNum(I_KIMONO,1);
	temp21->items.SetNum(I_KATANA,1);
	temp21->SetName(name);

	name = new AString("Merchant");
	temp22->SetMen( I_MERCHANT, 1 );
	temp22->SetFlag(FLAG_BEHIND,1);
	temp22->reveal = REVEAL_FACTION;
	temp22->type = U_BUSHIDO_MERCHANT;
	temp22->SetSkill(S_WAY_MERCHANT,3);
	temp22->SetSkill(S_CARAVANMASTER,1);
	temp22->items.SetNum(I_KIMONO,1);
	temp22->items.SetNum(I_TANTO,1);
	temp22->SetName(name);
	temp22->guard = GUARD_AVOID;

	name = new AString("Ninja");
	temp23->SetMen( I_NINJA, 1 );
	temp23->reveal = REVEAL_FACTION;
	temp23->SetFlag(FLAG_BEHIND,1);
;	temp23->type = U_BUSHIDO_NINJA;
	temp23->SetSkill(S_WAY_NINJA,3);
	temp23->SetSkill(S_COMBAT,1);
	temp23->SetSkill(S_STEALTH,3);
	temp23->items.SetNum(I_NINJATO,1);
	temp23->items.SetNum(I_KIMONO,1);
	temp23->SetName(name);
	temp23->guard = GUARD_AVOID;

	name = new AString("Sorceror");
	temp24->SetMen( I_SORCEROR, 1 );
	temp24->reveal = REVEAL_FACTION;
	temp24->type = U_BUSHIDO_SORCEROR;
	temp24->SetFlag(FLAG_BEHIND,1);
	temp24->SetSkill(S_WAY_SORCEROR,3);
	temp24->SetSkill(S_MIND_READING,1);
	temp24->SetSkill(S_MAGICAL_HEALING,1);
	temp24->items.SetNum(I_KIMONO,1);
	temp24->items.SetNum(I_TANTO,1);
	temp24->SetName(name);
	temp24->guard = GUARD_AVOID;

	name = new AString("peasant army");
	temp25->SetMen( I_MAN, 100 );
	temp25->reveal = REVEAL_FACTION;
	temp25->type = U_NORMAL;
	temp25->SetSkill(S_COMBAT,3);
	temp25->items.SetNum(I_YARI,100);
	temp25->items.SetNum(I_PADDED_ARMOUR,100);
	temp25->SetName(name);

	name = new AString("peasant farmers");
	temp26->SetMen( I_MAN, 10 );
	temp26->reveal = REVEAL_FACTION;
	temp26->type = U_NORMAL;
	temp26->SetSkill(S_FARMING,3);
	temp26->SetName(name);
	temp26->guard = GUARD_AVOID;
	temp26->SetFlag(FLAG_BEHIND,1);

	name = new AString("peasant builders");
	temp27->SetMen( I_MAN, 10 );
	temp27->reveal = REVEAL_FACTION;
	temp27->type = U_NORMAL;
	temp27->items.SetNum(I_WAGON,10);
	temp27->items.SetNum(I_HORSE,10);
	temp27->items.SetNum(I_WOOD,20);
	temp27->items.SetNum(I_STONE,20);
	temp27->SetSkill(S_BUILDING,3);
	temp27->SetName(name);
	temp27->guard = GUARD_AVOID;
	temp27->SetFlag(FLAG_BEHIND,1);

	name = new AString("peasant rabble");
	temp28->SetMen( I_MAN, 100 );
	temp28->reveal = REVEAL_FACTION;
	temp28->type = U_NORMAL;
	temp28->items.SetNum(I_GRAIN,2000);
	temp28->items.SetNum(I_HORSE,10);
	temp28->items.SetNum(I_WAGON,10);
	temp28->SetName(name);
	temp28->guard = GUARD_AVOID;
	temp28->SetFlag(FLAG_BEHIND,1);

	//random start location
	ARegion *reg = NULL;
	if(pFac->pStartLoc) {
	  reg = pFac->pStartLoc;
	} else {
	  ARegionArray *pArr = regions.GetRegionArray(ARegionArray::LEVEL_SURFACE);
	  while(!reg) {
	    reg = pArr->GetRegion(getrandom(pArr->x), getrandom(pArr->y));
	    if (reg) {
	      if (reg->type == R_OCEAN)
		reg = NULL;
	    }
	  }
	}

	temp21->MoveUnit( reg->GetDummy() );
	temp22->MoveUnit( reg->GetDummy() );
	temp23->MoveUnit( reg->GetDummy() );
	temp24->MoveUnit( reg->GetDummy() );
	temp25->MoveUnit( reg->GetDummy() );
	temp26->MoveUnit( reg->GetDummy() );
	temp27->MoveUnit( reg->GetDummy() );
	temp28->MoveUnit( reg->GetDummy() );
	return( 1 );
}

Faction *Game::CheckVictory()
{
	return NULL;
}

void Game::ModifyTablesPerRuleset(void)
{

  //turn on bushido skills
  //EnableSkill(S_WAY_KUGE);
  //EnableSkill(S_WAY_SAMURAI);
  //EnableSkill(S_WAY_NINJA);
  //EnableSkill(S_WAY_GEISHA);
  //EnableSkill(S_WAY_MERCHANT);
  //EnableSkill(S_WAY_ARTISAN);
  //EnableSkill(S_WAY_PRIEST);
  //EnableSkill(S_WAY_SORCEROR);
  EnableSkill(S_ENDURANCE);

  //other skills
  // if(Globals->APPRENTICES_EXIST)
  //  EnableSkill(S_MANIPULATE);
  
  //if(!Globals->GATES_EXIST)
  //  DisableSkill(S_GATE_LORE);
  
  //if (Globals->FULL_TRUESEEING_BONUS) {
    //    ModifyAttribMod("observation", 1, AttribModItem::SKILL,
		    //		    "TRUE", AttribModItem::UNIT_LEVEL, 1);
    //}
  //if (Globals->IMPROVED_AMTS) {
  //  ModifyAttribMod("observation", 2, AttribModItem::ITEM,
  //		    "AMTS", AttribModItem::CONSTANT, 3);
  //}
  //if (Globals->FULL_INVIS_ON_SELF) {
  //  ModifyAttribMod("stealth", 3, AttribModItem::SKILL,
  //		    "INVI", AttribModItem::UNIT_LEVEL, 1);
  //}
  
  if(Globals->NEXUS_IS_CITY && Globals->TOWNS_EXIST) {
    ClearTerrainRaces(R_NEXUS);
    //    ModifyTerrainRace(R_NEXUS, 0, I_HIGHELF);
    //ModifyTerrainRace(R_NEXUS, 1, I_VIKING);
    //ModifyTerrainRace(R_NEXUS, 2, I_PLAINSMAN);
    ClearTerrainItems(R_NEXUS);
    ModifyTerrainItems(R_NEXUS, 0, I_IRON, 100, 10);
    ModifyTerrainItems(R_NEXUS, 1, I_WOOD, 100, 10);
    ModifyTerrainItems(R_NEXUS, 2, I_STONE, 100, 10);
    ModifyTerrainEconomy(R_NEXUS, 1000, 15, 50, 2);
  }
  
  //EnableItem(I_PICK);
  //EnableItem(I_SPEAR);
  //EnableItem(I_AXE);
//  EnableItem(I_HAMMER);
  //EnableItem(I_MCROSSBOW);
  //EnableItem(I_MWAGON);
  //EnableItem(I_GLIDER);
  //EnableItem(I_NET);
  //EnableItem(I_LASSO);
  //EnableItem(I_BAG);
  //EnableItem(I_SPINNING);
  //EnableItem(I_LEATHERARMOR);
  //EnableItem(I_CLOTHARMOR);
  //EnableItem(I_BOOTS);
  //EnableItem(I_BAXE);
  //EnableItem(I_MBAXE);
  //EnableItem(I_IMARM);
  //EnableItem(I_SUPERBOW);
  //EnableItem(I_LANCE);
  //EnableItem(I_JAVELIN);
  //EnableItem(I_PIKE);
  
  //EnableSkill(S_ARMORCRAFT);
  //EnableSkill(S_WEAPONCRAFT);
  
  //EnableObject(O_ROADN);
  //EnableObject(O_ROADNE);
  //EnableObject(O_ROADNW);
  //EnableObject(O_ROADS);
  //EnableObject(O_ROADSE);
  //EnableObject(O_ROADSW);
  //EnableObject(O_TEMPLE);
  //EnableObject(O_MQUARRY);
  //EnableObject(O_AMINE);
  //EnableObject(O_PRESERVE);
  //EnableObject(O_SACGROVE);
  
  //EnableObject(O_ISLE);
  //EnableObject(O_DERELICT);
  //EnableObject(O_OCAVE);
  //EnableObject(O_WHIRL);
  //EnableItem(I_PIRATES);
  //EnableItem(I_KRAKEN);
  //EnableItem(I_MERFOLK);
  //EnableItem(I_ELEMENTAL);
  
  //if((Globals->UNDERDEEP_LEVELS > 0) || (Globals->UNDERWORLD_LEVELS > 1)) {
      //EnableItem(I_MUSHROOM);
    //EnableItem(I_HEALPOTION);
    //EnableItem(I_ROUGHGEM);
    //EnableItem(I_GEMS);
    //EnableSkill(S_GEMCUTTING);
    //}
  
  // Modify the various spells which are allowed to cross levels
  if(Globals->EASIER_UNDERWORLD) {
    ModifyRangeFlags("rng_teleport", RangeType::RNG_CROSS_LEVELS);
    ModifyRangeFlags("rng_farsight", RangeType::RNG_CROSS_LEVELS);
    ModifyRangeFlags("rng_clearsky", RangeType::RNG_CROSS_LEVELS);
    ModifyRangeFlags("rng_weather", RangeType::RNG_CROSS_LEVELS);
  }
  
  if (Globals->TRANSPORT & GameDefs::ALLOW_TRANSPORT) {
    //EnableSkill(S_QUARTERMASTER);
    EnableSkill(S_CARAVANMASTER);
    EnableObject(O_MARKETPLACE);
    //EnableObject(O_CARAVANSERAI);
  }
  // XXX -- This is just here to preserve existing behavior
  //ModifyItemProductionBooster(I_AXE, I_HAMMER, 1);
  return;
}


int Game::CountStealth(Faction *pFac)
{
  int i = 0;
  forlist(&regions) {
    ARegion *r = (ARegion *) elem;
    forlist(&r->objects) {
      Object *o = (Object *) elem;
      forlist(&o->units) {
	Unit *u = (Unit *) elem;
	if (u->faction == pFac) {
	  if (u->type == U_BUSHIDO_NINJA /*|| u->type == U_BUSHIDO_GEISHA*/) 
	    i++;
	}	
      }
    }
  }
  return(i);
}


int Game::CountMagic(Faction *pFac)
{
  int i = 0;
  forlist(&regions) {
    ARegion *r = (ARegion *) elem;
    forlist(&r->objects) {
      Object *o = (Object *) elem;
      forlist(&o->units) {
	Unit *u = (Unit *) elem;
	if (u->faction == pFac) {
	  if (u->type == U_BUSHIDO_SORCEROR /*|| u->type == U_BUSHIDO_PRIEST*/) 
	    i++;
	}
      }
    }
  }
  return(i);
}


int Game::CountTrade(Faction *pFac)
{
  int i = 0;
  forlist(&regions) {
    ARegion *r = (ARegion *) elem;
    forlist(&r->objects) {
      Object *o = (Object *) elem;
      forlist(&o->units) {
	Unit *u = (Unit *) elem;
	if (u->faction == pFac) {
	  if (u->type == U_BUSHIDO_MERCHANT /*|| u->type == U_BUSHIDO_ARTISAN*/) 
	    i++;
	}
      }
    }
  }
  return(i);
}


int Game::CountWar(Faction *pFac)
{
  int i = 0;
  forlist(&regions) {
    ARegion *r = (ARegion *) elem;
    forlist(&r->objects) {
      Object *o = (Object *) elem;
      forlist(&o->units) {
	Unit *u = (Unit *) elem;
	if (u->faction == pFac) {
	  if (u->type == U_BUSHIDO_SAMURAI /*|| u->type == U_BUSHIDO_KUGE*/) 
	    i++;
	}
      }
    }
  }
  return(i);
}


int Game::AllowedStealth(Faction *pFac)
{
	int points = pFac->type[F_COVERT];

	if (points < 0) points = 0;
	if (points > allowedStealthSize - 1)
		points = allowedStealthSize - 1;

	return allowedStealth[points];
}


int Game::AllowedTrade(Faction *pFac)
{
	int points = pFac->type[F_TRADE];

	if (points < 0) points = 0;
	if (points > allowedTradeSize - 1)
		points = allowedTradeSize - 1;

	return allowedTrade[points];
}


int Game::AllowedWar(Faction *pFac)
{
	int points = pFac->type[F_WAR];

	if (points < 0) points = 0;
	if (points > allowedWarSize - 1)
		points = allowedWarSize - 1;

	return allowedWar[points];
}


int Game::AllowedMagic(Faction *pFac)
{
	int points = pFac->type[F_MAGIC];

	if (points < 0) points = 0;
	if (points > allowedMagicSize - 1)
		points = allowedMagicSize - 1;

	return allowedMagic[points];
}


//replaces monthorders.cpp Do1StudyOrder
void Game::Do1StudyOrder(Unit *u,Object *obj)
{
  StudyOrder * o = (StudyOrder *) u->monthorders;
  int sk = o->skill;
  int cost = SkillCost(sk) * u->GetMen();
  int reset_man = -1;

  //check cost of skill study
  if (cost > u->GetSharedMoney()) {
    u->Error("STUDY: Not enough funds.");
    return;
  }
  
  //check that the skill can be studied
  if (SkillDefs[sk].flags & SkillType::NOSTUDY) {
    u->Error( AString("STUDY: ") + AString(SkillDefs[sk].name) + " cannot be studied.");
    return;
  }
  
  //BUSHIDO code for special unit types
  //BUSHIDO RONIN units
  //if (u->type == U_BUSHIDO_RONIN) {
  //  u->Error(AString("STUDY: Ronin cannot study any skills."));
  //  return;
  //}

  //BUSHIDO WAR units
  //BUSHIDO samuarai
  if ((SkillDefs[sk].flags & SkillType::S_BUSHIDO_SAMURAI) && u->type != U_BUSHIDO_SAMURAI) {
    
    //single unit check
    if (u->GetMen() != 1) {
      u->Error("STUDY: Only 1-man units can be a Samurai.");
      return;
    }
    
    //normal unit check
    if (u->type != U_NORMAL) { 
      u->Error("STUDY: Only non-specialist units can be a Samurai.");
      return;
    }

    //leader only check
    if (u->GetLeaders() != 1) {    
      u->Error("STUDY: Only leader units can be a Samurai.");
      return;
    }
    
    //check count of number already present
    if (CountWar(u->faction) < AllowedWar(u->faction)) {
      //convert leader unit to Samuarai unit
      u->Event("Becomes a Samurai specialist.");
      reset_man = U_NORMAL;
      u->type = U_BUSHIDO_SAMURAI;
    } else {
      //u->Error("STUDY: Can't have another Kuge or Samurai.");
      u->Error("STUDY: Can't have another Samurai.");
      return;
    }
  }
  //end BUSHIDO samauarai

  //BUSHIDO STEALTH units
  //BUSHIDO ninja
  if ((SkillDefs[sk].flags & SkillType::S_BUSHIDO_NINJA) && u->type != U_BUSHIDO_NINJA) {
    
    //single unit check
    if (u->GetMen() != 1) {
      u->Error("STUDY: Only 1-man units can be a Ninja.");
      return;
    }

    //normal unit check
    if (u->type != U_NORMAL) { 
      u->Error("STUDY: Only non-specialist units can be a Ninja.");
      return;
    }

    //leader only check
    if (u->GetLeaders() != 1) {    
      u->Error("STUDY: Only leader units can be a Ninja.");
      return;
    }
    
    //check count of number already present
    if (CountStealth(u->faction) < AllowedStealth(u->faction)) {
      //convert leader unit to Ninja unit
      u->Event("Becomes a Ninja specialist.");
      reset_man = U_NORMAL;
      u->type = U_BUSHIDO_NINJA;
    } else {
      //      u->Error("STUDY: Can't have another Geisha or Ninja.");
      u->Error("STUDY: Can't have another Ninja.");
      return;
    }
  }
  //end BUSHIDO ninja

  //BUSHIDO MAGIC units
  //BUSHIDO sorceror
  if ((SkillDefs[sk].flags & SkillType::S_BUSHIDO_SORCEROR) && u->type != U_BUSHIDO_SORCEROR) {
    
    //single unit check
    if (u->GetMen() != 1) {
      u->Error("STUDY: Only 1-man units can be a Sorceror.");
      return;
    }

    //normal unit check
    if (u->type != U_NORMAL) { 
      u->Error("STUDY: Only non-specialist units can be a Sorceror.");
      return;
    }

    //leader only check
    if (u->GetLeaders() != 1) {    
      u->Error("STUDY: Only leader units can be a Sorceror.");
      return;
    }
    
    //check count of number already present
    if (CountMagic(u->faction) < AllowedMagic(u->faction)) {
      //convert leader unit to Sorceror unit
      u->Event("Becomes a Sorceror specialist.");
      reset_man = U_NORMAL;
      u->type = U_BUSHIDO_SORCEROR;
    } else {
      //      u->Error("STUDY: Can't have another Priest or Sorceror.");
      u->Error("STUDY: Can't have another Sorceror.");
      return;
    }
  }
  //end BUSHIDO sorceror

  //BUSHIDO TRADE units
  //BUSHIDO merchant
  if ((SkillDefs[sk].flags & SkillType::S_BUSHIDO_MERCHANT) && u->type != U_BUSHIDO_MERCHANT) {
    
    //single unit check
    if (u->GetMen() != 1) {
      u->Error("STUDY: Only 1-man units can be a Merchant.");
      return;
    }
    
    //normal unit check
    if (u->type != U_NORMAL) { 
      u->Error("STUDY: Only non-specialist units can be a Merchant.");
      return;
    }

    //leader only check
    if (u->GetLeaders() != 1) {    
      u->Error("STUDY: Only leader units can be a Merchant.");
      return;
    }

    //check count of number already present
    if (CountTrade(u->faction) < AllowedTrade(u->faction)) {
      //convert leader unit to Merchant unit
      u->Event("Becomes a Merchant specialist.");
      reset_man = U_NORMAL;
      u->type = U_BUSHIDO_MERCHANT;
    } else {
      //      u->Error("STUDY: Can't have another Artisan or Merchant.");
      u->Error("STUDY: Can't have another Artisan or Merchant.");
      return;
    }
  }
  //end BUSHIDO merchant

  //check for BUSHIDO magic paths

  //tactics check - limit number of units at skill level 5
  //unless bushido Kuge or Samuarai class
  if (sk == S_TACTICS && u->GetSkill(sk) == 4 
      && u->skills.GetDays(sk)/u->GetMen() >= 300 
      && u->type != U_BUSHIDO_SAMURAI) {
    //&& !(u->type == U_BUSHIDO_KUGE || u->type == U_BUSHIDO_SAMURAI)) {

    //    u->Error("STUDY: Only Kuge and Samurai can study to level 5 in tactics.");
    u->Error("STUDY: Only Samurai can study to level 5 in tactics.");
    return;
  }
  //end tactics check
  
  //stealth check - limit number of units at skill level 5
  if (sk == S_STEALTH && u->GetSkill(sk) == 4 
      && u->skills.GetDays(sk)/u->GetMen() >= 300 
      && !(u->type == U_BUSHIDO_NINJA /*|| u->type == U_BUSHIDO_GEISHA*/)) {
    u->Error("STUDY: Only Ninja can study to level 5 in stealth.");
    return;
  }
  //end stealth check

  //observation check - limit number of units at skill level 5
  if (sk == S_OBSERVATION && u->GetSkill(sk) == 4 
      && u->skills.GetDays(sk)/u->GetMen() >= 300 
      && !(u->type == U_BUSHIDO_NINJA /*|| u->type == U_BUSHIDO_GEISHA*/)) {
    u->Error("STUDY: Only Ninja can study to level 5 in observation.");
    return;
  }
  //end observation check  

  //endurance check
  if (sk == S_ENDURANCE && u->GetLeaders()!=1) {
    u->Error("STUDY: Only leader units can study endurance.");
    return;
  }
  //end endurance check

  // adjust teaching for study rate
  int taughtdays = ((long int) o->days * u->skills.GetStudyRate(sk,u->GetMen())/30);
  
  int days = u->skills.GetStudyRate(sk,u->GetMen()) * u->GetMen() + taughtdays;


  //DO BUSHIDO MODS HERE FOR SKILLS ABOVE 2
  if(((SkillDefs[sk].flags & SkillType::S_BUSHIDO_SORCEROR) 
      || (SkillDefs[sk].flags & SkillType::S_BUSHIDO_NINJA)
      || (SkillDefs[sk].flags & SkillType::S_BUSHIDO_SAMURAI)
      || (SkillDefs[sk].flags & SkillType::S_BUSHIDO_MERCHANT))
     && u->GetSkill(sk) >= 2) {
    if(Globals->LIMITED_MAGES_PER_BUILDING) {
      
      if (obj->incomplete > 0 || obj->type == O_DUMMY) {
	u->Error("Warning: Specialist study rate outside of a building "
		 "cut in half above level 2.");
	days /= 2;
      } else if (u->type == U_BUSHIDO_SAMURAI) {
	if (obj->type == O_RYU) {
	  if (obj->mages == 0) {
	    u->Error("Warning: Study rate cut in half above level 2 due "
		     "to number of samurai studying in structure.");
	    days /= 2;
	  } else {
	    obj->mages--;
	  }
	} else {
	  u->Error("Warning: Samurai study rate outside of a ryu "
		   "cut in half above level 2.");
	  days /= 2;
	}
      } else if (u->type == U_BUSHIDO_NINJA) {
	if (obj->type == O_NINJA_RYU) {
	  if (obj->mages == 0) {
	    u->Error("Warning: Study rate cut in half above level 2 due "
		     "to number of ninja studying in structure.");
	    days /= 2;
	  } else {
	    obj->mages--;
	  }
	} else {
	  u->Error("Warning: Ninja study rate outside of a ryu "
		   "cut in half above level 2.");
	  days /= 2;
	}
      } else if (u->type == U_BUSHIDO_SORCEROR) {
	if (obj->type == O_TOWER) {
	  if (obj->mages == 0) {
	    u->Error("Warning: Study rate cut in half above level 2 due "
		     "to number of sorceror studying in structure.");
	    days /= 2;
	  } else {
	    obj->mages--;
	  }
	} else {
	  u->Error("Warning: Magic rate cut in half above level 2 due "
		   "to number of sorcerors studying in structure.");
	  days /= 2;
	}
      } else if (u->type == U_BUSHIDO_MERCHANT) {
	if (obj->type == O_YAKUZA_HOUSE) {
	  if (obj->mages == 0) {
	    u->Error("Warning: Study rate cut in half above level 2 due "
		     "to number of merchants studying in structure.");
	    days /= 2;
	  } else {
	    obj->mages--;
	  }
	} else {
	  u->Error("Warning: Merchant study rate outside of a XXX "
		   "cut in half above level 2.");
	  days /= 2;
	}
      } else if(!(ObjectDefs[obj->type].protect) 
		|| (obj->incomplete > 0)) {
	u->Error("Warning: Specialist study rate outside of a building cut in "
		 "half above level 2.");
	days /= 2;
      }
    }
  }  

  //slow study flags
  if(SkillDefs[sk].flags & SkillType::SLOWSTUDY) {
    days /= 2;
  }
  
  //check study
  if (u->Study(sk,days)) {
    u->ConsumeSharedMoney(cost);
    AString str("Studies ");
    str += SkillDefs[sk].name;
    taughtdays = taughtdays/u->GetMen();
    if (taughtdays) {
      str += " and was taught for ";
      str += taughtdays;
      str += " days";
    }
    str += ".";
    u->Event(str);
    // study to level order
    if(o->level != -1) {
      if(u->GetSkill(sk) < o->level) {
	TurnOrder *tOrder = new TurnOrder;
	AString order;
	tOrder->repeating = 0;
	order = AString("STUDY ") + SkillDefs[sk].abbr + " " + o->level;
	tOrder->turnOrders.Add(new AString(order));
	u->turnorders.Insert(tOrder);
      } else {
	AString msg("Completes study to level ");
	msg += o->level;
	msg += " in ";
	msg += SkillDefs[sk].name;
	msg += ".";
	u->Event(msg);
      }	
    }
  } else {
    // if we just tried to become a mage or apprentice, but
    // were unable to study, reset unit to whatever it was before.
    if(reset_man != -1)
      u->type = reset_man;
  }
}


void Game::CountAllSpecialists()
{
  forlist(&factions) {
    ((Faction *) elem)->numNinja = 0;
    ((Faction *) elem)->numGeisha = 0;
    ((Faction *) elem)->numKuge = 0;
    ((Faction *) elem)->numSamuarai = 0;
    ((Faction *) elem)->numArtisan = 0;
    ((Faction *) elem)->numMerchant = 0;
    ((Faction *) elem)->numPriest = 0;
    ((Faction *) elem)->numSorceror = 0;
    
  }
  
  {
    forlist(&regions) {
      ARegion *r = (ARegion *) elem;
      forlist(&r->objects) {
	Object *o = (Object *) elem;
	forlist(&o->units) {
	  Unit *u = (Unit *) elem;
	  if (u->type == U_BUSHIDO_NINJA) 
	    u->faction->numNinja++;
//if (u->type == U_BUSHIDO_GEISHA) 
//	    u->faction->numGeisha++;
//	  if (u->type == U_BUSHIDO_KUGE) 
//	    u->faction->numKuge++;
	  if (u->type == U_BUSHIDO_SAMURAI) 
	    u->faction->numSamuarai++;
//	  if (u->type == U_BUSHIDO_ARTISAN) 
//	    u->faction->numArtisan++;
	  if (u->type == U_BUSHIDO_MERCHANT) 
	    u->faction->numMerchant++;
//	  if (u->type == U_BUSHIDO_PRIEST) 
//	    u->faction->numPriest++;
	  if (u->type == U_BUSHIDO_SORCEROR) 
	    u->faction->numSorceror++;
	}
      }
    }
  }
}


