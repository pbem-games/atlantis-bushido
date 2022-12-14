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
#include "game.h"
#include "gamedata.h"

void Game::ProcessCastOrder(Unit * u,AString * o, OrdersCheck *pCheck )
{
  AString * token = o->gettoken();
  if (!token) {
    ParseError( pCheck, u, 0, "CAST: No skill given.");
    return;
  }

  int sk = ParseSkill(token);
  delete token;
  if (sk==-1) {
    ParseError( pCheck, u, 0, "CAST: Invalid skill.");
    return;
  }

  //BUSHIDO MODS
  //if( !( SkillDefs[sk].flags & SkillType::MAGIC )) {
  //    ParseError( pCheck, u, 0, "CAST: That is not a magic skill.");
  //    return;
  //}
  //END BUSHIDO MODS

  if( !( SkillDefs[sk].flags & SkillType::CAST )) {
    ParseError( pCheck, u, 0, "CAST: That skill cannot be CAST.");
    return;
  }

  RangeType *rt = NULL;
  if( !pCheck ) {
    //
    // XXX -- should be error checking spells
    //
    switch(sk) {
    case S_MIND_READING:
      ProcessMindReading(u,o, pCheck );
      break;
    case S_CONSTRUCT_PORTAL:
    case S_CONSTRUCT_GATE:
    case S_ENGRAVE_RUNES_OF_WARDING:
      break;

      //case S_SUMMON_IMPS:
    case S_SUMMON_DEMON:
      ProcessSummonDemon(u,o, pCheck );
      break;
    case S_SUMMON_UNDEAD:
      ProcessSummonUndead(u,o, pCheck );
      break;
    case S_SUMMON_SPIRITS:
      ProcessSummonSpirit(u,o, pCheck );
      break;
      //case S_SUMMON_BALROG:
      //case S_SUMMON_SKELETONS:
      //case S_RAISE_UNDEAD:
      //case S_SUMMON_LICH:
      //    case S_SUMMON_DRAGON:
    case S_DRAGON_LORE:
      //case S_WOLF_LORE:
    case S_EARTH_LORE:
      //case S_CREATE_RING_OF_INVISIBILITY:
      //case S_CREATE_CLOAK_OF_INVULNERABILITY:
      //case S_CREATE_STAFF_OF_FIRE:
      //case S_CREATE_STAFF_OF_LIGHTNING:
      //case S_CREATE_AMULET_OF_TRUE_SEEING:
      //case S_CREATE_AMULET_OF_PROTECTION:
      //case S_CREATE_RUNESWORD:
      //case S_CREATE_SHIELDSTONE:
      //case S_CREATE_MAGIC_CARPET:
      //case S_CAMOUFLAGE:
    case S_KEEN_EYES:
      //case S_CREATE_FOOD:
      ProcessGenericSpell(u,sk, pCheck );
      break;
    case S_CLEAR_SKIES:
      rt = FindRange(SkillDefs[sk].range);
      if (rt == NULL)
	ProcessGenericSpell(u, sk, pCheck);
      else
	ProcessRegionSpell(u, o, sk, pCheck);
      break;
    case S_FARSIGHT:
    case S_TELEPORTATION:
    case S_WEATHER_LORE:
      ProcessRegionSpell(u, o, sk, pCheck);
      break;

    case S_TEA_CEREMONY: {
      //check for ownership of building
      int run = CheckTeaCeremonyBuilding(u);
      if (run == 1)
	ProcessRegionSpell(u, o, sk, pCheck);
      }
      break;

    case S_HAWKING:
      ProcessHawking(u,o, pCheck );
      break;
    case S_INVISIBILITY:
      ProcessInvisibility(u,o, pCheck );
      break;
      //case S_CAMOUFLAGE:
      //ProcessCamouflage(u, pCheck );
      //break;
    case S_GATE_LORE:
      ProcessCastGateLore(u,o, pCheck );
      break;
    case S_PORTAL_LORE:
      ProcessCastPortalLore(u,o, pCheck );
      break;
      //case S_CREATE_PHANTASMAL_BEASTS:
      //ProcessPhanBeasts(u,o, pCheck );
      //break;
      //case S_CREATE_PHANTASMAL_UNDEAD:
      //ProcessPhanUndead(u,o, pCheck );
      //break;
      //case S_CREATE_PHANTASMAL_DEMONS:
      //ProcessPhanDemons(u,o, pCheck );
      //break;
    }
  }
}

void Game::ProcessMindReading(Unit *u,AString *o, OrdersCheck *pCheck )
{
  UnitId *id = ParseUnit(o);

  if (!id) {
    u->Error("CAST: No unit specified.");
    return;
  }

  CastMindOrder *order = new CastMindOrder;
  order->id = id;
  order->spell = S_MIND_READING;
  order->level = 1;

  u->ClearCastOrders();
  u->castorders = order;
}

/*
void Game::ProcessBirdLore(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Missing arguments.");
    return;
  }

  if (*token == "eagle") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_BIRD_LORE;
    order->level = 3;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "direction") {
    delete token;
    token = o->gettoken();

    if (!token) {
      u->Error("CAST: Missing arguments.");
      return;
    }

    int dir = ParseDir(token);
    delete token;
    if (dir == -1 || dir > NDIRS) {
      u->Error("CAST: Invalid direction.");
      return;
    }

    CastIntOrder *order = new CastIntOrder;
    order->spell = S_BIRD_LORE;
    order->level = 1;
    order->target = dir;
    u->ClearCastOrders();
    u->castorders = order;

    return;
  }

  u->Error("CAST: Invalid arguments.");
  delete token;
}
*/
//BUSHIDO MOD
void Game::ProcessHawking(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Missing arguments.");
    return;
  }

  if (*token == "hou_ou") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_HAWKING;
    order->level = 5;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "eagle") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_HAWKING;
    order->level = 3;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "direction") {
    delete token;
    token = o->gettoken();

    if (!token) {
      u->Error("CAST: Missing arguments.");
      return;
    }

    int dir = ParseDir(token);
    delete token;
    if (dir == -1 || dir > NDIRS) {
      u->Error("CAST: Invalid direction.");
      return;
    }

    CastIntOrder *order = new CastIntOrder;
    order->spell = S_HAWKING;
    order->level = 1;
    order->target = dir;
    u->ClearCastOrders();
    u->castorders = order;

    return;
  }

  u->Error("CAST: Invalid arguments.");
  delete token;
}

void Game::ProcessSummonDemon(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Missing arguments.");
    return;
  }

  if (*token == "dai_oni") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_DEMON;
    order->level = 5;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "oni") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_DEMON;
    order->level = 3;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "imp") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_DEMON;
    order->level = 1;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  u->Error("CAST: Invalid arguments.");
  delete token;
}

void Game::ProcessSummonSpirit(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Missing arguments.");
    return;
  }

  if (*token == "major_kami") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_SPIRITS;
    order->level = 5;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "kami") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_SPIRITS;
    order->level = 3;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "minor_kami") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_SPIRITS;
    order->level = 1;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  u->Error("CAST: Invalid arguments.");
  delete token;
}

void Game::ProcessSummonUndead(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Missing arguments.");
    return;
  }

  if (*token == "gaki") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_UNDEAD;
    order->level = 5;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "undead") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_UNDEAD;
    order->level = 3;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  if (*token == "skeleton") {
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_SUMMON_UNDEAD;
    order->level = 1;
    u->ClearCastOrders();
    u->castorders = order;
    return;
  }

  u->Error("CAST: Invalid arguments.");
  delete token;
}

//end BUSHIDO mod

void Game::ProcessInvisibility(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();

  //END BUSHIDO MOD
  if (!token || !(*token == "units")) {
    u->Error("CAST: Must specify units to render invisible.");
    return;
  }
  delete token;
  
  CastUnitsOrder *order;
  if (u->castorders && u->castorders->type == O_CAST &&
      ((CastOrder *) u->castorders)->spell == S_INVISIBILITY &&
      ((CastOrder *) u->castorders)->level == 1) {
    order = (CastUnitsOrder *) u->castorders;
  } else {
    order = new CastUnitsOrder;
    order->spell = S_INVISIBILITY;
    order->level = 1;
    u->ClearCastOrders();
    u->castorders = order;
  }

  UnitId *id = ParseUnit(o);
  while (id) {
    order->units.Add(id);
    id = ParseUnit(o);
  }
}

/*
void Game::ProcessPhanDemons(Unit *u,AString *o, OrdersCheck *pCheck )
{
  CastIntOrder *order = new CastIntOrder;
  order->spell = S_CREATE_PHANTASMAL_DEMONS;
  order->level = 0;
  order->target = 1;

  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Illusion to summon must be given.");
    delete order;
    return;
  }

  if (*token == "imp" || *token == "imps") {
    order->level = 1;
  }

  if (*token == "demon" || *token == "demons") {
    order->level = 3;
  }

  if (*token == "balrog" || *token == "balrogs") {
    order->level = 5;
  }

  delete token;

  if (!order->level) {
    u->Error("CAST: Can't summon that illusion.");
    delete order;
    return;
  }

  token = o->gettoken();

  if (!token) {
    order->target = 1;
  } else {
    order->target = token->value();
    delete token;
  }

  u->ClearCastOrders();
  u->castorders = order;
}
*/
/*

void Game::ProcessPhanUndead(Unit *u,AString *o, OrdersCheck *pCheck)
{
  CastIntOrder *order = new CastIntOrder;
  order->spell = S_CREATE_PHANTASMAL_UNDEAD;
  order->level = 0;
  order->target = 1;

  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Must specify which illusion to summon.");
    delete order;
    return;
  }

  if (*token == "skeleton" || *token == "skeletons") {
    order->level = 1;
  }

  if (*token == "undead") {
    order->level = 3;
  }

  if (*token == "lich" || *token == "liches") {
    order->level = 5;
  }

  delete token;

  if (!order->level) {
    u->Error("CAST: Must specify which illusion to summon.");
    delete order;
    return;
  }

  token = o->gettoken();

  if (token) {
    order->target = token->value();
    delete token;
  } else {
    order->target = 1;
  }

  u->ClearCastOrders();
  u->castorders = order;
}
*/

/*
void Game::ProcessPhanBeasts(Unit *u,AString *o, OrdersCheck *pCheck )
{
  CastIntOrder *order = new CastIntOrder;
  order->spell = S_CREATE_PHANTASMAL_BEASTS;
  order->level = 0;
  order->target = 1;

  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Must specify which illusion to summon.");
    delete order;
    return;
  }

  if (*token == "wolf" || *token == "wolves") {
    order->level = 1;
  }
  if (*token == "eagle" || *token == "eagles") {
    order->level = 3;
  }
  if (*token == "dragon" || *token == "dragon") {
    order->level = 5;
  }

  delete token;
  if (!order->level) {
    delete order;
    u->Error("CAST: Must specify which illusion to summon.");
    return;
  }

  token = o->gettoken();
  if (token) {
    order->target = token->value();
    delete token;
  }

  u->ClearCastOrders();
  u->castorders = order;
}
*/
void Game::ProcessGenericSpell(Unit *u,int spell, OrdersCheck *pCheck )
{
  CastOrder *orders = new CastOrder;
  orders->spell = spell;
  orders->level = 1;
  u->ClearCastOrders();
  u->castorders = orders;
}

void Game::ProcessRegionSpell(Unit *u, AString *o, int spell,
			      OrdersCheck *pCheck)
{
  AString *token = o->gettoken();
  int x = -1;
  int y = -1;
  int z = -1;
  RangeType *range = FindRange(SkillDefs[spell].range);

  if(token) {
    if(*token == "region") {
      delete token;
      token = o->gettoken();
      if(!token) {
	u->Error("CAST: Region X coordinate not specified.");
	return;
      }
      x = token->value();
      delete token;

      token = o->gettoken();
      if(!token) {
	u->Error("CAST: Region Y coordinate not specified.");
	return;
      }
      y = token->value();
      delete token;

      if(range && (range->flags & RangeType::RNG_CROSS_LEVELS)) {
	token = o->gettoken();
	if(token) {
	  z = token->value();
	  delete token;
	  if(z < 0 || (z >= Globals->UNDERWORLD_LEVELS +
		       Globals->UNDERDEEP_LEVELS +
		       Globals->ABYSS_LEVEL + 2)) {
	    u->Error("CAST: Invalid Z coordinate specified.");
	    return;
	  }
	}
      }
    } else {
      delete token;
    }
  }
  if(x == -1) x = u->object->region->xloc;
  if(y == -1) y = u->object->region->yloc;
  if(z == -1) z = u->object->region->zloc;

  CastRegionOrder *order;
  if(spell == S_TELEPORTATION)
    order = new TeleportOrder;
  else
    order = new CastRegionOrder;
  order->spell = spell;
  order->level = 1;
  order->xloc = x;
  order->yloc = y;
  order->zloc = z;

  u->ClearCastOrders();
  /* Teleports happen late in the turn! */
  if(spell == S_TELEPORTATION)
    u->teleportorders = (TeleportOrder *)order;
  else
    u->castorders = order;
}

void Game::ProcessCastPortalLore(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();
  if (!token) {
    u->Error("CAST: Requires a target sorceror.");
    return;
  }
  int gate = token->value();
  delete token;
  token = o->gettoken();

  if (!token) {
    u->Error("CAST: No units to teleport.");
    return;
  }

  if (!(*token == "units")) {
    u->Error("CAST: No units to teleport.");
    delete token;
    return;
  }

  TeleportOrder *order;

  if (u->teleportorders && u->teleportorders->spell == S_PORTAL_LORE) {
    order = u->teleportorders;
  } else {
    order = new TeleportOrder;
    u->ClearCastOrders();
    u->teleportorders = order;
  }

  order->gate = gate;
  order->spell = S_PORTAL_LORE;
  order->level = 1;

  UnitId *id = ParseUnit(o);
  while(id) {
    order->units.Add(id);
    id = ParseUnit(o);
  }
}

void Game::ProcessCastGateLore(Unit *u,AString *o, OrdersCheck *pCheck )
{
  AString *token = o->gettoken();

  if (!token) {
    u->Error("CAST: Missing argument.");
    return;
  }

  if ((*token) == "gate") {
    delete token;
    token = o->gettoken();

    if (!token) {
      u->Error("CAST: Requires a target gate.");
      return;
    }

    TeleportOrder *order;

    if (u->teleportorders && u->teleportorders->spell == S_GATE_LORE &&
	u->teleportorders->gate == token->value()) {
      order = u->teleportorders;
    } else {
      order = new TeleportOrder;
      u->ClearCastOrders();
      u->teleportorders = order;
    }

    order->gate = token->value();
    order->spell = S_GATE_LORE;
    order->level = 3;

    delete token;

    token = o->gettoken();

    if (!token) return;
    if (!(*token == "units")) {
      delete token;
      return;
    }

    UnitId *id = ParseUnit(o);
    while(id) {
      order->units.Add(id);
      id = ParseUnit(o);
    }
    return;
  }

  if ((*token) == "random") {
    TeleportOrder *order;

    if (u->teleportorders && u->teleportorders->spell == S_GATE_LORE &&
	u->teleportorders->gate == -1 ) {
      order = u->teleportorders;
    } else {
      order = new TeleportOrder;
      u->ClearCastOrders();
      u->teleportorders = order;
    }

    order->gate = -1;
    order->spell = S_GATE_LORE;
    order->level = 1;

    delete token;

    token = o->gettoken();

    if (!token) return;
    if (!(*token == "units")) {
      delete token;
      return;
    }

    UnitId *id = ParseUnit(o);
    while(id) {
      order->units.Add(id);
      id = ParseUnit(o);
    }
    return;
  }

  if ((*token) == "detect") {
    delete token;
    u->ClearCastOrders();
    CastOrder *to = new CastOrder;
    to->spell = S_GATE_LORE;
    to->level = 2;
    u->castorders = to;
    return;
  }

  delete token;
  u->Error("CAST: Invalid argument.");
}

void Game::RunACastOrder(ARegion * r,Object *o,Unit * u)
{
  int val;
  //BUSHIDO mod
  //POON ILLU and apprentice fix
  //if (!(u->type == U_MAGE || u->type == U_APPRENTICE)) {
  //	u->Error("CAST: Unit is not an apprentice or mage.");
  //	return;
  //}
  //end BUSHIDO mod

  if (u->castorders->level == 0) {
    u->castorders->level = u->GetSkill(u->castorders->spell);
  }

  if (u->GetSkill(u->castorders->spell) < u->castorders->level ||
      u->castorders->level == 0) {
    u->Error("CAST: Skill level isn't that high.");
    return;
  }

  int sk = u->castorders->spell;
  switch (sk) {
  case S_MIND_READING:
    val = RunMindReading(r,u);
    break;
    //case S_ENCHANT_ARMOR:
    //val = RunEnchantArmor(r,u);
    //break;
    //case S_ENCHANT_SWORDS:
    //val = RunEnchantSwords(r,u);
    //break;
  case S_CONSTRUCT_GATE:
    val = RunConstructGate(r,u,sk);
    //break;
  case S_ENGRAVE_RUNES_OF_WARDING:
    val = RunEngraveRunes(r,o,u);
    break;
  case S_CONSTRUCT_PORTAL:
    val = RunCreateArtifact(r,u,sk,I_PORTAL);
    break;
    //case S_CREATE_RING_OF_INVISIBILITY:
    //val = RunCreateArtifact(r,u,sk,I_RINGOFI);
    //break;
    //case S_CREATE_CLOAK_OF_INVULNERABILITY:
    //val = RunCreateArtifact(r,u,sk,I_CLOAKOFI);
    //break;
    //case S_CREATE_STAFF_OF_FIRE:
    //val = RunCreateArtifact(r,u,sk,I_STAFFOFF);
    //break;
    //case S_CREATE_STAFF_OF_LIGHTNING:
    //val = RunCreateArtifact(r,u,sk,I_STAFFOFL);
    //break;
    //case S_CREATE_AMULET_OF_TRUE_SEEING:
    //val = RunCreateArtifact(r,u,sk,I_AMULETOFTS);
    //break;
    //case S_CREATE_AMULET_OF_PROTECTION:
    //val = RunCreateArtifact(r,u,sk,I_AMULETOFP);
    //break;
    //case S_CREATE_RUNESWORD:
    //val = RunCreateArtifact(r,u,sk,I_RUNESWORD);
    //break;
    //case S_CREATE_SHIELDSTONE:
    //val = RunCreateArtifact(r,u,sk,I_SHIELDSTONE);
    //break;
    //case S_CREATE_MAGIC_CARPET:
    //val = RunCreateArtifact(r,u,sk,I_MCARPET);
    //break;
    //case S_CREATE_FLAMING_SWORD:
    //val = RunCreateArtifact(r,u,sk,I_FSWORD);
    //break;
    //case S_SUMMON_IMPS:
    //val = RunSummonImps(r,u);
    //break;
  case S_SUMMON_DEMON:
    val = RunSummonDemon(r,u);
    break;
  case S_SUMMON_UNDEAD:
    val = RunSummonUndead(r,u);
    break;
  case S_SUMMON_SPIRITS:
    val = RunSummonSpirit(r,u);
    break;
    //case S_SUMMON_BALROG:
    //val = RunSummonBalrog(r,u);
    //break;
    //case S_SUMMON_LICH:
    //val = RunSummonLich(r,u);
    //break;
    //case S_RAISE_UNDEAD:
    //val = RunRaiseUndead(r,u);
    //break;
    //case S_SUMMON_SKELETONS:
    //val = RunSummonSkeletons(r,u);
    //break;
  case S_SUMMON_DRAGON:
    val = RunSummonDragon(r,u);
    //break;
    //case S_BIRD_LORE:
    //val = RunBirdLore(r,u);
    //break;
    //case S_WOLF_LORE:
    //val = RunWolfLore(r,u);
    //break;
  case S_INVISIBILITY:
    val = RunInvisibility(r,u);
    break;
  case S_CAMOUFLAGE:
    val = RunCamouflage(r,u);
    break;
  case S_KEEN_EYES:
    val = RunKeenEyes(r,u);
    break;
    //case S_CREATE_PHANTASMAL_DEMONS:
    //val = RunPhanDemons(r,u);
    //break;
    //case S_CREATE_PHANTASMAL_UNDEAD:
    //val = RunPhanUndead(r,u);
    //break;
    //case S_CREATE_PHANTASMAL_BEASTS:
    //val = RunPhanBeasts(r,u);
    //break;
  case S_GATE_LORE:
    val = RunDetectGates(r,o,u);
    break;
  case S_FARSIGHT:
  case S_TEA_CEREMONY:
    val = RunFarsight(r,u);
    break;
  case S_EARTH_LORE:
    val = RunEarthLore(r,u);
    break;
  case S_WEATHER_LORE:
    val = RunWeatherLore(r, u);
    break;
  case S_CLEAR_SKIES:
    val = RunClearSkies(r,u);
    break;
    //case S_CREATE_FOOD:
    //val = RunCreateFood(r, u);
    //break;
    //BUSHIDO mod
  case S_HAWKING:
    val = RunHawking(r,u);
    break;
    //end BUSHIDO mod
  }
  if (val) {
    u->Practice(sk);
    r->NotifySpell(u, SkillDefs[sk].abbr, &regions);
  }

}

int Game::GetRegionInRange(ARegion *r, ARegion *tar, Unit *u, int spell)
{
  int level = u->GetSkill(spell);
  if(!level) {
    u->Error("CAST: You don't know that spell.");
    return 0;
  }

  RangeType *range = FindRange(SkillDefs[spell].range);
  if (range == NULL) {
    u->Error("CAST: Spell is not castable at range.");
    return 0;
  }

  int rtype = regions.GetRegionArray(r->zloc)->levelType;
  if((rtype == ARegionArray::LEVEL_NEXUS) &&
     !(range->flags & RangeType::RNG_NEXUS_SOURCE)) {
    u->Error("CAST: Spell does not work from the Nexus.");
    return 0;
  }

  if(!tar) {
    u->Error("CAST: No such region.");
    return 0;
  }

  rtype = regions.GetRegionArray(tar->zloc)->levelType;
  if((rtype == ARegionArray::LEVEL_NEXUS) &&
     !(range->flags & RangeType::RNG_NEXUS_TARGET)) {
    u->Error("CAST: Spell does not work to the Nexus.");
    return 0;
  }

  if((rtype != ARegionArray::LEVEL_SURFACE) &&
     (range->flags & RangeType::RNG_SURFACE_ONLY)) {
    u->Error("CAST: Spell can only target regions on the surface.");
    return 0;
  }
  if(!(range->flags&RangeType::RNG_CROSS_LEVELS) && (r->zloc != tar->zloc)) {
    u->Error("CAST: Spell is not able to work across levels.");
    return 0;
  }

  int maxdist;
  switch(range->rangeClass) {
  default:
  case RangeType::RNG_ABSOLUTE:
    maxdist = 1;
    break;
  case RangeType::RNG_LEVEL:
    maxdist = level;
    break;
  case RangeType::RNG_LEVEL2:
    maxdist = level * level;
    break;
  case RangeType::RNG_LEVEL3:
    maxdist = level * level * level;
    break;
  }
  maxdist *= range->rangeMult;

  int dist;
  if(range->flags & RangeType::RNG_CROSS_LEVELS)
    dist = regions.GetPlanarDistance(tar, r, range->crossLevelPenalty);
  else
    dist = regions.GetDistance(tar, r);
  if(dist > maxdist) {
    u->Error("CAST: Target region out of range.");
    return 0;
  }
  return 1;
}

int Game::RunMindReading(ARegion *r,Unit *u)
{
  CastMindOrder *order = (CastMindOrder *) u->castorders;
  int level = u->GetSkill(S_MIND_READING);

  Unit *tar = r->GetUnitId(order->id,u->faction->num);
  if (!tar) {
    u->Error("No such unit.");
    return 0;
  }

  AString temp = AString("Casts Mind Reading: ") + *(tar->name) + ", " +
    *(tar->faction->name);

  if (level < 4) {
    u->Event(temp + ".");
    return 1;
  }

  temp += tar->items.Report(2,5,0) + ". Skills: ";
  temp += tar->skills.Report(tar->GetMen()) + ".";

  u->Event(temp);
  return 1;
}

/*int Game::RunEnchantArmor(ARegion *r,Unit *u)
  {
  int level = u->GetSkill(S_ENCHANT_ARMOR);
  int max = ItemDefs[I_MPLATE].mOut * level;
  int num = 0;
  int count = 0;
  unsigned int c;
  int found;

  // Figure out how many components there are
  for(c=0; c<sizeof(ItemDefs[I_MPLATE].mInput)/sizeof(Materials); c++) {
  if(ItemDefs[I_MPLATE].mInput[c].item != -1) count++;
  }

  while(max) {
  int i, a;
  found = 0;
  // See if we have enough of all items
  for(c=0; c<sizeof(ItemDefs[I_MPLATE].mInput)/sizeof(Materials); c++) {
  i = ItemDefs[I_MPLATE].mInput[c].item;
  a = ItemDefs[I_MPLATE].mInput[c].amt;
  if(i != -1) {
  if(u->GetSharedNum(i) >= a) found++;
  }
  }
  // We do not, break.
  if(found != count) break;

  // Decrement our inputs
  for(c=0; c<sizeof(ItemDefs[I_MPLATE].mInput)/sizeof(Materials); c++) {
  i = ItemDefs[I_MPLATE].mInput[c].item;
  a = ItemDefs[I_MPLATE].mInput[c].amt;
  if(i != -1) {
  u->ConsumeShared(i, a);
  }
  }
  // We've made one.
  num++;
  max--;
  }

  u->items.SetNum(I_MPLATE, u->items.GetNum(I_MPLATE) + num);
  u->Event(AString("Enchants ") + num + " mithril armor.");
  if (num == 0) return 0;
  return 1;
  }*/

/*int Game::RunEnchantSwords(ARegion *r,Unit *u)
  {
  int level = u->GetSkill(S_ENCHANT_SWORDS);
  int max = ItemDefs[I_MSWORD].mOut * level;
  int num = 0;
  int count = 0;
  unsigned int c;
  int found;

  // Figure out how many components there are
  for(c=0; c<sizeof(ItemDefs[I_MSWORD].mInput)/sizeof(Materials); c++) {
  if(ItemDefs[I_MSWORD].mInput[c].item != -1) count++;
  }

  while(max) {
  int i, a;
  found = 0;
  // See if we have enough of all items
  for(c=0; c<sizeof(ItemDefs[I_MSWORD].mInput)/sizeof(Materials); c++) {
  i = ItemDefs[I_MSWORD].mInput[c].item;
  a = ItemDefs[I_MSWORD].mInput[c].amt;
  if(i != -1) {
  if(u->GetSharedNum(i) >= a) found++;
  }
  }
  // We do not, break.
  if(found != count) break;

  // Decrement our inputs
  for(c=0; c<sizeof(ItemDefs[I_MSWORD].mInput)/sizeof(Materials); c++) {
  i = ItemDefs[I_MSWORD].mInput[c].item;
  a = ItemDefs[I_MSWORD].mInput[c].amt;
  if(i != -1) {
  u->ConsumeShared(i, a);
  }
  }
  // We've made one.
  num++;
  max--;
  }

  u->items.SetNum(I_MSWORD,u->items.GetNum(I_MSWORD) + num);
  u->Event(AString("Enchants ") + num + " mithril swords.");
  if (num == 0) return 0;
  return 1;
  }*/

/*int Game::RunCreateFood(ARegion *r,Unit *u)
  {
  int level = u->GetSkill(S_CREATE_FOOD);
  int max = ItemDefs[I_FOOD].mOut * level;
  int num = 0;
  int count = 0;
  unsigned int c;
  int found;

  // Figure out how many components there are
  for(c=0; c<sizeof(ItemDefs[I_FOOD].mInput)/sizeof(Materials); c++) {
  if(ItemDefs[I_FOOD].mInput[c].item != -1) count++;
  }

  while(max) {
  int i, a;
  found = 0;
  // See if we have enough of all items
  for(c=0; c<sizeof(ItemDefs[I_FOOD].mInput)/sizeof(Materials); c++) {
  i = ItemDefs[I_FOOD].mInput[c].item;
  a = ItemDefs[I_FOOD].mInput[c].amt;
  if(i != -1) {
  if(u->GetSharedNum(i) >= a) found++;
  }
  }
  // We do not, break.
  if(found != count) break;

  // Decrement our inputs
  for(c=0; c<sizeof(ItemDefs[I_FOOD].mInput)/sizeof(Materials); c++) {
  i = ItemDefs[I_FOOD].mInput[c].item;
  a = ItemDefs[I_FOOD].mInput[c].amt;
  if(i != -1) {
  u->ConsumeShared(i, a);
  }
  }
  // We've made one.
  num++;
  max--;
  }

  u->items.SetNum(I_FOOD,u->items.GetNum(I_FOOD) + num);
  u->Event(AString("Creates ") + num + " food.");
  if (num == 0) return 0;
  return 1;
  }*/

int Game::RunConstructGate(ARegion *r,Unit *u, int spell)
{
  if (TerrainDefs[r->type].similar_type == R_OCEAN) {
    u->Error("Gates may not be constructed at sea.");
    return 0;
  }

  if (r->gate) {
    u->Error("There is already a gate in that region.");
    return 0;
  }

  if (u->GetSharedMoney() < 10000) {
    u->Error("Can't afford to construct a Gate.");
    return 0;
  }

  u->ConsumeSharedMoney(10000);

  int level = u->GetSkill(spell);
  int chance = level * 20;
  if (getrandom(100) >= chance) {
    u->Event("Attempts to construct a gate, but fails.");
    return 0;
  }

  u->Event(AString("Constructs a Gate in ")+r->ShortPrint( &regions )+".");
  regions.numberofgates++;
  r->gate = regions.numberofgates;
  if(Globals->GATES_NOT_PERENNIAL) {
    int dm = Globals->GATES_NOT_PERENNIAL / 2;
    int gm = month + 1 - getrandom(dm) - getrandom(dm) - getrandom(Globals->GATES_NOT_PERENNIAL % 2);
    while(gm < 0) gm += 12;
    r->gatemonth = gm;
  }
  return 1;
}

int Game::RunEngraveRunes(ARegion *r,Object *o,Unit *u)
{
  if (!o->IsBuilding()) {
    u->Error("Runes of Warding may only be engraved on a building.");
    return 0;
  }

  if (o->incomplete > 0) {
    u->Error( "Runes of Warding may only be engraved on a completed "
	      "building.");
    return 0;
  }

  int level = u->GetSkill(S_ENGRAVE_RUNES_OF_WARDING);

  switch (level) {
  case 5:
    if (o->type == O_MFORTRESS) break;
  case 4:
    if (o->type == O_CITADEL) break;
  case 3:
    if (o->type == O_CASTLE) break;
  case 2:
    if (o->type == O_FORT) break;
    if (o->type == O_MTOWER) break;
  case 1:
    if (o->type == O_TOWER) break;
  default:
    u->Error("Not high enough level to engrave Runes of Warding on "
	     "that building.");
    return 0;
  }

  if (u->GetSharedMoney() < 600) {
    u->Error("Can't afford to engrave Runes of Warding.");
    return 0;
  }

  u->ConsumeSharedMoney(600);
  if( o->type == O_MFORTRESS ) {
    o->runes = 5;
  } else if(o->type == O_MTOWER) {
    o->runes = 4;
  } else {
    o->runes = 3;
  }
  u->Event(AString("Engraves Runes of Warding on ") + *(o->name) + ".");
  return 1;
}

/*
int Game::RunSummonBalrog(ARegion *r,Unit *u)
{
  //  if (u->items.GetNum(I_BALROG) >= ItemDefs[I_BALROG].max_inventory) {
  if (u->items.GetNum(I_DAI_ONI) >= ItemDefs[I_DAI_ONI].max_inventory) {
    u->Error("Can't control any more balrogs.");
    return 0;
  }

  int level = u->GetSkill(S_SUMMON_BALROG);

  int num = (level * 20 + getrandom(100)) / 100;

  //  u->items.SetNum(I_BALROG,u->items.GetNum(I_BALROG) + num);
  //u->Event(AString("Summons ") + ItemString(I_BALROG,num) + ".");
  u->items.SetNum(I_DAI_ONI,u->items.GetNum(I_DAI_ONI) + num);
  u->Event(AString("Summons ") + ItemString(I_DAI_ONI,num) + ".");
  return 1;
}
*/

int Game::RunSummonDemon(ARegion *r,Unit *u)
{

  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int level = u->GetSkill(S_SUMMON_DEMON);

  //summon imps  
  if (order->level < 3) {
    int summoned = level * level;
    
    u->items.SetNum(I_IMP,u->items.GetNum(I_IMP) + summoned);
    u->Event(AString("Summons ") + ItemString(I_IMP,summoned) + ".");    
    return 1;
  }
    
  //summon oni - level-2 squared
  if (order->level < 5) {
    int max = (level) * (level);
    int summoned = (level-2) * (level-2);
    int current = u->items.GetNum(I_ONI);

    //check limits
    int maxSummoned = max - current;
    if (maxSummoned < summoned)
      summoned = maxSummoned;
    
    if (u->items.GetNum(I_ONI) >= max) {
      u->Error("CAST: Unit can't summon more oni.");
      return 0;
    }
    u->items.SetNum(I_ONI,u->items.GetNum(I_ONI) + summoned);
    u->Event(AString("Summons ") + ItemString(I_ONI,summoned) + ".");    
    return 1;      
  }

  //summon dai-oni
  int max = 1;
    
  if (u->items.GetNum(I_DAI_ONI) >= max) {
    u->Error("CAST: Unit can't summon more dai-oni.");
    return 0;
  }
    
  u->items.SetNum(I_DAI_ONI,u->items.GetNum(I_DAI_ONI) + 1);
  u->Event(AString("Summons ") + ItemString(I_DAI_ONI,1) + ".");

  return 1;
}

int Game::RunSummonUndead(ARegion *r,Unit *u)
{

  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int level = u->GetSkill(S_SUMMON_UNDEAD);

  //summon skeletons  
  if (order->level < 3) {
    int summoned = level * level;
    
    u->items.SetNum(I_SKELETON,u->items.GetNum(I_SKELETON) + summoned);
    u->Event(AString("Summons ") + ItemString(I_SKELETON,summoned) + ".");    
    return 1;
  }
    
  //summon undead - level-2 squared
  if (order->level < 5) {
    int max = (level) * (level);
    int summoned = (level-2) * (level-2);
    int current = u->items.GetNum(I_UNDEAD);

    //check limits
    int maxSummoned = max - current;
    if (maxSummoned < summoned)
      summoned = maxSummoned;
    
    if (u->items.GetNum(I_UNDEAD) >= max) {
      u->Error("CAST: Unit can't summon more undead.");
      return 0;
    }
    u->items.SetNum(I_UNDEAD,u->items.GetNum(I_UNDEAD) + summoned);
    u->Event(AString("Summons ") + ItemString(I_UNDEAD,summoned) + ".");    
    return 1;      
  }

  //summon gaki
  int max = 1;
    
  if (u->items.GetNum(I_GAKI) >= max) {
    u->Error("CAST: Unit can't summon more gaki.");
    return 0;
  }
    
  u->items.SetNum(I_GAKI,u->items.GetNum(I_GAKI) + 1);
  u->Event(AString("Summons ") + ItemString(I_GAKI,1) + ".");

  return 1;
}

int Game::RunSummonSpirit(ARegion *r,Unit *u)
{

  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int level = u->GetSkill(S_SUMMON_SPIRITS);

  //summon minor kami
  if (order->level < 3) {
    int summoned = level * level;
    
    u->items.SetNum(I_MINOR_KAMI,u->items.GetNum(I_MINOR_KAMI) + summoned);
    u->Event(AString("Summons ") + ItemString(I_MINOR_KAMI,summoned) + ".");    
    return 1;
  }
    
  //summon kami - level-2 squared
  if (order->level < 5) {
    int max = (level) * (level);
    int summoned = (level-2) * (level-2);
    int current = u->items.GetNum(I_KAMI);

    //check limits
    int maxSummoned = max - current;
    if (maxSummoned < summoned)
      summoned = maxSummoned;
    
    if (u->items.GetNum(I_KAMI) >= max) {
      u->Error("CAST: Unit can't summon more kami.");
      return 0;
    }
    u->items.SetNum(I_KAMI,u->items.GetNum(I_KAMI) + summoned);
    u->Event(AString("Summons ") + ItemString(I_KAMI, summoned) + ".");    
    return 1;      
  }

  //summon dai-oni
  int max = 1;
    
  if (u->items.GetNum(I_MAJOR_KAMI) >= max) {
    u->Error("CAST: Unit can't summon more major kami.");
    return 0;
  }
    
  u->items.SetNum(I_MAJOR_KAMI,u->items.GetNum(I_MAJOR_KAMI) + 1);
  u->Event(AString("Summons ") + ItemString(I_MAJOR_KAMI,1) + ".");

  return 1;
}

/*
int Game::RunSummonImps(ARegion *r,Unit *u)
{
  int level = u->GetSkill(S_SUMMON_IMPS);

  u->items.SetNum(I_IMP,u->items.GetNum(I_IMP) + level);
  u->Event(AString("Summons ") + ItemString(I_IMP,level) + ".");
  return 1;
}
*/

int Game::RunCreateArtifact(ARegion *r,Unit *u,int skill,int item)
{
  int level = u->GetSkill(skill);
  unsigned int c;
  for(c = 0; c < sizeof(ItemDefs[item].mInput)/sizeof(Materials); c++) {
    if(ItemDefs[item].mInput[c].item == -1) continue;
    int amt = u->GetSharedNum(ItemDefs[item].mInput[c].item);
    int cost = ItemDefs[item].mInput[c].amt;
    if(amt < cost) {
      u->Error(AString("Doesn't have sufficient ") +
	       ItemDefs[ItemDefs[item].mInput[c].item].name +
	       " to create that.");
      return 0;
    }
  }

  // Deduct the costs
  for(c = 0; c < sizeof(ItemDefs[item].mInput)/sizeof(Materials); c++) {
    if(ItemDefs[item].mInput[c].item == -1) continue;
    int cost = ItemDefs[item].mInput[c].amt;
    u->ConsumeShared(ItemDefs[item].mInput[c].item, cost);
  }

  int num = (level * ItemDefs[item].mOut + getrandom(100))/100;

  u->items.SetNum(item,u->items.GetNum(item) + num);
  u->Event(AString("Creates ") + ItemString(item,num) + ".");
  if (num == 0) return 0;
  return 1;
}
/*
int Game::RunSummonLich(ARegion *r,Unit *u)
{
  int level = u->GetSkill(S_SUMMON_LICH);

  int num = ((2 * level * level) + getrandom(100))/100;

  u->items.SetNum(I_GAKI,u->items.GetNum(I_GAKI) + num);
  u->Event(AString("Summons ") + ItemString(I_GAKI,num) + ".");
  //  u->items.SetNum(I_LICH,u->items.GetNum(I_LICH) + num);
  //u->Event(AString("Summons ") + ItemString(I_LICH,num) + ".");
  if (num == 0) return 0;
  return 1;
}
*/
/*
int Game::RunRaiseUndead(ARegion *r,Unit *u)
{
  int level = u->GetSkill(S_RAISE_UNDEAD);

  int num = ((10 * level * level) + getrandom(100))/100;

  u->items.SetNum(I_UNDEAD,u->items.GetNum(I_UNDEAD) + num);
  u->Event(AString("Raises ") + ItemString(I_UNDEAD,num) + ".");
  if (num == 0) return 0;
  return 1;
}
*/
/*
int Game::RunSummonSkeletons(ARegion *r,Unit *u)
{
  int level = u->GetSkill(S_SUMMON_SKELETONS);

  int num = ((40 * level * level) + getrandom(100))/100;

  u->items.SetNum(I_SKELETON,u->items.GetNum(I_SKELETON) + num);
  u->Event(AString("Summons ") + ItemString(I_SKELETON,num) + ".");
  if (num == 0) return 0;
  return 1;
}
*/
int Game::RunSummonDragon(ARegion *r, Unit *u)
{
  int level = u->GetSkill(S_SUMMON_DRAGON);

  int num = u->items.GetNum(I_DRAGON);
  if (num >= level) {
    u->Error("Mage may not summon more dragons.");
    return 0;
  }

  int chance = level * level * 4;
  if (getrandom(100) < chance) {
    u->items.SetNum(I_DRAGON,num + 1);
    u->Event("Summons a dragon.");
    num = 1;
  } else {
    u->Event("Attempts to summon a dragon, but fails.");
    num = 0;
  }
  if (num == 0) return 0;
  return 1;
}

/*
int Game::RunBirdLore(ARegion *r,Unit *u)
{
  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int type = regions.GetRegionArray(r->zloc)->levelType;

  if(type != ARegionArray::LEVEL_SURFACE) {
    AString error = "CAST: Bird Lore may only be cast on the surface of ";
    error += Globals->WORLD_NAME;
    error += ".";
    u->Error(error.Str());
    return 0;
  }

  if (order->level < 3) {
    int dir = order->target;
    ARegion *tar = r->neighbors[dir];
    if (!tar) {
      u->Error("CAST: No such region.");
      return 0;
    }

    Farsight *f = new Farsight;
    f->faction = u->faction;
    f->level = u->GetSkill(S_BIRD_LORE);
    tar->farsees.Add(f);
    u->Event(AString("Sends birds to spy on ") +
	     tar->Print( &regions ) + ".");
    return 1;
  }

  int level = u->GetSkill(S_BIRD_LORE);
  int max = (level - 2) * (level - 2);

  if (u->items.GetNum(I_EAGLE) >= max) {
    u->Error("CAST: Mage can't summon more eagles.");
    return 0;
  }

  u->items.SetNum(I_EAGLE,u->items.GetNum(I_EAGLE) + 1);
  u->Event("Summons an eagle.");
  return 1;
}

*/

//BUSHIDO mod
int Game::RunHawking(ARegion *r,Unit *u)
{
  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int type = regions.GetRegionArray(r->zloc)->levelType;

  if(type != ARegionArray::LEVEL_SURFACE) {
    AString error = "CAST: Hawking may only be cast on the surface of ";
    error += Globals->WORLD_NAME;
    error += ".";
    u->Error(error.Str());
    return 0;
  }

  if (order->level < 3) {
    int dir = order->target;
    ARegion *tar = r->neighbors[dir];
    if (!tar) {
      u->Error("CAST: No such region.");
      return 0;
    }

    Farsight *f = new Farsight;
    f->faction = u->faction;
    f->level = u->GetSkill(S_HAWKING);
    tar->farsees.Add(f);
    u->Event(AString("Sends birds to spy on ") +
	     tar->Print( &regions ) + ".");
    return 1;
  }
    
  int level = u->GetSkill(S_HAWKING);

  if (order->level < 5) {
    int max = (level) * (level);
      
    if (u->items.GetNum(I_EAGLE) >= max) {
      u->Error("CAST: Unit can't summon more eagles.");
      return 0;
    }
      
    u->items.SetNum(I_EAGLE,u->items.GetNum(I_EAGLE) + 1);
    u->Event("Summons an eagle.");
    return 1;
  }
    
  int max = level;
    
  if (u->items.GetNum(I_HOU_OU) >= max) {
    u->Error("CAST: Unit can't summon more hou-ou.");
    return 0;
  }
    
  u->items.SetNum(I_HOU_OU,u->items.GetNum(I_HOU_OU) + 1);
  u->Event("Summons an hou-ou.");
  return 1;
}
//end BUSHIDO mod

/*
int Game::RunWolfLore(ARegion *r,Unit *u)
{
  if (TerrainDefs[r->type].similar_type != R_MOUNTAIN &&
      TerrainDefs[r->type].similar_type != R_FOREST) {
    u->Error("CAST: Can only summon wolves in mountain and "
	     "forest regions.");
    return 0;
  }

  int level = u->GetSkill(S_WOLF_LORE);
  int max = level * level * 4;

  int num = u->items.GetNum(I_WOLF);
  int summon = max - num;
  if (summon > level) summon = level;
  if (summon < 0) summon = 0;

  u->Event(AString("Casts Wolf Lore, summoning ") +
	   ItemString(I_WOLF,summon) + ".");
  u->items.SetNum(I_WOLF,num + summon);
  if (summon == 0) return 0;
  return 1;
}
*/

int Game::RunInvisibility(ARegion *r,Unit *u)
{
  CastUnitsOrder *order = (CastUnitsOrder *) u->castorders;
  int max = u->GetSkill(S_INVISIBILITY);
  max = max * max;

  int num = 0;
  forlist (&(order->units)) {
    Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
    if (!tar) continue;
    if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
    num += tar->GetSoldiers();
  }

  if (num > max) {
    u->Error("CAST: Can't render that many men or creatures invisible.");
    return 0;
  }

  if (!num) {
    u->Error("CAST: No valid targets to turn invisible.");
    return 0;
  }
  {
    forlist (&(order->units)) {
      Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
      if (!tar) continue;
      if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
      tar->SetFlag(FLAG_INVIS,1);
      tar->Event(AString("Is rendered invisible by ") +
		 *(u->name) + ".");
    }
  }

  u->Event("Casts invisibility.");
  return 1;
}


int Game::RunCamouflage(ARegion *r,Unit *u)
{
  //CastUnitsOrder *order = (CastUnitsOrder *) u->castorders;
  //int max = u->GetSkill(S_CAMOUFLAGE);
  //max = max * max;

  //int num = 0;
  //forlist (&(order->units)) {
  //  Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
  //  if (!tar) continue;
  //  if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
  //  num += tar->GetSoldiers();
  //}

  //if (num > max) {
  //  u->Error("CAST: Can't render that many men or creatures invisible.");
  //  return 0;
  //}

  //if (!num) {
  //  u->Error("CAST: No valid targets to turn invisible.");
  //  return 0;
  //}

  u->SetFlag(FLAG_CAMOUFLAGE,1);

  //{
  //  forlist (&(order->units)) {
  //    Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
  //    if (!tar) continue;
  //    if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
  //    tar->SetFlag(FLAG_INVIS,1);
  //    tar->Event(AString("Is rendered invisible by ") +
  //		 *(u->name) + ".");
  //  }
  //}

  u->Event("Casts camouflage.");
  return 1;
}


int Game::RunKeenEyes(ARegion *r,Unit *u)
{
  //CastUnitsOrder *order = (CastUnitsOrder *) u->castorders;
  //int max = u->GetSkill(S_CAMOUFLAGE);
  //max = max * max;

  //int num = 0;
  //forlist (&(order->units)) {
  //  Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
  //  if (!tar) continue;
  //  if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
  //  num += tar->GetSoldiers();
  //}

  //if (num > max) {
  //  u->Error("CAST: Can't render that many men or creatures invisible.");
  //  return 0;
  //}

  //if (!num) {
  //  u->Error("CAST: No valid targets to turn invisible.");
  //  return 0;
  //}

  u->SetFlag(FLAG_SEARCHING,1);

  //{
  //  forlist (&(order->units)) {
  //    Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
  //    if (!tar) continue;
  //    if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
  //    tar->SetFlag(FLAG_INVIS,1);
  //    tar->Event(AString("Is rendered invisible by ") +
  //		 *(u->name) + ".");
  //  }
  //}

  u->Event("Casts keen eyes.");
  return 1;
}

/*
int Game::RunPhanDemons(ARegion *r,Unit *u)
{
  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int level = u->GetSkill(S_CREATE_PHANTASMAL_DEMONS);
  int create,max;

  if (order->level < 3) {
    create = I_IIMP;
    max = level * level * 4;
  } else {
    if (order->level < 5) {
      create = I_IDEMON;
      max = (level - 2) * (level - 2);
    } else {
      create = I_IBALROG;
      max = 1;
    }
  }

  if (order->target > max || order->target <= 0) {
    u->Error("CAST: Can't create that many Phantasmal Demons.");
    return 0;
  }

  u->items.SetNum(create,order->target);
  u->Event("Casts Create Phantasmal Demons.");
  return 1;
}
*/
/*
int Game::RunPhanUndead(ARegion *r,Unit *u)
{
  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int level = u->GetSkill(S_CREATE_PHANTASMAL_UNDEAD);
  int create,max;

  if (order->level < 3) {
    create = I_ISKELETON;
    max = level * level * 4;
  } else {
    if (order->level < 5) {
      create = I_IUNDEAD;
      max = (level - 2) * (level - 2);
    } else {
      create = I_ILICH;
      max = 1;
    }
  }

  if (order->target > max || order->target <= 0) {
    u->Error("CAST: Can't create that many Phantasmal Undead.");
    return 0;
  }

  u->items.SetNum(create,order->target);
  u->Event("Casts Create Phantasmal Undead.");
  return 1;
}
*/
/*
int Game::RunPhanBeasts(ARegion *r,Unit *u)
{
  CastIntOrder *order = (CastIntOrder *) u->castorders;
  int level = u->GetSkill(S_CREATE_PHANTASMAL_BEASTS);
  int create,max;

  if (order->level < 3) {
    create = I_IWOLF;
    max = level * level * 4;
  } else {
    if (order->level < 5) {
      create = I_IEAGLE;
      max = (level - 2) * (level - 2);
    } else {
      create = I_IDRAGON;
      max = 1;
    }
  }

  if (order->target > max || order->target <= 0) {
    u->Error("CAST: Can't create that many Phantasmal Beasts.");
    return 0;
  }

  u->items.SetNum(create,order->target);
  u->Event("Casts Create Phantasmal Beasts.");
  return 1;
}
*/

int Game::RunEarthLore(ARegion *r,Unit *u)
{
  int level = u->GetSkill(S_EARTH_LORE);

  if (level > r->earthlore) r->earthlore = level;
  int amt = r->Wages() * level * 2 / 10;

  u->items.SetNum(I_SILVER,u->items.GetNum(I_SILVER) + amt);
  u->Event(AString("Casts Earth Lore, raising ") + amt + " silver.");
  return 1;
}

int Game::RunClearSkies(ARegion *r, Unit *u)
{
  ARegion *tar = r;
  AString temp = "Casts Clear Skies";
  int val;

  CastRegionOrder *order = (CastRegionOrder *)u->castorders;

  RangeType *range = FindRange(SkillDefs[S_CLEAR_SKIES].range);
  if(range != NULL) {
    tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
    val = GetRegionInRange(r, tar, u, S_CLEAR_SKIES);
    if(!val) return 0;
    temp += " on ";
    temp += tar->ShortPrint(&regions);
  }
  temp += ".";
  int level = u->GetSkill(S_CLEAR_SKIES);
  if (level > r->clearskies) r->clearskies = level;
  u->Event(temp);
  return 1;
}

int Game::RunWeatherLore(ARegion *r, Unit *u)
{
  ARegion *tar;
  int val, i;

  CastRegionOrder *order = (CastRegionOrder *)u->castorders;

  tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
  val = GetRegionInRange(r, tar, u, S_WEATHER_LORE);
  if(!val) return 0;

  int level = u->GetSkill(S_WEATHER_LORE);
  int months = 3;
  if(level >= 5) months = 12;
  else if (level >= 3) months = 6;

  AString temp = "Casts Weather Lore on ";
  temp += tar->ShortPrint(&regions);
  temp += ". It will be ";
  int weather, futuremonth;
  for(i = 0; i <= months; i++) {
    futuremonth = (month + i)%12;
    weather=regions.GetWeather(tar, futuremonth);
    temp += SeasonNames[weather];
    temp += " in ";
    temp += MonthNames[futuremonth];
    if(i < (months-1))
      temp += ", ";
    else if(i == (months-1))
      temp += " and ";
    else
      temp += ".";
  }
  u->Event(temp);
  return 1;
}

int Game::RunFarsight(ARegion *r,Unit *u)
{
  ARegion *tar;
  int val;
  int tea = 0;
  
  Farsight *f = new Farsight;
  f->faction = u->faction;
  f->level = u->GetSkill(S_FARSIGHT);

  //BUSHIDO mod
  if (f->level < 1) {
    f->level = u->GetSkill(S_TEA_CEREMONY);
    tea = 1;
  }
  //end bushido mod

  CastRegionOrder *order = (CastRegionOrder *)u->castorders;

  tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
  if (tea == 0)
    val = GetRegionInRange(r, tar, u, S_FARSIGHT);
  else
    val = GetRegionInRange(r, tar, u, S_TEA_CEREMONY);
  if(!val) return 0;

  //  Farsight *f = new Farsight;
  //f->faction = u->faction;
  //f->level = u->GetSkill(S_FARSIGHT);

  //BUSHIDO mod
  //  if (f->level < 1) {
  //  f->level = u->GetSkill(S_TEA_CEREMONY);
  // tea = 1;
  //}
  //end bushido mod

  f->unit = u;
  f->observation = u->GetAttribute("observation");
  tar->farsees.Add(f);
  AString temp;
  if (tea == 0) {
    temp += "Casts Farsight on ";
  } else {
    temp += "Receives gossip though the Geisha House on ";
  }
  temp += tar->ShortPrint(&regions);
  temp += ".";
  u->Event(temp);
  return 1;
}

int Game::RunDetectGates(ARegion *r,Object *o,Unit *u)
{
  int level = u->GetSkill(S_GATE_LORE);

  if (level == 1) {
    u->Error("CAST: Casting Gate Lore at level 1 has no effect.");
    return 0;
  }

  u->Event("Casts Gate Lore, detecting nearby Gates:");
  int found = 0;
  if ((r->gate) && (!r->gateopen)) {
    u->Event(AString("Identified local gate number ") + (r->gate) +
	     " in " + r->ShortPrint(&regions) + ".");
  }
  for (int i=0; i<NDIRS; i++) {
    ARegion *tar = r->neighbors[i];
    if (tar) {
      if (tar->gate) {
	if(Globals->DETECT_GATE_NUMBERS) {
	  u->Event(tar->Print(&regions) +
		   " contains Gate " + tar->gate +
		   ".");
	} else {
	  u->Event(tar->Print(&regions) +
		   " contains a Gate.");
	}
	found = 1;
      }
    }
  }
  if (!found)
    u->Event("There are no nearby Gates.");
  return 1;
}

int Game::RunTeleport(ARegion *r,Object *o,Unit *u)
{
  ARegion *tar;
  int val;

  CastRegionOrder *order = (CastRegionOrder *)u->teleportorders;

  tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
  val = GetRegionInRange(r, tar, u, S_TELEPORTATION);
  if(!val) return 0;

  int level = u->GetSkill(S_TELEPORTATION);
  int maxweight = level * 15;

  if (u->Weight() > maxweight) {
    u->Error("CAST: Can't carry that much when teleporting.");
    return 0;
  }

  // Presume they had to open the portal to see if target is ocean
  if (TerrainDefs[tar->type].similar_type == R_OCEAN) {
    u->Error(AString("CAST: ") + tar->Print(&regions) +
	     " is an ocean.");
    return 1;
  }
  u->DiscardUnfinishedShips();
  u->Event(AString("Teleports to ") + tar->Print(&regions) + ".");
  u->MoveUnit(tar->GetDummy());
  return 1;
}

int Game::RunGateJump(ARegion *r,Object *o,Unit *u)
{
  int level = u->GetSkill(S_GATE_LORE);
  int nexgate = 0;
  if( !level ) {
    u->Error( "CAST: Unit doesn't have that skill." );
    return 0;
  }

  TeleportOrder *order = u->teleportorders;

  if (order->gate != -1 && level < 3) {
    u->Error("CAST: Unit Doesn't know Gate Lore at that level.");
    return 0;
  }

  nexgate = Globals->NEXUS_GATE_OUT &&
    (TerrainDefs[r->type].similar_type == R_NEXUS);
  if (!r->gate && !nexgate) {
    u->Error("CAST: There is no gate in that region.");
    return 0;
  }

  if (!r->gateopen) {
    u->Error("CAST: Gate not open at this time of year.");
    return 0;
  }

  int maxweight = 10;
  if (order->gate != -1) level -= 2;
  switch (level) {
  case 1:
    maxweight = 15;
    break;
  case 2:
    maxweight = 100;
    break;
  case 3:
  case 4:
  case 5:
    maxweight = 1000;
    break;
  }

  int weight = u->Weight();

  forlist (&(order->units)) {
    Unit *taru = r->GetUnitId((UnitId *) elem,u->faction->num);
    if (taru && taru != u) weight += taru->Weight();
  }

  if (weight > maxweight) {
    u->Error( "CAST: That mage cannot carry that much weight "
	      "through a Gate.");
    return 0;
  }

  ARegion *tar;
  if (order->gate == -1) {
    int good = 0;
    int gatenum = getrandom(regions.numberofgates);
    tar = regions.FindGate(gatenum+1);

    if(tar && tar->zloc == r->zloc) good = 1;
    if(tar && nexgate && tar->zloc == ARegionArray::LEVEL_SURFACE)
      good = 1;

    while( !good ) {
      gatenum = getrandom(regions.numberofgates);
      tar = regions.FindGate(gatenum+1);
      if(tar && tar->zloc == r->zloc) good = 1;
      if(tar && nexgate && tar->zloc == ARegionArray::LEVEL_SURFACE)
	good = 1;
    }

    u->Event("Casts Random Gate Jump.");
  } else {
    if (order->gate < 1 || order->gate > regions.numberofgates) {
      u->Error("CAST: No such target gate.");
      return 0;
    }

    tar = regions.FindGate(order->gate);
    if (!tar) {
      u->Error("CAST: No such target gate.");
      return 0;
    }
    if(!tar->gateopen) {
      u->Error("CAST: Target gate not open at this time of year.");
      return 0;
    }

    u->Event("Casts Gate Jump.");
  }

  int comma = 0;
  AString unitlist; {
    forlist(&(order->units)) {
      Location *loc = r->GetLocation((UnitId *) elem,u->faction->num);
      if (loc) {
	/* Don't do the casting unit yet */
	if (loc->unit == u) {
	  delete loc;
	  continue;
	}

	if (loc->unit->GetAttitude(r,u) < A_ALLY) {
	  u->Error("CAST: Unit is not allied.");
	} else {
	  if (comma) {
	    unitlist += AString(", ") + AString(loc->unit->num);
	  } else {
	    unitlist += AString(loc->unit->num);
	    comma = 1;
	  }
	  loc->unit->DiscardUnfinishedShips();
	  loc->unit->Event(AString("Is teleported through a ") +
			   "Gate to " + tar->Print(&regions) + " by " +
			   *u->name + ".");
	  loc->unit->MoveUnit( tar->GetDummy() );
	  if (loc->unit != u) loc->unit->ClearCastOrders();
	}
	delete loc;
      } else {
	u->Error("CAST: No such unit.");
      }
    }
  }
  u->DiscardUnfinishedShips();
  u->Event(AString("Jumps through a Gate to ") +
	   tar->Print( &regions ) + ".");
  if (comma) {
    u->Event(unitlist + " follow through the Gate.");
  }
  u->MoveUnit( tar->GetDummy() );
  return 1;
}

int Game::RunPortalLore(ARegion *r,Object *o,Unit *u)
{
  int level = u->GetSkill(S_PORTAL_LORE);
  TeleportOrder *order = u->teleportorders;

  if (!level) {
    u->Error("CAST: Doesn't know Portal Lore.");
    return 0;
  }

  if (!u->items.GetNum(I_PORTAL)) {
    u->Error("CAST: Unit doesn't have a Portal.");
    return 0;
  }

  int maxweight = 50 * level;
  int weight = 0;
  forlist (&(order->units)) {
    Unit *taru = r->GetUnitId((UnitId *) elem,u->faction->num);
    if (taru) weight += taru->Weight();
  }

  if (weight > maxweight) {
    u->Error("CAST: That mage cannot teleport that much weight through a "
	     "Portal.");
    return 0;
  }

  Location *tar = regions.FindUnit(order->gate);
  if (!tar) {
    u->Error("CAST: No such target mage.");
    return 0;
  }

  if (tar->unit->faction->GetAttitude(u->faction->num) < A_FRIENDLY) {
    u->Error("CAST: Target mage is not friendly.");
    return 0;
  }

  if (tar->unit->type != U_MAGE) {
    u->Error("CAST: Target is not a mage.");
    return 0;
  }

  if (!tar->unit->items.GetNum(I_PORTAL)) {
    u->Error("CAST: Target does not have a Portal.");
    return 0;
  }

  if (!GetRegionInRange(r, tar->region, u, S_PORTAL_LORE)) return 0;

  u->Event("Casts Portal Jump.");

  {
    forlist(&(order->units)) {
      Location *loc = r->GetLocation((UnitId *) elem,u->faction->num);
      if (loc) {
	if (loc->unit->GetAttitude(r,u) < A_ALLY) {
	  u->Error("CAST: Unit is not allied.");
	} else {
	  loc->unit->DiscardUnfinishedShips();
	  loc->unit->Event(AString("Is teleported to ") +
			   tar->region->Print( &regions ) +
			   " by " + *u->name + ".");
	  loc->unit->MoveUnit( tar->obj );
	  if (loc->unit != u) loc->unit->ClearCastOrders();
	}
	delete loc;
      } else {
	u->Error("CAST: No such unit.");
      }
    }
  }

  delete tar;
  return 1;
}

void Game::RunTeleportOrders()
{
  int val = 1;
  forlist(&regions) {
    ARegion * r = (ARegion *) elem;
    forlist(&r->objects) {
      Object * o = (Object *) elem;
      int foundone = 1;
      while (foundone) {
	foundone = 0;
	forlist(&o->units) {
	  Unit * u = (Unit *) elem;
	  if (u->teleportorders) {
	    foundone = 1;
	    switch (u->teleportorders->spell) {
	    case S_GATE_LORE:
	      val = RunGateJump(r,o,u);
	      break;
	    case S_TELEPORTATION:
	      val = RunTeleport(r,o,u);
	      break;
	    case S_PORTAL_LORE:
	      val = RunPortalLore(r,o,u);
	      break;
	    }
	    if (val)
	      u->Practice(u->teleportorders->spell);
	    delete u->teleportorders;
	    u->teleportorders = 0;
	    break;
	  }
	}
      }
    }
  }
}


int Game::CheckTeaCeremonyBuilding(Unit *u) {

  Object *obj = u->object;

  if (obj->type == O_GEISHA_HOUSE &&
      u == obj->GetOwner()) {
    return 1;
  } else {
    u->Error("CAST: Unit not owner of a GEISHA HOUSE.");
    return 0;
  }

  return 0;
}
