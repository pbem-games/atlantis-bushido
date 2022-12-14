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
// Date			Person				Comments
// ----			------				--------
// 2000/MAR/14	Larry Stanbery		Added production enhancement.
// 2000/MAR/21	Azthar Septragen	Added roads.
// 2001/Feb/21	Joseph Traub		Added FACLIM_UNLIMITED
#include "game.h"
#include "gamedata.h"

void Game::RunSailOrders()
{
    // ALT 28-Jul-2000
    // Fixes to prevent sailing of incomplete ships
    int tmpError = 0;
    
    forlist(&regions) {
	ARegion * r = (ARegion *) elem;
	AList regs;
	forlist(&r->objects) {
	    Object * o = (Object *) elem;
	    Unit * u = o->GetOwner();
	    if (u && u->monthorders &&
		u->monthorders->type == O_SAIL &&
		o->type == O_FLEET) {
		if(o->incomplete < 50) {
		    ARegionPtr * p = new ARegionPtr;
		    p->ptr = Do1SailOrder(r,o,u);
		    regs.Add(p);
		} else {
		    tmpError = 1;
		}
	    } else {
		tmpError = 2;
	    }

	    if(tmpError) {
		forlist(&o->units) {
		    Unit * u2 = (Unit *) elem;
		    if (u2->monthorders && u2->monthorders->type == O_SAIL) {
			delete u2->monthorders;
			u2->monthorders = 0;
			switch(tmpError) {
			    case 1:
				u2->Error("SAIL: Fleet is too damaged to sail.");
				break;
			    case 2:
				u2->Error("SAIL: Owner must issue fleet directions.");
				break;
			}
		    }
		}
	    }
	}
	{
	    forlist(&regs) {
		ARegion * r2 = ((ARegionPtr *) elem)->ptr;
		DoAutoAttacksRegion(r2);
	    }
	}
    }
}

ARegion * Game::Do1SailOrder(ARegion * reg,Object * fleet,Unit * cap)
{
    SailOrder * o = (SailOrder *) cap->monthorders;
    int movepoints = fleet->GetFleetSpeed(0);
    int moveok = 0;
    int moved = 0;

    AList facs;
    int wgt = 0;
    int slr = 0;
    forlist(&fleet->units) {
	Unit * unit = (Unit *) elem;
	if (unit->guard == GUARD_GUARD) unit->guard = GUARD_NONE;
	if (!GetFaction2(&facs,unit->faction->num)) {
	    FactionPtr * p = new FactionPtr;
	    p->ptr = unit->faction;
	    facs.Add(p);
	}
	wgt += unit->Weight();
	if (unit->monthorders && unit->monthorders->type == O_SAIL) {
	    slr += unit->GetSkill(S_SAILING) * unit->GetMen();
	}
    }

    if (wgt > fleet->FleetCapacity()) {
	cap->Error("SAIL: Fleet is overloaded.");
	moveok = 1;
    } else {
	if (slr < fleet->GetFleetSize()) {
	    cap->Error("SAIL: Not enough sailors.");
	    moveok = 1;
	} else {
	    while (o->dirs.Num()) {
		MoveDir * x = (MoveDir *) o->dirs.First();
		o->dirs.Remove(x);
		int i = x->dir;
		delete x;
		ARegion * newreg = reg->neighbors[i];
		if (!newreg) {
		    cap->Error("SAIL: Can't sail that way.");
		    break;
		}
		int cost = 1;
		if(Globals->WEATHER_EXISTS) {
		    if (newreg->weather != W_NORMAL && !newreg->clearskies) cost = 2;
		}

		if (fleet->flying < 1 && !newreg->IsCoastal()) {
		    cap->Error("SAIL: Can't sail inland.");
		    break;
		}

		if ((fleet->flying < 1) &&
		    (TerrainDefs[reg->type].similar_type != R_OCEAN) &&
		    (TerrainDefs[newreg->type].similar_type != R_OCEAN)) {
		    cap->Error("SAIL: Can't sail inland.");
		    break;
		}

		// Check to see if sailing THROUGH land!
		// always allow retracing steps
		if (Globals->PREVENT_SAIL_THROUGH &&
		    (TerrainDefs[reg->type].similar_type != R_OCEAN) &&
		    (fleet->flying < 1) &&
		    (fleet->prevdir != -1) &&
		    (fleet->prevdir != i)) {
		    int blocked1 = 0;
		    int blocked2 = 0;
		    int d1 = fleet->prevdir;
		    int d2 = i;
		    if (d1 > d2) {
			int tmp = d1;
			d1 = d2;
			d2 = tmp;
		    }
		    for (int k = d1+1; k < d2; k++) {
			ARegion *land1 = reg->neighbors[k];
			if ((!land1) ||
			    (TerrainDefs[land1->type].similar_type !=
			     R_OCEAN))
			    blocked1 = 1;
		    }
		    int sides = NDIRS - 2 - (d2 - d1 - 1);
		    for (int l = d2+1; l <= d2 + sides; l++) {
			int dl = l;
			if (dl >= NDIRS) dl -= NDIRS;
			ARegion *land2 = reg->neighbors[dl];
			if ((!land2) ||
			    (TerrainDefs[land2->type].similar_type !=
			     R_OCEAN))
			    blocked2 = 1;
		    }
		    if ((blocked1) && (blocked2))
		    {
			cap->Error(AString("SAIL: Could not sail ") +
				   DirectionStrs[i] + AString(" from ") +
				   reg->ShortPrint(&regions) +
				   ". Cannot sail through land.");
			break;
		    }
		}

		if (movepoints < cost) {
		    cap->Error("SAIL: Can't sail that far;"
			       " remaining moves queued.");
		    TurnOrder *tOrder = new TurnOrder;
		    tOrder->repeating = 0;
		    AString order = "SAIL ";
		    order += DirectionAbrs[i];
		    forlist(&o->dirs) {
			MoveDir *move = (MoveDir *) elem;
			order += " ";
			order += DirectionAbrs[move->dir];
		    }
		    tOrder->turnOrders.Add(new AString(order));
		    cap->turnorders.Insert(tOrder);
		    break;
		}

		movepoints -= cost;
		moved = 1;
		fleet->MoveObject(newreg);
		fleet->SetPrevDir(reg->GetRealDirComp(i));
		forlist(&facs) {
		    Faction * f = ((FactionPtr *) elem)->ptr;
		    f->Event(*fleet->name + AString(" sails from ") +
			     reg->ShortPrint(&regions) + AString(" to ") +
			     newreg->ShortPrint(&regions) + AString("."));
		}
		if(Globals->TRANSIT_REPORT != GameDefs::REPORT_NOTHING) {
		    forlist(&fleet->units) {
			// Everyone onboard gets to see the sights
			Unit *unit = (Unit *)elem;
						
			unit->DiscardUnfinishedShips();		
			Farsight *f;
			// Note the hex being left
			forlist(&reg->passers) {
			    f = (Farsight *)elem;
			    if(f->unit == unit) {
				// We moved into here this turn
				f->exits_used[i] = 1;
			    }
			}
			// And mark the hex being entered
			f = new Farsight;
			f->faction = unit->faction;
			f->level = 0;
			f->unit = unit;
			f->exits_used[reg->GetRealDirComp(i)] = 1;
			newreg->passers.Add(f);
		    }
		}
		reg = newreg;
		if (newreg->ForbiddenShip(fleet)) {
		    cap->faction->Event(*fleet->name +
					AString(" is stopped by guards in ") +
					newreg->ShortPrint(&regions) + AString("."));
		    break;
		}
	    }
	}
    }

    /* Clear out everyone's orders */
    {
	forlist(&fleet->units) {
	    Unit * unit = (Unit *) elem;
	    if (!moveok) {
		unit->alias = 0;
	    }

	    unit->PracticeAttribute("wind");

	    if (unit->monthorders) {
		if (unit->monthorders->type == O_SAIL) {
		    unit->Practice(S_SAILING);
		    delete unit->monthorders;
		    unit->monthorders = 0;
		}
		else if (!moveok && unit->monthorders->type == O_MOVE) {
		    delete unit->monthorders;
		    unit->monthorders = 0;
		}
	    }
	}
    }

    return reg;
}

void Game::RunTeachOrders()
{
    forlist((&regions)) {
	ARegion * r = (ARegion *) elem;
	forlist((&r->objects)) {
	    Object * obj = (Object *) elem;
	    forlist((&obj->units)) {
		Unit * u = (Unit *) elem;
		if (u->monthorders) {
		    if (u->monthorders->type == O_TEACH) {
			Do1TeachOrder(r,u);
			delete u->monthorders;
			u->monthorders = 0;
		    }
		}
	    }
	}
    }
}

void Game::Do1TeachOrder(ARegion * reg,Unit * unit)
{
    /* First pass, find how many to teach */
    if(Globals->LEADERS_EXIST && !unit->IsLeader()) {
	/* small change to handle Ceran's mercs */
	//if(!unit->GetMen(I_MERC)) {
	    // Mercs can teach even though they are not leaders.
	    // They cannot however improve their own skills
      unit->Error("TEACH: Only leaders can teach.");
      return;
      //}
    }

    int students = 0;
    TeachOrder * order = (TeachOrder *) unit->monthorders;
    forlist(&order->targets) {
	UnitId * id = (UnitId *) elem;
	Unit * target = reg->GetUnitId(id,unit->faction->num);
	if (!target) {
	    order->targets.Remove(id);
	    unit->Error("TEACH: No such unit.");
	    delete id;
	} else {
	    if (target->faction->GetAttitude(unit->faction->num) < A_FRIENDLY) {
		unit->Error(AString("TEACH: ") + *(target->name) +
			    " is not a member of a friendly faction.");
		order->targets.Remove(id);
		delete id;
	    } else {
		if (!target->monthorders ||
		    target->monthorders->type != O_STUDY) {
		    unit->Error(AString("TEACH: ") + *(target->name) +
				" is not studying.");
		    order->targets.Remove(id);
		    delete id;
		} else {
		    int sk = ((StudyOrder *) target->monthorders)->skill;
		    if (unit->GetRealSkill(sk) <= target->GetRealSkill(sk)) {
			unit->Error(AString("TEACH: ") +
				    *(target->name) + " is not studying "
				    "a skill you can teach.");
			order->targets.Remove(id);
			delete id;
		    } else {
			// Check whether it's a valid skill to teach
			if (SkillDefs[sk].flags & SkillType::NOTEACH) {
			    unit->Error(AString("TEACH: ") + 
					AString(SkillDefs[sk].name) + 
					" cannot be taught.");
			    return;
			} else {
			    students += target->GetMen();
			}
		    }
		}
	    }
	}
    }
    if (!students) return;

    int days = (30 * unit->GetMen() * Globals->STUDENTS_PER_TEACHER);

    /* We now have a list of valid targets */
    {
	forlist(&order->targets) {
	    UnitId * id = (UnitId *) elem;
	    Unit * u = reg->GetUnitId(id,unit->faction->num);

	    int umen = u->GetMen();
	    int tempdays = (umen * days) / students;
	    if (tempdays > 30 * umen) tempdays = 30 * umen;
	    days -= tempdays;
	    students -= umen;

	    StudyOrder * o = (StudyOrder *) u->monthorders;
	    o->days += tempdays;
	    if (o->days > 30 * umen)
	    {
		days += o->days - 30 * umen;
		o->days = 30 * umen;
	    }
	    unit->Event(AString("Teaches ") + SkillDefs[o->skill].name +
			" to " + *u->name + ".");
	    // The TEACHER may learn something in this process!
	    unit->Practice(o->skill);
	}
    }
}

void Game::Run1BuildOrder(ARegion * r,Object * obj,Unit * u)
{
    if (!TradeCheck(r, u->faction)) {
	u->Error("BUILD: Faction can't produce in that many regions.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    AString skname = ObjectDefs[obj->type].skill;
    int sk = LookupSkill(&skname);
    if (sk == -1) {
	u->Error("BUILD: Can't build that.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    int usk = u->GetSkill(sk);
    if (usk < ObjectDefs[obj->type].level) {
	u->Error("BUILD: Can't build that.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }
	

    int needed = obj->incomplete;
    int type = obj->type;
    // AS
    if(((ObjectDefs[type].flags&ObjectType::NEVERDECAY) || !Globals->DECAY) &&
       needed < 1) {
	u->Error("BUILD: Object is finished.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    // AS
    if(needed <= -(ObjectDefs[type].maxMaintenance)) {
	u->Error("BUILD: Object does not yet require maintenance.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    int it = ObjectDefs[type].item;
    int itn;
    if (it == I_WOOD_OR_STONE) {
	itn = u->GetSharedNum(I_WOOD) + u->GetSharedNum(I_STONE);
    } else {
	itn = u->GetSharedNum(it);
    }

    if (itn == 0) {
	u->Error("BUILD: Don't have the required materials.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    int num = u->GetMen() * usk;

    // AS
    AString job;
    if (needed < 1) {
	// This looks wrong, but isn't.
	// If a building has a maxMaintainence of 75 and the road is at
	// -70 (ie, 5 from max) then we want the value of maintMax to be
	// 5 here.  Then we divide by maintFactor (some things are easier
	// to refix than others) to get how many items we need to fix it.
	// Then we fix it by that many items * maintFactor
	int maintMax = ObjectDefs[type].maxMaintenance + needed;
	maintMax /= ObjectDefs[type].maintFactor;
	if (num > maintMax) num = maintMax;
	if (itn < num) num = itn;
	job = " maintenance ";
	obj->incomplete -= (num * ObjectDefs[type].maintFactor);
	if (obj->incomplete < -(ObjectDefs[type].maxMaintenance))
	    obj->incomplete = -(ObjectDefs[type].maxMaintenance);
    } else if(needed > 0) {
	if (num > needed) num = needed;
	if (itn < num) num = itn;
	job = " construction ";
	obj->incomplete -= num;
	if (obj->incomplete == 0) {
	    obj->incomplete = -(ObjectDefs[type].maxMaintenance);
	}
    }

    /* Perform the build */
	
    u->MoveUnit(obj);

    if (it == I_WOOD_OR_STONE) {
	if (num > u->GetSharedNum(I_STONE)) {
	    num -= u->GetSharedNum(I_STONE);
	    u->ConsumeShared(I_STONE, u->GetSharedNum(I_STONE));
	    u->ConsumeShared(I_WOOD, num);
	} else {
	    u->ConsumeShared(I_STONE, num);
	}
    } else {
	u->ConsumeShared(it, num);
    }
	
    /* Regional economic improvement */
    r->improvement += num;

    // AS
    u->Event(AString("Performs") + job + "on " + *(obj->name) + ".");
    u->Practice(sk);

    delete u->monthorders;
    u->monthorders = 0;
}

/* Alternate processing for building item-type ship
 * objects and instantiating fleets.
 */
void Game::RunBuildShipOrder(ARegion * r,Object * obj,Unit * u)
{
    int ship = abs(u->build);
    AString skname = ItemDefs[ship].pSkill;
    int skill = LookupSkill(&skname);
    int level = u->GetSkill(skill);

    // get needed to complete
    int maxbuild = 0;
    if((u->monthorders) && 
       (u->monthorders->type == O_BUILD)) {
	BuildOrder *border = (BuildOrder *) u->monthorders;
	maxbuild = border->needtocomplete;
    }
    int output = ShipConstruction(r, u, level, maxbuild, ship);
	
    if (output < 1) return;
	
    // are there unfinished ship items of the given type?
    int unfinished = u->items.GetNum(ship);
	
    // set up unfinished items
    if((unfinished == 0) && (maxbuild > 0)) {
	unfinished = ItemDefs[ship].pMonths;
	u->items.SetNum(ship, unfinished);	
    }

    // practice
    u->Practice(skill);

    // Now reduce unfinished by produced amount
    unfinished -= output;
    if(unfinished < 0) unfinished = 0;
    u->items.SetNum(ship,unfinished);
    if(unfinished == 0) {
	u->Event(AString("Finishes building a ") + ItemDefs[ship].name + " in " +
		 r->ShortPrint(&regions) + ".");
	CreateShip(r, u, ship);
    } else {
	int percent = 100 * output / ItemDefs[ship].pMonths;
	u->Event(AString("Performs construction work on a ") + 
		 ItemDefs[ship].name + " (" + percent + "%) in " +
		 r->ShortPrint(&regions) + ".");
    }
}

void Game::RunBuildHelpers(ARegion *r)
{
    forlist((&r->objects)) {
	Object *obj = (Object *) elem;
	forlist ((&obj->units)) {
	    Unit *u = (Unit *) elem;
	    if (u->monthorders) {
		if (u->monthorders->type == O_BUILD) {
		    BuildOrder *o = (BuildOrder *)u->monthorders;
		    Object *tarobj = NULL;
		    if(o->target) {
			Unit *target = r->GetUnitId(o->target,u->faction->num);
			if(!target) {
			    u->Error("BUILD: No such unit to help.");
			    delete u->monthorders;
			    u->monthorders = 0;
			    continue;
			}
			// Make sure that unit is building
			if (target->monthorders &&
			    target->monthorders->type != O_BUILD) {
			    u->Error("BUILD: Unit isn't building.");
			    delete u->monthorders;
			    u->monthorders = 0;
			    continue;
			}
			// Make sure that unit considers you friendly!
			if(target->faction->GetAttitude(u->faction->num) <
			   A_FRIENDLY) {
			    u->Error("BUILD: Unit you are helping rejects "
				     "your help.");
			    delete u->monthorders;
			    u->monthorders = 0;
			    continue;
			}
			if(target->build > 0) {
			    u->build=target->build;
			    tarobj = r->GetObject(target->build);
			}
			else {
			    // help build ships
			    int ship = abs(target->build);
			    AString skname = ItemDefs[ship].pSkill;
			    int skill = LookupSkill(&skname);
			    int level = u->GetSkill(skill);
			    int needed = 0;
			    if((target->monthorders) && 
			       (target->monthorders->type == O_BUILD)) {
				BuildOrder *border = (BuildOrder *) target->monthorders;
				needed = border->needtocomplete;
			    }
			    int output = ShipConstruction(r, u, level, needed, ship);
			    if(output < 1) continue;
							
			    int unfinished = target->items.GetNum(ship);
			    // set up unfinished items
			    if(unfinished == 0) {
				unfinished = ItemDefs[ship].pMonths;
				target->items.SetNum(ship, unfinished);	
			    }
			    unfinished -= output;
							
			    // practice
			    u->Practice(skill);
							
			    if(unfinished > 0) {
				target->items.SetNum(ship, unfinished);
				if((target->monthorders) && 
				   (target->monthorders->type == O_BUILD)) {
				    BuildOrder *border = (BuildOrder *) target->monthorders;
				    border->needtocomplete -= output;
				    target->monthorders = border;
				}
			    } else {
				CreateShip(r, target, ship);
				if((target->monthorders) && 
				   (target->monthorders->type == O_BUILD)) {
				    BuildOrder *border = (BuildOrder *) target->monthorders;
				    border->needtocomplete = 0;
				    target->monthorders = border;
				}
			    } 
			    // hack: avoid that target starts
			    // building anew
			    if(unfinished == 0) unfinished = -1;
			    int percent = 100 * output / ItemDefs[ship].pMonths;
			    u->Event(AString("Helps ") +
				     *(target->name) + "  with construction of a " + 
				     ItemDefs[ship].name + " (" + percent + "%) in " +
				     r->ShortPrint(&regions) + ".");							
			}
			// no need to move unit if item-type ships
			// are being built. (leave this commented out)
			// if (tarobj == NULL) tarobj = target->object;
			if((tarobj != NULL) && (u->object != tarobj))
			    u->MoveUnit(tarobj);
		    } else {
			Object *buildobj = r->GetDummy();
			// don't move if finishing a building
			if ((u->object->type != O_DUMMY) && (u->object->type == u->build)) continue;
			if (u->build > 0) buildobj = r->GetObject(u->build);
			if (!buildobj) continue; // don't move into nonexistent objects
			if (buildobj != r->GetDummy() && buildobj != u->object)
			{
			    u->MoveUnit(buildobj);
			}
		    }
		}
	    }
	}
    }
}

/* Creates a new ship - either by adding it to a 
 * compatible fleet object or creating a new fleet
 * object with Unit u as owner consisting of exactly
 * ONE ship of the given type.
 */
void Game::CreateShip(ARegion *r, Unit * u, int ship)
{
    Object * obj = u->object;
    // Do we need to create a new fleet?
    int newfleet = 1;
    if(u->object->type == O_FLEET) {
	newfleet = 0;
	int flying = obj->flying;
	// are the fleets compatible?
	if((flying > 0) && (ItemDefs[ship].fly < 1)) newfleet = 1;
	if((flying < 1) && (ItemDefs[ship].fly > 0)) newfleet = 1;
    }
    if(newfleet != 0) {
	// create a new fleet
	Object * fleet = new Object(r);
	fleet->type = O_FLEET;
	fleet->num = shipseq++;
	fleet->name = new AString(AString("Ship [") + fleet->num + "]");
	fleet->AddShip(ship);
	u->object->region->objects.Add(fleet);
	u->MoveUnit(fleet);
    } else {
	obj->AddShip(ship);
    }
}

/* Checks and returns the amount of ship construction,
 * handles material use and practice for both the main
 * shipbuilders and the helpers.
 */
int Game::ShipConstruction(ARegion * r, Unit * u, int level, int needed, int ship)
{
    if (!TradeCheck(r, u->faction)) {
	u->Error("BUILD: Faction can't produce in that many regions.");
	delete u->monthorders;
	u->monthorders = 0;
	return 0;
    }

    if (level < ItemDefs[ship].pLevel) {
	u->Error("BUILD: Can't build that.");
	delete u->monthorders;
	u->monthorders = 0;
	return 0;
    }
		
    // are there unfinished ship items of the given type?
    int unfinished = u->items.GetNum(ship);

    int number = u->GetMen() * level + u->GetProductionBonus(ship);

    // find the max we can possibly produce based on man-months of labor
    int maxproduced;
    if (ItemDefs[ship].flags & ItemType::SKILLOUT)
	maxproduced = u->GetMen();
    else
	// don't adjust for pMonths
	// - pMonths represents total requirement
	maxproduced = number;
	
    // adjust maxproduced for items needed until completion
    if(needed < maxproduced) maxproduced = needed;
		
    // adjust maxproduced for unfinished ships
    if((unfinished > 0) && (maxproduced > unfinished))
	maxproduced = unfinished;

    if (ItemDefs[ship].flags & ItemType::ORINPUTS) {
	// Figure out the max we can produce based on the inputs
	int count = 0;
	unsigned int c;
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[ship].pInput[c].item;
	    if(i != -1)
		count += u->GetSharedNum(i) / ItemDefs[ship].pInput[c].amt;
	}
	if (maxproduced > count)
	    maxproduced = count;
	count = maxproduced;
		
	// no required materials?
	if(count < 1) {
	    u->Error("BUILD: Don't have the required materials.");
	    delete u->monthorders;
	    u->monthorders = 0;
	    return 0;
	}
		
	/* regional economic improvement */
	r->improvement += count;

	// Deduct the items spent
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[ship].pInput[c].item;
	    int a = ItemDefs[ship].pInput[c].amt;
	    if(i != -1) {
		int amt = u->GetSharedNum(i);
		if (count > amt / a) {
		    count -= amt / a;
		    u->ConsumeShared(i, (amt/a)*a);
		} else {
		    u->ConsumeShared(i, count * a);
		    count = 0;
		}
	    }
	}
    }
    else {
	// Figure out the max we can produce based on the inputs
	unsigned int c;
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[ship].pInput[c].item;
	    if(i != -1) {
		int amt = u->GetSharedNum(i);
		if(amt/ItemDefs[ship].pInput[c].amt < maxproduced) {
		    maxproduced = amt/ItemDefs[ship].pInput[c].amt;
		}
	    }
	}
		
	// no required materials?
	if(maxproduced < 1) {
	    u->Error("BUILD: Don't have the required materials.");
	    delete u->monthorders;
	    u->monthorders = 0;
	    return 0;
	}
		
	/* regional economic improvement */
	r->improvement += maxproduced;
		
	// Deduct the items spent
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[ship].pInput[c].item;
	    int a = ItemDefs[ship].pInput[c].amt;
	    if(i != -1) {
		u->ConsumeShared(i, maxproduced*a);
	    }
	}
    }
    int output = maxproduced * ItemDefs[ship].pOut;
    if (ItemDefs[ship].flags & ItemType::SKILLOUT)
	output *= level;

    delete u->monthorders;
    u->monthorders = 0;
	
    return output;
}

void Game::RunMonthOrders()
{
    forlist(&regions) {
	ARegion * r = (ARegion *) elem;
	RunIdleOrders(r);
	RunGambleOrders(r);
	RunStudyOrders(r);
	RunSettleOrders(r);
	RunBuildHelpers(r);
	RunProduceOrders(r);
    }
}

void Game::RunUnitProduce(ARegion * r, Object *obj, Unit * u)
{
    ProduceOrder * o = (ProduceOrder *) u->monthorders;

    if (o->item == I_SILVER) {
	u->Error("Can't do that in this region.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    int input = ItemDefs[o->item].pInput[0].item;
    if (input == -1) {
	u->Error("PRODUCE: Can't produce that, no input material.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    int level = u->GetSkill(o->skill);
    if (level < ItemDefs[o->item].pLevel) {
	u->Error("PRODUCE: Can't produce that, skill level too low.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    //check for building
    //BUSHIDO MOD
    int flags = ItemDefs[o->item].flags;
    if (flags & ItemType::NEEDBUILDING) {
	//get the building type
	int required = ItemDefs[o->item].pBuilding;
	int current = obj->type;

	//check current building vs required building
	if (required != current) {
	    AString building = ObjectDefs[required].name;
	    AString error1 = "PRODUCE: Must be in ";
	    AString error2 = " to be able to produce ";
	    AString error3 = ".";
	    AString itemName = ItemDefs[o->item].name;
	    u->Error(error1 + building + error2 + itemName + error3);
	    delete u->monthorders;
	    u->monthorders = 0;
	    return;
	}
    }
    //end BUSHIDO mod

    // LLS
    int number = u->GetMen() * level + u->GetProductionBonus(o->item);

    if (!TradeCheck(r, u->faction)) {
	u->Error("PRODUCE: Faction can't produce in that many regions.");
	delete u->monthorders;
	u->monthorders = 0;
	return;
    }

    // find the max we can possibly produce based on man-months of labor
    int maxproduced;
    if (ItemDefs[o->item].flags & ItemType::SKILLOUT)
	maxproduced = u->GetMen();
    else
	maxproduced = number/ItemDefs[o->item].pMonths;

    if (ItemDefs[o->item].flags & ItemType::ORINPUTS) {
	// Figure out the max we can produce based on the inputs
	int count = 0;
	unsigned int c;
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[o->item].pInput[c].item;
	    if(i != -1)
		count += u->GetSharedNum(i) / ItemDefs[o->item].pInput[c].amt;
	}
	if (maxproduced > count)
	    maxproduced = count;
	count = maxproduced;
		
	/* regional economic improvement */
	r->improvement += count;

	// Deduct the items spent
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[o->item].pInput[c].item;
	    int a = ItemDefs[o->item].pInput[c].amt;
	    if(i != -1) {
		int amt = u->GetSharedNum(i);
		if (count > amt / a) {
		    count -= amt / a;
		    u->ConsumeShared(i, (amt/a)*a);
		} else {
		    u->ConsumeShared(i, count * a);
		    count = 0;
		}
	    }
	}
    }
    else {
	// Figure out the max we can produce based on the inputs
	unsigned int c;
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[o->item].pInput[c].item;
	    if(i != -1) {
		int amt = u->GetSharedNum(i);
		if(amt/ItemDefs[o->item].pInput[c].amt < maxproduced) {
		    maxproduced = amt/ItemDefs[o->item].pInput[c].amt;
		}
	    }
	}
		
	/* regional economic improvement */
	r->improvement += maxproduced;
		
	// Deduct the items spent
	for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
	    int i = ItemDefs[o->item].pInput[c].item;
	    int a = ItemDefs[o->item].pInput[c].amt;
	    if(i != -1) {
		u->ConsumeShared(i, maxproduced*a);
	    }
	}
    }

    // Now give the items produced
    int output = maxproduced * ItemDefs[o->item].pOut;
    if (ItemDefs[o->item].flags & ItemType::SKILLOUT)
	output *= level;
    u->items.SetNum(o->item,u->items.GetNum(o->item) + output);
    u->Event(AString("Produces ") + ItemString(o->item,output) + " in " +
	     r->ShortPrint(&regions) + ".");
    u->Practice(o->skill);
    delete u->monthorders;
    u->monthorders = 0;
}

void Game::RunProduceOrders(ARegion * r)
{
    forlist ((&r->products))
	RunAProduction(r,(Production *) elem);
    {
	forlist((&r->objects)) {
	    Object * obj = (Object *) elem;
  
	    forlist ((&obj->units)) {
		Unit * u = (Unit *) elem;
		if (u->monthorders) {
		    if (u->monthorders->type == O_PRODUCE) {
			RunUnitProduce(r,obj,u);
		    } else {
			if (u->monthorders->type == O_BUILD) {
			    if(u->build > 0) {
				Run1BuildOrder(r,obj,u);
			    } else {
				RunBuildShipOrder(r,obj,u);
			    }
			}
		    }
		}
	    }
	}
    }
}

int Game::ValidProd(Unit * u,ARegion * r, Production * p)
{
    if (u->monthorders->type != O_PRODUCE) return 0;

    ProduceOrder * po = (ProduceOrder *) u->monthorders;
    if (p->itemtype == po->item && p->skill == po->skill) {
	if (p->skill == -1) {
	    /* Factor for fractional productivity: 10 */
	    po->productivity = (int) ((float) (u->GetMen() * p->productivity / 10));
	    return po->productivity;
	}
	int level = u->GetSkill(p->skill);
	//	if (level < p->level) {
	if (level < ItemDefs[p->itemtype].pLevel) {
	    u->Error("PRODUCE: Unit isn't skilled enough.");
	    delete u->monthorders;
	    u->monthorders = 0;
	    return 0;
	}

	//
	// Check faction limits on production. If the item is silver, then the
	// unit is entertaining or working, and the limit does not apply
	//
	if (p->itemtype != I_SILVER && !TradeCheck(r, u->faction)) {
	    u->Error("PRODUCE: Faction can't produce in that many regions.");
	    delete u->monthorders;
	    u->monthorders = 0;
	    return 0;
	}

	/* check for bonus production */
	// LLS
	int bonus = u->GetProductionBonus(p->itemtype);
	/* Factor for fractional productivity: 10 */
	po->productivity = (int) ((float) (u->GetMen() * level * p->productivity / 10)) + bonus;
	return po->productivity;
    }
    return 0;
}

int Game::FindAttemptedProd(ARegion * r,Production * p)
{
    int attempted = 0;
    forlist((&r->objects)) {
	Object * obj = (Object *) elem;
	forlist((&obj->units)) {
	    Unit * u = (Unit *) elem;
	    if ((u->monthorders) && (u->monthorders->type == O_PRODUCE)) {
		attempted += ValidProd(u,r,p);
	    }
	}
    }
    return attempted;
}

void Game::RunAProduction(ARegion * r,Production * p)
{
    p->activity = 0;
    if (p->amount == 0) return;

    /* First, see how many units are trying to work */
    int attempted = FindAttemptedProd(r,p);
    int amt = p->amount;
    if (attempted < amt) attempted = amt;
    forlist((&r->objects)) {
	Object * obj = (Object *) elem;
	forlist((&obj->units)) {
	    Unit * u = (Unit *) elem;
	    if(!u->monthorders || u->monthorders->type != O_PRODUCE)
	    {
		continue;
	    }

	    ProduceOrder * po = (ProduceOrder *) u->monthorders;
	    if (po->skill != p->skill || po->item != p->itemtype)
	    {
		continue;
	    }

	    /* We need to implement a hack to avoid overflowing */
	    int uatt, ubucks;

	    uatt = po->productivity;
	    if (uatt && amt && attempted)
	    {
		double dUbucks = ((double) amt) * ((double) uatt)
		    / ((double) attempted);
		ubucks = (int) dUbucks;
	    }
	    else
	    {
		ubucks = 0;
	    }

	    amt -= ubucks;
	    attempted -= uatt;
	    u->items.SetNum(po->item,u->items.GetNum(po->item)
			    + ubucks);
	    p->activity += ubucks;

	    /* Show in unit's events section */
	    if (po->item == I_SILVER)
	    {
		//
		// WORK
		//
		if (po->skill == -1)
		{
		    u->Event(AString("Earns ") + ubucks + " silver working in "
			     + r->ShortPrint(&regions) + ".");
		}
		else if (po->skill == S_ENTERTAINMENT)
		{
		    //
		    // ENTERTAIN
		    //
		    u->Event(AString("Earns ") + ubucks
			     + " silver entertaining in " +
			     r->ShortPrint(&regions)
			     + ".");
		    // If they don't have PHEN, then this will fail safely
		    //u->Practice(S_PHANTASMAL_ENTERTAINMENT);
		    u->Practice(S_ENTERTAINMENT);
		} else if (po->skill == S_GAMBLING) {
		  //GAMBLING
		  u->Event(AString("Collects ") + ubucks
			   + " silver as gambling proceeds in " +
			   r->ShortPrint(&regions)
			   + ".");
		  u->Practice(S_GAMBLING);
		}
	    }
	    else
	    {
		/* Everything else */
		u->Event(AString("Produces ") + ItemString(po->item,ubucks) +
			 " in " + r->ShortPrint(&regions) + ".");
		u->Practice(po->skill);
	    }
	    delete u->monthorders;
	    u->monthorders = 0;
	}
    }
}

void Game::RunStudyOrders(ARegion * r)
{
    forlist((&r->objects)) {
	Object * obj = (Object *) elem;
	forlist((&obj->units)) {
	    Unit * u = (Unit *) elem;
	    if (u->monthorders) {
		if (u->monthorders->type == O_STUDY) {
		    Do1StudyOrder(u,obj);
		    delete u->monthorders;
		    u->monthorders = 0;
		}
	    }
	}
    }
}

void Game::RunIdleOrders(ARegion *r)
{
    forlist((&r->objects)) {
	Object *obj = (Object *)elem;
	forlist((&obj->units)) {
	    Unit *u = (Unit *)elem;
	    if (u->monthorders && u->monthorders->type == O_IDLE) {
		u->Event("Sits idle.");
		delete u->monthorders;
		u->monthorders = 0;
	    }
	}
    }
}


/*
  void Game::Do1StudyOrder(Unit *u,Object *obj)
  {
  StudyOrder * o = (StudyOrder *) u->monthorders;
  int sk = o->skill;
  int cost = SkillCost(sk) * u->GetMen();
  int reset_man = -1;
  int apprentice_to_mage = -1;
  int normal_to_apprentice = -1;
  if (cost > u->GetSharedMoney()) {
  u->Error("STUDY: Not enough funds.");
  return;
  }

  // Check that the skill can be studied
  if (SkillDefs[sk].flags & SkillType::NOSTUDY) {
  u->Error( AString("STUDY: ") + AString(SkillDefs[sk].name) + " cannot be studied.");
  return;
  }
	
  // Small patch for Ceran Mercs
  if(u->GetMen(I_MERC)) {
  u->Error("STUDY: Mercenaries are not allowed to study.");
  return;
  }

  //patch for POON apprentices
  if(((SkillDefs[sk].flags & SkillType::MAGIC) && (u->type != U_MAGE))
  || ((SkillDefs[sk].flags & SkillType::APPRENTICE) && (u->type != U_APPRENTICE))) {
  //check for apprentice into mage condition
  if (Globals->APPRENTICE_MAGIC_LEVEL == Globals->ALL_MAGIC_LEVEL_ONE) {
  if (CountMages(u->faction) < AllowedMages(u->faction) && u->type == U_APPRENTICE && u->GetRealSkill(sk) > 0) {
  apprentice_to_mage = 1;
  u->Event("STUDY: Becomes a mage.");
  } else if ((CountApprentices(u->faction) < AllowedApprentices(u->faction)) && u->type == U_NORMAL) {
  normal_to_apprentice = 1;
  u->Event("STUDY: Becomes an apprentice.");
  } else if ((u->type == U_APPRENTICE) && u->GetRealSkill(sk) == 1) {
  u->Error("STUDY: An apprentice cannot be made into an mage.");
  return;
  } else if (u->type == U_NORMAL && CountApprentices(u->faction) >= AllowedApprentices(u->faction)) {
  u->Error("STUDY: Can't have another apprentice.");
  return;
  }
  } else {
  if(u->type == U_APPRENTICE && !SkillDefs[sk].flags&SkillType::APPRENTICE) {
  u->Error("STUDY: An apprentice cannot be made into an mage.");
  return;
  } 
  }
	  
  //check for mage into apprentice condition (MANI only?)
  if(u->type == U_MAGE && !SkillDefs[sk].flags & SkillType::MAGIC) {
  u->Error("STUDY: A mage cannot be made into an apprentice.");
  return;
  }
	  
  //check faction limits - mages and apprentices
  if(Globals->FACTION_LIMIT_TYPE != GameDefs::FACLIM_UNLIMITED) {
  if (u->type == U_MAGE) {
  if (CountMages(u->faction) >= AllowedMages(u->faction)) {
  u->Error("STUDY: Can't have another magician.");
  return;
  }
  } else if (u->type == U_APPRENTICE) {
  if (CountApprentices(u->faction) >= AllowedApprentices(u->faction)) {
  u->Error("STUDY: Can't have another apprentice.");
  return;
  }
  }
  }
	  
  //check for single man units
  if (u->GetMen() != 1) {
  u->Error("STUDY: Only 1-man units can be apprentices or magicians.");
  return;
  }
	  
  //check for non leaders
  if(!(Globals->MAGE_NONLEADERS)) {
  if (u->GetLeaders() != 1) {
  u->Error("STUDY: Only leaders may study magic.");
  return;
  }
  }

  //convert into mage or apprentice based on skill
  if (u->type == U_APPRENTICE && apprentice_to_mage == 1) {
  u->type = U_MAGE;
  reset_man = u->type;
  } else if (u->type == U_NORMAL && SkillDefs[sk].flags & SkillType::APPRENTICE) {
  u->type = U_APPRENTICE;
  reset_man = u->type;
  } else if (u->type == U_NORMAL && SkillDefs[sk].flags & SkillType::MAGIC) {
  if (Globals->APPRENTICE_MAGIC_LEVEL == Globals->ALL_MAGIC_LEVEL_ONE) {
  if (normal_to_apprentice == 1)
  u->type = U_APPRENTICE;
  } else {
  u->type = U_MAGE;
  }
  reset_man = u->type;
  } 
  }
  //END POON apprentice patch

  if ((Globals->TRANSPORT & GameDefs::ALLOW_TRANSPORT) &&
  (sk == S_QUARTERMASTER) && (u->GetSkill(S_QUARTERMASTER) == 0) &&
  (Globals->FACTION_LIMIT_TYPE == GameDefs::FACLIM_FACTION_TYPES)) {
  if (CountQuarterMasters(u->faction) >=
  AllowedQuarterMasters(u->faction)) {
  u->Error("STUDY: Can't have another quartermaster.");
  return;
  }
  if(u->GetMen() != 1) {
  u->Error("STUDY: Only 1-man units can be quartermasters.");
  return;
  }
  }
	
  // If TACTICS_NEEDS_WAR is enabled, and the unit is trying to study to tact-5,
  // check that there's still space...
  if (Globals->TACTICS_NEEDS_WAR && sk == S_TACTICS && 
  u->GetSkill(sk) == 4 && u->skills.GetDays(sk)/u->GetMen() >= 300) {
  if (CountTacticians(u->faction) >=
  AllowedTacticians(u->faction)) {
  u->Error("STUDY: Can't start another level 5 tactics leader.");
  return;
  }
  if (u->GetMen() != 1) {
  u->Error("STUDY: Only 1-man units can study to level 5 in tactics.");
  return;
  }
		
  } // end tactics check
	
  // If ASSASSINS_NEED_STEALTH is enabled, and the unit is trying to study to stea-5,
  // check that there's still space...
  if (Globals->ASSASSINS_NEED_STEALTH && sk == S_STEALTH && 
  u->GetSkill(sk) == 4 && u->skills.GetDays(sk)/u->GetMen() >= 300) {
  if (CountAssassins(u->faction) >=
  AllowedAssassins(u->faction)) {
  u->Error("STUDY: Can't start another level 5 stealth leader.");
  return;
  }
  if (u->GetMen() != 1) {
  u->Error("STUDY: Only 1-man units can study to level 5 in stealth.");
  return;
  }
	  
  } // end stealth check
	
  // adjust teaching for study rate
  int taughtdays = ((long int) o->days * u->skills.GetStudyRate(sk, u->GetMen()) / 30);

  int days = u->skills.GetStudyRate(sk, u->GetMen()) * u->GetMen() + taughtdays;

  if(((SkillDefs[sk].flags & SkillType::MAGIC)  || (SkillDefs[sk].flags & SkillType::APPRENTICE)) && u->GetSkill(sk) >= 2) {
  if(Globals->LIMITED_MAGES_PER_BUILDING) {
  if (obj->incomplete > 0 || obj->type == O_DUMMY) {
  u->Error("Warning: Magic study rate outside of a building "
  "cut in half above level 2.");
  days /= 2;
  } else if(obj->mages == 0) {
  u->Error("Warning: Magic rate cut in half above level 2 due "
  "to number of mages studying in structure.");
  days /= 2;
  } else {
  obj->mages--;
  }
  } else if(!(ObjectDefs[obj->type].protect) || (obj->incomplete > 0)) {
  u->Error("Warning: Magic study rate outside of a building cut in "
  "half above level 2.");
  days /= 2;
  }
  }

  if(SkillDefs[sk].flags & SkillType::SLOWSTUDY) {
  days /= 2;
  }

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
*/

void Game::RunMoveOrders()
{
    for (int phase = 0; phase < Globals->MAX_SPEED; phase++) {
	{
	    forlist((&regions)) {
		ARegion * region = (ARegion *) elem;
		forlist((&region->objects)) {
		    Object * obj = (Object *) elem;
		    forlist(&obj->units) {
			Unit * unit = (Unit *) elem;
			Object *tempobj = obj;
			DoMoveEnter(unit,region,&tempobj);
		    }
		}
	    }
	}

	AList * locs = new AList;
	forlist((&regions)) {
	    ARegion * region = (ARegion *) elem;
	    forlist((&region->objects)) {
		Object * obj = (Object *) elem;
		forlist(&obj->units) {
		    Unit * unit = (Unit *) elem;
		    if (phase == unit->movepoints && unit->monthorders &&
			(unit->monthorders->type == O_MOVE ||
			 unit->monthorders->type == O_ADVANCE) &&
			!unit->nomove) {
			locs->Add(DoAMoveOrder(unit,region,obj));
		    }
		}
	    }
	    /*
	      DoAdvanceAttacks(locs);
	      locs->DeleteAll();
	    */
	}
	DoAdvanceAttacks(locs);
	locs->DeleteAll();
    }
}


void Game::DoMoveEnter(Unit * unit,ARegion * region,Object **obj)
{
    MoveOrder * o;
    if (!unit->monthorders ||
	((unit->monthorders->type != O_MOVE) &&
	 (unit->monthorders->type != O_ADVANCE)))
	return;
    o = (MoveOrder *) unit->monthorders;

    while (o->dirs.Num()) {
	MoveDir * x = (MoveDir *) o->dirs.First();
	int i = x->dir;
	if (i != MOVE_OUT && i < MOVE_ENTER) return;
	o->dirs.Remove(x);
	delete x;

	if (i >= MOVE_ENTER) {
	    Object * to = region->GetObject(i - MOVE_ENTER);
	    if (!to) {
		unit->Error("MOVE: Can't find object.");
		continue;
	    }

	    if (!to->CanEnter(region,unit)) {
		unit->Error("ENTER: Can't enter that.");
		continue;
	    }

	    Unit *forbid = to->ForbiddenBy(region, unit);
	    if (forbid && !o->advancing) {
		unit->Error("ENTER: Is refused entry.");
		continue;
	    }

	    if(forbid && region->IsSafeRegion())
	    {
		unit->Error("ENTER: No battles allowed in safe regions.");
		continue;
	    }

	    if (forbid && !(unit->canattack && unit->IsAlive())) {
		unit->Error(AString("ENTER: Unable to attack ") +
			    *(forbid->name));
		continue;
	    }

	    int done = 0;
	    while (forbid)
	    {
		int result = RunBattle(region, unit, forbid, 0, 0);
		if(result == BATTLE_IMPOSSIBLE) {
		    unit->Error(AString("ENTER: Unable to attack ")+
				*(forbid->name));
		    done = 1;
		    break;
		}
		if (!unit->canattack || !unit->IsAlive()) {
		    done = 1;
		    break;
		}
		forbid = to->ForbiddenBy(region, unit);
	    }
	    if (done) continue;

	    unit->MoveUnit(to);
	    unit->Event(AString("Enters ") + *(to->name) + ".");
	    *obj = to;
	} else {
	    if (i == MOVE_OUT) {
		if(TerrainDefs[region->type].similar_type == R_OCEAN &&
		   (!unit->CanSwim() ||
		    unit->GetFlag(FLAG_NOCROSS_WATER)))
		{
		    unit->Error("MOVE: Can't leave ship.");
		    continue;
		}

		Object * to = region->GetDummy();
		unit->MoveUnit(to);
		*obj = to;
	    }
	}
    }
}

Location * Game::DoAMoveOrder(Unit * unit, ARegion * region, Object * obj)
{
    Location * loc = new Location;
    MoveOrder * o = (MoveOrder *) unit->monthorders;
    int movetype = unit->MoveType();
    AString road;

    if (unit->guard == GUARD_GUARD) unit->guard = GUARD_NONE;
    if (o->advancing) unit->guard = GUARD_ADVANCE;

    /* Ok, now we can move a region */
    if (o->dirs.Num()) {
	MoveDir * x = (MoveDir *) o->dirs.First();
	o->dirs.Remove(x);
	int i = x->dir;
	int startmove = 0;
	delete x;

	/* Setup region to move to */
	ARegion * newreg;
	if (i == MOVE_IN) {
	    if (obj->inner == -1) {
		unit->Error("MOVE: Can't move IN there.");
		goto done_moving;
	    }
	    newreg = regions.GetRegion(obj->inner);
	} else {
	    newreg = region->neighbors[i];
	}

	if (!newreg) {
	    unit->Error(AString("MOVE: Can't move that direction."));
	    goto done_moving;
	}

	if(region->type == R_NEXUS && newreg->IsStartingCity())
	    startmove = 1;

	if((TerrainDefs[region->type].similar_type == R_OCEAN) &&
	   (!unit->CanSwim() || unit->GetFlag(FLAG_NOCROSS_WATER))) {
	    unit->Error(AString("MOVE: Can't move while in the ocean."));
	    goto done_moving;
	}

	road = "";
	int cost = newreg->MoveCost(movetype, region, i, &road);

	if (region->type != R_NEXUS &&
	    unit->CalcMovePoints() - unit->movepoints < cost) {
	    if(unit->MoveType() == M_NONE) {
		unit->Error("MOVE: Unit is overloaded and cannot move.");
	    } else {
		unit->Error("MOVE: Unit has insufficient movement points;"
			    " remaining moves queued.");
		TurnOrder *tOrder = new TurnOrder;
		AString order;
		tOrder->repeating = 0;
		if (o->advancing) order = "ADVANCE ";
		else order = "MOVE ";
		if (i < NDIRS) order += DirectionAbrs[i];
		else if (i == MOVE_IN) order += "IN";
		else if (i == MOVE_OUT) order += "OUT";
		else order += i - MOVE_ENTER;
		forlist(&o->dirs) {
		    MoveDir *move = (MoveDir *) elem;
		    order += " ";
		    if (move->dir < NDIRS) order += DirectionAbrs[move->dir];
		    else if (move->dir == MOVE_IN) order += "IN";
		    else if (move->dir == MOVE_OUT) order += "OUT";
		    else order += move->dir - MOVE_ENTER;
		}
		tOrder->turnOrders.Add(new AString(order));
		unit->turnorders.Insert(tOrder);
	    }
	    goto done_moving;
	}

	if((TerrainDefs[newreg->type].similar_type == R_OCEAN) &&
	   (!unit->CanSwim() || unit->GetFlag(FLAG_NOCROSS_WATER))) {
	    unit->Event(AString("Discovers that ") +
			newreg->ShortPrint(&regions) + " is " +
			TerrainDefs[newreg->type].name + ".");
	    goto done_moving;
	}

	if (unit->type == U_WMON && newreg->town && newreg->IsGuarded()) {
	    unit->Event("Monsters don't move into guarded towns.");
	    goto done_moving;
	}

	if (unit->guard == GUARD_ADVANCE) {
	    Unit *ally = newreg->ForbiddenByAlly(unit);
	    if (ally && !startmove) {
		unit->Event(AString("Can't ADVANCE: ") + *(newreg->name) +
			    " is guarded by " + *(ally->name) + ", an ally.");
		goto done_moving;
	    }
	}

	Unit * forbid = newreg->Forbidden(unit);
	if (forbid && !startmove && unit->guard != GUARD_ADVANCE) {
	    int obs = unit->GetAttribute("observation");
	    unit->Event(AString("Is forbidden entry to ") +
			newreg->ShortPrint(&regions) + " by " +
			forbid->GetName(obs) + ".");
	    obs = forbid->GetAttribute("observation");
	    forbid->Event(AString("Forbids entry to ") +
			  unit->GetName(obs) + ".");
	    goto done_moving;
	}

	/* Clear the unit's alias out, so as not to interfere with TEACH */
	unit->alias = 0;

	unit->movepoints += cost;
	unit->MoveUnit(newreg->GetDummy());
	unit->DiscardUnfinishedShips();

	AString temp;
	switch (movetype) {
	    case M_WALK:
		temp = AString("Walks ") + road;
		if(TerrainDefs[newreg->type].similar_type == R_OCEAN)
		    temp = "Swims ";
		break;
	    case M_RIDE:
		temp = AString("Rides ") + road;
		break;
	    case M_FLY:
		temp = "Flies ";
		break;
	}
	unit->Event(temp + AString("from ") + region->ShortPrint(&regions)
		    + AString(" to ") + newreg->ShortPrint(&regions) +
		    AString("."));
	if (forbid) {
	    unit->advancefrom = region;
	}
	if(Globals->TRANSIT_REPORT != GameDefs::REPORT_NOTHING) {
	    // Update our visit record in the region we are leaving.
	    Farsight *f;
	    forlist(&region->passers) {
		f = (Farsight *)elem;
		if(f->unit == unit) {
		    // We moved into here this turn
		    if(i < MOVE_IN) {
			f->exits_used[i] = 1;
		    }
		}
	    }
	    // And mark the hex being entered
	    f = new Farsight;
	    f->faction = unit->faction;
	    f->level = 0;
	    f->unit = unit;
	    if(i < MOVE_IN) {
		f->exits_used[region->GetRealDirComp(i)] = 1;
	    }
	    newreg->passers.Add(f);
	}
	region = newreg;
    }

    loc->unit = unit;
    loc->region = region;
    loc->obj = obj;
    return loc;

 done_moving:
    delete o;
    unit->monthorders = 0;
    loc->unit = unit;
    loc->region = region;
    loc->obj = obj;
    return loc;
}




void Game::RunSettleOrders(ARegion *r)
{
    forlist((&r->objects)) {
	Object *obj = (Object *) elem;
	forlist((&obj->units)) {
	    Unit *u = (Unit *) elem;
	    if (u->monthorders) {
		if (u->monthorders->type == O_SETTLE) {
		    SettleOrder *s = (SettleOrder *)u->monthorders;
		    Do1SettleOrder(u,r,s->name);
		    delete u->monthorders;
		    u->monthorders = 0;
		}
	    }
	}
    }
}



void Game::RunGambleOrders(ARegion *r)
{
  int max = r->population;
  
  forlist((&r->objects)) {
    Object *obj = (Object *) elem;
    forlist((&obj->units)) {
      Unit *u = (Unit *) elem;
      if (u->monthorders) {
	if (u->monthorders->type == O_GAMBLE) {
	  GambleOrder *g = (GambleOrder *)u->monthorders;
	  Do1GambleOrder(u,obj,max);
	  delete u->monthorders;
	  u->monthorders = 0;
	}
      }
    }
  }
}

void Game::Do1GambleOrder(Unit *unit, Object *obj, int max) {

  int type = obj->type;
  int level = unit->GetSkill(S_GAMBLING);
  int pay = 0;

  if (type == O_GAMBLING_DEN && obj->incomplete > 0) {
    if (unit != obj->GetOwner()) {
      unit->Error("GAMBLE: Unit is not owner of the Gambling Den.");
    } else {
      if (level > 0) {
	pay = level * 1000;
	unit->items.SetNum(I_SILVER, unit->items.GetNum(I_SILVER)+pay);
	AString event = "Collects ";
	event += pay;
	event += " silver in gambling proceeds.";
	unit->Event(event);
      }
    }
  } else {
    unit->Error("GAMBLE: Unit must be owner of a Gambling Den.");
  }
}



void Game::Do1SettleOrder(Unit *unit, ARegion *region, AString *name) {

    //check to see if town exists
    if (region->town) {
	unit->Error("SETTLE: Existing settlement already here.");
	return;
    }
  
    //check to see if there are adjacent settlements within x hexes
    if (!GetNearestSettlementDistance(region)) {
	unit->Error("SETTLE: Cannot settle here as other settlements are nearby.");
	return;
    }
  
    //check to see if region contains the required structures
    if (!GetSettlementStructures(region)) {
	unit->Error("SETTLE: Missing required structures. A new settlement needs an INN and a TEMPLE.");
	return;
    }

    region->AddTown(TOWN_VILLAGE, name);
    unit->Event(AString("Creates new settlement in ") + region->ShortPrint(&regions) + AString("."));
}

      


  
int Game::GetSettlementStructures(ARegion *region) {
   
    int found_inn = 0;
    int found_temple = 0;

    //only one object - DUMMY object
    if (region->objects.Num() < 2)
	return 0;
  
    forlist(&region->objects) {
	Object * obj = (Object *)elem;
    
	if (obj->type == O_INN) {
	  //check to see if INN is complete
	  if (obj->incomplete < 1) {
	    found_inn = 1;
	  }
	} else if (obj->type == O_TEMPLE) {
	  //check to see if TEMPLE is complete
	  if (obj->incomplete < 1) {
	    found_temple = 1;
	  }
	}
    }

    if (found_inn == 1 && found_temple == 1)
	return 1;

    return 0;
}


int Game::GetNearestSettlementDistance(ARegion *region) {

  int x, y, z = 0;
  ARegion *reg;
  
  x = region->xloc;
  y = region->yloc;
  z = region->zloc;
  
  if (Globals->SETTLE_MIN_DISTANCE > 0) {
    
    ARegionArray *pArr = regions.GetRegionArray(z);
    
    int xMin = 0;
    int xMax = pArr->x;
    int yMin = 0;
    int yMax = pArr->y;
    
    for (int xx = xMin; xx < xMax; xx++) {
      for (int yy = yMin; yy < yMax; yy++) {
	
	// Get region
	reg = pArr->GetRegion(xx,yy);
	if (reg) {
	  if (reg->town != 0) {
	    if (regions.GetDistance(region, reg) < Globals->SETTLE_MIN_DISTANCE)
	      return 0;
	  }
	}
      }
    }
  }
  
  return 1;
}
