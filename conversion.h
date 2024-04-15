#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MAX_STRING_LENGTH 20480
#define MAX_INPUT_LENGTH 160
#define WORLD_SIZE 30000

#define ZONE_FILE "./lib/world.zon"
#define WORLD_FILE "./lib/world.wld"
#define MOB_FILE "./lib/world.mob"
#define OBJ_FILE "./lib/world.obj"

#define CREATE(result, type, number)                                           \
  do {                                                                         \
    if (!((result) = (type *)calloc((number), sizeof(type)))) {                \
      perror("malloc failure");                                                \
      abort( );                                                                \
    }                                                                          \
    memset(result, 0, sizeof(type));                                           \
  } while (0)

#define IS_SET(flag, bit) ((flag) & (bit))
#define SET_BIT(var, bit) ((var) = (var) | (bit))

/* For 'Sector types' */

#define SECT_INSIDE 0
#define SECT_CITY 1
#define SECT_FIELD 2
#define SECT_FOREST 3
#define SECT_HILLS 4
#define SECT_MOUNTAIN 5
#define SECT_WATER_SWIM 6
#define SECT_WATER_NOSWIM 7
#define SECT_AIR 8
#define SECT_UNDERWATER 9
#define SECT_DESERT 10
#define SECT_TREE 11

#define TELE_LOOK 1
#define TELE_COUNT 2
#define TELE_RANDOM 4
#define TELE_SPIN 8

/* Bitvector For 'room_flags' */

#define DARK 1
#define DEATH 2
#define NO_MOB 4
#define INDOORS 8
#define PEACEFUL 16 /* No fighting */
#define NOSTEAL 32  /* No Thieving */
#define NO_SUM 64   /* no summoning */
#define NO_MAGIC 128
#define TUNNEL 256 /* Limited #s of people in room */
#define PRIVATE 512
#define SILENCE 1024
#define LARGE 2048
#define NO_DEATH 4096
#define SAVE_ROOM 8192 /* room will save eq and load at reboot */

#define EX_ISDOOR 1
#define EX_CLOSED 2
#define EX_LOCKED 4
#define EX_SECRET 8
#define EX_RSLOCKED 16
#define EX_PICKPROOF 32
#define EX_CLIMB 64

typedef char sbyte;
typedef unsigned char ubyte;
typedef short int sh_int;
typedef unsigned short int ush_int;
// typedef char bool;
typedef char byte;

struct room_direction_data {
  char *general_description; /* When look DIR.                  */
  char *keyword;             /* for open/close                  */
  sh_int exit_info;          /* Exit info                       */
  int key;                   /* Key's number (-1 for no key)    */
  int to_room;               /* Where direction leeds (NOWHERE) */
};

struct extra_descr_data {
  char *keyword;                 /* Keyword in look/examine          */
  char *description;             /* What to see                      */
  struct extra_descr_data *next; /* Next in list                     */
};

struct room_data {
  sh_int number;    /* Rooms number                       */
  sh_int zone;      /* Room zone (for resetting)          */
  sh_int continent; /* Which continent/mega-zone          */
  int sector_type;  /* sector type (move/hide)            */

  int river_dir;   /* dir of flow on river               */
  int river_speed; /* speed of flow on river             */

  int tele_time;  /* time to a teleport                 */
  int tele_targ;  /* target room of a teleport          */
  char tele_mask; /* flags for use with teleport        */
  int tele_cnt;   /* countdown teleports                */

  unsigned char moblim; /* # of mobs allowed in room.         */

  char *name;        /* Rooms name 'You are ...'           */
  char *description; /* Shown when entered                 */
  struct extra_descr_data *ex_description;     /* for examine/look       */
  struct room_direction_data *dir_option[ 6 ]; /* Directions           */
  long room_flags; /* DEATH,DARK ... etc                 */
  byte light;      /* Number of lightsources in room     */
  byte dark;
  int (*funct)( ); /* special procedure                  */

  // struct obj_data *contents;   /* List of items in room              */
  // struct char_data *people;    /* List of NPC / PC in room           */

  // struct large_room_data *large;  /* special for large rooms         */
};

/* structure for the reset commands */
struct reset_com {
  char command; /* current command                      */
  bool if_flag; /* if TRUE: exe only if preceding exe'd */
  int arg1;     /*                                      */
  int arg2;     /* Arguments to the command             */
  int arg3;     /*                                      */

  /*
   *  Commands:              *
   *  'M': Read a mobile     *
   *  'O': Read an object    *
   *  'G': Give obj to mob   *
   *  'P': Put obj in obj    *
   *  'G': Obj to char       *
   *  'E': Obj to char equip *
   *  'D': Set state of door *
   */
};

struct zone_data {
  char *name;   /* name of this zone                  */
  int lifespan; /* how long between resets (minutes)  */
  int age;      /* current age of this zone (minutes) */
  int top;      /* upper limit for rooms in this zone */
  short start;  /* has this zone been reset yet?      */

  int reset_mode;        /* conditions for reset (see below)   */
  struct reset_com *cmd; /* command table for reset	           */
  char races[ 50 ];      /* races that are tolerated here */
  float CurrTotGold, LastTotGold;
  float CurrToPCGold, LastToPCGold;

  /*
   *  Reset mode:                              *
   *  0: Don't reset, and don't update age.    *
   *  1: Reset if no PC's are located in zone. *
   *  2: Just reset.                           *
   *  4: and onwards indicate different types  *
   *     of terrain and planar stuff. easier   *
   *     to stick here, than alter the form.   *
   */
};

extern int top_of_zone_table;
extern long top_of_world;
extern struct zone_data *zone_table;
extern struct room_data *room_db[ WORLD_SIZE ];
extern long room_count;

void boot_world(void);
void boot_zones(void);
char *fread_string(FILE *f1);

/* element in monster and object index-tables   */
struct index_data {
  int virtual_number; /* virtual number of this mob/obj           */
  long pos;           /* file position of this field              */
  int number;         /* number of existing units of this mob/obj	*/
  int (*func)( );     /* special procedure for this mob/obj       */
  char *name;
  char *short_desc;
  char *long_desc;
};

extern struct index_data *mob_index; /* index table for mobile file     */
extern struct index_data *obj_index; /* index table for object file     */

extern struct index_data *generate_indices(FILE *fl, int *top);
void boot_mobiles(void);

#define REAL 0
#define VIRTUAL 1

#define MAX_OBJ_AFFECT 5 /* Used in OBJ_FILE_ELEM *DO*NOT*CHANGE* */
#define APPLY_NONE 0
#define NOWHERE -1

#define ITEM_FIGURINE 8388608

#define ACT_FIGURINE (1 << 21)

#define ITEM_LIGHT 1
#define ITEM_SCROLL 2
#define ITEM_WAND 3
#define ITEM_STAFF 4
#define ITEM_WEAPON 5
#define ITEM_FIREWEAPON 6
#define ITEM_MISSILE 7
#define ITEM_TREASURE 8
#define ITEM_ARMOR 9
#define ITEM_POTION 10
#define ITEM_WORN 11
#define ITEM_OTHER 12
#define ITEM_TRASH 13
#define ITEM_TRAP 14
#define ITEM_CONTAINER 15
#define ITEM_NOTE 16
#define ITEM_DRINKCON 17
#define ITEM_KEY 18
#define ITEM_FOOD 19
#define ITEM_MONEY 20
#define ITEM_PEN 21
#define ITEM_BOAT 22
#define ITEM_AUDIO 23
#define ITEM_BOARD 24
#define ITEM_TREE 25
#define ITEM_ROCK 26
#define ITEM_WARPSTONE 27

#define ITEM_TAKE 1
#define ITEM_WEAR_FINGER 2
#define ITEM_WEAR_NECK 4
#define ITEM_WEAR_BODY 8
#define ITEM_WEAR_HEAD 16
#define ITEM_WEAR_LEGS 32
#define ITEM_WEAR_FEET 64
#define ITEM_WEAR_HANDS 128
#define ITEM_WEAR_ARMS 256
#define ITEM_WEAR_SHIELD 512
#define ITEM_WEAR_ABOUT 1024
#define ITEM_WEAR_WAISTE 2048
#define ITEM_WEAR_WRIST 4096
#define ITEM_WIELD 8192
#define ITEM_HOLD 16384
#define ITEM_THROW 32768

#define ITEM_GLOW 1
#define ITEM_HUM 2
#define ITEM_METAL 4    /* undefined...  */
#define ITEM_MINERAL 8  /* undefined?    */
#define ITEM_ORGANIC 16 /* undefined?    */
#define ITEM_INVISIBLE 32
#define ITEM_MAGIC 64
#define ITEM_NODROP 128
#define ITEM_BLESS 256
#define ITEM_ANTI_GOOD 512     /* not usable by good people    */
#define ITEM_ANTI_EVIL 1024    /* not usable by evil people    */
#define ITEM_ANTI_NEUTRAL 2048 /* not usable by neutral people */
#define ITEM_ANTI_CLERIC 4096
#define ITEM_ANTI_MAGE 8192
#define ITEM_ANTI_THIEF 16384
#define ITEM_ANTI_FIGHTER 32768
#define ITEM_BRITTLE 65536      /* weapons that break after 1 hit */
                                /* armor that breaks when hit?    */
#define ITEM_RESISTANT 131072   /* resistant to damage */
#define ITEM_ILLUSION 262144    /* Item is illusionary */
#define ITEM_ANTI_MEN 524288    /* men can't wield */
#define ITEM_ANTI_WOMEN 1048576 /* women can't wield */
#define ITEM_ANTI_SUN 2097152   /* item is sensitive to being in the sun */
#define ITEM_INSET 4194304      /* item has been inset w/another item */
#define ITEM_FIGURINE 8388608   /* Item is a figurine */

#define APPLY_NONE 0
#define APPLY_STR 1
#define APPLY_DEX 2
#define APPLY_INT 3
#define APPLY_WIS 4
#define APPLY_CON 5
#define APPLY_CHR 6
#define APPLY_SEX 7
#define APPLY_LEVEL 8
#define APPLY_AGE 9
#define APPLY_CHAR_WEIGHT 10
#define APPLY_CHAR_HEIGHT 11
#define APPLY_MANA 12
#define APPLY_HIT 13
#define APPLY_MOVE 14
#define APPLY_GOLD 15
#define APPLY_EXP 16
#define APPLY_AC 17
#define APPLY_ARMOR 17
#define APPLY_HITROLL 18
#define APPLY_DAMROLL 19
#define APPLY_SAVING_PARA 20
#define APPLY_SAVING_ROD 21
#define APPLY_SAVING_PETRI 22
#define APPLY_SAVING_BREATH 23
#define APPLY_SAVING_SPELL 24
#define APPLY_SAVE_ALL 25
#define APPLY_IMMUNE 26
#define APPLY_SUSC 27
#define APPLY_M_IMMUNE 28
#define APPLY_SPELL 29
#define APPLY_WEAPON_SPELL 30
#define APPLY_EAT_SPELL 31
#define APPLY_BACKSTAB 32
#define APPLY_KICK 33
#define APPLY_SNEAK 34
#define APPLY_HIDE 35
#define APPLY_BASH 36
#define APPLY_PICK 37
#define APPLY_STEAL 38
#define APPLY_TRACK 39
#define APPLY_HITNDAM 40
#define APPLY_SPELLFAIL 41
#define APPLY_ATTACKS 42
#define APPLY_HASTE 43
#define APPLY_SLOW 44
#define APPLY_BV2 45
#define APPLY_FIND_TRAPS 46
#define APPLY_RIDE 47
#define APPLY_RACE_SLAYER 48
#define APPLY_ALIGN_SLAYER 49
#define APPLY_MANA_REGEN 50
#define APPLY_HIT_REGEN 51
#define APPLY_MOVE_REGEN 52
#define APPLY_MOVE_BONUS 53

struct obj_flag_data {
  int value[ 4 ];   /* Values of the item (see list)    */
  byte type_flag;   /* Type of item                     */
  int wear_flags;   /* Where you can wear it            */
  long extra_flags; /* If it hums,glows etc             */
  int weight;       /* Weigt what else                  */
  int cost;         /* Value when sold (gp.)            */
  int cost_per_day; /* Cost to keep pr. real day        */
  int timer;        /* Timer for object                 */
  long bitvector;   /* To set chars bits                */
};

struct obj_affected_type {
  short location;         /* Which ability to change (APPLY_XXX) */
  unsigned long modifier; /* How much it changes by      */
};

struct obj_data {
  sh_int item_number;             /* Where in data-base             */
  int in_room;                    /* In what room -1 when conta/carr  */
  struct obj_flag_data obj_flags; /* Object information             */
  struct obj_affected_type
      affected[ MAX_OBJ_AFFECT ]; /* Which abilities in PC to change*/

  sh_int sector;                           /* for large rooms      */
  sh_int char_vnum;                        /* for ressurection     */
  long char_f_pos;                         /* for ressurection     */
  char *name;                              /* Title of object :get etc.      */
  char *description;                       /* When in room                   */
  char *short_description;                 /* when worn/carry/in cont.       */
  char *action_description;                /* What to write when used        */
  struct extra_descr_data *ex_description; /* extra descriptions   */
  struct char_data *carried_by;            /* Carried by :NULL in room/conta */
  byte eq_pos;                             /* what is the equip. pos?        */
  struct char_data *equipped_by;           /* equipped by :NULL in room/conta*/
  struct obj_data *in_obj;                 /* In what object NULL when none  */
  struct obj_data *contains;               /* Contains objects               */
  struct obj_data *next_content;           /* For 'contains' lists           */
  struct obj_data *next;                   /* For the object list            */
  struct char_data *link;                  /* Actual char linked for figurine*/
};

void boot_objects(void);

extern struct obj_data *object_list;
extern int top_of_objt;
struct obj_data *read_object(FILE *obj_f, int nr, int type);

void boot_mobiles(void);
extern int top_of_mobt;
struct char_data *read_mobile(FILE *mob_f, int nr, int type);
extern struct index_data *mob_index;

#define ACT_SPEC (1 << 0)       /* special routine to be called if exist   */
#define ACT_SENTINEL (1 << 1)   /* this mobile not to be moved             */
#define ACT_SCAVENGER (1 << 2)  /* pick up stuff lying around              */
#define ACT_ISNPC (1 << 3)      /* This bit is set for use with IS_NPC()   */
#define ACT_NICE_THIEF (1 << 4) /* Set if a thief should NOT be killed     */
#define ACT_AGGRESSIVE (1 << 5) /* Set if automatic attack on NPC's        */
#define ACT_STAY_ZONE (1 << 6)  /* MOB Must stay inside its own zone       */
#define ACT_WIMPY (1 << 7)      /* MOB Will flee when injured, and if      */
                                /* aggressive only attack sleeping players */
#define ACT_ANNOYING (1 << 8)   /* MOB is so utterly irritating that other */
                                /* monsters will attack it...              */
#define ACT_HATEFUL (1 << 9)    /* MOB will attack a PC or NPC matching a  */
                                /* specified name                          */
#define ACT_AFRAID (1 << 10)    /* MOB is afraid of a certain PC or NPC,   */
                                /* and will always run away ....           */
#define ACT_IMMORTAL (1 << 11)  /* MOB is a natural event, can't be kiled  */
#define ACT_HUNTING (1 << 12)   /* MOB is hunting someone                  */
#define ACT_DEADLY (1 << 13)    /* MOB has deadly poison                   */
#define ACT_POLYSELF (1 << 14)  /* MOB is a polymorphed person             */
#define ACT_META_AGG (1 << 15)  /* MOB is _very_ aggressive                */
#define ACT_GUARDIAN (1 << 16)  /* MOB will guard master                   */
#define ACT_ILLUSION (1 << 17)  /* MOB is illusionary                      */
#define ACT_HUGE (1 << 18)      /* MOB is too large to go indoors          */
#define ACT_SCRIPT (1 << 19)    /* MOB has a script assigned to it DO NOT SET */
#define ACT_GREET (1 << 20)     /* MOB greets people */
#define ACT_FIGURINE (1 << 21)  /* MOB is a figurine */
#define ACT_BRIEF (1 << 22)     /* MOB is in brief mode (polys) */
#define ACT_SHOWEXITS (1 << 23) /* MOB (poly) will see exits when looking */

/* 'class' for PC's */
#define CLASS_MAGIC_USER 1
#define CLASS_CLERIC 2
#define CLASS_WARRIOR 4
#define CLASS_THIEF 8
#define CLASS_DRUID 16
#define CLASS_MONK 32
#define CLASS_ALL 63

/* index values for classes having skills that require a minimum level  */
/* attained to learn                                                    */
#define MIN_LEVEL_MAGIC 0
#define MIN_LEVEL_CLERIC 1
#define MIN_LEVEL_DRUID 2
#define MIN_LEVEL_NUM 3 /* number of such indexes required         */

/* sex */
#define SEX_NEUTRAL 0
#define SEX_MALE 1
#define SEX_FEMALE 2

/* positions */
#define POSITION_DEAD 0
#define POSITION_MORTALLYW 1
#define POSITION_INCAP 2
#define POSITION_STUNNED 3
#define POSITION_SLEEPING 4
#define POSITION_RESTING 5
#define POSITION_SITTING 6
#define POSITION_FIGHTING 7
#define POSITION_STANDING 8
#define POSITION_MOUNTED 9

#define MAX_TONGUE 3
#define MAX_SKILLS 400 /* Used in CHAR_FILE_U *DO*NOT*CHANGE* */
#define MAX_WEAR 18
#define MAX_AFFECT 25 /* Used in CHAR_FILE_U *DO*NOT*CHANGE* */

struct last_checked {
  int mana;
  int mmana;
  int hit;
  int mhit;
  int move;
  int mmove;
  int exp;
  int gold;
};

typedef struct alias_type {
  char *com[ 10 ]; /* 10 aliases */
} Alias;

typedef struct {
  struct char_list *clist;
  int sex;        /*number 1=male,2=female,3=both,4=neut,5=m&n,6=f&n,7=all*/
  int race;       /*number */
  int class_type; /* 1=m,2=c,4=f,8=t */
  int vnum;       /* # */
  int evil;       /* align < evil = attack */
  int good;       /* align > good = attack */
} Opinion;

/* These data contain information about a players time data */
struct time_data {
  time_t birth; /* This represents the characters age                */
  time_t logon; /* Time of the last logon (used to calculate played) */
  int played;   /* This is the total accumulated time played in secs */
};

struct char_player_data {

  byte sex;                 /* PC / NPC s sex                       */
  short weight;             /* PC / NPC s weight                    */
  short height;             /* PC / NPC s height                    */
  bool talks[ MAX_TONGUE ]; /* PC s Tounges 0 for NPC           */

  char *name;         /* PC / NPC s name (kill ...  )         */
  char *short_descr;  /* for 'actions'                        */
  char *long_descr;   /* for 'look'.. Only here for testing   */
  char *description;  /* Extra descriptions                   */
  char *title;        /* PC / NPC s title                     */
  char *sounds;       /* Sound that the monster makes (in room) */
  char *distant_snds; /* Sound that the monster makes (other) */

  long class_type;       /* PC s class or NPC alignment          */
  int hometown;          /* PC s Hometown (zone)                 */
  long extra_flags;      /* ressurection, etc */
  struct time_data time; /* PC s AGE in days                  */
  byte level[ 8 ];       /* PC / NPC s level                     */
};

struct char_ability_data {
  sbyte str;
  sbyte str_add; /* 000 - 100 if strength 18             */
  sbyte intel;
  sbyte wis;
  sbyte dex;
  sbyte con;
  sbyte chr;
};

/* Used in CHAR_FILE_U *DO*NOT*CHANGE* */
struct char_point_data {
  sh_int mana;
  sh_int max_mana;
  byte mana_gain;

  sh_int hit;
  sh_int max_hit; /* Max hit for NPC                         */
  byte hit_gain;

  sh_int move;
  sh_int max_move; /* Max move for NPC                        */
  byte move_gain;

  sh_int armor; /* Internal -100..100, external -10..10 AC */
  int gold;     /* Money carried                           */
  int bankgold; /* gold in the bank.                       */
  int exp;      /* The experience of the player            */

  sbyte hitroll; /* Any bonus or penalty to the hit roll    */
  sbyte damroll; /* Any bonus or penalty to the damage roll */
};

struct char_special_data {
  byte spellfail;       /* max # for spell failure (101) */
  byte tick;            /* the tick that the mob/player is on  */
  byte pmask;           /* poof mask                           */
  byte position;        /* Standing or ...                        */
  byte default_pos;     /* Default position for NPC              */
  byte spells_to_learn; /* How many can you learn yet this level*/
  byte carry_items;     /* Number of items carried              */
  byte damnodice;       /* The number of damage dice's         */
  byte damsizedice;     /* The size of the damage dice's       */
  byte last_direction;  /* The last direction the monster went */
  char sev;             /* log severety level for gods */
  byte move_cost;       /* bonus or subtraction from movement cost */

  int start_room;        /* so people can be set to start certain places */
  int edit;              /* edit state */
  sbyte conditions[ 3 ]; /* Drunk full etc.                     */
  int prompt;            /* And what kinda prompt will you be having? */
  int permissions;       /* What zone are they allowed to edit? */
  int zone;              /* zone that an NPC lives in */
  int carry_weight;      /* Carried weight                       */
  int timer;             /* Timer for update                     */
  int was_in_room;       /* storage of location for linkdead people */
  int attack_type;       /* The Attack Type Bitvector for NPC's */
  int alignment;         /* +-1000 for alignments               */
  int pct;               /* For storing Wimpyness Percentage */
  int flee;              /* How many rooms do you want to run? */
  int quest;             /* which quest have we assigned this fool */
  bool loot;             /* Autoloot */
  bool split;            /* Autosplit */
  bool bitten;           /* vamp feeding on them? */
  char *poofin;
  char *poofout;

  Alias *A_list;
  struct char_data *misc;
  struct char_data *fighting; /* Opponent                          */

  struct char_data *hunting; /* Hunting person..                  */

  struct char_data *ridden_by;
  struct char_data *mounted_on;

  long affected_by;  /* Bitvector for spells/skills affected by    */
  long affected_by2; /* Other special things                       */

  long intrinsics; /* certain things have permanent  */
  /* unalienable abilities (infra etc) */
  /*   long intrinsics2; -> affected_by2 has no intrinsics in it, yet */

  unsigned long act; /* flags for NPC behavior               */

  sh_int apply_saving_throw[ 5 ]; /* Saving throw (Bonuses)         */
};

struct char_skill_data {
  byte learned; /* % chance for success 0 = not learned   */
  byte flags;
};

struct char_data {
  sh_int nr;   /* monster nr (pos in file) */
  int in_room; /* Location                    */

  int term;
  int size;
  struct last_checked last; /* For displays                */
  unsigned immune;          /* Immunities                  */
  unsigned M_immune;        /* Meta Immunities             */
  unsigned susc;            /* susceptibilities            */
  float mult_att;           /* the number of attacks      */
  byte attackers;
  byte sector;   /* which part of a large room
                    am i in?  */
  int generic;   /* generic int */
  int commandp;  /* command poitner for scripts */
  int waitp;     /* waitp for scripts           */
  int commandp2; /* place-holder for gosubs, etc. */
  int script;

  sh_int race;
  sh_int hunt_dist; /* max dist the player can hunt */

  unsigned short hatefield;
  unsigned short fearfield;

  Opinion hates;
  Opinion fears;

  sh_int persist;
  int old_room;

  void *act_ptr; /* numeric argument for the mobile
                               actions */

  struct char_player_data player;        /* Normal data            */
  struct char_ability_data abilities;    /* Abilities              */
  struct char_ability_data tmpabilities; /* The abilities we use  */
  struct char_point_data points;         /* Points                 */
  struct char_special_data specials;     /* Special plaing constant */
  struct char_skill_data *skills;        /* Skills                */

  struct affected_type *affected;         /* affected by what spells */
  struct obj_data *equipment[ MAX_WEAR ]; /* Equipment array         */

  struct obj_data *carrying;    /* Head of list            */
  struct descriptor_data *desc; /* NULL for mobiles        */
  struct char_data *orig;       /* Special for polymorph   */

  struct char_data *next_in_room;  /* For room->people - list   */
  struct char_data *next;          /* all in game list  */
  struct char_data *next_fighting; /* For fighting list         */

  struct follow_type *followers; /* List of chars followers */
  struct char_data *master;      /* Who is char following?  */
  int invis_level;               /* visibility of gods */

  struct obj_data *link; /* Link for figurines */
};

#define GET_AC(ch) ((ch)->points.armor)
#define GET_HIT(ch) ((ch)->points.hit)
#define GET_MAX_HIT(ch) (hit_limit(ch))
#define GET_MOVE(ch) ((ch)->points.move)
#define GET_MAX_MOVE(ch) (move_limit(ch))
#define GET_MANA(ch) ((ch)->points.mana)
#define GET_MAX_MANA(ch) (mana_limit(ch))
#define GET_GOLD(ch) ((ch)->points.gold)
#define GET_BANK(ch) ((ch)->points.bankgold)
#define GET_ZONE(ch) ((ch)->specials.permissions)
#define GET_EXP(ch) ((ch)->points.exp)
#define GET_HEIGHT(ch) ((ch)->player.height)
#define GET_WEIGHT(ch) ((ch)->player.weight)
#define GET_SEX(ch) ((ch)->player.sex)
#define GET_RACE(ch) ((ch)->race)
#define GET_HITROLL(ch) ((ch)->points.hitroll)
#define GET_DAMROLL(ch) ((ch)->points.damroll)
#define GET_COND(ch, i) ((ch)->specials.conditions[ (i) ])

#define GET_POS(ch) ((ch)->specials.position)
#define GET_NAME(ch) ((ch)->player.name)
#define GET_TITLE(ch) ((ch)->player.title)
#define GET_CLASS(ch) ((ch)->player.class)
#define GET_HOME(ch) ((ch)->player.hometown)
#define GET_AGE(ch) (age(ch).year)
#define GET_STR(ch) ((ch)->tmpabilities.str)
#define GET_ADD(ch) ((ch)->tmpabilities.str_add)
#define GET_DEX(ch) ((ch)->tmpabilities.dex)
#define GET_INT(ch) ((ch)->tmpabilities.intel)
#define GET_WIS(ch) ((ch)->tmpabilities.wis)
#define GET_CON(ch) ((ch)->tmpabilities.con)
#define GET_CHR(ch) ((ch)->tmpabilities.chr)

#define MAX_CLASS 6

#define MAGE_LEVEL_IND 0
#define CLERIC_LEVEL_IND 1
#define WARRIOR_LEVEL_IND 2
#define THIEF_LEVEL_IND 3
#define DRUID_LEVEL_IND 4
#define MONK_LEVEL_IND 5

#define GET_LEVEL(ch, i) ((ch)->player.level[ (i) ])

#define RACE_HALFBREED 0
#define RACE_HUMAN 1
#define RACE_ELVEN 2
#define RACE_DWARF 3
#define RACE_HALFLING 4
#define RACE_GNOME 5

/* end of old player races */

#define RACE_REPTILE 6
#define RACE_SPECIAL 7
#define RACE_LYCANTH 8
#define RACE_DRAGON 9
#define RACE_UNDEAD 10
#define RACE_ORC 11
#define RACE_INSECT 12
#define RACE_ARACHNID 13
#define RACE_DINOSAUR 14
#define RACE_FISH 15
#define RACE_BIRD 16
#define RACE_GIANT 17
#define RACE_PREDATOR 18
#define RACE_PARASITE 19
#define RACE_SLIME 20
#define RACE_DEMON 21
#define RACE_SNAKE 22
#define RACE_HERBIV 23
#define RACE_TREE 24
#define RACE_VEGGIE 25
#define RACE_ELEMENT 26
#define RACE_PLANAR 27
#define RACE_DEVIL 28
#define RACE_GHOST 29
#define RACE_GOBLIN 30
#define RACE_TROLL 31
#define RACE_VEGMAN 32
#define RACE_MFLAYER 33
#define RACE_PRIMATE 34
#define RACE_ENFAN 35
#define RACE_DROW 36
#define RACE_GOLEM 37
#define RACE_SKEXIE 38
#define RACE_TROGMAN 39
#define RACE_PATRYN 40
#define RACE_LABRAT 41
#define RACE_SARTAN 42
#define RACE_TYTAN 43
#define RACE_SMURF 44
#define RACE_ROO 45
#define RACE_HORSE 46
#define RACE_DRAAGDIM 47
#define RACE_ASTRAL 48
#define RACE_GOD 49
#define RACE_HALFELF 50
#define RACE_HALFORC 51
#define RACE_HUMANTWO 52
#define RACE_VAMPIRE 53
#define RACE_OGRE 54
#define RACE_FAERIE 55

#define MAX_RACE 55

#define AFF_BLIND 0x00000001
#define AFF_INVISIBLE 0x00000002
#define AFF_DETECT_EVIL 0x00000004
#define AFF_DETECT_INVISIBLE 0x00000008
#define AFF_DETECT_MAGIC 0x00000010
#define AFF_SENSE_LIFE 0x00000020
#define AFF_LIFE_PROT 0x00000040
#define AFF_SANCTUARY 0x00000080
#define AFF_DRAGON_RIDE 0x00000100
#define AFF_GROWTH 0x00000200 /* this was the one that was missing*/

#define AFF_CURSE 0x00000400
#define AFF_FLYING 0x00000800
#define AFF_POISON 0x00001000
#define AFF_TREE_TRAVEL 0x00002000
#define AFF_PARALYSIS 0x00004000
#define AFF_INFRAVISION 0x00008000
#define AFF_WATERBREATH 0x00010000
#define AFF_SLEEP 0x00020000
#define AFF_TRAVELLING 0x00040000 /* i.e. can't be stoned */
#define AFF_SNEAK 0x00080000
#define AFF_HIDE 0x00100000
#define AFF_SILENCE 0x00200000
#define AFF_CHARM 0x00400000
#define AFF_FOLLOW 0x00800000
#define AFF_UNDEF_1 0x01000000 /* saved objects?? */
#define AFF_TRUE_SIGHT 0x02000000
#define AFF_SCRYING 0x04000000 /* seeing other rooms */
#define AFF_FIRESHIELD 0x08000000
#define AFF_GROUP 0x10000000
#define AFF_UNDEF_6 0x20000000
#define AFF_TEAM_GREY 0x40000000
#define AFF_TEAM_AMBER 0x80000000

/* affects 2 */

#define AFF2_ANIMAL_INVIS 0x00000001 /* the spell */
#define AFF2_HEAT_STUFF 0x00000002   /* the spell */
#define AFF2_LOG_ME 0x00000004       /* am I logged? */
#define AFF2_ONE_LIFER 0x00000008    /* am I stupid? */
#define AFF2_SUN_BLIND 0x00000010    /* do I not see well in daylight? */
#define AFF2_FEEDING 0x00000020      /* am I feeding (vampires) */
#define AFF2_BERSERK 0x00000040      /* ogres go ape! */

#define IMM_FIRE 1
#define IMM_COLD 2
#define IMM_ELEC 4
#define IMM_ENERGY 8
#define IMM_BLUNT 16
#define IMM_PIERCE 32
#define IMM_SLASH 64
#define IMM_ACID 128
#define IMM_POISON 256
#define IMM_DRAIN 512
#define IMM_SLEEP 1024
#define IMM_CHARM 2048
#define IMM_HOLD 4096
#define IMM_NONMAG 8192
#define IMM_PLUS1 16384
#define IMM_PLUS2 32768
#define IMM_PLUS3 65536
#define IMM_PLUS4 131072

#define IS_NPC(ch) (IS_SET((ch)->specials.act, ACT_ISNPC))
#define IS_PC(ch) (!IS_NPC((ch)) || IS_SET((ch)->specials.act, ACT_POLYSELF))

#define SKILL_SNEAK 45     /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_HIDE 46      /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_STEAL 47     /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_BACKSTAB 48  /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_PICK_LOCK 49 /* Reserved Skill[] DO NOT CHANGE */

#define SKILL_KICK 50        /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_BASH 51        /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_RESCUE 52      /* MAXIMUM SKILL NUMBER  */
#define SKILL_DUAL_WIELD 169 /* */

#define SKILL_FIRST_AID 170
#define SKILL_SIGN 171
#define SKILL_RIDE 172
#define SKILL_SWITCH_OPP 173
#define SKILL_DODGE 174
#define SKILL_REMOVE_TRAP 175
#define SKILL_RETREAT 176
#define SKILL_QUIV_PALM 177
#define SKILL_SAFE_FALL 178
#define SKILL_FEIGN_DEATH 179
#define SKILL_HUNT 180
#define SKILL_LOCATE_TRAP 181
#define SKILL_SPRING_LEAP 182
#define SKILL_DISARM 183
#define SKILL_READ_MAGIC 184
#define SKILL_EVALUATE 185
#define SKILL_SPY 186
#define SKILL_DOORBASH 187
#define SKILL_SWIM 188
#define SKILL_CONS_UNDEAD 189
#define SKILL_CONS_VEGGIE 190
#define SKILL_CONS_DEMON 191
#define SKILL_CONS_ANIMAL 192
#define SKILL_CONS_REPTILE 193
#define SKILL_CONS_PEOPLE 194
#define SKILL_CONS_GIANT 195
#define SKILL_CONS_OTHER 196
#define SKILL_DISGUISE 197
#define SKILL_CLIMB 198
#define SKILL_INSET 199 /* skill at insetting stones */

#define TICK_WRAP_COUNT 3

#define HATE_SEX 1
#define HATE_RACE 2
#define HATE_CHAR 4
#define HATE_CLASS 8
#define HATE_EVIL 16
#define HATE_GOOD 32
#define HATE_VNUM 64

#define FEAR_SEX 1
#define FEAR_RACE 2
#define FEAR_CHAR 4
#define FEAR_CLASS 8
#define FEAR_EVIL 16
#define FEAR_GOOD 32
#define FEAR_VNUM 64

#define OP_SEX 1
#define OP_RACE 2
#define OP_CHAR 3
#define OP_CLASS 4
#define OP_EVIL 5
#define OP_GOOD 6
#define OP_VNUM 7

/* Predifined  conditions */
#define DRUNK 0
#define FULL 1
#define THIRST 2

#define TYPE_HIT 406
#define TYPE_BLUDGEON 407
#define TYPE_PIERCE 408
#define TYPE_SLASH 409
#define TYPE_WHIP 410
#define TYPE_CLAW 411
#define TYPE_BITE 412
#define TYPE_STING 413
#define TYPE_CRUSH 414
#define TYPE_CLEAVE 415
#define TYPE_STAB 416
#define TYPE_SMASH 417
#define TYPE_SMITE 418
#define TYPE_BLAST 419
#define TYPE_SUFFERING 420

#define ZONE_NEVER 0
#define ZONE_EMPTY 1
#define ZONE_ALWAYS 2
#define ZONE_ASTRAL 4
#define ZONE_DESERT 8
#define ZONE_ARCTIC 16
#define ZONE_HADES 32
#define ZONE_OLYMPUS 64
#define ZONE_ABYSS 128
#define ZONE_PMP 256 /* Prime Material Plane */
#define ZONE_LIMBO 512

#define WEAR_LIGHT 0
#define WEAR_FINGER_R 1
#define WEAR_FINGER_L 2
#define WEAR_NECK_1 3
#define WEAR_NECK_2 4
#define WEAR_BODY 5
#define WEAR_HEAD 6
#define WEAR_LEGS 7
#define WEAR_FEET 8
#define WEAR_HANDS 9
#define WEAR_ARMS 10
#define WEAR_SHIELD 11
#define WEAR_ABOUT 12
#define WEAR_WAISTE 13
#define WEAR_WRIST_R 14
#define WEAR_WRIST_L 15
#define WIELD 16
#define HOLD 17
