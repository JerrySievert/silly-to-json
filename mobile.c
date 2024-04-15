/*
 * Read all room files from SillyMUD format, ripped out of the SillyMUD
 * code.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conversion.h"

struct index_data *mob_index; /* index table for mobile file     */
int top_of_mobt = 0;

/* generate index table for object or monster file */
struct index_data *generate_indices(FILE *fl, int *top) {
  int i   = 0;
  long bc = 1500;
  struct index_data *index;
  char buf[ 82 ];

  rewind(fl);

  for (;;) {
    if (fgets(buf, sizeof(buf), fl)) {
      if (*buf == '#') {
        if (!i) /* first cell */
          CREATE(index, struct index_data, bc);
        else if (i >= bc) {
          if (!(index = (struct index_data *)realloc(
                    index, (i + 50) * sizeof(struct index_data)))) {
            perror("load indices");
            assert(0);
          }
          bc += 50;
        }
        sscanf(buf, "#%d", &index[ i ].virtual_number);
        index[ i ].pos    = ftell(fl);
        index[ i ].number = 0;
        index[ i ].func   = 0;
        index[ i ].name = (index[ i ].virtual_number < 99999) ? fread_string(fl)
                                                              : strdup("omega");
        i++;
      } else {
        if (*buf == '$') /* EOF */
          break;
      }
    } else {
      fprintf(stderr, "generate indices");
      assert(0);
    }
  }
  *top = i - 2;

  return (index);
}

int real_mobile(int virtual) {
  int bot, top, mid;

  bot = 0;
  top = top_of_mobt;

  /* perform binary search on mob-table */
  for (;;) {
    mid = (bot + top) / 2;

    if ((mob_index + mid)->virtual_number == virtual)
      return (mid);
    if (bot >= top)
      return (-1);
    if ((mob_index + mid)->virtual_number > virtual)
      top = mid - 1;
    else
      bot = mid + 1;
  }
}

int dice(int number, int size) {
  int r;
  int sum = 0;

  assert(size >= 0);

  if (size == 0)
    return (0);

  for (r = 1; r <= number; r++)
    sum += ((random( ) % size) + 1);
  return (sum);
}

int number(int from, int to) {
  if (to - from + 1)
    return ((random( ) % (to - from + 1)) + from);
  else
    return (from);
}

void boot_mobiles(void) {
  FILE *fl;
  int zon = 0, cmd_no = 0, expand, tmp, bc = 100, cc = 22;
  char *check, buf[ 81 ];

  if (!(fl = fopen(MOB_FILE, "r"))) {
    perror("boot_mobiles");
    assert(0);
  }

  mob_index = generate_indices(fl, &top_of_mobt);
}

void clear_char(struct char_data *ch) {
  memset(ch, '\0', sizeof(struct char_data));

  ch->in_room              = NOWHERE;
  ch->specials.was_in_room = NOWHERE;
  ch->specials.position    = POSITION_STANDING;
  ch->specials.default_pos = POSITION_STANDING;
  GET_AC(ch)               = 100; /* Basic Armor */
}

int MIN(int a, int b) { return a < b ? a : b; }

int MAX(int a, int b) { return a > b ? a : b; }

int IsSmall(struct char_data *ch) {
  switch (GET_RACE(ch)) {
  case RACE_SMURF:
  case RACE_GNOME:
  case RACE_HALFLING:
  case RACE_GOBLIN:
  case RACE_ENFAN:
    return (1);
  default:
    return (0);
  }
}

int IsGiant(struct char_data *ch) {
  switch (GET_RACE(ch)) {
  case RACE_GIANT:
  case RACE_TYTAN:
  case RACE_GOD:
    return (1);
  default:
    return (0);
  }
}

int VerifyMob(struct char_data *ch) {
  char buf[ 256 ];
  /* check to see that the mob falls within certain parameters */

  if (ch->specials.damnodice < 0) {
    sprintf(buf, "%s's number of damage dice is negative\n", ch->player.name);
    printf("%s", buf);
  }

  if (ch->specials.damsizedice < 0) {
    sprintf(buf, "%s's size of damage dice is negative\n", ch->player.name);
    printf("%s", buf);
  }
  return (1);
}

int GetMaxLevel(struct char_data *ch) {
  register int max = 0, i;

  for (i = 0; i <= MAX_CLASS; i++) {
    if (GET_LEVEL(ch, i) > max)
      max = GET_LEVEL(ch, i);
  }

  return (max);
}

int DetermineExp(struct char_data *mob, int exp_flags) {

  int base;
  int phit;
  int sab;
  char buf[ 200 ];

  if (exp_flags > 100) {
    sprintf(buf, "Exp flags on %s are > 100 (%d)", GET_NAME(mob), exp_flags);
    printf("%s\n", buf);
  }

  /*
  reads in the monster, and adds the flags together
  for simplicity, 1 exceptional ability is 2 special abilities
  */

  if (GetMaxLevel(mob) < 0)
    return (1);

  switch (GetMaxLevel(mob)) {

  case 0:
    base = 5;
    phit = 1;
    sab  = 10;
    break;

  case 1:
    base = 20;
    phit = 2;
    sab  = 15;
    break;

  case 2:
    base = 35;
    phit = 3;
    sab  = 20;
    break;

  case 3:
    base = 50;
    phit = 4;
    sab  = 25;
    break;

  case 4:
    base = 75;
    phit = 5;
    sab  = 30;
    break;

  case 5:
    base = 110;
    phit = 6;
    sab  = 40;
    break;

  case 6:
    base = 150;
    phit = 6;
    sab  = 75;
    break;

  case 7:
    base = 225;
    phit = 8;
    sab  = 125;
    break;

  case 8:
    base = 600;
    phit = 12;
    sab  = 175;
    break;

  case 9:
    base = 900;
    phit = 14;
    sab  = 300;
    break;

  case 10:
    base = 1100;
    phit = 15;
    sab  = 450;
    break;

  case 11:
    base = 1300;
    phit = 16;
    sab  = 700;
    break;

  case 12:
    base = 1550;
    phit = 17;
    sab  = 700;
    break;

  case 13:
    base = 1800;
    phit = 18;
    sab  = 950;
    break;

  case 14:
    base = 2100;
    phit = 19;
    sab  = 950;
    break;

  case 15:
    base = 2400;
    phit = 20;
    sab  = 1250;
    break;

  case 16:
    base = 2700;
    phit = 23;
    sab  = 1250;
    break;

  case 17:
    base = 3000;
    phit = 25;
    sab  = 1550;
    break;

  case 18:
    base = 3500;
    phit = 28;
    sab  = 1550;
    break;

  case 19:
    base = 4000;
    phit = 30;
    sab  = 2100;
    break;

  case 20:
    base = 4500;
    phit = 33;
    sab  = 2100;
    break;

  case 21:
    base = 5000;
    phit = 35;
    sab  = 2600;
    break;

  case 22:
    base = 6000;
    phit = 40;
    sab  = 3000;
    break;

  case 23:
    base = 7000;
    phit = 45;
    sab  = 3500;
    break;

  case 24:
    base = 8000;
    phit = 50;
    sab  = 4000;
    break;

  case 25:
    base = 9000;
    phit = 55;
    sab  = 4500;
    break;

  case 26:
    base = 10000;
    phit = 60;
    sab  = 5000;
    break;

  case 27:
    base = 12000;
    phit = 70;
    sab  = 6000;
    break;

  case 28:
    base = 14000;
    phit = 80;
    sab  = 7000;
    break;

  case 29:
    base = 16000;
    phit = 90;
    sab  = 8000;
    break;

  case 30:
    base = 20000;
    phit = 100;
    sab  = 10000;
    break;

  case 32:
  case 33:
  case 34:
  case 31:
  default:
    base = 22000;
    phit = 120;
    sab  = 12000;
    break;
  }
#if 0 /* removed 6/13, this was ridiculous */
    case 35:			/* higher level people can allready clear */
    case 36:			/* the mud. -keith */
    case 37:
    case 38:
    case 39:
      base = 32000;
      phit = 140;
      sab  = 14000;
      break;

    case 40:
    case 41:
      base = 42000;
      phit = 160;
      sab  = 16000;
      break;

    case 42:
    case 43:
      base = 52000;
      phit = 180;
      sab  = 20000;
      break;

    case 44:
    case 45:
      base = 72000;
      phit = 200;
      sab  = 24000;
      break;

    case 46:
    case 47:
      base = 92000;
      phit = 225;
      sab  = 28000;
      break;

    case 48:
    case 49:
      base = 122000;
      phit = 250;
      sab  = 32000;
      break;

    case 50:
      base = 150000;
      phit = 275;
      sab  = 36000;
      break;

    default:
      base = 200000;
      phit = 300;
      sab  = 40000;
      break;
#endif

  return (base + (phit * GET_HIT(mob)) + (sab * exp_flags));
}

void SetRacialStuff(struct char_data *mob) {

  switch (GET_RACE(mob)) {
  case RACE_VEGMAN:
    SET_BIT(mob->specials.intrinsics, AFF_WATERBREATH);
    SET_BIT(mob->specials.intrinsics, AFF_TREE_TRAVEL);
    break;
  case RACE_FAERIE:
    SET_BIT(mob->specials.intrinsics, AFF_FLYING);
    SET_BIT(mob->specials.intrinsics, AFF_DETECT_INVISIBLE);
    SET_BIT(mob->specials.intrinsics, AFF_DETECT_MAGIC);
    SET_BIT(mob->specials.intrinsics, AFF_INFRAVISION);
    SET_BIT(mob->immune, IMM_ELEC);
    SET_BIT(mob->susc, IMM_FIRE);
    break;
  case RACE_MFLAYER:
    SET_BIT(mob->specials.intrinsics, AFF_INFRAVISION);
    SET_BIT(mob->specials.intrinsics, AFF_DETECT_INVISIBLE);
    SET_BIT(mob->specials.intrinsics, AFF_SENSE_LIFE);
    break;
  case RACE_SKEXIE:
    SET_BIT(mob->specials.intrinsics, AFF_FLYING);
    SET_BIT(mob->specials.intrinsics, AFF_DETECT_INVISIBLE);
    SET_BIT(mob->immune, IMM_COLD);
    SET_BIT(mob->immune, IMM_ELEC);
    SET_BIT(mob->susc, IMM_FIRE);
    break;
  case RACE_BIRD:
    SET_BIT(mob->specials.intrinsics, AFF_FLYING);
    break;
  case RACE_ELVEN:
    SET_BIT(mob->M_immune, IMM_HOLD);
    SET_BIT(mob->M_immune, IMM_SLEEP);
    SET_BIT(mob->specials.intrinsics, AFF_INFRAVISION);
    break;
  case RACE_DRAAGDIM:
    SET_BIT(mob->immune, IMM_CHARM);
    SET_BIT(mob->immune, IMM_POISON);
    SET_BIT(mob->specials.intrinsics, AFF_INFRAVISION);
    break;
  case RACE_FISH:
  case RACE_REPTILE:
  case RACE_SNAKE:
    SET_BIT(mob->specials.intrinsics, AFF_WATERBREATH);
    break;
  case RACE_DROW:
  case RACE_DWARF:
  case RACE_GNOME:
  case RACE_TROLL:
  case RACE_ORC:
  case RACE_GOBLIN:
  case RACE_HALFORC:
  case RACE_OGRE:
  case RACE_VAMPIRE:
    SET_BIT(mob->specials.intrinsics, AFF_INFRAVISION);
    break;
  case RACE_INSECT:
  case RACE_ARACHNID:
    if (IS_PC(mob)) {
      GET_STR(mob) = 18;
      GET_ADD(mob) = 100;
    }
    break;
  case RACE_LYCANTH:
    SET_BIT(mob->M_immune, IMM_NONMAG);
    break;
  case RACE_PREDATOR:
    if (mob->skills)
      mob->skills[ SKILL_HUNT ].learned = 100;
    break;
  case RACE_PATRYN:
    SET_BIT(mob->specials.intrinsics, AFF_DETECT_MAGIC);
    break;

  default:
    break;
  }

  /* height and weight */
  switch (GET_RACE(mob)) {
  case RACE_HUMAN:
  case RACE_HUMANTWO:
  case RACE_ELVEN:
  case RACE_GNOME:
  case RACE_DWARF:
  case RACE_HALFLING:
  case RACE_LYCANTH:
  case RACE_UNDEAD:
  case RACE_VEGMAN:
  case RACE_MFLAYER:
  case RACE_DROW:
  case RACE_SKEXIE:
  case RACE_TROGMAN:
  case RACE_SARTAN:
  case RACE_PATRYN:
  case RACE_DRAAGDIM:
  case RACE_ASTRAL:
  case RACE_HALFELF:
  case RACE_VAMPIRE:
    break;
  case RACE_HORSE:
    mob->player.weight = 400;
    mob->player.height = 175;
  case RACE_ORC:
  case RACE_HALFORC:
    mob->player.weight = 150;
    mob->player.height = 140;
    break;
  case RACE_OGRE:
    mob->player.weight = 300;
    mob->player.height = 240;
    break;
  case RACE_SMURF:
  case RACE_FAERIE:
    mob->player.weight = 40;
    mob->player.height = 121;
    break;
  case RACE_GOBLIN:
  case RACE_ENFAN:
    mob->player.weight = 120;
    mob->player.height = 100;
    break;
  case RACE_LABRAT:
  case RACE_INSECT:
  case RACE_ARACHNID:
  case RACE_REPTILE:
  case RACE_DINOSAUR:
  case RACE_FISH:
  case RACE_PREDATOR:
  case RACE_SNAKE:
  case RACE_HERBIV:
  case RACE_VEGGIE:
  case RACE_ELEMENT:
  case RACE_PRIMATE:
  case RACE_GOLEM:
    mob->player.weight = 10 + GetMaxLevel(mob) * GetMaxLevel(mob) * 2;
    mob->player.height = 20 + MIN(mob->player.weight, 600);
    break;
  case RACE_DRAGON:
    mob->player.weight = MAX(60, GetMaxLevel(mob) * GetMaxLevel(mob) * 2);
    mob->player.height = 100 + MIN(mob->player.weight, 500);
    break;
  case RACE_BIRD:
  case RACE_PARASITE:
  case RACE_SLIME:
  case RACE_GHOST:
    mob->player.weight = GetMaxLevel(mob) * (GetMaxLevel(mob) / 5);
    mob->player.height = 10 * GetMaxLevel(mob);
    break;
  case RACE_TROLL:
  case RACE_GIANT:
  case RACE_DEMON:
  case RACE_DEVIL:
  case RACE_PLANAR:
    mob->player.height = 200 + GetMaxLevel(mob) * 15;
    mob->player.weight = (int)mob->player.height * 1.5;
    break;
  case RACE_GOD:
  case RACE_TREE:
  case RACE_TYTAN:
    mob->player.weight = MAX(500, GetMaxLevel(mob) * GetMaxLevel(mob) * 10);
    mob->player.height = GetMaxLevel(mob) / 2 * 100;
    break;
  }
}

struct char_data *read_mobile(FILE *mob_f, int nr, int type) {
  int i;
  int tmp, tmp2, tmp3, bc = 0;
  struct char_data *mob;
  char buf[ 100 ];
  char letter;

  int mob_tick_count;
  long mob_count;

  i = nr;
  if (type == VIRTUAL)
    if ((nr = real_mobile(nr)) < 0) {
      sprintf(buf, "Mobile (V) %d does not exist in database.", i);
      return (0);
    }

  fseek(mob_f, mob_index[ nr ].pos, 0);

  CREATE(mob, struct char_data, 1);
  bc = sizeof(struct char_data);
  clear_char(mob);

  mob->specials.last_direction = -1; /* this is a fix for wander */

  mob->specials.position = POSITION_STANDING;

  /***** String data *** */

  mob->player.name = fread_string(mob_f);
  if (*mob->player.name)
    bc += strlen(mob->player.name);
  mob->player.short_descr = fread_string(mob_f);
  if (*mob->player.short_descr)
    bc += strlen(mob->player.short_descr);
  mob->player.long_descr = fread_string(mob_f);
  if (*mob->player.long_descr)
    bc += strlen(mob->player.long_descr);
  mob->player.description = fread_string(mob_f);
  if (mob->player.description && *mob->player.description)
    bc += strlen(mob->player.description);
  mob->player.title = 0;

  /* *** Numeric data *** */

  mob->mult_att           = 1.0;
  mob->specials.spellfail = 101;

  fscanf(mob_f, "%d ", &tmp);
  mob->specials.act = tmp;
  SET_BIT(mob->specials.act, ACT_ISNPC);

  fscanf(mob_f, " %d ", &tmp);
  mob->specials.affected_by = tmp;

  fscanf(mob_f, " %d ", &tmp);
  mob->specials.alignment = tmp;

  mob->player.class_type = CLASS_WARRIOR;

  fscanf(mob_f, " %c ", &letter);

  if (letter == 'S') {

    fscanf(mob_f, "\n");

    fscanf(mob_f, " %d ", &tmp);
    GET_LEVEL(mob, WARRIOR_LEVEL_IND) = tmp;

    mob->abilities.str   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.intel = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.wis   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.dex   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.con   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.chr   = 9 + number(1, (MAX(1, tmp / 5 - 1)));

    fscanf(mob_f, " %d ", &tmp);
    mob->points.hitroll = 20 - tmp;

    fscanf(mob_f, " %d ", &tmp);

    if (tmp > 10 || tmp < -10)
      tmp /= 10;

    mob->points.armor = 10 * tmp;

    fscanf(mob_f, " %dd%d+%d ", &tmp, &tmp2, &tmp3);
    mob->points.max_hit = dice(tmp, tmp2) + tmp3;
    mob->points.hit     = mob->points.max_hit;

    fscanf(mob_f, " %dd%d+%d \n", &tmp, &tmp2, &tmp3);
    mob->points.damroll       = tmp3;
    mob->specials.damnodice   = tmp;
    mob->specials.damsizedice = tmp2;

    mob->points.mana     = 10;
    mob->points.max_mana = 10;

    mob->points.move     = 50;
    mob->points.max_move = 50;

    fscanf(mob_f, " %d ", &tmp);
    if (tmp == -1) {
      fscanf(mob_f, " %d ", &tmp);
      mob->points.gold = tmp;
      fscanf(mob_f, " %d ", &tmp);
      GET_EXP(mob) = tmp;
      fscanf(mob_f, " %d \n", &tmp);
      GET_RACE(mob) = tmp;
      if (IsGiant(mob))
        mob->abilities.str += number(1, 4);
      if (IsSmall(mob))
        mob->abilities.str -= 1;
    } else {
      mob->points.gold = tmp;
      fscanf(mob_f, " %d \n", &tmp);
      GET_EXP(mob) = tmp;
    }
    fscanf(mob_f, " %d ", &tmp);
    mob->specials.position = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->specials.default_pos = tmp;

    fscanf(mob_f, " %d ", &tmp);
    if (tmp < 3) {
      mob->player.sex = tmp;
      mob->immune     = 0;
      mob->M_immune   = 0;
      mob->susc       = 0;
    } else if (tmp < 6) {
      mob->player.sex = (tmp - 3);
      fscanf(mob_f, " %d ", &tmp);
      mob->immune = tmp;
      fscanf(mob_f, " %d ", &tmp);
      mob->M_immune = tmp;
      fscanf(mob_f, " %d ", &tmp);
      mob->susc = tmp;
    } else {
      mob->player.sex = 0;
      mob->immune     = 0;
      mob->M_immune   = 0;
      mob->susc       = 0;
    }

    fscanf(mob_f, "\n");

    mob->player.class_type = 0;

    mob->player.time.birth  = time(0);
    mob->player.time.played = 0;
    mob->player.time.logon  = time(0);
    mob->player.weight      = 200;
    mob->player.height      = 198;

    for (i = 0; i < 3; i++)
      GET_COND(mob, i) = -1;

    for (i = 0; i < 5; i++)
      mob->specials.apply_saving_throw[ i ] =
          MAX(20 - GET_LEVEL(mob, WARRIOR_LEVEL_IND), 2);

  } else if ((letter == 'A') || (letter == 'N') || (letter == 'B') ||
             (letter == 'L')) {

    if ((letter == 'A') || (letter == 'B') || (letter == 'L')) {
      fscanf(mob_f, " %d ", &tmp);
      mob->mult_att = (float)tmp;
      /*
      **  read in types:
      */
    }

    fscanf(mob_f, "\n");

    fscanf(mob_f, " %d ", &tmp);
    GET_LEVEL(mob, WARRIOR_LEVEL_IND) = tmp;

    mob->abilities.str   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.intel = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.wis   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.dex   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.con   = 9 + number(1, (MAX(1, tmp / 5 - 1)));
    mob->abilities.chr   = 9 + number(1, (MAX(1, tmp / 5 - 1)));

    fscanf(mob_f, " %d ", &tmp);
    mob->points.hitroll = 20 - tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->points.armor = 10 * tmp;
    fscanf(mob_f, " %d ", &tmp);
    mob->points.max_hit = dice(GET_LEVEL(mob, WARRIOR_LEVEL_IND), 8) + tmp;
    mob->points.hit     = mob->points.max_hit;

    fscanf(mob_f, " %dd%d+%d \n", &tmp, &tmp2, &tmp3);
    mob->points.damroll       = tmp3;
    mob->specials.damnodice   = tmp;
    mob->specials.damsizedice = tmp2;

    mob->points.mana     = 10;
    mob->points.max_mana = 10;

    mob->points.move     = 50;
    mob->points.max_move = 50;

    fscanf(mob_f, " %d ", &tmp);

    if (tmp == -1) {
      fscanf(mob_f, " %d ", &tmp);
      mob->points.gold = tmp;
      fscanf(mob_f, " %d ", &tmp);
      if (tmp >= 0)
        GET_EXP(mob) = (DetermineExp(mob, tmp) + mob->points.gold);
      else
        GET_EXP(mob) = -tmp;
      fscanf(mob_f, " %d ", &tmp);
      GET_RACE(mob) = tmp;
      if (IsGiant(mob))
        mob->abilities.str += number(1, 4);
      if (IsSmall(mob))
        mob->abilities.str -= 1;
    } else {
      mob->points.gold = tmp;

      /*
        this is where the new exp will come into play
        */
      fscanf(mob_f, " %d \n", &tmp);
      GET_EXP(mob) = (DetermineExp(mob, tmp) + mob->points.gold);
    }

    fscanf(mob_f, " %d ", &tmp);
    mob->specials.position = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->specials.default_pos = tmp;

    fscanf(mob_f, " %d \n", &tmp);
    if (tmp < 3) {
      mob->player.sex = tmp;
      mob->immune     = 0;
      mob->M_immune   = 0;
      mob->susc       = 0;
    } else if (tmp < 6) {
      mob->player.sex = (tmp - 3);
      fscanf(mob_f, " %d ", &tmp);
      mob->immune = tmp;
      fscanf(mob_f, " %d ", &tmp);
      mob->M_immune = tmp;
      fscanf(mob_f, " %d ", &tmp);
      mob->susc = tmp;
    } else {
      mob->player.sex = 0;
      mob->immune     = 0;
      mob->M_immune   = 0;
      mob->susc       = 0;
    }

    /*
     *   read in the sound string for a mobile
     */
    if (letter == 'L') {
      mob->player.sounds = fread_string(mob_f);
      if (mob->player.sounds && *mob->player.sounds)
        bc += strlen(mob->player.sounds);

      mob->player.distant_snds = fread_string(mob_f);
      if (mob->player.distant_snds && *mob->player.distant_snds)
        bc += strlen(mob->player.distant_snds);
    } else {
      mob->player.sounds       = 0;
      mob->player.distant_snds = 0;
    }

    if (letter == 'B') {
      SET_BIT(mob->specials.act, ACT_HUGE);
    }

    mob->player.class_type = 0;

    mob->player.time.birth  = time(0);
    mob->player.time.played = 0;
    mob->player.time.logon  = time(0);
    mob->player.weight      = 200;
    mob->player.height      = 198;

    for (i = 0; i < 3; i++)
      GET_COND(mob, i) = -1;

    for (i = 0; i < 5; i++)
      mob->specials.apply_saving_throw[ i ] =
          MAX(20 - GET_LEVEL(mob, WARRIOR_LEVEL_IND), 2);

  } else { /* The old monsters are down below here */

    fscanf(mob_f, "\n");

    fscanf(mob_f, " %d ", &tmp);
    mob->abilities.str = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->abilities.intel = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->abilities.wis = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->abilities.dex = tmp;

    fscanf(mob_f, " %d \n", &tmp);
    mob->abilities.con = tmp;

    fscanf(mob_f, " %d ", &tmp);
    fscanf(mob_f, " %d ", &tmp2);

    mob->points.max_hit = number(tmp, tmp2);
    mob->points.hit     = mob->points.max_hit;

    fscanf(mob_f, " %d ", &tmp);
    mob->points.armor = 10 * tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->points.mana     = tmp;
    mob->points.max_mana = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->points.move     = tmp;
    mob->points.max_move = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->points.gold = tmp;

    fscanf(mob_f, " %d \n", &tmp);
    GET_EXP(mob) = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->specials.position = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->specials.default_pos = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->player.sex = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->player.class_type = tmp;

    fscanf(mob_f, " %d ", &tmp);
    GET_LEVEL(mob, WARRIOR_LEVEL_IND) = tmp;

    fscanf(mob_f, " %d ", &tmp);
    mob->player.time.birth  = time(0);
    mob->player.time.played = 0;
    mob->player.time.logon  = time(0);

    fscanf(mob_f, " %d ", &tmp);
    mob->player.weight = tmp;

    fscanf(mob_f, " %d \n", &tmp);
    mob->player.height = tmp;

    for (i = 0; i < 3; i++) {
      fscanf(mob_f, " %d ", &tmp);
      GET_COND(mob, i) = tmp;
    }
    fscanf(mob_f, " \n ");

    for (i = 0; i < 5; i++) {
      fscanf(mob_f, " %d ", &tmp);
      mob->specials.apply_saving_throw[ i ] = tmp;
    }

    fscanf(mob_f, " \n ");

    /* Set the damage as some standard 1d4 */
    mob->points.damroll       = 0;
    mob->specials.damnodice   = 1;
    mob->specials.damsizedice = 6;

    /* Calculate THAC0 as a formula of Level */
    mob->points.hitroll = MAX(1, GET_LEVEL(mob, WARRIOR_LEVEL_IND) - 3);
  }

  mob->tmpabilities = mob->abilities;

  for (i = 0; i < MAX_WEAR; i++) /* Initialisering Ok */
    mob->equipment[ i ] = 0;

  mob->nr = nr;

  mob->desc = 0;

  if (!IS_SET(mob->specials.act, ACT_ISNPC))
    SET_BIT(mob->specials.act, ACT_ISNPC);

  mob->generic   = 0;
  mob->commandp  = 0;
  mob->commandp2 = 0;
  mob->waitp     = 0;

  VerifyMob(mob); /* check it for acceptability */

  /* tell the spec_proc (if there is one) that we've been born */
  /*  if(mob_index[nr].func)
      (*mob_index[nr].func)(mob, 0, "", mob, EVENT_BIRTH);
      */
  /* insert in list */

#if NEW_RENT
  if (mob->points.gold >= 10)
    mob->points.gold /= 10;
  else if (mob->points.gold > 0)
    mob->points.gold = 1;
#endif

  if (mob->points.gold > GET_LEVEL(mob, WARRIOR_LEVEL_IND) * 1500) {
    char buf[ 200 ];
    sprintf(buf, "%s has gold > level * 1500 (%d)", mob->player.short_descr,
            mob->points.gold);
    printf("%s\n", buf);
  }

  /* set up things that all members of the race have */
  mob->specials.intrinsics = 0;
  SetRacialStuff(mob); /* this sets intrinsics */
  mob->specials.affected_by |= mob->specials.intrinsics;

  /* change exp for wimpy mobs (lower) */
  if (IS_SET(mob->specials.act, ACT_WIMPY))
    GET_EXP(mob) -= GET_EXP(mob) / 10;

  /* change exp for agressive mobs (higher) */
  if (IS_SET(mob->specials.act, ACT_AGGRESSIVE)) {
    GET_EXP(mob) += GET_EXP(mob) / 10;
    /* big bonus for fully aggressive mobs for now */
    if (!IS_SET(mob->specials.act, ACT_WIMPY) ||
        IS_SET(mob->specials.act, ACT_META_AGG))
      GET_EXP(mob) += (GET_EXP(mob) / 2);
  }

  /* set up distributed movement system */

  mob->specials.tick = mob_tick_count++;

  if (mob_tick_count == TICK_WRAP_COUNT)
    mob_tick_count = 0;

  mob_index[ nr ].number++;

#if BYTE_COUNT
  fprintf(stderr, "Mobile [%d]: byte count: %d\n", mob_index[ nr ].virtual, bc);
#endif

  mob_count++;
  return (mob);
}
