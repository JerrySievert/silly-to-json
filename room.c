/*
 * Read all room files from SillyMUD format, ripped out of the SillyMUD
 * code.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conversion.h"
#include "jansson.h"

int top_of_zone_table = 0;
long top_of_world     = 0;
struct zone_data *zone_table;
struct room_data *room_db[ WORLD_SIZE ];
long room_count = 0;

char *fread_string(FILE *f1) {
  char buf[ MAX_STRING_LENGTH ];
  int i = 0, tmp;

  buf[ 0 ] = '\0';

  while (i < MAX_STRING_LENGTH - 2) {
    tmp = fgetc(f1);
    if (!tmp) {
      perror("Fread_string");
      assert(0);
    }

    if (tmp == '~') {
      break;
    }

    buf[ i++ ] = (char)tmp;
    if (buf[ i - 1 ] == '\n')
      buf[ i++ ] = '\r';
  }

  if (i == MAX_STRING_LENGTH - 3) { /* We filled the buffer */
    buf[ i ] = '\0';
    fprintf(stderr, "File too long (fread_string).\n");
    while ((tmp = fgetc(f1)))
      if (tmp == '~')
        break;
  }

  else
    buf[ i ] = '\0';

  fgetc(f1);

  return ((char *)strdup(buf));
}

struct room_data *real_roomp(int virtual) {
  return ((virtual<WORLD_SIZE &&virtual> - 1) ? room_db[ virtual ] : 0);
}

void setup_dir(FILE *fl, int room, int dir) {
  int tmp;
  struct room_data *rp, dummy;

  rp = real_roomp(room);

  if (!rp) {
    rp           = &dummy; /* this is a quick fix to make the game */
    dummy.number = room;   /* stop crashing   */
  }

  CREATE(rp->dir_option[ dir ], struct room_direction_data, 1);

  rp->dir_option[ dir ]->general_description = fread_string(fl);
  rp->dir_option[ dir ]->keyword             = fread_string(fl);

  fscanf(fl, "%d ", &tmp);
  switch (tmp) {
  case 1:
    rp->dir_option[ dir ]->exit_info = EX_ISDOOR;
    break;
  case 2:
    rp->dir_option[ dir ]->exit_info = EX_ISDOOR | EX_PICKPROOF;
    break;
  case 3:
    rp->dir_option[ dir ]->exit_info = EX_ISDOOR | EX_SECRET;
    break;
  case 4:
    rp->dir_option[ dir ]->exit_info = EX_ISDOOR | EX_SECRET | EX_PICKPROOF;
    break;
  case 5:
    rp->dir_option[ dir ]->exit_info = EX_CLIMB;
    break;
  case 6:
    rp->dir_option[ dir ]->exit_info = EX_CLIMB | EX_ISDOOR;
    break;
  case 7:
    rp->dir_option[ dir ]->exit_info = EX_CLIMB | EX_ISDOOR | EX_PICKPROOF;
    break;
  default:
    rp->dir_option[ dir ]->exit_info = 0;
  }

  fscanf(fl, " %d ", &tmp);
  rp->dir_option[ dir ]->key = tmp;

  fscanf(fl, " %d ", &tmp);
  rp->dir_option[ dir ]->to_room = tmp;
}

void load_one_room(FILE *fl, struct room_data *rp) {
  char chk[ 50 ];
  int bc                = 0;
  unsigned long int tmp = 0;

  struct extra_descr_data *new_descr;

  bc = sizeof(struct room_data);

  rp->name = fread_string(fl);
  if (rp->name && *rp->name)
    bc += strlen(rp->name);
  rp->description = fread_string(fl);
  if (rp->description && *rp->description)
    bc += strlen(rp->description);

  if (top_of_zone_table >= 0) {
    int zone;
    fscanf(fl, " %*d ");

    /* OBS: Assumes ordering of input rooms */

    for (zone = 0;
         rp->number > zone_table[ zone ].top && zone <= top_of_zone_table;
         zone++)
      ;

    if (zone > top_of_zone_table) {
      fprintf(stderr, "Room %d is outside of any zone.\n", rp->number);
      assert(0);
    }
    rp->zone = zone;
  }

  fscanf(fl, " %ld ", &tmp);
  rp->room_flags = tmp;
  fscanf(fl, " %ld ", &tmp);
  rp->sector_type = tmp;

  if (tmp == -1) {
    fscanf(fl, " %ld", &tmp);
    rp->tele_time = tmp;
    fscanf(fl, " %ld", &tmp);
    rp->tele_targ = tmp;
    fscanf(fl, " %ld", &tmp);
    rp->tele_mask = tmp;
    if (IS_SET(TELE_COUNT, rp->tele_mask)) {
      fscanf(fl, "%ld ", &tmp);
      rp->tele_cnt = tmp;
    } else {
      rp->tele_cnt = 0;
    }
    fscanf(fl, " %ld", &tmp);
    rp->sector_type = tmp;
  } else {
    rp->tele_time = 0;
    rp->tele_targ = 0;
    rp->tele_mask = 0;
    rp->tele_cnt  = 0;
  }

  if (tmp == SECT_WATER_NOSWIM || tmp == SECT_UNDERWATER) { /* river */
    /* read direction and rate of flow */
    fscanf(fl, " %ld ", &tmp);
    rp->river_speed = tmp;
    fscanf(fl, " %ld ", &tmp);
    rp->river_dir = tmp;
  }

  if (rp->room_flags & TUNNEL) { /* read in mobile limit on tunnel */
    fscanf(fl, " %ld ", &tmp);
    rp->moblim = tmp;
  }

  rp->funct = 0;
  rp->light = 0; /* Zero light sources */

  for (tmp = 0; tmp <= 5; tmp++)
    rp->dir_option[ tmp ] = 0;

  rp->ex_description = 0;

  while (1 == fscanf(fl, " %s \n", chk)) {
    static char buf[ MAX_INPUT_LENGTH ];
    switch (*chk) {
    case 'D':
      setup_dir(fl, rp->number, atoi(chk + 1));
      bc += sizeof(struct room_direction_data);
      /*      bc += strlen(rp->dir_option[atoi(chk + 1)]->general_description);
            bc += strlen(rp->dir_option[atoi(chk + 1)]->keyword);
      */
      break;
    case 'E': /* extra description field */

      CREATE(new_descr, struct extra_descr_data, 1);
      bc += sizeof(struct extra_descr_data);

      new_descr->keyword = fread_string(fl);
      if (new_descr->keyword && *new_descr->keyword)
        bc += strlen(new_descr->keyword);
      else
        fprintf(stderr, "No keyword in room %d\n", rp->number);

      new_descr->description = fread_string(fl);
      if (new_descr->description && *new_descr->description)
        bc += strlen(new_descr->description);
      else
        fprintf(stderr, "No desc in room %d\n", rp->number);

      new_descr->next    = rp->ex_description;
      rp->ex_description = new_descr;
      break;
    case 'S': /* end of current room */

#if BYTE_COUNT
      if (bc >= 1000)
        fprintf(stderr, "Byte count for this room[%d]: %d\n", rp->number, bc);
#endif
      // total_bc += bc;
      room_count++;
#if 0
      if(IS_SET(rp->room_flags, SAVE_ROOM)) {
         saved_rooms[number_of_saved_rooms] = rp->number;
         number_of_saved_rooms++;
      }
      {
       FILE *fp;
       char buf[255];

       sprintf(buf, "world/%d", rp->number);
       fp = fopen(buf, "r");
       if(fp) {
          saved_rooms[number_of_saved_rooms] = rp->number;
          number_of_saved_rooms++;
          fclose(fp);
	}
      }
#endif
      return;
    default:
      sprintf(buf, "unknown auxiliary code `%s' in room load of #%d", chk,
              rp->number);
      fprintf(stderr, "%s\n", buf);
      break;
    }
  }
}

void boot_zones( ) {
  FILE *fl;
  int zon = 0, cmd_no = 0, expand, tmp, bc = 100, cc = 22;
  char *check, buf[ 81 ];

  if (!(fl = fopen(ZONE_FILE, "r"))) {
    perror("boot_zones");
    assert(0);
  }

  for (;;) {
    fscanf(fl, " #%*d\n");
    check = fread_string(fl);

    if (*check == '$')
      break; /* end of file */

    /* alloc a new zone */

    if (!zon)
      CREATE(zone_table, struct zone_data, bc);
    else if (zon >= bc) {
      if (!(zone_table = (struct zone_data *)realloc(
                zone_table, (zon + 10) * sizeof(struct zone_data)))) {
        perror("boot_zones realloc");
        assert(0);
      }
      bc += 10;
    }
    zone_table[ zon ].name = check;
    fscanf(fl, " %d ", &zone_table[ zon ].top);
    fscanf(fl, " %d ", &zone_table[ zon ].lifespan);
    fscanf(fl, " %d ", &zone_table[ zon ].reset_mode);
    /* read the command table */

    /*
      new code to allow the game to be 'static' i.e. all the mobs are saved
    in one big zone file, and restored later.
    */

    cmd_no = 0;

    if (zon == 0)
      cc = 20;

    for (expand = 1;;) {
      if (expand) {
        if (!cmd_no) {
          CREATE(zone_table[ zon ].cmd, struct reset_com, cc);
        } else if (cmd_no >= cc) {
          cc += 5;
          if (!(zone_table[ zon ].cmd = (struct reset_com *)realloc(
                    zone_table[ zon ].cmd, (cc * sizeof(struct reset_com))))) {
            perror("reset command load");
            assert(0);
          }
        }
      }

      expand = 1;

      fscanf(fl, " "); /* skip blanks */
      fscanf(fl, "%c", &zone_table[ zon ].cmd[ cmd_no ].command);

      if (zone_table[ zon ].cmd[ cmd_no ].command == 'S')
        break;

      if (zone_table[ zon ].cmd[ cmd_no ].command == '*') {
        expand = 0;
        fgets(buf, 80, fl); /* skip command */
        continue;
      }

      fscanf(fl, " %d %d %d", &tmp, &zone_table[ zon ].cmd[ cmd_no ].arg1,
             &zone_table[ zon ].cmd[ cmd_no ].arg2);

      zone_table[ zon ].cmd[ cmd_no ].if_flag = tmp;

      if (zone_table[ zon ].cmd[ cmd_no ].command == 'M' ||
          zone_table[ zon ].cmd[ cmd_no ].command == 'O' ||
          zone_table[ zon ].cmd[ cmd_no ].command == 'C' ||
          zone_table[ zon ].cmd[ cmd_no ].command == 'E' ||
          zone_table[ zon ].cmd[ cmd_no ].command == 'P' ||
          zone_table[ zon ].cmd[ cmd_no ].command == 'D')
        fscanf(fl, " %d", &zone_table[ zon ].cmd[ cmd_no ].arg3);

      fgets(buf, 80, fl); /* read comment */
      cmd_no++;
    }
    zon++;
    if (zon == 1) {
      /* fix the cheat */
      /*      if (fl != tmp_fl && fl != 0)
              fclose(fl);
            fl = tmp_fl;*/
    }
  }
  top_of_zone_table = --zon;
  free(check);
  fclose(fl);
}

void init_world(struct room_data *room_db[]) {

  bzero(room_db,
        sizeof(struct room_data *) * WORLD_SIZE); /* zero out the world */
}

struct room_data *room_find(struct room_data *room_db[], int key) {
  return ((key < WORLD_SIZE && key > -1) ? room_db[ key ] : 0);
}

struct room_data *room_find_or_create(struct room_data *rb[], int key) {
  struct room_data *rv;

  rv = room_find(rb, key);
  if (rv)
    return rv;

  rv = (struct room_data *)malloc(sizeof(struct room_data));

  rb[ key ] = rv;

  return rv;
}

void allocate_room(int room_number) {
  if (room_number > top_of_world)
    top_of_world = room_number;
  room_find_or_create(room_db, room_number);
}

void boot_world( ) {
  FILE *fl;
  int virtual_nr, last;
  struct room_data *rp;

  init_world(room_db);

  // character_list = 0;
  // object_list    = 0;

  if (!(fl = fopen(WORLD_FILE, "r"))) {
    perror("fopen");
    fprintf(stderr, "boot_world: could not open world file.\n");
    assert(0);
  }

  last = 0;
  while (1 == fscanf(fl, " #%d\n", &virtual_nr)) {
    allocate_room(virtual_nr);
    rp = real_roomp(virtual_nr);
    if (rp)
      bzero(rp, sizeof(*rp));
    else {
      fprintf(stderr, "Error, room %d not in database!(%d)\n", virtual_nr,
              last);
      assert(0);
    }

    rp->number = virtual_nr;
    load_one_room(fl, rp);
    last = virtual_nr;
  }

  fclose(fl);
}

/*
{
  "number": 4000,
  "zone": 1,
  "continent": 0,
  "sector_type": SECT_INSIDE,
  "river": {
    "direction": "north",
    "speed": 3,
  }.
  "teleport": {
    "time": 3,
    "target": 4002,
    "flags": [],
    "countdown": 0
  },
  "mobile_limit": 12,
  "name": "...",
  "description": "...",
  "extra_description": {

  },
  "exits": {
    "description": "...",
    "keyword": "...",
    "key": -1,
    "to_room": -1,
    "exit_info": flags - EX_ISDOOR, etc
  },
  "light": 0,
  "dark": 0

}
*/
