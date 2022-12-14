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
#ifndef ITEMS_CLASS
#define ITEMS_CLASS

class Item;
class ItemType;

#include "fileio.h"
#include "gamedefs.h"
#include "alist.h"
#include "astring.h"

enum {
    ATTACK_COMBAT,
    ATTACK_ENERGY,
    ATTACK_SPIRIT,
    ATTACK_WEATHER,
    ATTACK_RIDING,
    ATTACK_RANGED,
    NUM_ATTACK_TYPES
};

enum {
    IT_NORMAL = 0x0001,
    IT_ADVANCED = 0x0002,
    IT_TRADE = 0x0004,
    IT_MAN = 0x0008,
    IT_MONSTER = 0x0010,
    IT_MAGIC = 0x0020,
    IT_WEAPON = 0x0040,
    IT_ARMOR = 0x0080,
    IT_MOUNT = 0x0100,
    IT_BATTLE = 0x0200,
    IT_SPECIAL = 0x0400,
    IT_TOOL = 0x0800,
    IT_FOOD = 0x1000,
    IT_ILLUSION = 0x2000,
    IT_UNDEAD = 0x4000,
    IT_DEMON = 0x8000,
    IT_LEADER = 0x10000,
    IT_MONEY = 0x20000,
    IT_ANIMAL = 0x40000,
    IT_SHIP = 0x80000,
    I_BUSHIDO_SAMURAI = 0x100000,
    I_BUSHIDO_NINJA = 0x200000,
    I_BUSHIDO_MERCHANT = 0x400000,
    I_BUSHIDO_SORCEROR = 0x800000,
    IT_SPIRIT = 0x1000000,
    IT_ARTIFACT = 0x2000000,
    IT_MONSTER_DROP = 0x4000000,
    IT_NOSPOIL = 0x8000000,
};

struct Materials
{
    int item;
    int amt;
};


class VehicleType
{

 public:
    char *name;
    char *names;
    char *abr;
    int flags;
};
    
class ItemType
{
 public:
    char *name;
    char *names;
    char *abr;

    enum {
	CANTGIVE = 0x1,
	DISABLED = 0x2,
	NOMARKET = 0x4,
	// This item requires ANY of its inputs, not ALL
	// of them
	ORINPUTS = 0x8,
	// A number of items are produced equal to the
	// producer's skill, based on a fixed number of
	// inputs
	SKILLOUT = 0x10,
	// This item cannot be transported.
	NOTRANSPORT = 0x20,
	//This item can only be produced in a building
	NEEDBUILDING = 0x40,
    };
    int flags;

    int pBuilding; //production building required - links to object type enum
    char *pSkill; // production skill
    int pLevel; // production skill level
    int pMonths; // Man months required for production
    int pOut; // How many of the item we get
    Materials pInput[8];

    char *mSkill; // magical production skill
    int mLevel; // magical production skill level
    int mOut; // How many of the item are conjured
    Materials mInput[4];

    //use level?
    char *uSkill;  //use skill
    int uLevel;  //use level

    int weight;
    int type;
    int baseprice;
    int combat;

    int walk;
    int ride;
    int fly;
    int swim;

    int hitchItem;
    int hitchwalk;
    // LLS
    int mult_item;
    int mult_val;

    int max_inventory;  // if non-zero, amount allowed in inventory.

    enum {
	// LOSE_LINKED only make sense with ESC_LEV_SQUARE or ESC_LEV_QUAD
	LOSE_LINKED = 0x01,		// All items of same type will be lost.
	// The rest of these are mutually exclusive
	HAS_SKILL = 0x02,		// Check skill, if exists at level, no loss
	ESC_LEV_SQUARE = 0x04,	// bottom of formula based on level square
	ESC_LEV_QUAD = 0x08,	// bottom of formula based on level quad
	LOSS_CHANCE = 0x10,		// flat chance of loss.
	ESC_LEV = 0x20,  //bottom of formula based on level
    };
    int escape;
    char *esc_skill;
    int esc_val; // level for has_skill, constant for all others

    char *grantSkill;
    char *fromSkills[4];
    int minGrant, maxGrant;
    
    //added for boats?
    //int defense[NUM_ATTACK_TYPES];
    //int maxMages;
};

extern ItemType *ItemDefs;

class ManType
{
 public:
    char *abbr;
    int terrain;
    int speciallevel;
    int defaultlevel;
    char *skills[6];
		
    int CanProduce(int);
    int CanUse(int);
};

extern ManType *ManDefs;

class MonType
{
 public:
    int attackLevel;
    int defense[NUM_ATTACK_TYPES];

    int numAttacks;
    int hits;
    int regen;

    int tactics;
    int stealth;
    int obs;

    char *special;
    int specialLevel;

    int silver;
    int spoiltype;
    int hostile; /* Percent */
    int number;
    char *name;
    char *abbr;
};

extern MonType *MonDefs;

enum {
    SLASHING,		// e.g. sword attack (This is default)
    PIERCING,		// e.g. spear or arrow attack
    CRUSHING,		// e.g. mace attack
    CLEAVING,		// e.g. axe attack
    ARMORPIERCING,		// e.g. crossbow double bow
    MAGIC_ENERGY,		// e.g. fire, dragon breath
    MAGIC_SPIRIT,		// e.g. black wind
    MAGIC_WEATHER,		// e.g. tornado
    NUM_WEAPON_CLASSES
};


class WeaponType
{
 public:
    char *abbr;

    enum {
	NEEDSKILL = 0x1, // No bonus or use unless skilled
	ALWAYSREADY = 0x2, // Ignore the 50% chance to attack
	NODEFENSE = 0x4, // No combat defense against this weapon
	NOFOOT = 0x8, // Weapon cannot be used on foot (e.g. lance)
	NOMOUNT = 0x10, // Weapon cannot be used mounted (e.g. pike)
	SHORT = 0x20, // Short melee weapon (e.g. shortsword, hatchet)
	LONG = 0x40, // Long melee weapon (e.g. lance, pike)
	RANGED = 0x80, // Missile weapon
	NOATTACKERSKILL = 0x100, // Attacker gets no combat/skill defense.
	RIDINGBONUS = 0x200, // Unit gets riding bonus on att and def.
	RIDINGBONUSDEFENSE = 0x400, // Unit gets riding bonus on def only.
	W_USEINASSASSINATE = 0x1000,
	W_BUSHIDO_SAMURAI = 0x100000,
	W_BUSHIDO_KUGE = 0x200000,
	W_BUSHIDO_NINJA = 0x400000,
	W_BUSHIDO_GEISHA = 0x800000,
	W_BUSHIDO_MERCHANT = 0x1000000,
	W_BUSHIDO_ARTISAN = 0x2000000,
	W_BUSHIDO_PRIEST = 0x4000000,
	W_BUSHIDO_SORCEROR = 0x8000000,
    };
    int flags;

    char *baseSkill;
    char *orSkill; //alternate skill

    int weapClass;
    int attackType;
    //
    // For numAttacks:
    // - A positive number is the number of attacks per round.
    // - A negative number is the number of rounds per attack.
    // - NUM_ATTACKS_HALF_SKILL indicates that the weapon gives as many
    //   attacks as the skill of the user divided by 2, rounded up.
    // - NUM_ATTACKS_HALF_SKILL+1 indicates that the weapon gives an extra
    //   attack above that, etc.
    // - NUM_ATTACKS_SKILL indicates the the weapon gives as many attacks
    //   as the skill of the user.
    // - NUM_ATTACKS_SKILL+1 indicates the the weapon gives as many
    //   attacks as the skill of the user + 1, etc.
    //
    enum {
	NUM_ATTACKS_HALF_SKILL = 50,
	NUM_ATTACKS_SKILL = 100,
    };
    int numAttacks;

    int attackBonus;
    int defenseBonus;
    int mountBonus;
};

extern WeaponType *WeaponDefs;

class ArmorType
{
 public:
    char *abbr;

    enum {
	USEINASSASSINATE = 0x1,
	A_BUSHIDO_SAMURAI = 0x100000,
	A_BUSHIDO_KUGE = 0x200000,
	A_BUSHIDO_NINJA = 0x400000,
	A_BUSHIDO_GEISHA = 0x800000,
	A_BUSHIDO_MERCHANT = 0x1000000,
	A_BUSHIDO_ARTISAN = 0x2000000,
	A_BUSHIDO_PRIEST = 0x4000000,
	A_BUSHIDO_SORCEROR = 0x8000000
    };

    int flags;
    //
    // Against attacks, the chance of the armor protecting the wearer
    // is: <type>Chance / from
    //
    int from;
    int saves[NUM_WEAPON_CLASSES];
};

extern ArmorType *ArmorDefs;

class MountType
{
 public:
    char *abbr;

    //
    // This is the skill needed to use this mount.
    //
    char *skill;

    //
    // This is the minimum bonus (and minimal skill level) for this mount.
    //
    int minBonus;

    //
    // This is the maximum bonus this mount will grant.
    //
    int maxBonus;

    //
    // This is the max bonus a mount will grant if it can normally fly
    // but the region doesn't allow flying mounts
    int maxHamperedBonus;

    // If the mount has a special effect it generates when ridden in
    // combat
    char *mountSpecial;
    int specialLev;
};

extern MountType *MountDefs;

class BattleItemType
{
 public:
    char *abbr;

    enum {
	MAGEONLY = 0x1,
	SPECIAL = 0x2,
	SHIELD = 0x4,
	EXCLUSIVE = 0x8,
	USEINASSASSINATE = 0x1000,
	B_BUSHIDO_SAMURAI = 0x100000,
	B_BUSHIDO_KUGE = 0x200000,
	B_BUSHIDO_NINJA = 0x400000,
	B_BUSHIDO_GEISHA = 0x800000,
	B_BUSHIDO_MERCHANT = 0x1000000,
	B_BUSHIDO_ARTISAN = 0x2000000,
	B_BUSHIDO_PRIEST = 0x4000000,
	B_BUSHIDO_SORCEROR = 0x8000000,
    };

    int flags;
    char *special;
    int skillLevel;
};

extern BattleItemType *BattleItemDefs;

extern int ParseGiveableItem(AString *);
extern int ParseAllItems(AString *);
extern int ParseEnabledItem(AString *);
extern int ParseTransportableItem(AString *);
extern int LookupItem(AString *);
extern int ParseTransportableItemKuge(AString *);

extern BattleItemType *FindBattleItem(char *abbr);
extern ArmorType *FindArmor(char *abbr);
extern WeaponType *FindWeapon(char *abbr);
extern MountType *FindMount(char *abbr);
extern MonType *FindMonster(char *abbr, int illusion);
extern ManType *FindRace(char *abbr);
extern AString AttType(int atype);

enum {
    FULLNUM = 0x01,
    ALWAYSPLURAL = 0x02
};
extern AString ItemString(int type, int num, int flags=0);
extern AString *ItemDescription(int item, int full);

extern int IsSoldier(int);
extern int IsMonster(int);

class Item : public AListElem
{
 public:
    Item();
    ~Item();

    void Readin(Ainfile *);
    void Writeout(Aoutfile *);
		
    AString Report(int);

    int type;
    int num;
    int selling;
    int checked; // flag whether item has been reported, counted etc.
};

class ItemList : public AList
{
 public:
    void Readin(Ainfile *);
    void Writeout(Aoutfile *);

    AString Report(int, int, int);
    AString BattleReport();
    AString ReportByType(int, int, int, int);

    int Weight();
    int GetNum(int);
    void SetNum(int, int); /* type, number */
    int CanSell(int);
    void Selling(int, int); /* type, number */
    void UncheckAll(); // re-set checked flag for all
};

extern AString ShowSpecial(char *special, int level, int expandLevel,
			   int fromItem);

#endif
