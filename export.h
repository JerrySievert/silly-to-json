#pragma once

#include "jansson.h"

#include "conversion.h"

const char *direction_string_from_number(int direction);
json_t *direction_from_number(int direction);
json_t *door_state(unsigned int state);
json_t *sector_type_from_number(int type);
json_t *room_flags(int room_flags);
json_t *direction_data(struct room_direction_data *dir);
json_t *export_room(struct room_data *room);
json_t *zone_command(char command);
json_t *reset_mode(int reset_mode);
json_t *export_zone(struct zone_data *zone, long zone_number);
json_t *sprintbit(unsigned long vector, const char *names[]);
json_t *obj_flag_data(struct obj_flag_data *flags);
json_t *obj_affected_type(struct obj_affected_type *affected);
json_t *export_object(struct obj_data *obj);
json_t *get_race(int race);
json_t *immunities(unsigned int immune);
json_t *get_class(int class_no);
json_t *get_sex(int sex_no);
json_t *hates_name(unsigned int field);
json_t *hates_value(unsigned int field, unsigned int argument);
json_t *hates(unsigned int field, Opinion opinion, _Bool hateful);
json_t *fears_name(unsigned int field);
json_t *fears_value(unsigned int field, unsigned int argument);
json_t *fears(unsigned int field, Opinion opinion);
json_t *get_levels(byte levels[ 8 ]);
json_t *abilities(struct char_ability_data *abilities);
json_t *points(struct char_point_data *points);
json_t *get_position(byte position);
json_t *conditions(sbyte conditions[ 3 ]);
json_t *attack_type(int attack_type);
json_t *affected_by(long affected_by, long affected_by2);
json_t *act(long act);
json_t *saving_throw(sh_int apply_saving_throw[ 5 ]);
json_t *specials(struct char_special_data *specials);
json_t *equipment_slot(unsigned int slot);
json_t *export_mob(struct char_data *chr);
