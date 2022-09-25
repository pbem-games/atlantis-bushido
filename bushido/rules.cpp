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

#include "gamedata.h"
#include "gamedefs.h"

//
// Define the various globals for this game.
//
// If you change any of these, it is incumbent on you, the GM to change
// the html file containing the rules to correctly reflect the changes!
//

//static int am[] = { 1, 1, 2, 3, 5, 8, 13 };
//int *allowedMages = am;
//int allowedMagesSize = sizeof(am) / sizeof(am[0]);

//static int aa[] = { 0, 2, 4, 6, 10, 14, 18 };
//int *allowedApprentices = aa;
//int allowedApprenticesSize = sizeof(aa) / sizeof(aa[0]);

static int aw[] = { 0, 4, 9, 15, 22, 30 };
int *allowedTaxes = aw;
int allowedTaxesSize = sizeof(aw) / sizeof(aw[0]);

static int at[] = { 0, 4, 9, 15, 22, 30 };
int *allowedTrades = at;
int allowedTradesSize = sizeof(at) / sizeof(at[0]);

//static int aq[] = { 0, 1, 2, 4, 6, 10, 15 };
//int *allowedQuartermasters = aq;
//int allowedQuartermastersSize = sizeof(aq) / sizeof(aq[0]);

//static int ag[] = { 0, 1, 2, 4, 6, 10, 15 };
//int *allowedTacticians = ag;
//int allowedTacticiansSize = sizeof(ag) / sizeof(ag[0]);

//static int as[] = { 0, 1, 2, 4, 6, 10, 15 };
//int *allowedAssassins = as;
//int allowedAssassinsSize = sizeof(as) / sizeof(as[0]);


//bushido modifications
static int war[] = { 0, 1, 2, 3, 5, 7 };
int *allowedWar = war;
int allowedWarSize = sizeof(war) / sizeof(war[0]);

static int stealth[] = { 0, 1, 2, 3, 5, 7 };
int *allowedStealth = stealth;
int allowedStealthSize = sizeof(stealth) / sizeof(stealth[0]);

static int trade[] = { 0, 2, 4, 6, 10, 14 };
int *allowedTrade = trade;
int allowedTradeSize = sizeof(trade) / sizeof(trade[0]);

static int magic[] = { 0, 1, 2, 3, 5, 7 };
int *allowedMagic = magic;
int allowedMagicSize = sizeof(magic) / sizeof(magic[0]);
//end bushido modifications

static GameDefs g = {
	"Way of the Sword",				// RULESET_NAME
	MAKE_ATL_VER( 0, 0, 1 ), // RULESET_VERSION

	2, /* FOOT_SPEED */
	4, /* HORSE_SPEED */
	4, /* SHIP_SPEED */
	6, /* FLY_SPEED */
	8, /* MAX_SPEED */

	10, /* STUDENTS_PER_TEACHER */
	10, /* MAINTENANCE_COST */
	20, /* LEADER_COST */

	20,  /* MAINTAINENCE_MULTIPLIER */
	GameDefs::MULT_LEADERS, /* MULTIPLIER_USE */

	33, /* STARVE_PERCENT */
	GameDefs::STARVE_MAGES, /* SKILL_STARVATION */

	20020, /* START_MONEY */
	2, /* WORK_FRACTION */
	20, /* ENTERTAIN_FRACTION */
	20, /* ENTERTAIN_INCOME */

	50, /* TAX_BASE_INCOME */
	10, /* TAX_BONUS_WEAPON */
	10, /* TAX_BONUS_ARMOR */
	30, /* TAX_BONUS_FORT */
	GameDefs::TAX_NORMAL, // WHO_CAN_TAX
	1,	// TAX_PILLAGE_MONTH_LONG

	10, /* HEALS_PER_MAN */

	10, /* GUARD_REGEN */ /* percent */
	250, /* CITY_GUARD */
	100, /* GUARD_MONEY */
	20000, /* CITY_POP */

	25, /* WMON_FREQUENCY */
	25, /* LAIR_FREQUENCY */

	5, /* FACTION_POINTS */

	100, /* TIMES_REWARD */

	0, // TOWNS_EXIST
	1, // LEADERS_EXIST
	1, // SKILL_LIMIT_NONLEADERS
	0, // MAGE_NONLEADERS
	0, // RACES_EXIST
	0, // GATES_EXIST
	1, // FOOD_ITEMS_EXIST
	1, // COASTAL_FISH
	1, // CITY_MONSTERS_EXIST
	1, // WANDERING_MONSTERS_EXIST
	1, // LAIR_MONSTERS_EXIST
	1, // WEATHER_EXISTS
	1, // OPEN_ENDED
	1, // NEXUS_EXISTS
	0, // CONQUEST_GAME

	1, // RANDOM_ECONOMY
	1, // VARIABLE_ECONOMY

	50, // CITY_MARKET_NORMAL_AMT
	20, // CITY_MARKET_ADVANCED_AMT
	50, // CITY_MARKET_TRADE_AMT
	20, // CITY_MARKET_MAGIC_AMT
	1,  // MORE_PROFITABLE_TRADE_GOODS

	50,	// BASE_MAN_COST
	0, // LASTORDERS_MAINTAINED_BY_SCRIPTS
	10, // MAX_INACTIVE_TURNS

	1, // EASIER_UNDERWORLD

	0, // DEFAULT_WORK_ORDER

	GameDefs::FACLIM_FACTION_TYPES, // FACTION_LIMIT_TYPE

	GameDefs::WFLIGHT_MUST_LAND,	// FLIGHT_OVER_WATER

	0,   // START_CITIES_EXIST
	0,   // SAFE_START_CITIES
	500, // AMT_START_CITY_GUARDS
	0,   // START_CITY_GUARDS_PLATE
	0,   // START_CITY_MAGES
	3,   // START_CITY_TACTICS
	0,   // APPRENTICES_EXIST

	"Atlantis", // WORLD_NAME

	0,  // NEXUS_GATE_OUT
	0,  // NEXUS_IS_CITY
	1,	// BATTLE_FACTION_INFO
	1,	// ALLOW_WITHDRAW
	0,	// CITY_RENAME_COST
	0,	// MULTI_HEX_NEXUS
	1,	// UNDERWORLD_LEVELS
	0,	// UNDERDEEP_LEVELS
	0,	// ABYSS_LEVEL
	0,      // SPIRIT LEVEL
	1,	  // TOWN_PROBABILITY
	0,	// TOWN_SPREAD
	0,	// TOWNS_NOT_ADJACENT
	0,	// LESS_ARCTIC_TOWNS
	30, // OCEAN
	20, // CONTINENT_SIZE
	3, // TERRAIN_GRANULARITY
	0, // LAKES
	16,	// ARCHIPELAGO
	60, // SEVER_LAND_BRIDGES
	15, // SEA_LIMIT	
	GameDefs::ALL, // LAKE_WAGE_EFFECT
	1,	// LAKESIDE_IS_COASTAL
	0,	// ODD_TERRAIN
	0,	// IMPROVED_FARSIGHT
	1,	// GM_REPORT
	0,	// DECAY
	1,	// LIMITED_MAGES_PER_BUILDING
	GameDefs::REPORT_NOTHING, // TRANSIT_REPORT
	1,  // MARKETS_SHOW_ADVANCED_ITEMS
	GameDefs::PREPARE_STRICT, // USE_PREPARE_COMMAND
	25,	// MONSTER_ADVANCE_MIN_PERCENT
	15,	// MONSTER_ADVANCE_HOSTILE_PERCENT
	1,	// HAVE_EMAIL_SPECIAL_COMMANDS
	0,	// START_CITIES_START_UNLIMITED
	1,	// PROPORTIONAL_AMTS_USAGE
	1,  // USE_WEAPON_ARMOR_COMMAND
	2,  // MONSTER_NO_SPOILS
	2,  // MONSTER_SPOILS_RECOVERY
	3,  // MAX_ASSASSIN_FREE_ATTACKS
	1,  // RELEASE_MONSTERS
	1,  // CHECK_MONSTER_CONTROL_MID_TURN
	1,  // DETECT_GATE_NUMBERS
	GameDefs::ARMY_ROUT_HITS_INDIVIDUAL,  // ARMY_ROUT
	1,  // ADVANCED_FORTS
	0,	// FULL_TRUESEEING_BONUS
	0,	// IMPROVED_AMTS
	1,	// FULL_INVIS_ON_SELF
	1,	// MONSTER_BATTLE_REGEN
	3,	// SKILL_PRACTICE_AMOUNT
	0, // REQUIRED_EXPERIENCE
	0,	// UPKEEP_MINIMUM_FOOD
	-1,	// UPKEEP_MAXIMUM_FOOD
	10,	// UPKEEP_FOOD_VALUE
	1,	// PREVENT_SAIL_THROUGH
	1,	// ALLOW_TRIVIAL_PORTAGE
	6,  // GATES_NOT_PERENNIAL
	1,  // START_GATES_OPEN
	1,  // SHOW_CLOSED_GATES
	7,	// TRANSPORT
	3,	// LOCAL_TRANSPORT
	7,	// NONLOCAL_TRANSPORT
	5,	// SHIPPING_COST
	500,	// FRACTIONAL_WEIGHT
	0, // GROW_RACES
	0,  // DYNAMIC_POPULATION
	100, // POP_GROWTH
	3, // DELAY_MORTALITY
	6, // DELAY_GROWTH
	1, // TOWN_DEVELOPMENT range 1-100
	1, //TACTICS_NEEDS_WAR
	0, // ALLIES_NOAID
	0, // HARDER_ASSASSINATION
	1, //ASSASSINS_NEED_STEALTH
	1, //ENABLE_ARTIFACTS
	0, //ILLU_FOR_APPRENTICE
	4, //MIN_MEN_RAZE_MULT
	4, //TAX_RAZE_MULT
	1, //INCREMENTAL_RAZE
	2, //MIN_MEN_PILLAGE_MULT
	2, //TAX_PILLAGE_MULT
	4, //SETTLE_MIN_DISTANCE
	1, //ALLOW_BUILDING_OBSE
};

GameDefs *Globals = &g;
