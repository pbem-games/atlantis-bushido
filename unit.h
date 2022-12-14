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
// Date        Person          Comments
// ----        ------          --------
// 2000/MAR/14 Larry Stanbery  Replaced specfic skill bonus functions with
//                             generic function.
//                             Added function to compute production bonus.
// 2001/FEB/07 Joseph Traub    Changes to allow mage support for city guards.
// 2001/Feb/18 Joseph Traub    Added support for Apprentices.
// 2001/Feb/25 Joseph Traub    Added a flag preventing units from crossing
//                             water.

#ifndef UNIT_CLASS
#define UNIT_CLASS

class Unit;
class UnitId;

#include "faction.h"
#include "alist.h"
#include "gameio.h"
#include "orders.h"
#include "fileio.h"
#include "skills.h"
#include "items.h"
#include "object.h"

enum {
	GUARD_NONE,
	GUARD_GUARD,
	GUARD_AVOID,
	GUARD_SET,
	GUARD_ADVANCE
};

enum {
	TAX_NONE,
	TAX_TAX,
	TAX_PILLAGE,
	TAX_RAZE,
	TAX_AUTO,
};

enum {
	REVEAL_NONE,
	REVEAL_UNIT,
	REVEAL_FACTION
};

enum {
	U_NORMAL,
	U_MAGE,
	U_GUARD,
	U_WMON,
	U_GUARDMAGE,
	U_APPRENTICE,
	U_BUSHIDO_RONIN,     //RONIN - NPC purchase unit
	U_BUSHIDO_SAMURAI = 0x100000,
	U_BUSHIDO_KUGE = 0x200000,
	U_BUSHIDO_NINJA = 0x400000,
	U_BUSHIDO_GEISHA = 0x800000,
	U_BUSHIDO_MERCHANT = 0x1000000,
	U_BUSHIDO_ARTISAN = 0x2000000,
	U_BUSHIDO_PRIEST = 0x4000000,
	U_BUSHIDO_SORCEROR = 0x8000000,
	NUNITTYPES
};

#define MAX_READY 4 // maximum number of ready weapons or armors

#define FLAG_BEHIND				0x0001
#define FLAG_NOCROSS_WATER		0x0002
#define FLAG_AUTOTAX			0x0004
#define FLAG_HOLDING			0x0008
#define FLAG_NOAID				0x0010
#define FLAG_INVIS				0x0020
#define FLAG_CONSUMING_UNIT		0x0040
#define FLAG_CONSUMING_FACTION	0x0080
#define FLAG_NOSPOILS			0x0100
#define FLAG_FLYSPOILS			0x0200
#define FLAG_WALKSPOILS			0x0400
#define FLAG_RIDESPOILS			0x0800
#define FLAG_SHARING			0x1000
#define FLAG_SEARCHING                  0x10000 //actively searching OBSE bonus
#define FLAG_CAMOUFLAGE                 0x20000 //actively hiding STEA bonus

class UnitId : public AListElem {
	public:
		UnitId();
		~UnitId();
		AString Print();

		int unitnum; /* if 0, it is a new unit */
		int alias;
		int faction;
};

class UnitPtr : public AListElem {
	public:
		Unit * ptr;
};
UnitPtr *GetUnitList(AList *, Unit *);

class Unit : public AListElem
{
	public:
		Unit();
		Unit(int,Faction *,int = 0);
		~Unit();

		void SetMonFlags();
		void MakeWMon(char *,int,int);

		void Writeout( Aoutfile *f );
		void Readin( Ainfile *f, AList *, ATL_VER v );

		AString SpoilsReport(void);
		int CanGetSpoil(Item *i);
		void WriteReport(Areport *,int,int,int,int, int);
		AString GetName(int);
		AString MageReport();
		AString ReadyItem();
		AString StudyableSkills();
		AString * BattleReport(int);
		AString TemplateReport();

		void ClearOrders();
		void ClearCastOrders();
		void DefaultOrders(Object *);
		void SetName(AString *);
		void SetDescribe(AString *);
		void PostTurn(ARegion *reg);

		int IsLeader();
		int IsNormal();
		int GetMons();
		int GetMen();
		int GetLeaders();
		int GetSoldiers();
		int GetMen(int);
		void SetMen(int,int);
		int GetMoney();
		void SetMoney(int);
		int GetSharedNum(int);
		void ConsumeShared(int,int);
		int GetSharedMoney();
		void ConsumeSharedMoney(int);
		int IsAlive();

		int MaintCost();
		void Short(int, int);
		int SkillLevels();
		void SkillStarvation();
		Skill *GetSkillObject(int);

		int GetAttackRiding();
		int GetDefenseRiding();

		//
		// These are rule-set specific, in extra.cpp.
		//
		// LLS
		int GetAttribute(char *ident);
		int PracticeAttribute(char *ident);
		int GetProductionBonus(int);

		int GetSkill(int);
		void SetSkill(int,int);
		void SetSkillDays(int,int);
		int GetSkillMax(int);
		int GetAvailSkill(int);
		int GetRealSkill(int);
		int GetRealSkillDays(int);
		void ForgetSkill(int);
		int CheckDepend(int,SkillDepend &s);
		int CanStudy(int);
		int Study(int,int); /* Returns 1 if it succeeds */
		int Practice(int);
		void AdjustSkills();

		/* Return 1 if can see, 2 if can see faction */
		int CanSee(ARegion *,Unit *, int practice = 0);
		int CanCatch(ARegion *,Unit *);
		int AmtsPreventCrime(Unit *);
		int GetAttitude(ARegion *,Unit *); /* Get this unit's attitude toward
											  the Unit parameter */
		int Hostile();
		int Forbids(ARegion *,Unit *);
		int Weight();
	        int FlyingCapacity();
	        int RidingCapacity();
	        int SwimmingCapacity();
	        int WalkingCapacity();
		int CanFly(int);
		int CanRide(int);
		int CanWalk(int);
		int CanFly();
		int CanSwim();
		int CanReallySwim();
		int MoveType();
		int CalcMovePoints();
		int CanMoveTo(ARegion *,ARegion *);
		int GetFlag(int);
		void SetFlag(int,int);
		void CopyFlags(Unit *);
		int GetBattleItem(AString &itm);
		int GetArmor(AString &itm, int ass);
		int GetMount(AString &itm, int canFly, int canRide, int &bonus, int assassination);
		int GetWeapon(AString &itm, int riding, int ridingBonus,
				int &attackBonus, int &defenseBonus, int &attacks, int assassination, int &ranged);
		int CanUseWeapon(WeaponType *pWep, int riding, int assassination);
		int CanUseWeapon(WeaponType *pWep, int assassination);
		int Taxers(int);

		void MoveUnit( Object *newobj );
		void DiscardUnfinishedShips();

		void Event(const AString &);
		void Error(const AString &);

		//BUSHIDO mods
		int GetForcedMarchBonus();
		int GetNinjaMovementType(int,int);
		int GetWaterWalkingBonus();
		int GetWildernessMovementBonus();

		Faction *faction;
		Faction *formfaction;
		Object *object;
		AString *name;
		AString *describe;
		int num;
		int type;
		int alias;
		int gm_alias; /* used for gm manual creation of new units */
		int guard; /* Also, avoid- see enum above */
		int reveal;
		int flags;
		int taxing;
		int movepoints;
		int canattack;
		int nomove;
		SkillList skills;
		ItemList items;
		int combat;
		int readyItem;
		int readyWeapon[MAX_READY];
		int readyArmor[MAX_READY];
		AList oldorders;
		int needed; /* For assessing maintenance */
		int hunger;
		int stomach_space;
		int losses;
		int free;
		int practiced; // Has this unit practiced a skill this turn
		int forcedMarchBonus; //Forced march bonus/check

		/* Orders */
		int destroy;
		int enter;
		int build;
		int leftShip;
		UnitId *promote;
		AList findorders;
		AList giveorders;
		AList withdraworders;
		AList bankorders;
		AList buyorders;
		AList sellorders;
		AList forgetorders;
		CastOrder *castorders;
		TeleportOrder *teleportorders;
		Order *stealorders;
		Order *monthorders;
		AttackOrder *attackorders;
		EvictOrder *evictorders;
		ARegion *advancefrom;

		AList exchangeorders;
		AList turnorders;
		int inTurnBlock;
		Order *presentMonthOrders;
		int presentTaxing;
		AList transportorders;
		AList distributeorders;
		AList settleorders;
		AList razeorders;
		Unit *former;

		//BUSHIDO mods
		int hits;
		int GetHits();
		int wilderness;
		int water;
};

#endif
