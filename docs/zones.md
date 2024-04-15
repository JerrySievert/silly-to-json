# Zones

Zones are defined in the [Zones JSON schema](zones.json).

## `reset_modes`

`NEVER`, `EMPTY`, and `ALWAYS` control the reset mode for the zone, the others are zone types. It might make sense to move these to their own key at some point.

| Mode                    | Description          |
| ----------------------- | -------------------- |
| `NEVER`                 | Never reset zone     |
| `EMPTY`                 | Reset when empty     |
| `ALWAYS`                | Always reset zone    |
| `ASTRAL`                | Astral zone type     |
| `DESERT`                | Desert zone type     |
| `ARCTIC`                | Arctic zone type     |
| `HADES`                 | Hades zone type      |
| `OLYMPUS`               | Olympus zone type    |
| `ABYSS`                 | Abyss zone type      |
| `PRIMAL_MATERIAL_PLANE` | Prime material plane |
| `LIMBO`                 | Limbo                |

## `reset_commands`

Reset commands are to be executed when a zone resets. There is a conditional flag: `if_flag` that is set, along with three arguments. These are dependent on function execution for the actual behavior.

### `MOBILE_READ`

Checks to see if mobile of number `arg1` has less than `arg2` copies in the world; if not spawns a new copy and places it in the room of number `arg3`.

```javascript
if (total_mobile_count[arg1] < arg2) {
  mobile = read_mobile(arg1);
  place_mobile(mobile, arg3);
}
```

| Modifier | Description                 |
| -------- | --------------------------- |
| `arg1`   | Mobile to read              |
| `arg2`   | Maximum mobile count        |
| `arg3`   | Room number to place mobile |

### `MOBILE_READ_AND_CHARM`

Reads a mobile `arg1` if the total unique count is less `arg2`, the maximum count in the world; setting it as charmed by the previous mobile read, and with a special `act` on the mobile of `arg3`.

| Modifier | Description                        |
| -------- | ---------------------------------- |
| `arg1`   | Mobile to read                     |
| `arg2`   | Maximum mobile count               |
| `arg3`   | Special `act` to set on the mobile |

### `SET_LAST_MOBILE`

Sets the last mobile read's `zone` to `arg1`.

| Modifier | Description                    |
| -------- | ------------------------------ |
| `arg1`   | Zone to set the last mobile to |

### `READ_OBJECT`

Read object `arg1` if there are less than `arg2` total of the object in the world; place the object in room `arg3`.

| Modifier | Description                   |
| -------- | ----------------------------- |
| `arg1`   | Object to read                |
| `arg2`   | Maximum object count          |
| `arg3`   | Room to place the object into |

### `OBJECT_TO_OBJECT`

Read object `arg1` if there are less than `arg2` in the world and place the object inside object identified by `arg3`.

| Modifier | Description                     |
| -------- | ------------------------------- |
| `arg1`   | Object to read                  |
| `arg2`   | Maximum object count            |
| `arg3`   | Object to place the object into |

### `OBJECT_TO_CHAR`

Read object `arg1` if there are less than `arg2` and give to the last mobile read.

| Modifier | Description          |
| -------- | -------------------- |
| `arg1`   | Object to read       |
| `arg2`   | Maximum object count |

### `HATRED_TO_CHAR`

Add hatred of type `arg1` to the last read mobile with `arg2` being the argument specific setting.

| Modifier | Description                    |
| -------- | ------------------------------ |
| `arg1`   | Hatred to add                  |
| `arg2`   | Argument to the hatred setting |

### `FEAR_TO_CHAR`

Add fear of type `arg1` to the last read mobile with `arg2` being the argument specific setting.

| Modifier | Description                  |
| -------- | ---------------------------- |
| `arg1`   | Fear to add                  |
| `arg2`   | Argument to the fear setting |

### `OBJECT_TO_EQUIPMENT_LIST`

Read object `arg1` if there are less than `arg2` and equip it to the last mobile read in position `arg3`.

| Modifier | Description          |
| -------- | -------------------- |
| `arg1`   | Object to read       |
| `arg2`   | Maximum object count |
| `arg3`   | Slot to equip in     |

### `SET_STATE_OF_DOOR`

Set the state of a single door in a room.

| Modifier | Description       |
| -------- | ----------------- |
| `arg1`   | Room to modify    |
| `arg2`   | Direction of exit |
| `arg3`   | Array of statuses |

Statuses:

- `DOOR` - becomes a door
- `CLOSED` - the door is closed
- `LOCKED` - the door is locked
