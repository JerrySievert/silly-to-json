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

int top_of_objt              = 0;
struct obj_data *object_list = NULL;
struct index_data *obj_index; /* index table for object file     */
int obj_count = 0;

int real_object(int virtual_number) {
  int bot, top, mid;

  bot = 0;
  top = top_of_objt;

  /* perform binary search on obj-table */
  for (;;) {
    mid = (bot + top) / 2;

    if ((obj_index + mid)->virtual_number == virtual_number)
      return (mid);
    if (bot >= top)
      return (-1);
    if ((obj_index + mid)->virtual_number > virtual_number)
      top = mid - 1;
    else
      bot = mid + 1;
  }
}

void clear_object(struct obj_data *obj) {
  memset(obj, '\0', sizeof(struct obj_data));

  obj->item_number = -1;
  obj->in_room     = NOWHERE;
  obj->eq_pos      = -1;
}

struct obj_data *read_object(FILE *obj_f, int nr, int type) {
  struct obj_data *obj;
  int tmp, i;
  long bc;
  char chk[ 50 ], buf[ 100 ];
  struct extra_descr_data *new_descr;

  i = nr;
  if (type == VIRTUAL) {
    nr = real_object(nr);
  }
  if (nr < 0 || nr > top_of_objt) {
    sprintf(buf, "Object (V) %d does not exist in database.", i);
    return (0);
  }

  fseek(obj_f, obj_index[ nr ].pos, 0);

  CREATE(obj, struct obj_data, 1);
  bc = sizeof(struct obj_data);

  clear_object(obj);

  /* *** string data *** */

  obj->name = fread_string(obj_f);
  if (obj->name && *obj->name) {
    bc += strlen(obj->name);
  }
  obj->short_description = fread_string(obj_f);
  if (obj->short_description && *obj->short_description) {
    bc += strlen(obj->short_description);
  }
  obj->description = fread_string(obj_f);
  if (obj->description && *obj->description) {
    bc += strlen(obj->description);
  }
  obj->action_description = fread_string(obj_f);
  if (obj->action_description && *obj->action_description) {
    bc += strlen(obj->action_description);
  }

  /* *** numeric data *** */

  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.type_flag = tmp;
  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.extra_flags = tmp;
  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.wear_flags = tmp;
  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.value[ 0 ] = tmp;
  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.value[ 1 ] = tmp;
  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.value[ 2 ] = tmp;
  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.value[ 3 ] = tmp;
  fscanf(obj_f, " %d ", &tmp);
  obj->obj_flags.weight = tmp;
  fscanf(obj_f, " %d \n", &tmp);
  obj->obj_flags.cost = tmp;
  fscanf(obj_f, " %d \n", &tmp);
  obj->obj_flags.cost_per_day = tmp;

  /* *** extra descriptions *** */

  obj->ex_description = 0;

  while (fscanf(obj_f, " %s \n", chk), *chk == 'E') {
    CREATE(new_descr, struct extra_descr_data, 1);
    bc += sizeof(struct extra_descr_data);
    new_descr->keyword = fread_string(obj_f);
    if (new_descr->keyword && *new_descr->keyword)
      bc += strlen(new_descr->keyword);
    new_descr->description = fread_string(obj_f);
    if (new_descr->description && *new_descr->description)
      bc += strlen(new_descr->description);

    new_descr->next     = obj->ex_description;
    obj->ex_description = new_descr;
  }

  for (i = 0; (i < MAX_OBJ_AFFECT) && (*chk == 'A'); i++) {
    fscanf(obj_f, " %d ", &tmp);
    obj->affected[ i ].location = tmp;
    fscanf(obj_f, " %d \n", &tmp);
    obj->affected[ i ].modifier = tmp;
    fscanf(obj_f, " %s \n", chk);
  }

  for (; (i < MAX_OBJ_AFFECT); i++) {
    obj->affected[ i ].location = APPLY_NONE;
    obj->affected[ i ].modifier = 0;
  }

  obj->in_room      = NOWHERE;
  obj->next_content = 0;
  obj->carried_by   = 0;
  obj->equipped_by  = 0;
  obj->eq_pos       = -1;
  obj->in_obj       = 0;
  obj->contains     = 0;
  obj->item_number  = nr;
  obj->in_obj       = 0;

  obj->next   = object_list;
  object_list = obj;

  obj_index[ nr ].number++;

  if (IS_SET(obj->obj_flags.extra_flags, ITEM_FIGURINE)) {
    // figuring number is read from the figurine table, best to ignore for now
    // just save that it is a figurine and figure out the mapping later if we
    // can
  }

  else
    obj->link = NULL;

  obj_count++;
#if BYTE_COUNT
  fprintf(stderr, "Object [%d] uses %d bytes\n", obj_index[ nr ].virtual, bc);
#endif

  return (obj);
}

void boot_objects(void) {
  FILE *fl;
  int zon = 0, cmd_no = 0, expand, tmp, bc = 100, cc = 22;
  char *check, buf[ 81 ];

  if (!(fl = fopen(OBJ_FILE, "r"))) {
    perror("boot_objects");
    assert(0);
  }

  obj_index = generate_indices(fl, &top_of_objt);
}
