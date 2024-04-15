#include <jansson.h>
#include <mach/memory_object_types.h>
#include <stdio.h>

#include "conversion.h"
#include "export.h"

json_t *direction_from_number(int direction);
json_t *sector_type_from_number(int type);

const char *direction_string_from_number(int direction) {
  switch (direction) {
  case 0:
    return "NORTH";

  case 1:
    return "EAST";

  case 2:
    return "SOUTH";

  case 3:
    return "WEST";

  case 4:
    return "UP";

  case 5:
    return "DOWN";
  }

  return NULL;
}

json_t *direction_from_number(int direction) {
  json_t *ret = NULL;

  switch (direction) {
  case 0:
    ret = json_string("NORTH");
    break;

  case 1:
    ret = json_string("EAST");
    break;

  case 2:
    ret = json_string("SOUTH");
    break;

  case 3:
    ret = json_string("WEST");
    break;

  case 4:
    ret = json_string("UP");
    break;

  case 5:
    ret = json_string("DOWN");
    break;
  }

  return ret;
}

json_t *door_state(unsigned int state) {
  json_t *array = json_array( );

  if (state == 0) {
    json_array_append(array, json_string("DOOR"));
  }

  if (state == 1) {
    json_array_append(array, json_string("DOOR"));
    json_array_append(array, json_string("CLOSED"));
  }

  if (state == 2) {
    json_array_append(array, json_string("DOOR"));
    json_array_append(array, json_string("CLOSED"));
    json_array_append(array, json_string("LOCKED"));
  }

  return array;
}

json_t *sector_type_from_number(int type) {
  json_t *sector_type;

  switch (type) {
  case SECT_INSIDE:
    sector_type = json_string("INSIDE");
    break;

  case SECT_CITY:
    sector_type = json_string("CITY");
    break;

  case SECT_FIELD:
    sector_type = json_string("FIELD");
    break;

  case SECT_FOREST:
    sector_type = json_string("FOREST");
    break;

  case SECT_HILLS:
    sector_type = json_string("HILLS");
    break;

  case SECT_MOUNTAIN:
    sector_type = json_string("MOUNTAIN");
    break;

  case SECT_WATER_SWIM:
    sector_type = json_string("WATER SWIM");
    break;

  case SECT_WATER_NOSWIM:
    sector_type = json_string("WATER NOSWIM");
    break;

  case SECT_AIR:
    sector_type = json_string("AIR");
    break;

  case SECT_UNDERWATER:
    sector_type = json_string("UNDERWATER");
    break;

  case SECT_DESERT:
    sector_type = json_string("DESERT");
    break;

  case SECT_TREE:
    sector_type = json_string("TREE");
    break;

  default:
    sector_type = json_string("UNKNOWN");
  }

  return sector_type;
}

json_t *room_flags(int room_flags) {
  json_t *flags = json_array( );

  if (room_flags & DARK) {
    json_t *flag = json_string("DARK");
    json_array_append(flags, flag);
  }

  if (room_flags & DEATH) {
    json_t *flag = json_string("DEATH");
    json_array_append(flags, flag);
  }

  if (room_flags & NO_MOB) {
    json_t *flag = json_string("NO_MOB");
    json_array_append(flags, flag);
  }

  if (room_flags & INDOORS) {
    json_t *flag = json_string("INDOORS");
    json_array_append(flags, flag);
  }

  if (room_flags & PEACEFUL) {
    json_t *flag = json_string("PEACEFUL");
    json_array_append(flags, flag);
  }

  if (room_flags & NOSTEAL) {
    json_t *flag = json_string("NO_STEAL");
    json_array_append(flags, flag);
  }

  if (room_flags & NO_SUM) {
    json_t *flag = json_string("NO_SUMMONING");
    json_array_append(flags, flag);
  }

  if (room_flags & NO_MAGIC) {
    json_t *flag = json_string("NO_MAGIC");
    json_array_append(flags, flag);
  }

  if (room_flags & TUNNEL) {
    json_t *flag = json_string("TUNNEL");
    json_array_append(flags, flag);
  }

  if (room_flags & PRIVATE) {
    json_t *flag = json_string("PRIVATE");
    json_array_append(flags, flag);
  }

  if (room_flags & SILENCE) {
    json_t *flag = json_string("SILENCE");
    json_array_append(flags, flag);
  }

  if (room_flags & LARGE) {
    json_t *flag = json_string("LARGE");
    json_array_append(flags, flag);
  }

  if (room_flags & NO_DEATH) {
    json_t *flag = json_string("NO_DEATH");
    json_array_append(flags, flag);
  }

  if (room_flags & SAVE_ROOM) {
    json_t *flag = json_string("SAVE_ROOM");
    json_array_append(flags, flag);
  }

  return flags;
}

json_t *direction_data(struct room_direction_data *dir) {
  json_t *direction = json_object( );

  if (dir->general_description && dir->general_description[ 0 ] != '\0') {
    json_t *description = json_string(dir->general_description);
    json_object_set(direction, "description", description);
  }

  if (dir->keyword && dir->keyword[ 0 ] != '\0') {
    json_t *keyword = json_string(dir->keyword);
    json_object_set(direction, "keyword", keyword);
  }

  if (dir->key != -1) {
    json_t *key = json_integer(dir->key);
    json_object_set(direction, "key", key);
  }

  json_t *to_room = json_integer(dir->to_room);
  json_object_set(direction, "to_room", to_room);

  if (dir->exit_info) {
    json_t *flags = json_array( );

    if (dir->exit_info & EX_ISDOOR) {
      json_t *flag = json_string("DOOR");
      json_array_append(flags, flag);
    }

    if (dir->exit_info & EX_CLOSED) {
      json_t *flag = json_string("CLOSED");
      json_array_append(flags, flag);
    }

    if (dir->exit_info & EX_LOCKED) {
      json_t *flag = json_string("LOCKED");
      json_array_append(flags, flag);
    }

    if (dir->exit_info & EX_SECRET) {
      json_t *flag = json_string("SECRET");
      json_array_append(flags, flag);
    }

    if (dir->exit_info & EX_PICKPROOF) {
      json_t *flag = json_string("PICKPROOF");
      json_array_append(flags, flag);
    }

    if (dir->exit_info & EX_CLIMB) {
      json_t *flag = json_string("CLIMB");
      json_array_append(flags, flag);
    }

    json_object_set(direction, "flags", flags);
  }

  return direction;
}

json_t *export_room(struct room_data *room) {
  json_t *root = json_object( );

  json_t *room_number = json_integer(room->number);
  json_object_set(root, "number", room_number);

  json_t *zone = json_integer(room->zone);
  json_object_set(root, "zone", zone);

  json_t *continent = json_integer(room->continent);
  json_object_set(root, "continent", continent);

  json_t *sector_type = sector_type_from_number(room->sector_type);
  if (sector_type) {
    json_object_set(root, "sector_type", sector_type);
  }

  if (room->river_dir != -1) {
    json_t *river = json_object( );

    json_t *river_direction = direction_from_number(room->river_dir);
    json_object_set(river, "direction", river_direction);

    json_t *river_speed = json_integer(room->river_speed);
    json_object_set(river, "speed", river_speed);

    json_object_set(root, "river", river);
  }

  if (room->tele_time != -1) {
    json_t *teleport = json_object( );

    json_t *tele_time = json_integer(room->tele_time);
    json_object_set(teleport, "time", tele_time);

    json_t *tele_target = json_integer(room->tele_targ);
    json_object_set(teleport, "target", tele_target);

    json_t *tele_count = json_integer(room->tele_cnt);
    json_object_set(teleport, "count", tele_count);

    json_t *tele_flags = json_array( );

    if (room->tele_mask & TELE_LOOK) {
      json_t *value = json_string("LOOK");
      json_array_append(tele_flags, value);
    }

    if (room->tele_mask & TELE_COUNT) {
      json_t *value = json_string("COUNT");
      json_array_append(tele_flags, value);
    }

    if (room->tele_mask & TELE_RANDOM) {
      json_t *value = json_string("RANDOM");
      json_array_append(tele_flags, value);
    }

    if (room->tele_mask & TELE_SPIN) {
      json_t *value = json_string("SPIN");
      json_array_append(tele_flags, value);
    }
  }

  json_t *mob_limit = json_integer(room->moblim);
  json_object_set(root, "mob_limit", mob_limit);

  json_t *name = json_string(room->name);
  json_object_set(root, "name", name);

  json_t *description = json_string(room->description);
  json_object_set(root, "description", description);

  if (room->ex_description) {
    json_t *obj = json_object( );

    for (struct extra_descr_data *d = room->ex_description; d != NULL;
         d                          = d->next) {
      json_t *description = json_string(d->description);
      json_object_set(obj, d->keyword, description);
    }

    json_object_set(root, "extra_descriptions", obj);
  }

  json_t *exits = json_object( );

  for (int i = 0; i < 6; i++) {
    if (room->dir_option[ i ]) {
      json_t *data = direction_data(room->dir_option[ i ]);

      json_object_set(exits, direction_string_from_number(i), data);
    }
  }

  json_object_set(root, "exits", exits);

  json_t *flags = room_flags(room->room_flags);
  json_object_set(root, "flags", flags);

  json_t *light = json_integer(room->light);
  json_object_set(root, "light", light);

  json_t *dark = json_integer(room->dark);
  json_object_set(root, "dark", dark);

  return root;
}

json_t *zone_command(char command) {
  json_t *cmd = NULL;

  switch (command) {
  case 'M':
    cmd = json_string("MOBILE_READ");
    break;

  case 'C':
    cmd = json_string("MOBILE_READ_CHARM");
    break;

  case 'Z':
    cmd = json_string("SET_LAST_MOBILE");
    break;

  case 'O':
    cmd = json_string("READ_OBJECT");
    break;

  case 'P':
    cmd = json_string("OBJECT_TO_OBJECT");
    break;

  case 'G':
    cmd = json_string("OBJECT_TO_CHAR");
    break;

  case 'H':
    cmd = json_string("HATRED_TO_CHAR");
    break;

  case 'F':
    cmd = json_string("FEAR_TO_CHAR");
    break;

  case 'E':
    cmd = json_string("OBJECT_TO_EQUIPMENT_LIST");
    break;

  case 'D':
    cmd = json_string("SET_STATE_OF_DOOR");
    break;
  }

  return cmd;
}

json_t *arguments_for_command(struct reset_com *command) {
  json_t *arg1 = NULL, *arg2 = NULL, *arg3 = NULL;

  switch (command->command) {
  case 'M':
    arg1 = json_integer(command->arg1);
    arg2 = json_integer(command->arg2);
    arg3 = json_integer(command->arg3);
    break;

  case 'C':
    arg1 = json_integer(command->arg1);
    arg2 = json_integer(command->arg2);
    arg3 = act(command->arg3);
    break;

  case 'Z':
    arg1 = json_integer(command->arg1);
    break;

  case 'O':
    arg1 = json_integer(command->arg1);
    arg2 = json_integer(command->arg2);
    arg3 = json_integer(command->arg3);
    break;

  case 'P':
    arg1 = json_integer(command->arg1);
    arg2 = json_integer(command->arg2);
    arg3 = json_integer(command->arg3);
    break;

  case 'G':
    arg1 = json_integer(command->arg1);
    arg2 = json_integer(command->arg2);
    arg3 = json_integer(command->arg3);
    break;

  case 'H':
    arg1 = hates_name(command->arg1);
    arg2 = hates_value(command->arg1, command->arg2);
    break;

  case 'F':
    arg1 = fears_name(command->arg1);
    arg2 = fears_value(command->arg1, command->arg2);
    break;

  case 'E':
    arg1 = json_integer(command->arg1);
    arg2 = json_integer(command->arg2);
    arg3 = equipment_slot(command->arg3);
    break;

  case 'D':
    arg1 = json_integer(command->arg1);
    arg2 = direction_from_number(command->arg2);
    arg3 = door_state(command->arg3);
    break;
  }

  json_t *arguments = json_object( );

  json_object_set(arguments, "arg1", arg1);
  json_object_set(arguments, "arg2", arg2);
  json_object_set(arguments, "arg3", arg3);

  return arguments;
}

json_t *reset_mode(int reset_mode) {
  json_t *array = json_array( );

  if (reset_mode & ZONE_NEVER) {
    json_array_append(array, json_string("NEVER"));
  }

  if (reset_mode & ZONE_EMPTY) {
    json_array_append(array, json_string("EMPTY"));
  }

  if (reset_mode & ZONE_ALWAYS) {
    json_array_append(array, json_string("ALWAYS"));
  }

  if (reset_mode & ZONE_ASTRAL) {
    json_array_append(array, json_string("ASTRAL"));
  }

  if (reset_mode & ZONE_DESERT) {
    json_array_append(array, json_string("DESERT"));
  }

  if (reset_mode & ZONE_ARCTIC) {
    json_array_append(array, json_string("ARCTIC"));
  }

  if (reset_mode & ZONE_HADES) {
    json_array_append(array, json_string("HADES"));
  }

  if (reset_mode & ZONE_OLYMPUS) {
    json_array_append(array, json_string("OLYMPUS"));
  }

  if (reset_mode & ZONE_ABYSS) {
    json_array_append(array, json_string("ABYSS"));
  }

  if (reset_mode & ZONE_PMP) {
    json_array_append(array, json_string("PRIME_MATERIAL_PLANE"));
  }

  if (reset_mode & ZONE_LIMBO) {
    json_array_append(array, json_string("LIMBO"));
  }

  return array;
}

json_t *export_zone(struct zone_data *zone, long zone_number) {
  json_t *root = json_object( );

  json_object_set(root, "zone_number", json_integer(zone_number));

  json_t *name = json_string(zone->name);
  json_object_set(root, "name", name);

  json_t *lifespan = json_integer(zone->lifespan);
  json_object_set(root, "lifespan", lifespan);

  json_t *top = json_integer(zone->top);
  json_object_set(root, "top", top);

  json_object_set(root, "reset_mode", reset_mode(zone->reset_mode));

  if (zone->cmd) {
    json_t *reset_array   = json_array( );
    struct reset_com *cmd = zone->cmd;

    for (void *iter = cmd; iter; iter += sizeof(struct reset_com)) {
      json_t *reset_command = json_object( );
      json_t *command       = zone_command(((struct reset_com *)iter)->command);

      if (command == NULL) {
        break;
      }

      json_object_set(reset_command, "command", command);

      struct reset_com *cmd = (struct reset_com *)iter;
      json_t *if_flag       = json_boolean(cmd->if_flag);
      json_object_set(reset_command, "if_flag", if_flag);

      json_object_set(reset_command, "arguments", arguments_for_command(cmd));

      json_array_append(reset_array, reset_command);
    }

    json_object_set(root, "reset_commands", reset_array);
  }

  json_t *current_total_gold = json_real(zone->CurrTotGold);
  json_object_set(root, "current_total_gold", current_total_gold);

  json_t *last_total_gold = json_real(zone->LastTotGold);
  json_object_set(root, "last_total_gold", last_total_gold);

  json_t *current_total_pc_gold = json_real(zone->CurrToPCGold);
  json_object_set(root, "current_total_pc_gold", current_total_pc_gold);

  json_t *last_total_pc_gold = json_real(zone->LastToPCGold);
  json_object_set(root, "last_total_pc_gold", last_total_pc_gold);

  return root;
}

const char *affected_bits[] = { "BLIND",
                                "INVISIBLE",
                                "DETECT-EVIL",
                                "DETECT-INVISIBLE",
                                "DETECT-MAGIC",
                                "SENSE-LIFE",
                                "HOLD",
                                "SANCTUARY",
                                "DRAGON-RIDE",
                                "GROWTH",
                                "CURSE",
                                "FLYING",
                                "POISON",
                                "TREE-TRAVEL",
                                "PARALYSIS",
                                "INFRAVISION",
                                "WATER-BREATH",
                                "SLEEP",
                                "TRAVELING",
                                "SNEAK",
                                "HIDE",
                                "SILENCE",
                                "CHARM",
                                "FOLLOW",
                                "SAVED-OBJECTS",
                                "TRUE-SIGHT",
                                "SCRYING",
                                "FIRESHIELD",
                                "GROUP",
                                "UNDEF_6",
                                "TEAM-GREY",
                                "TEAM-AMBER",
                                "\n" };

json_t *sprintbit(unsigned long vector, const char *names[]) {
  long nr;
  json_t *array = json_array( );

  for (nr = 0; vector; vector >>= 1) {
    if (IS_SET(1, vector)) {
      if (*names[ nr ] != '\n') {
        json_array_append(array, json_string(names[ nr ]));
      }
    }

    if (*names[ nr ] != '\n') {
      nr++;
    }
  }

  return array;
}

json_t *obj_flag_data(struct obj_flag_data *flags) {
  json_t *obj = json_object( );

  json_t *values = json_array( );
  for (int i = 0; i < 4; i++) {
    json_t *value = json_integer(flags->value[ i ]);
    json_array_append(values, value);
  }

  json_t *type = NULL;

  switch (flags->type_flag) {
  case ITEM_LIGHT:
    type = json_string("LIGHT");
    break;

  case ITEM_SCROLL:
    type = json_string("SCROLL");
    break;

  case ITEM_WAND:
    type = json_string("WAND");
    break;

  case ITEM_STAFF:
    type = json_string("STAFF");
    break;

  case ITEM_WEAPON:
    type = json_string("WEAPON");
    break;

  case ITEM_FIREWEAPON:
    type = json_string("FIREWEAPON");
    break;

  case ITEM_MISSILE:
    type = json_string("MISSILE");
    break;

  case ITEM_TREASURE:
    type = json_string("TREASURE");
    break;

  case ITEM_ARMOR:
    type = json_string("ARMOR");
    break;

  case ITEM_POTION:
    type = json_string("POTION");
    break;

  case ITEM_WORN:
    type = json_string("WORN");
    break;

  case ITEM_OTHER:
    type = json_string("OTHER");
    break;

  case ITEM_TRASH:
    type = json_string("TRASH");
    break;

  case ITEM_TRAP:
    type = json_string("TRAP");
    break;

  case ITEM_CONTAINER:
    type = json_string("CONTAINER");
    break;

  case ITEM_NOTE:
    type = json_string("NOTE");
    break;

  case ITEM_DRINKCON:
    type = json_string("DRINK_CONTAINER");
    break;

  case ITEM_KEY:
    type = json_string("KEY");
    break;

  case ITEM_FOOD:
    type = json_string("FOOD");
    break;

  case ITEM_MONEY:
    type = json_string("MONEY");
    break;

  case ITEM_PEN:
    type = json_string("PEN");
    break;

  case ITEM_BOAT:
    type = json_string("BOAT");
    break;

  case ITEM_AUDIO:
    type = json_string("AUDIO");
    break;

  case ITEM_BOARD:
    type = json_string("BOARD");
    break;

  case ITEM_TREE:
    type = json_string("TREE");
    break;

  case ITEM_ROCK:
    type = json_string("ROCK");
    break;

  case ITEM_WARPSTONE:
    type = json_string("WARPSTONE");
    break;
  }

  if (type) {
    json_object_set(obj, "type", type);
  }

  json_t *wear_flags = json_array( );

  if (flags->wear_flags & ITEM_TAKE) {
    json_array_append(wear_flags, json_string("TAKE"));
  }

  if (flags->wear_flags & ITEM_WEAR_FINGER) {
    json_array_append(wear_flags, json_string("WEAR_FINGER"));
  }

  if (flags->wear_flags & ITEM_WEAR_NECK) {
    json_array_append(wear_flags, json_string("WEAR_NECK"));
  }

  if (flags->wear_flags & ITEM_WEAR_BODY) {
    json_array_append(wear_flags, json_string("WEAR_BODY"));
  }

  if (flags->wear_flags & ITEM_WEAR_HEAD) {
    json_array_append(wear_flags, json_string("WEAR_HEAD"));
  }

  if (flags->wear_flags & ITEM_WEAR_LEGS) {
    json_array_append(wear_flags, json_string("WEAR_LEGS"));
  }

  if (flags->wear_flags & ITEM_WEAR_FEET) {
    json_array_append(wear_flags, json_string("WEAR_FEET"));
  }

  if (flags->wear_flags & ITEM_WEAR_HANDS) {
    json_array_append(wear_flags, json_string("WEAR_HANDS"));
  }

  if (flags->wear_flags & ITEM_WEAR_ARMS) {
    json_array_append(wear_flags, json_string("WEAR_ARMS"));
  }

  if (flags->wear_flags & ITEM_WEAR_SHIELD) {
    json_array_append(wear_flags, json_string("WEAR_SHIELD"));
  }

  if (flags->wear_flags & ITEM_WEAR_ABOUT) {
    json_array_append(wear_flags, json_string("WEAR_ABOUT"));
  }

  if (flags->wear_flags & ITEM_WEAR_WAISTE) {
    json_array_append(wear_flags, json_string("WEAR_WAIST"));
  }

  if (flags->wear_flags & ITEM_WEAR_WRIST) {
    json_array_append(wear_flags, json_string("WEAR_WRIST"));
  }

  if (flags->wear_flags & ITEM_WIELD) {
    json_array_append(wear_flags, json_string("WIELD"));
  }

  if (flags->wear_flags & ITEM_HOLD) {
    json_array_append(wear_flags, json_string("HOLD"));
  }

  if (flags->wear_flags & ITEM_THROW) {
    json_array_append(wear_flags, json_string("THROW"));
  }

  json_object_set(obj, "wear_flags", wear_flags);

  json_t *extra_flags = json_array( );

  if (flags->extra_flags & ITEM_GLOW) {
    json_array_append(extra_flags, json_string("GLOW"));
  }

  if (flags->extra_flags & ITEM_HUM) {
    json_array_append(extra_flags, json_string("HUM"));
  }

  if (flags->extra_flags & ITEM_METAL) {
    json_array_append(extra_flags, json_string("METAL"));
  }

  if (flags->extra_flags & ITEM_MINERAL) {
    json_array_append(extra_flags, json_string("MINERAL"));
  }

  if (flags->extra_flags & ITEM_ORGANIC) {
    json_array_append(extra_flags, json_string("ORGANIC"));
  }

  if (flags->extra_flags & ITEM_INVISIBLE) {
    json_array_append(extra_flags, json_string("INVISIBLE"));
  }

  if (flags->extra_flags & ITEM_MAGIC) {
    json_array_append(extra_flags, json_string("MAGIC"));
  }

  if (flags->extra_flags & ITEM_NODROP) {
    json_array_append(extra_flags, json_string("NODROP"));
  }

  if (flags->extra_flags & ITEM_BLESS) {
    json_array_append(extra_flags, json_string("BLESS"));
  }

  if (flags->extra_flags & ITEM_ANTI_GOOD) {
    json_array_append(extra_flags, json_string("ANTI_GOOD"));
  }

  if (flags->extra_flags & ITEM_ANTI_EVIL) {
    json_array_append(extra_flags, json_string("ANTI_EVIL"));
  }

  if (flags->extra_flags & ITEM_ANTI_NEUTRAL) {
    json_array_append(extra_flags, json_string("ANTI_NEUTRAL"));
  }

  if (flags->extra_flags & ITEM_ANTI_CLERIC) {
    json_array_append(extra_flags, json_string("ANTI_CLERIC"));
  }

  if (flags->extra_flags & ITEM_ANTI_MAGE) {
    json_array_append(extra_flags, json_string("ANTI_MAGE"));
  }

  if (flags->extra_flags & ITEM_ANTI_THIEF) {
    json_array_append(extra_flags, json_string("ANTI_THIEF"));
  }

  if (flags->extra_flags & ITEM_ANTI_FIGHTER) {
    json_array_append(extra_flags, json_string("ANTI_FIGHTER"));
  }

  if (flags->extra_flags & ITEM_BRITTLE) {
    json_array_append(extra_flags, json_string("BRITTLE"));
  }

  if (flags->extra_flags & ITEM_RESISTANT) {
    json_array_append(extra_flags, json_string("RESISTANT"));
  }

  if (flags->extra_flags & ITEM_ILLUSION) {
    json_array_append(extra_flags, json_string("ILLUSION"));
  }

  if (flags->extra_flags & ITEM_ANTI_MEN) {
    json_array_append(extra_flags, json_string("ANTI_MEN"));
  }

  if (flags->extra_flags & ITEM_ANTI_WOMEN) {
    json_array_append(extra_flags, json_string("ANTI_WOMEN"));
  }

  if (flags->extra_flags & ITEM_ANTI_SUN) {
    json_array_append(extra_flags, json_string("ANTI_SUN"));
  }

  if (flags->extra_flags & ITEM_INSET) {
    json_array_append(extra_flags, json_string("INSET"));
  }

  if (flags->extra_flags & ITEM_FIGURINE) {
    json_array_append(extra_flags, json_string("FIGURINE"));
  }

  json_object_set(obj, "extra_flags", extra_flags);

  json_object_set(obj, "weight", json_integer(flags->weight));
  json_object_set(obj, "cost", json_integer(flags->cost));
  json_object_set(obj, "cost_per_day", json_integer(flags->cost_per_day));
  json_object_set(obj, "timer", json_integer(flags->timer));

  json_object_set(obj, "char_affects",
                  sprintbit(flags->bitvector, affected_bits));

  return obj;
}

json_t *obj_affected_type(struct obj_affected_type *affected) {
  json_t *array = json_array( );

  for (int i = 0; i < MAX_OBJ_AFFECT; i++) {
    json_t *obj = json_object( );

    if (affected->location == APPLY_NONE) {
      continue;
    }

    switch (affected->location) {
    case APPLY_STR:
      json_object_set(obj, "apply", json_string("STR"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_DEX:
      json_object_set(obj, "apply", json_string("DEX"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_INT:
      json_object_set(obj, "apply", json_string("INT"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_WIS:
      json_object_set(obj, "apply", json_string("WIS"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_CON:
      json_object_set(obj, "apply", json_string("CON"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_CHR:
      json_object_set(obj, "apply", json_string("CHR"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SEX:
      json_object_set(obj, "apply", json_string("SEX"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_LEVEL:
      json_object_set(obj, "apply", json_string("LEVEL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_AGE:
      json_object_set(obj, "apply", json_string("AGE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_CHAR_WEIGHT:
      json_object_set(obj, "apply", json_string("WEIGHT"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_CHAR_HEIGHT:
      json_object_set(obj, "apply", json_string("HEIGHT"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_MANA:
      json_object_set(obj, "apply", json_string("MANA"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_HIT:
      json_object_set(obj, "apply", json_string("HIT"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_MOVE:
      json_object_set(obj, "apply", json_string("MOVE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_GOLD:
      json_object_set(obj, "apply", json_string("GOLD"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_EXP:
      json_object_set(obj, "apply", json_string("EXP"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_AC:
      json_object_set(obj, "apply", json_string("AC"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_HITROLL:
      json_object_set(obj, "apply", json_string("HITROLL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_DAMROLL:
      json_object_set(obj, "apply", json_string("DAMROLL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SAVING_PARA:
      json_object_set(obj, "apply", json_string("SAVING_PARA"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SAVING_ROD:
      json_object_set(obj, "apply", json_string("SAVING_ROD"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SAVING_PETRI:
      json_object_set(obj, "apply", json_string("SAVING_PETRI"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SAVING_BREATH:
      json_object_set(obj, "apply", json_string("SAVING_BREATH"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SAVING_SPELL:
      json_object_set(obj, "apply", json_string("SAVING_SPELL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SAVE_ALL:
      json_object_set(obj, "apply", json_string("SAVE_ALL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_IMMUNE:
      json_object_set(obj, "apply", json_string("IMMUNE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SUSC:
      json_object_set(obj, "apply", json_string("SUSC"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_M_IMMUNE:
      json_object_set(obj, "apply", json_string("M_IMMUNE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SPELL:
      json_object_set(obj, "apply", json_string("SPELL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_WEAPON_SPELL:
      json_object_set(obj, "apply", json_string("WEAPON_SPELL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_EAT_SPELL:
      json_object_set(obj, "apply", json_string("EAT_SPELL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_BACKSTAB:
      json_object_set(obj, "apply", json_string("BACKSTAB"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_KICK:
      json_object_set(obj, "apply", json_string("KICK"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SNEAK:
      json_object_set(obj, "apply", json_string("SNEAK"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_HIDE:
      json_object_set(obj, "apply", json_string("HIDE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_BASH:
      json_object_set(obj, "apply", json_string("BASH"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_PICK:
      json_object_set(obj, "apply", json_string("PICK"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_STEAL:
      json_object_set(obj, "apply", json_string("STEAL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_TRACK:
      json_object_set(obj, "apply", json_string("TRACK"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_HITNDAM:
      json_object_set(obj, "apply", json_string("HIT_AND_DAMAGE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SPELLFAIL:
      json_object_set(obj, "apply", json_string("SPELLFAIL"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_ATTACKS:
      json_object_set(obj, "apply", json_string("ATTACKS"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_HASTE:
      json_object_set(obj, "apply", json_string("HASTE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_SLOW:
      json_object_set(obj, "apply", json_string("SLOW"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_BV2:
      json_object_set(obj, "apply", json_string("BV2"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_FIND_TRAPS:
      json_object_set(obj, "apply", json_string("FIND_TRAPS"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_RIDE:
      json_object_set(obj, "apply", json_string("RIDE"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_RACE_SLAYER:
      json_object_set(obj, "apply", json_string("RACE_SLAYER"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_ALIGN_SLAYER:
      json_object_set(obj, "apply", json_string("ALIGN_SLAYER"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_MANA_REGEN:
      json_object_set(obj, "apply", json_string("MANA_REGEN"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_HIT_REGEN:
      json_object_set(obj, "apply", json_string("HIT_REGEN"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_MOVE_REGEN:
      json_object_set(obj, "apply", json_string("MOVE_REGEN"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    case APPLY_MOVE_BONUS:
      json_object_set(obj, "apply", json_string("MOVE_BONUS"));
      json_object_set(obj, "modifier", json_integer(affected->modifier));
      json_array_append(array, obj);
      break;

    default:
      fprintf(stderr, "Unknown apply type: %ld", affected->modifier);
    }
  }

  return array;
}

json_t *export_object(struct obj_data *obj) {
  json_t *root = json_object( );

  json_t *number = json_integer(obj->item_number);
  json_object_set(root, "number", number);

  if (obj->in_room != -1) {
    json_t *in_room = json_integer(obj->in_room);
    json_object_set(root, "in_room", in_room);
  }

  json_object_set(root, "flags", obj_flag_data(&obj->obj_flags));
  json_object_set(root, "affected_type", obj_affected_type(obj->affected));

  json_object_set(root, "name", json_string(obj->name));
  json_object_set(root, "description", json_string(obj->description));
  json_object_set(root, "short_description",
                  json_string(obj->short_description));
  json_object_set(root, "action_description",
                  json_string(obj->action_description));

  if (obj->ex_description) {
    json_t *o = json_object( );

    for (struct extra_descr_data *d = obj->ex_description; d != NULL;
         d                          = d->next) {
      json_t *description = json_string(d->description);
      json_object_set(o, d->keyword, description);
    }

    json_object_set(root, "extra_descriptions", o);
  }

  return root;
}

json_t *get_race(int race) {
  json_t *r;

  switch (race) {
  case RACE_HALFBREED:
    return json_string("HALFBREED");
  case RACE_HUMAN:
    return json_string("HUMAN");
  case RACE_ELVEN:
    return json_string("ELVEN");
  case RACE_DWARF:
    return json_string("DWARF");
  case RACE_HALFLING:
    return json_string("HALFLING");
  case RACE_GNOME:
    return json_string("GNOME");
  case RACE_REPTILE:
    return json_string("REPTILE");
  case RACE_SPECIAL:
    return json_string("SPECIAL");
  case RACE_LYCANTH:
    return json_string("LYCANTH");
  case RACE_DRAGON:
    return json_string("DRAGON");
  case RACE_UNDEAD:
    return json_string("UNDEAD");
  case RACE_ORC:
    return json_string("ORC");
  case RACE_INSECT:
    return json_string("INSECT");
  case RACE_ARACHNID:
    return json_string("ARACHNID");
  case RACE_DINOSAUR:
    return json_string("DINOSAUR");
  case RACE_FISH:
    return json_string("FISH");
  case RACE_BIRD:
    return json_string("BIRD");
  case RACE_GIANT:
    return json_string("GIANT");
  case RACE_PREDATOR:
    return json_string("PREDATOR");
  case RACE_PARASITE:
    return json_string("PARASITE");
  case RACE_SLIME:
    return json_string("SLIME");
  case RACE_DEMON:
    return json_string("DEMON");
  case RACE_SNAKE:
    return json_string("SNAKE");
  case RACE_HERBIV:
    return json_string("HERBIVORE");
  case RACE_TREE:
    return json_string("TREE");
  case RACE_VEGGIE:
    return json_string("VEGGIE");
  case RACE_ELEMENT:
    return json_string("ELEMENT");
  case RACE_PLANAR:
    return json_string("PLANAR");
  case RACE_DEVIL:
    return json_string("DEVIL");
  case RACE_GHOST:
    return json_string("GHOST");
  case RACE_GOBLIN:
    return json_string("GOBLIN");
  case RACE_TROLL:
    return json_string("TROLL");
  case RACE_VEGMAN:
    return json_string("VEGMAN");
  case RACE_MFLAYER:
    return json_string("MINDFLAYER");
  case RACE_PRIMATE:
    return json_string("PRIMATE");
  case RACE_ENFAN:
    return json_string("ENFAN");
  case RACE_DROW:
    return json_string("DROW");
  case RACE_GOLEM:
    return json_string("GOLEM");
  case RACE_SKEXIE:
    return json_string("SKEXIE");
  case RACE_TROGMAN:
    return json_string("TROGMAN");
  case RACE_PATRYN:
    return json_string("PATRYN");
  case RACE_LABRAT:
    return json_string("LABRAT");
  case RACE_SARTAN:
    return json_string("SARTAN");
  case RACE_TYTAN:
    return json_string("TYTAN");
  case RACE_SMURF:
    return json_string("SMURF");
  case RACE_ROO:
    return json_string("ROO");
  case RACE_HORSE:
    return json_string("HORSE");
  case RACE_DRAAGDIM:
    return json_string("DRAAGDIM");
  case RACE_ASTRAL:
    return json_string("ASTRAL");
  case RACE_GOD:
    return json_string("GOD");
  case RACE_HALFELF:
    return json_string("HALFELF");
  case RACE_HALFORC:
    return json_string("HALFORC");
  case RACE_HUMANTWO:
    return json_string("HUMANTWO");
  case RACE_VAMPIRE:
    return json_string("VAMPIRE");
  case RACE_OGRE:
    return json_string("OGRE");
  case RACE_FAERIE:
    return json_string("FAERIE");
  }

  char buf[ 255 ];
  sprintf(buf, "UNKNOWN: %d", race);

  return json_string(buf);
}

json_t *immunities(unsigned int immune) {
  json_t *arr = json_array( );

  if (immune & IMM_FIRE) {
    json_array_append(arr, json_string("FIRE"));
  }

  if (immune & IMM_COLD) {
    json_array_append(arr, json_string("COLD"));
  }

  if (immune & IMM_ELEC) {
    json_array_append(arr, json_string("ELECTRICITY"));
  }

  if (immune & IMM_ENERGY) {
    json_array_append(arr, json_string("ENERGY"));
  }

  if (immune & IMM_BLUNT) {
    json_array_append(arr, json_string("BLUNT"));
  }

  if (immune & IMM_PIERCE) {
    json_array_append(arr, json_string("PIERCE"));
  }

  if (immune & IMM_SLASH) {
    json_array_append(arr, json_string("SLASH"));
  }

  if (immune & IMM_ACID) {
    json_array_append(arr, json_string("ACID"));
  }

  if (immune & IMM_POISON) {
    json_array_append(arr, json_string("POISON"));
  }

  if (immune & IMM_DRAIN) {
    json_array_append(arr, json_string("DRAIN"));
  }

  if (immune & IMM_SLEEP) {
    json_array_append(arr, json_string("SLEEP"));
  }

  if (immune & IMM_CHARM) {
    json_array_append(arr, json_string("CHARM"));
  }

  if (immune & IMM_HOLD) {
    json_array_append(arr, json_string("HOLD"));
  }

  if (immune & IMM_NONMAG) {
    json_array_append(arr, json_string("NON_MAGIC"));
  }

  if (immune & IMM_PLUS1) {
    json_array_append(arr, json_string("PLUS1"));
  }

  if (immune & IMM_PLUS2) {
    json_array_append(arr, json_string("PLUS2"));
  }

  if (immune & IMM_PLUS3) {
    json_array_append(arr, json_string("PLUS3"));
  }

  if (immune & IMM_PLUS4) {
    json_array_append(arr, json_string("PLUS4"));
  }

  return arr;
}

json_t *get_class(int class_no) {
  if (class_no == 0) {
    return NULL;
  }

  switch (class_no) {
  case CLASS_WARRIOR:
    return json_string("WARRIOR");
  case CLASS_MAGIC_USER:
    return json_string("MAGIC USER");
  case CLASS_CLERIC:
    return json_string("CLERIC");
  case CLASS_THIEF:
    return json_string("THIEF");
  case CLASS_DRUID:
    return json_string("DRUID");
  case CLASS_MONK:
    return json_string("MONK");
  case CLASS_ALL:
    return json_string("ALL");
  default: {
    char buf[ 255 ];

    sprintf(buf, "UNKNOWN: %d", class_no);
    return json_string(buf);
  }
  }
}

json_t *get_sex(int sex_no) {
  switch (sex_no) {
  case SEX_NEUTRAL:
    return json_string("NEUTRAL");
  case SEX_FEMALE:
    return json_string("FEMALE");
  case SEX_MALE:
    return json_string("MALE");
  default:
    return json_string("UNKNOWN");
  }
}

json_t *hates_name(unsigned int field) {
  if (field == HATE_VNUM) {
    return json_string("vnum");
  }

  if (field == HATE_CLASS) {
    return json_string("class");
  }

  if (field == HATE_EVIL) {
    return json_string("evil");
  }

  if (field == HATE_GOOD) {
    return json_string("good");
  }

  if (field == HATE_SEX) {
    return json_string("sex");
  }

  if (field == HATE_RACE) {
    return json_string("race");
  }

  return NULL;
}

json_t *hates_value(unsigned int field, unsigned int argument) {
  if (field == HATE_VNUM) {
    return json_integer(argument);
  }

  if (field == HATE_CLASS) {
    return get_class(argument);
  }

  if (field == HATE_EVIL) {
    return json_integer(argument);
  }

  if (field == HATE_GOOD) {
    return json_integer(argument);
  }

  if (field == HATE_SEX) {
    return get_sex(argument);
  }

  if (field == HATE_RACE) {
    return get_race(argument);
  }

  return NULL;
}

json_t *hates(unsigned int field, Opinion opinion, bool hateful) {
  json_t *obj = json_object( );

  if (field & HATE_VNUM) {
    json_object_set(obj, "vnum", json_integer(opinion.vnum));
  }

  if (field & HATE_CLASS) {
    json_object_set(obj, "class", get_class(opinion.class_type));
  }

  if (field & HATE_EVIL) {
    json_object_set(obj, "evil", json_boolean(opinion.evil));
  }

  if (field & HATE_GOOD) {
    json_object_set(obj, "good", json_boolean(opinion.good));
  }

  if (field & HATE_SEX) {
    json_object_set(obj, "sex", get_sex(opinion.sex));
  }

  if (field & HATE_RACE) {
    json_object_set(obj, "race", get_race(opinion.race));
  }

  if (hateful) {
    json_object_set(obj, "hateful", json_boolean(true));
  }

  return obj;
}

json_t *fears_name(unsigned int field) {
  if (field == FEAR_VNUM) {
    return json_string("vnum");
  }

  if (field == FEAR_CLASS) {
    return json_string("class");
  }

  if (field == FEAR_EVIL) {
    return json_string("evil");
  }

  if (field == FEAR_GOOD) {
    return json_string("good");
  }

  if (field == FEAR_SEX) {
    return json_string("sex");
  }

  if (field == FEAR_RACE) {
    return json_string("race");
  }

  return NULL;
}

json_t *fears_value(unsigned int field, unsigned int argument) {
  if (field == FEAR_VNUM) {
    return json_integer(argument);
  }

  if (field == FEAR_CLASS) {
    return get_class(argument);
  }

  if (field == FEAR_EVIL) {
    return json_integer(argument);
  }

  if (field == FEAR_GOOD) {
    return json_integer(argument);
  }

  if (field == FEAR_SEX) {
    return get_sex(argument);
  }

  if (field == FEAR_RACE) {
    return get_race(argument);
  }

  return NULL;
}

json_t *fears(unsigned int field, Opinion opinion) {
  json_t *obj = json_object( );

  if (field & FEAR_VNUM) {
    json_object_set(obj, "vnum", json_integer(opinion.vnum));
  }

  if (field & FEAR_CLASS) {
    json_object_set(obj, "class", get_class(opinion.class_type));
  }

  if (field & FEAR_EVIL) {
    json_object_set(obj, "evil", json_boolean(opinion.evil));
  }

  if (field & FEAR_GOOD) {
    json_object_set(obj, "good", json_boolean(opinion.good));
  }

  if (field & FEAR_SEX) {
    json_object_set(obj, "sex", get_sex(opinion.sex));
  }

  if (field & FEAR_RACE) {
    json_object_set(obj, "race", get_race(opinion.race));
  }

  return obj;
}

json_t *get_levels(byte levels[ 8 ]) {
  json_t *obj = json_object( );

  for (int i = 0; i < 6; i++) {
    byte level = levels[ i ];

    if (level != 0) {
      switch (i) {
      case 0:
        json_object_set(obj, "magic_user", json_integer(level));
        break;

      case 1:
        json_object_set(obj, "cleric", json_integer(level));
        break;

      case 2:
        json_object_set(obj, "warrior", json_integer(level));
        break;

      case 3:
        json_object_set(obj, "thief", json_integer(level));
        break;

      case 4:
        json_object_set(obj, "druid", json_integer(level));
        break;

      case 5:
        json_object_set(obj, "monk", json_integer(level));
        break;
      }
    }
  }

  return obj;
}

json_t *abilities(struct char_ability_data *abilities) {
  json_t *obj = json_object( );

  json_object_set(obj, "strength", json_integer(abilities->str));
  json_object_set(obj, "strength_add", json_integer(abilities->str_add));
  json_object_set(obj, "intelligence", json_integer(abilities->intel));
  json_object_set(obj, "wisdom", json_integer(abilities->wis));
  json_object_set(obj, "dexterity", json_integer(abilities->dex));
  json_object_set(obj, "constitution", json_integer(abilities->con));
  json_object_set(obj, "charisma", json_integer(abilities->chr));

  return obj;
}

json_t *points(struct char_point_data *points) {
  json_t *obj = json_object( );

  json_object_set(obj, "mana", json_integer(points->mana));
  json_object_set(obj, "max_mana", json_integer(points->max_mana));
  json_object_set(obj, "mana_gain", json_integer(points->mana_gain));

  json_object_set(obj, "hit", json_integer(points->hit));
  json_object_set(obj, "max_hit", json_integer(points->max_hit));
  json_object_set(obj, "hit_gain", json_integer(points->hit_gain));

  json_object_set(obj, "move", json_integer(points->move));
  json_object_set(obj, "max_move", json_integer(points->max_move));
  json_object_set(obj, "move_gain", json_integer(points->move_gain));

  json_object_set(obj, "armor", json_integer(points->armor));
  json_object_set(obj, "gold", json_integer(points->gold));

  json_object_set(obj, "hit_roll", json_integer(points->hitroll));
  json_object_set(obj, "damage_roll", json_integer(points->damroll));

  return obj;
}

json_t *get_position(byte position) {
  switch (position) {
  case POSITION_DEAD:
    return json_string("DEAD");

  case POSITION_MORTALLYW:
    return json_string("MORTALLY WOUNDED");

  case POSITION_INCAP:
    return json_string("INCAPACITATED");

  case POSITION_STUNNED:
    return json_string("STUNNED");

  case POSITION_SLEEPING:
    return json_string("SLEEPING");

  case POSITION_RESTING:
    return json_string("RESTING");

  case POSITION_SITTING:
    return json_string("INCAPACITATED");

  case POSITION_FIGHTING:
    return json_string("FIGHTING");

  case POSITION_STANDING:
    return json_string("STANDING");

  case POSITION_MOUNTED:
    return json_string("MOUNTED");

  default:
    return json_string("UNKNOWN");
  }
}

json_t *conditions(sbyte conditions[ 3 ]) {
  json_t *obj = json_object( );

  json_object_set(obj, "drunk", json_integer(conditions[ DRUNK ]));
  json_object_set(obj, "full", json_integer(conditions[ FULL ]));
  json_object_set(obj, "thirst", json_integer(conditions[ THIRST ]));

  return obj;
}

json_t *attack_type(int attack_type) {
  switch (attack_type) {
  case TYPE_HIT:
    return json_string("HIT");

  case TYPE_BLUDGEON:
    return json_string("BLUDGEON");

  case TYPE_PIERCE:
    return json_string("PIERCE");

  case TYPE_SLASH:
    return json_string("SLASH");

  case TYPE_WHIP:
    return json_string("WHIP");

  case TYPE_CLAW:
    return json_string("CLAW");

  case TYPE_BITE:
    return json_string("BITE");

  case TYPE_STING:
    return json_string("STING");

  case TYPE_CRUSH:
    return json_string("CRUSH");

  case TYPE_CLEAVE:
    return json_string("CLEAVE");

  case TYPE_STAB:
    return json_string("STAB");

  case TYPE_SMASH:
    return json_string("SMASH");

  case TYPE_SMITE:
    return json_string("SMITE");

  case TYPE_BLAST:
    return json_string("BLAST");

  case TYPE_SUFFERING:
    return json_string("SUFFERING");

  default:
    return json_string("UNKNOWN");
  }
}

json_t *affected_by(long affected_by, long affected_by2) {
  json_t *array = json_array( );

  if (affected_by & AFF_BLIND) {
    json_array_append(array, json_string("BLIND"));
  }

  if (affected_by & AFF_INVISIBLE) {
    json_array_append(array, json_string("INVISIBLE"));
  }

  if (affected_by & AFF_DETECT_EVIL) {
    json_array_append(array, json_string("DETECT_EVIL"));
  }

  if (affected_by & AFF_DETECT_INVISIBLE) {
    json_array_append(array, json_string("DETECT_INVISIBLE"));
  }

  if (affected_by & AFF_SENSE_LIFE) {
    json_array_append(array, json_string("SENSE_LIFE"));
  }

  if (affected_by & AFF_LIFE_PROT) {
    json_array_append(array, json_string("LIFE_PROTECTION"));
  }

  if (affected_by & AFF_SANCTUARY) {
    json_array_append(array, json_string("SANCTUARY"));
  }

  if (affected_by & AFF_DRAGON_RIDE) {
    json_array_append(array, json_string("DRAGON_RIDE"));
  }

  if (affected_by & AFF_GROWTH) {
    json_array_append(array, json_string("GROWTH"));
  }

  if (affected_by & AFF_CURSE) {
    json_array_append(array, json_string("CURSE"));
  }

  if (affected_by & AFF_FLYING) {
    json_array_append(array, json_string("FLYING"));
  }

  if (affected_by & AFF_POISON) {
    json_array_append(array, json_string("POISON"));
  }

  if (affected_by & AFF_TREE_TRAVEL) {
    json_array_append(array, json_string("TREE_TRAVEL"));
  }

  if (affected_by & AFF_PARALYSIS) {
    json_array_append(array, json_string("PARALYSIS"));
  }

  if (affected_by & AFF_INFRAVISION) {
    json_array_append(array, json_string("INFRAVISION"));
  }

  if (affected_by & AFF_WATERBREATH) {
    json_array_append(array, json_string("WATERBREATH"));
  }

  if (affected_by & AFF_SLEEP) {
    json_array_append(array, json_string("SLEEP"));
  }

  if (affected_by & AFF_TRAVELLING) {
    json_array_append(array, json_string("TRAVELING"));
  }

  if (affected_by & AFF_SNEAK) {
    json_array_append(array, json_string("SNEAK"));
  }

  if (affected_by & AFF_HIDE) {
    json_array_append(array, json_string("HIDE"));
  }

  if (affected_by & AFF_SILENCE) {
    json_array_append(array, json_string("SILENCE"));
  }

  if (affected_by & AFF_CHARM) {
    json_array_append(array, json_string("CHARM"));
  }

  if (affected_by & AFF_FOLLOW) {
    json_array_append(array, json_string("FOLLOW"));
  }

  if (affected_by & AFF_TRUE_SIGHT) {
    json_array_append(array, json_string("TRUE_SIGHT"));
  }

  if (affected_by & AFF_SCRYING) {
    json_array_append(array, json_string("SCRYING"));
  }

  if (affected_by & AFF_FIRESHIELD) {
    json_array_append(array, json_string("FIRESHIELD"));
  }

  if (affected_by2 & AFF2_ANIMAL_INVIS) {
    json_array_append(array, json_string("ANIMAL_INVISIBILITY"));
  }

  if (affected_by2 & AFF2_HEAT_STUFF) {
    json_array_append(array, json_string("HEAT_STUFF"));
  }

  if (affected_by2 & AFF2_SUN_BLIND) {
    json_array_append(array, json_string("SUN_BLIND"));
  }

  if (affected_by2 & AFF2_FEEDING) {
    json_array_append(array, json_string("FEEDING"));
  }

  if (affected_by2 & AFF2_BERSERK) {
    json_array_append(array, json_string("BERSERK"));
  }

  return array;
}

json_t *act(long act) {
  json_t *array = json_array( );

  if (act & ACT_SENTINEL) {
    json_array_append(array, json_string("SENTINEL"));
  }

  if (act & ACT_SCAVENGER) {
    json_array_append(array, json_string("SCAVENGER"));
  }

  if (act & ACT_NICE_THIEF) {
    json_array_append(array, json_string("NICE_THIEF"));
  }

  if (act & ACT_AGGRESSIVE) {
    json_array_append(array, json_string("AGGRESSIVE"));
  }

  if (act & ACT_STAY_ZONE) {
    json_array_append(array, json_string("STAY_ZONE"));
  }

  if (act & ACT_WIMPY) {
    json_array_append(array, json_string("WIMPY"));
  }

  if (act & ACT_ANNOYING) {
    json_array_append(array, json_string("ANNOYING"));
  }

  if (act & ACT_IMMORTAL) {
    json_array_append(array, json_string("IMMORTAL"));
  }

  if (act & ACT_DEADLY) {
    json_array_append(array, json_string("DEADLY"));
  }

  if (act & ACT_ILLUSION) {
    json_array_append(array, json_string("ILLUSION"));
  }

  if (act & ACT_HUGE) {
    json_array_append(array, json_string("HUGE"));
  }

  if (act & ACT_GREET) {
    json_array_append(array, json_string("GREET"));
  }

  return array;
}

json_t *saving_throw(sh_int apply_saving_throw[ 5 ]) {
  json_t *array = json_array( );

  for (int i = 0; i < 5; i++) {
    json_array_append(array, json_integer(apply_saving_throw[ i ]));
  }

  return array;
}

json_t *specials(struct char_special_data *specials) {
  json_t *obj = json_object( );

  json_object_set(obj, "spell_fail", json_integer(specials->spellfail));
  json_object_set(obj, "position", get_position(specials->position));
  json_object_set(obj, "default_position", get_position(specials->default_pos));

  json_object_set(obj, "damage_dice", json_integer(specials->damnodice));
  json_object_set(obj, "damage_dice_size", json_integer(specials->damsizedice));

  json_object_set(obj, "conditions", conditions(specials->conditions));

  json_object_set(obj, "zone", json_integer(specials->zone));
  json_object_set(obj, "attack_type", attack_type(specials->attack_type));

  json_object_set(obj, "wimpiness", json_integer(20));
  json_object_set(obj, "flee", json_integer(1));

  json_object_set(obj, "affected_by",
                  affected_by(specials->affected_by, specials->affected_by2));
  json_object_set(obj, "act", act(specials->act));

  json_object_set(obj, "intrinsics", affected_by(specials->intrinsics, 0));

  json_object_set(obj, "apply_saving_throw",
                  saving_throw(specials->apply_saving_throw));

  return obj;
}

json_t *equipment_slot(unsigned int slot) {
  switch (slot) {
  case WEAR_LIGHT:
    return json_string("LIGHT");

  case WEAR_FINGER_L:
    return json_string("LEFT FINGER");

  case WEAR_FINGER_R:
    return json_string("RIGHT FINGER");

  case WEAR_NECK_1:
    return json_string("NECK 1");

  case WEAR_NECK_2:
    return json_string("NECK 2");

  case WEAR_BODY:
    return json_string("BODY");

  case WEAR_HEAD:
    return json_string("HEAD");

  case WEAR_LEGS:
    return json_string("LEGS");

  case WEAR_ARMS:
    return json_string("ARMS");

  case WEAR_SHIELD:
    return json_string("SHIELD");

  case WEAR_ABOUT:
    return json_string("ABOUT");

  case WEAR_WAISTE:
    return json_string("WAIST");

  case WEAR_WRIST_L:
    return json_string("LEFT WRIST");

  case WEAR_WRIST_R:
    return json_string("RIGHT WRIST");

  case WIELD:
    return json_string("WIELD");

  case HOLD:
    return json_string("HOLD");

  default:
    return NULL;
  }
}

json_t *export_mob(struct char_data *chr) {
  json_t *root = json_object( );

  json_object_set(root, "number", json_integer(chr->nr));
  json_object_set(root, "size", json_integer(chr->size));

  json_object_set(root, "race", get_race(chr->race));
  json_object_set(root, "immunities", immunities(chr->immune));
  json_object_set(root, "M_immunities", immunities(chr->M_immune));
  json_object_set(root, "susceptabilities", immunities(chr->susc));

  json_object_set(root, "multi_attack", json_integer(chr->mult_att));
  json_object_set(root, "hunt_distance", json_integer(chr->hunt_dist));

  json_object_set(
      root, "hates",
      hates(chr->hatefield, chr->hates, chr->specials.act & ACT_HATEFUL));
  json_object_set(root, "fears", fears(chr->hatefield, chr->hates));

  /* player data */
  json_object_set(root, "sex", get_sex(chr->player.sex));
  json_object_set(root, "weight", json_integer(chr->player.weight));
  json_object_set(root, "height", json_integer(chr->player.height));
  json_object_set(root, "name", json_string(chr->player.name));
  json_object_set(root, "short_description",
                  json_string(chr->player.short_descr));
  json_object_set(root, "long_description",
                  json_string(chr->player.long_descr));
  json_object_set(root, "description", json_string(chr->player.description));
  json_object_set(root, "title", json_string(chr->player.title));
  json_object_set(root, "sounds", json_string(chr->player.sounds));
  json_object_set(root, "distant_sounds",
                  json_string(chr->player.distant_snds));
  json_object_set(root, "class", get_class(chr->player.class_type));
  json_object_set(root, "levels", get_levels(chr->player.level));

  json_object_set(root, "abilities", abilities(&chr->abilities));
  json_object_set(root, "points", points(&chr->points));

  json_object_set(root, "special", specials(&chr->specials));

  return root;
}

int main(void) {
  boot_zones( );
  boot_world( );
  boot_mobiles( );
  boot_objects( );

  json_t *rooms = json_object( );

  for (long i = 0; i < room_count; i++) {
    struct room_data *room = room_db[ i ];
    if (room == NULL) {
      continue;
    }

    json_t *data = export_room(room);

    char number[ 255 ];
    sprintf(number, "%d", room->number);
    json_object_set(rooms, number, data);
  }

  FILE *rooms_file = fopen("./rooms.json", "wt");
  fprintf(rooms_file, "%s\n", json_dumps(rooms, JSON_INDENT(2)));
  fclose(rooms_file);

  json_t *zones = json_array( );

  for (long i = 0; i < top_of_zone_table; i++) {
    char number[ 255 ];
    sprintf(number, "%ld", i);
    json_t *zone = export_zone(&zone_table[ i ], i);
    json_array_append(zones, zone);
  }

  FILE *zones_file = fopen("./zones.json", "wt");
  fprintf(zones_file, "%s\n", json_dumps(zones, JSON_INDENT(2)));
  fclose(zones_file);

  json_t *objs = json_object( );

  FILE *in_obj_file = fopen(OBJ_FILE, "r");
  for (int i = 0; i < top_of_objt; i++) {
    char number[ 255 ];
    sprintf(number, "%d", obj_index[ i ].virtual_number);
    struct obj_data *obj = read_object(in_obj_file, i, REAL);
    json_object_set(objs, number, export_object(obj));
  }

  FILE *obj_file = fopen("./objects.json", "wt");
  fprintf(obj_file, "%s\n", json_dumps(objs, JSON_INDENT(2)));
  fclose(obj_file);

  json_t *mobs = json_object( );

  FILE *in_mob_file = fopen(MOB_FILE, "r");
  for (int i = 0; i < top_of_mobt; i++) {
    char number[ 255 ];
    sprintf(number, "%d", mob_index[ i ].virtual_number);

    struct char_data *chr = read_mobile(in_mob_file, i, REAL);
    json_object_set(mobs, number, export_mob(chr));
  }

  FILE *mob_file = fopen("./mobiles.json", "wt");
  fprintf(mob_file, "%s\n", json_dumps(mobs, JSON_INDENT(2)));
  fclose(mob_file);

  return 0;
}
