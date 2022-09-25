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

#ifndef RULES_H
#define RULES_H

//
// The items
//
enum {
	I_LEADERS,
	I_SAMURAI,
	I_NINJA,
	I_MERCHANT,
	I_SORCEROR,
	I_MAN,

	I_SILVER,

	I_GRAIN,
	I_LIVESTOCK,
	I_FISH,

	I_IRON,
	I_WOOD,
	I_STONE,
	I_FUR,
	I_HERBS,

	I_PEARLS,
	I_JEWELS,
	I_FIGURINES,
	I_WINE,
	I_SPICES,
	I_PAPER,
	I_CLOTH,
	I_GOLD,
	I_PERFUME,
	I_SILK,
	I_DYES,
	I_ROUGHGEMS,
	I_GEMS,

	I_STEEL,
	I_MUSHROOM,
	I_ADMANTIUM,
	I_MITHRIL,
	I_HARDWOOD,
	I_FLOATER,
	I_ROOTSTONE,
	I_YEW,

	I_HORSE,
	I_WARHORSE,
	I_WHORSE,
	I_DRAGON,

	//boats
	I_BARGE,
	//I_LONGSHIP,
	//I_WARGALLEY,
	//I_KNARR,
	//I_GALLEY,
	//I_TRIREME,
	//I_DROMON,
	//I_COG,
	//I_CARRACK,
	I_BALLOON,
	//I_ZEPPELIN,
	//I_CLOUDSHIP,

	//bushido boats
	I_JUNK,
	I_TREASURE_GALLEON,
	I_RED_SEAL_SHIP,
	I_ATAKEBUNE,
	I_TEKKOUSEN,
	
	I_WAGON,
	I_MAGIC_WAGON,

	I_SKELETON,
	I_UNDEAD,
	I_IMP,
	I_EAGLE,
	I_AMULETOFI,
	I_RINGOFI,
	I_CLOAKOFI,
	I_AMULETOFTS,
	I_AMULETOFP,
	I_SHIELDSTONE,
	I_MCARPET,
	I_PORTAL,
	//I_PEASANT,
	I_GLIDER,
	//I_BOOTS,
	I_PIRATES,
	I_KRAKEN,
	I_MERFOLK,
	I_ELEMENTAL,
	I_HYDRA,
	I_HEALPOTION,
	I_MWOLF,
	I_MSPIDER,
	I_MOLE,

	//Bushido weapons
	I_ONO,
	I_MASAKARI,
	I_BOSTAFF,
	I_JOSTAFF,
	I_HISHI,
	I_TANTO,
	I_KOZUKA,
	I_SAI,
	I_NINJATO,
	I_KATANA,
	I_WAKIZASHI,
	I_NO_DACHI,
	I_DAISHO,
	//I_KYOKUTSU_SHOGEI,
	//I_KUSARI_FUNDO,
	//I_KUSARI_GAMA,
	//I_KUSARI,
	//I_KAGINAWA,
	I_NUNCHAKU,
	I_NAGINATA,
	//I_KUMADE,
	//I_SHINOBI_ZUE,
	I_NAGIMAKI,
	I_YARI,
	I_DAI_KYU,
	I_HAN_KYU,
	I_OYUMI,
	I_SHURIKEN,

	//Bushido armour
	I_KIMONO,
	I_PADDED_ARMOUR,
	I_LEATHER_ARMOUR,
	I_SAMURAI_ARMOUR,
	I_ASHIGURU_ARMOUR,
	I_NINJA_GI,

	//Bushido masterwork weapons - IRWD/MITH
	I_SHURIKEN_MASTERWORK,
	I_KATANA_MASTERWORK,
	I_WAKIZASHI_MASTERWORK,
	I_NO_DACHI_MASTERWORK,
	I_DAISHO_MASTERWORK,
	I_TANTO_MASTERWORK,
	I_KOZUKA_MASTERWORK,
	I_NINJATO_MASTERWORK,
	I_SAI_MASTERWORK,
	I_DAI_KYU_MASTERWORK,
	I_OYUMI_MASTERWORK,

	//Bushido masterwork armour - IRWD/MITH
	I_SAMURAI_ARMOUR_MASTERWORK,
	I_ASHIGURU_ARMOUR_MASTERWORK,
	I_NINJA_GI_MASTERWORK,

	//Bushido magical weapons - YEW/ADMT
	I_SHURIKEN_MAGICAL,
	I_KATANA_MAGICAL,
	I_WAKIZASHI_MAGICAL,
	I_NO_DACHI_MAGICAL,
	I_DAISHO_MAGICAL,
	I_TANTO_MAGICAL,
	I_KOZUKA_MAGICAL,
	I_NINJATO_MAGICAL,
	I_SAI_MAGICAL,
	I_DAI_KYU_MAGICAL,
	I_OYUMI_MAGICAL,

	//Bushido magical armour - YEW/ADMT
	I_SAMURAI_ARMOUR_MAGICAL,
	I_ASHIGURU_ARMOUR_MAGICAL,
	I_NINJA_GI_MAGICAL,

	//BUSHIDO monsters
	I_WOLVES,
	I_BEARS,
	I_MINOR_KAMI,
	I_KAMI,
	I_MAJOR_KAMI,
	I_KAPPA,
	I_RATS,
	I_SHIKOME,
	I_OBAKEMONO,
	I_HOU_OU,
	I_PEASANT_RABBLE,
	I_ONI,
	I_DAI_ONI,
	I_KITSUNE,
	I_BAKEMONO_SHO,
	I_YAMATOKAGE,
	I_KUMO,
	I_WILDDOGS,
	I_YAMABUSHI,
	I_RONIN,
	I_SHUGENJA,
	I_PILGRIMS,
	I_BANDITS,
	I_GAKI,
	
	//BUSHIDO poisoned weapons
	I_NINJATO_POISONED,
	I_TANTO_POISONED,
	I_KOZUKA_POISONED,
	I_HISHI_POISONED,
	I_NINJATO_MASTERWORK_POISONED,
	I_KOZUKA_MASTERWORK_POISONED,
	I_TANTO_MASTERWORK_POISONED,
	I_NINJATO_MAGICAL_POISONED,
	I_KOZUKA_MAGICAL_POISONED,
	I_TANTO_MAGICAL_POISONED,
	I_SHURIKEN_MAGICAL_POISONED,
	I_SHURIKEN_MASTERWORK_POISONED,
	I_SHURIKEN_POISONED,

	//Major monster drop spoils
	I_DRAGON_BLOOD,
	I_DRAGON_CLAW,
	I_DRAGON_SCALES,
	I_GAKI_DUST,
	I_GAKI_FINGER,
	I_GAKI_ICHOR,
	I_KAMI_HAIR,
	I_KAMI_HEART,
	I_KAMI_RIB,
	I_ONI_BLOOD,
	I_ONI_HIDE,
	I_ONI_HORN,

	//Bushido items
	//arm
	I_JAZERAINT_ARMOUR,
	I_KIMONO_KAMI,
	I_DAI_ONI_ARMOUR,
	I_KIMONO_GAKI,

	//carm
	I_BANNER_COMMAND,
	I_STANDARD_FOUL_BLESS,
	I_STAFF_LIGHTNING,
	I_STAFF_FIRE,

	//jewel
	I_GEM_KAMI,
	I_EYE_KAMI,
	I_JEWEL_KAMI,

	//weap
	I_RUNEBLADES,
	I_TATSU_KATANA,
	I_BLADE_FLIGHT,
	I_NAGINATA_DEATH,
	I_KAMI_KATANA,
	I_PIERCING_NAGIMAKI,
	I_FIRE_BOW,
	NITEMS
};

//
// Types of skills.
//
enum {
	S_MINING,
	S_LUMBERJACK,
	S_QUARRYING,
	S_HUNTING,
	S_FISHING,
	S_HERBLORE,
	S_HORSETRAINING,
	S_WEAPONSMITH,
	S_ARMORER,
	S_CARPENTER,
	S_BUILDING,
	S_SHIPBUILDING,
	S_ENTERTAINMENT,
	S_TACTICS,
	S_COMBAT,
	S_RIDING,
	S_CROSSBOW,
	S_LONGBOW,
	S_STEALTH,
	S_OBSERVATION,
	S_HEALING,
	S_SAILING,
	S_FARMING,
	S_RANCHING,
	//S_FORCE,
	//S_PATTERN,
	//S_SPIRIT,
	//S_FIRE,
	//S_EARTHQUAKE,
	//S_FORCE_SHIELD,
	//S_ENERGY_SHIELD,
	//S_SPIRIT_SHIELD,
	//S_MAGICAL_HEALING,
	//S_GATE_LORE,
	//S_FARSIGHT,
	//S_TELEPORTATION,
	//S_PORTAL_LORE,
	//S_MIND_READING,
	//S_WEATHER_LORE,
	//S_SUMMON_WIND,
	//S_SUMMON_STORM,
	//S_SUMMON_TORNADO,
	//S_CALL_LIGHTNING,
	//S_CLEAR_SKIES,
	//S_EARTH_LORE,
	//S_WOLF_LORE,
	//S_BIRD_LORE,
	//S_DRAGON_LORE,
	//S_NECROMANCY,
	//S_SUMMON_SKELETONS,
	//S_RAISE_UNDEAD,
	//S_SUMMON_LICH,
	//S_CREATE_AURA_OF_FEAR,
	//S_SUMMON_BLACK_WIND,
	//	S_BANISH_UNDEAD,
	//S_DEMON_LORE,
	//S_SUMMON_IMPS,
	//S_SUMMON_DEMON,
	//S_SUMMON_BALROG,
	//S_BANISH_DEMONS,
	//S_ILLUSION,
	//S_PHANTASMAL_ENTERTAINMENT,
	//S_CREATE_PHANTASMAL_BEASTS,
	//S_CREATE_PHANTASMAL_UNDEAD,
	//S_CREATE_PHANTASMAL_DEMONS,
	//S_INVISIBILITY,
	//S_TRUE_SEEING,
	//S_DISPEL_ILLUSIONS,
	//S_ARTIFACT_LORE,
	//S_CREATE_RING_OF_INVISIBILITY,
	//S_CREATE_CLOAK_OF_INVULNERABILITY,
	//S_CREATE_STAFF_OF_FIRE,
	//S_CREATE_STAFF_OF_LIGHTNING,
	//S_CREATE_AMULET_OF_TRUE_SEEING,
	//S_CREATE_AMULET_OF_PROTECTION,
	//S_CREATE_RUNESWORD,
	//S_CREATE_SHIELDSTONE,
	//S_CREATE_MAGIC_CARPET,
	//S_ENGRAVE_RUNES_OF_WARDING,
	//S_CONSTRUCT_GATE,
	//S_ENCHANT_SWORDS,
	//S_ENCHANT_ARMOR,
	//S_CONSTRUCT_PORTAL,
	//S_MANIPULATE,
	// Skills for Ceran
	//S_WEAPONCRAFT,
	//S_ARMORCRAFT,
	//S_CAMELTRAINING,
	//S_GEMCUTTING,
	//S_MONSTERTRAINING,
	//S_CREATE_FLAMING_SWORD,

	//BUSHIDO Generic skills
	S_ENDURANCE,
	S_JEWELLER,
	S_FORCED_MARCH,

	//BUSHIDO CLASS STARTERS
	S_WAY_SAMURAI,
	S_WAY_NINJA,
	S_WAY_MERCHANT,
	S_WAY_SORCEROR,

	//BUSHIDO KUGE
	S_QUARTERMASTER,

	//BUSHIDO SAMURAI
	S_HAWKING,
	S_MULTIPLE_ATTACKS,
	S_DEFLECT_ARROWS,
	S_TWO_WEAPON_COMBO,
	S_CREATE_FEAR,
	S_RESIST_FEAR,
	
	//BUSHIDO MERCHANT
	S_CARAVANMASTER,
	S_COMMERCE,
	S_GAMBLING,
	S_TEA_CEREMONY,

        //BUSHIDO ARTISAN
	S_MASTER_ARMOUR,
	S_MASTER_WEAPON,
	S_MASTER_BUILDER,
	S_MASTER_CARPENTER,
	S_MASTER_SHIPBILDER,
	S_MASTER_JEWELLER,

	//BUSHIDO SORCEROR
	//CORE LORE
	S_DEMON_LORE,
	S_DRAGON_LORE,
	S_EARTH_LORE,
	S_FIRE_LORE,
	S_GATE_LORE,
	S_SHIELD_LORE,
	S_SPIRIT_LORE,
	S_UNDEAD_LORE,
	S_WATER_LORE,
	S_WEATHER_LORE,

	//miscellaneous
	S_INVISIBILITY,
	S_MAGICAL_HEALING,
	S_MIND_READING,
	S_TRUE_SEEING,

	//demon lore
	S_SUMMON_DEMON,
	S_BANISH_DEMONS,

	//dragon lore
	S_SUMMON_DRAGON,
	S_BANISH_DRAGONS,

	//earth lore
	S_EARTHQUAKE,

	//fire lore

	//gate lore
	S_FARSIGHT,
	S_TELEPORTATION,
	S_PORTAL_LORE,
	S_CONSTRUCT_GATE,
	S_CONSTRUCT_PORTAL,

	//shield lore
	S_ENERGY_SHIELD,
	S_FORCE_SHIELD,
	S_SPIRIT_SHIELD,
	S_ENGRAVE_RUNES_OF_WARDING,	

	//spirit lore
	S_BANISH_SPIRITS,
	S_SUMMON_SPIRITS,

	//undead lore
	S_BANISH_UNDEAD,
	S_SUMMON_UNDEAD,

	//water lore

	//weather lore
	S_SUMMON_WIND,
	S_SUMMON_STORM,
	S_SUMMON_TORNADO,
	S_CALL_LIGHTNING,
	S_CLEAR_SKIES,

	//hard core spells
	S_SUMMON_BLACK_WIND,	
	S_HELLFIRE,
	
	//BUSHIDO NINJA
	S_RESIST_ELEMENTS,
	S_WATER_WALKING,
	S_WILDERNESS_MOVEMENT,
	S_POISON_WEAPONS,
	S_THROWING_STAR,
	S_KEEN_EYES,
	S_CAMOUFLAGE,
	NSKILLS
};

//
// Types of objects.
//
enum {
	O_DUMMY,
	O_FLEET,
//	O_VEHICLE,
	O_TOWER,
	O_FORT,
	O_CASTLE,
	O_CITADEL,
	O_SHAFT,
	O_LAIR,
	O_RUIN,
	O_CAVE,
	O_DEMONPIT,
	O_CRYPT,
	O_MFORTRESS,
	O_MCITADEL,
	O_MINE,
	O_GOLDMINE,
	O_FARM,
	O_RANCH,
	O_TIMBERYARD,
	O_INN,
	O_QUARRY,
	// LLS
	// New ocean lairs - implement
	O_ISLE,
	O_DERELICT,
	O_OCAVE,
	O_WHIRL,
	// AS
	O_ROADN,
	O_ROADNW,
	O_ROADNE,
	O_ROADSW,
	O_ROADSE,
	O_ROADS,
	O_TEMPLE,
	O_MQUARRY,
	O_AMINE,
	O_PRESERVE,
	O_SACGROVE,
	// JT
	// Abyss Lair
	//O_BKEEP,
	//O_PALACE,
	// For Ceran
	//O_DCLIFFS,
	O_MTOWER,
	//O_HUT,
	O_STOCKADE,
	//O_CPALACE,
	//O_NGUILD,
	//O_AGUILD,
	//O_ATEMPLE,
	//O_HTOWER,
	// Tzargs monster lairs
	//O_MAGETOWER,
	//O_DARKTOWER,
	//O_GIANTCASTLE,
	//O_ILAIR,
	//O_ICECAVE,
	//O_BOG,
	//O_TRAPPINGHUT,
	O_STABLE,
	O_MSTABLE,
	O_TRAPPINGLODGE,
	O_FAERIERING,
	O_ALCHEMISTLAB,
	//O_OASIS,
	O_GEMAPPRAISER,
	//O_HPTOWER,
	//O_CARAVANSERAI,
	//Bushido objects
	O_FORGE,
	O_MARKETPLACE,
	O_MAGICAL_FORGE,
	O_GEISHA_HOUSE,
	O_RANCH_OR_FARM,
	O_RYU,
	O_NINJA_RYU,
	O_WATCHTOWER,
	O_GAMBLING_DEN,
	O_YAKUZA_HOUSE,
	NOBJECTS
};

//
// Types of terrain
//
/* ARegion Types */
enum {
	R_OCEAN,
	R_PLAIN,
	R_FOREST,
	R_MOUNTAIN,
	R_SWAMP,
	R_JUNGLE,
	R_DESERT,
	R_TUNDRA,
	R_CAVERN,
	R_UFOREST,
	R_TUNNELS,
	R_NEXUS,
	/*R_ISLAND_PLAIN,
	R_ISLAND_SWAMP,
	R_ISLAND_MOUNTAIN,
	R_CERAN_PLAIN1,
	R_CERAN_PLAIN2,
	R_CERAN_PLAIN3,
	R_CERAN_FOREST1,
	R_CERAN_FOREST2,
	R_CERAN_FOREST3,
	R_CERAN_MYSTFOREST,
	R_CERAN_MYSTFOREST1,
	R_CERAN_MYSTFOREST2,
	R_CERAN_MOUNTAIN1,
	R_CERAN_MOUNTAIN2,
	R_CERAN_MOUNTAIN3,*/
	R_CERAN_HILL,
	/*	R_CERAN_HILL1,
	R_CERAN_HILL2,
	R_CERAN_SWAMP1,
	R_CERAN_SWAMP2,
	R_CERAN_SWAMP3,
	R_CERAN_JUNGLE1,
	R_CERAN_JUNGLE2,
	R_CERAN_JUNGLE3,
	R_CERAN_DESERT1,
	R_CERAN_DESERT2,
	R_CERAN_DESERT3,
	R_CERAN_WASTELAND,
	R_CERAN_WASTELAND1,
	R_CERAN_LAKE,
	R_CERAN_TUNDRA1,
	R_CERAN_TUNDRA2,
	R_CERAN_TUNDRA3,
	R_CERAN_CAVERN1,
	R_CERAN_CAVERN2,
	R_CERAN_CAVERN3,
	R_CERAN_UFOREST1,
	R_CERAN_UFOREST2,
	R_CERAN_UFOREST3,
	R_CERAN_TUNNELS1,
	R_CERAN_TUNNELS2,*/
	R_GROTTO,
	R_DFOREST,
	R_CHASM,
	/*	R_CERAN_GROTTO1,
	R_CERAN_DFOREST1,
	R_CERAN_CHASM1,
	R_VOLCANO,*/
	R_LAKE,
	R_SPIRIT,
	R_NUM
};

#endif