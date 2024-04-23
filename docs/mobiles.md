# Mobiles

Mobiles are defined in the [Mobiles JSON schema](mobiles.json).

## `race`

Not every race is directly accounted for in code.

| Race         | Description                   |
| ------------ | ----------------------------- |
| `HALFBREED`  | Half-breed                    |
| `HUMAN`      | Human                         |
| `ELVEN`      | Elven                         |
| `DWARF`      | Dwarf                         |
| `HALFLING`   | Halfling                      |
| `GNOME`      | Gnome                         |
| `REPTILE`    | Reptile                       |
| `SPECIAL`    | Unknown Special               |
| `LYCANTH`    | Lycanthrope                   |
| `DRAGON`     | Dragon                        |
| `UNDEAD`     | Undead                        |
| `ORC`        | Orc                           |
| `INSECT`     | Insect                        |
| `ARACHNID`   | Arachnid                      |
| `DINOSAUR`   | Dinosaur                      |
| `FISH`       | Fish                          |
| `BIRD`       | Bird                          |
| `GIANT`      | Giant                         |
| `PREDATOR`   | Predator                      |
| `PARASITE`   | Parasite                      |
| `SLIME`      | Slime                         |
| `DEMON`      | Demon                         |
| `SNAKE`      | Snake                         |
| `HERBIVORE`  | Herbivore                     |
| `TREE`       | Tree                          |
| `VEGGIE`     | Vegetable                     |
| `ELEMENT`    | Element                       |
| `PLANAR`     | Planar                        |
| `DEVIL`      | Devil                         |
| `GHOST`      | Ghost                         |
| `GOBLIN`     | Goblin                        |
| `TROLL`      | Troll                         |
| `VEGMAN`     | Vegetable Person              |
| `MINDFLAYER` | Mindflayer                    |
| `PRIMATE`    | Primate                       |
| `ENFAN`      | Enfan                         |
| `DROW`       | Drow                          |
| `GOLEM`      | Golem                         |
| `SKEXIE`     | Skexie                        |
| `TROGMAN`    | Trogman                       |
| `PATRYN`     | Patryn                        |
| `LABRAT`     | Lab Rat                       |
| `TYTAN`      | Titan                         |
| `SMURF`      | Smurf (papa or otherwise)     |
| `ROO`        | Roo                           |
| `HORSE`      | Horse                         |
| `DRAAGDIM`   | Draagdim                      |
| `ASTRAL`     | Astral                        |
| `GOD`        | God                           |
| `HALFELF`    | Half-elf (same as halfbreed?) |
| `HALFORC`    | Half-Orc                      |
| `HUMANTWO`   | Human Two                     |
| `VAMPIRE`    | Vampire                       |
| `OGRE`       | Ogre                          |
| `FAERIE`     | Faerie                        |

## `immunities`

Immunities of the mobile. These appear to be resistances rather than full immunities.

| Immunity      | Description      |
| ------------- | ---------------- |
| `FIRE`        | Fire             |
| `COLD`        | Cold             |
| `ELECTRICITY` | Electricity      |
| `ENERGY`      | Energy           |
| `BLUNT`       | Blunt Force      |
| `PIERCE`      | Pierce           |
| `SLASH`       | Slash            |
| `ACID`        | Acid             |
| `POISON`      | Poison           |
| `DRAIN`       | Drain            |
| `SLEEP`       | Sleep            |
| `CHARM`       | Charm            |
| `NON_MAGIC`   | Non-magic attack |
| `PLUS1`       | Plus 1           |
| `PLUS2`       | Plus 2           |
| `PLUS3`       | Plus 3           |
| `PLUS4`       | Plus 4           |

## `M_immunities`

Immunities of the mobile. These appear to be actual immunities.

| Immunity      | Description      |
| ------------- | ---------------- |
| `FIRE`        | Fire             |
| `COLD`        | Cold             |
| `ELECTRICITY` | Electricity      |
| `ENERGY`      | Energy           |
| `BLUNT`       | Blunt Force      |
| `PIERCE`      | Pierce           |
| `SLASH`       | Slash            |
| `ACID`        | Acid             |
| `POISON`      | Poison           |
| `DRAIN`       | Drain            |
| `SLEEP`       | Sleep            |
| `CHARM`       | Charm            |
| `NON_MAGIC`   | Non-magic attack |
| `PLUS1`       | Plus 1           |
| `PLUS2`       | Plus 2           |
| `PLUS3`       | Plus 3           |
| `PLUS4`       | Plus 4           |

## `susceptibilities`

What the mobile is susceptible to.

| Immunity      | Description      |
| ------------- | ---------------- |
| `FIRE`        | Fire             |
| `COLD`        | Cold             |
| `ELECTRICITY` | Electricity      |
| `ENERGY`      | Energy           |
| `BLUNT`       | Blunt Force      |
| `PIERCE`      | Pierce           |
| `SLASH`       | Slash            |
| `ACID`        | Acid             |
| `POISON`      | Poison           |
| `DRAIN`       | Drain            |
| `SLEEP`       | Sleep            |
| `CHARM`       | Charm            |
| `NON_MAGIC`   | Non-magic attack |
| `PLUS1`       | Plus 1           |
| `PLUS2`       | Plus 2           |
| `PLUS3`       | Plus 3           |
| `PLUS4`       | Plus 4           |

## `hates`

Things that this mobile hates, along with a value.

| Hates     | Description                                      |
| --------- | ------------------------------------------------ |
| `vnum`    | Mobile number that is hated                      |
| `class`   | Class that is hated                              |
| `evil`    | Whether evil is hated (`true`/`false`)           |
| `good`    | Whether good is hated (`true`/`false`)           |
| `sex`     | Sex that is hated                                |
| `race`    | Race that is hated                               |
| `hateful` | Whether just hateful in general (`true`/`false`) |

## `fears`

Things that this mobile fears, along with a value.

| Hates   | Description                             |
| ------- | --------------------------------------- |
| `vnum`  | Mobile number that is feared            |
| `class` | Class that is feared                    |
| `evil`  | Whether evil is feared (`true`/`false`) |
| `good`  | Whether good is feared (`true`/`false`) |
| `sex`   | Sex that is feared                      |
| `race`  | Race that is feared                     |

## `class`

Class of the mobile.

| Class        | Description  |
| ------------ | ------------ |
| `WARRIOR`    | Warrior      |
| `MAGIC USER` | Magic User   |
| `CLERIC`     | Cleric       |
| `THIEF`      | Thief        |
| `DRUID`      | Druid        |
| `MONK`       | Monk         |
| `ALL`        | All Classes? |

## `levels`

Level by class.

| Class        | Level            |
| ------------ | ---------------- |
| `magic_user` | Magic User level |
| `cleric`     | Cleric level     |
| `warrior`    | Warrior level    |
| `thief`      | Thief level      |
| `druid`      | Druid level      |
| `monk`       | Monk level       |

## `abilities`

Abilities, generally `1`-`20`.

| Ability        | Amount                    |
| -------------- | ------------------------- |
| `strength`     | Strength                  |
| `strength_add` | Amount to add to Strength |
| `intelligence` | Intelligence              |
| `wisdom`       | Wisdom                    |
| `dexterity`    | Dexterity                 |
| `constitution` | Constitution              |
| `charisma`     | Charisma                  |

## `points`

Miscellaneous stats for the mobile.

| Stat          | Description                  |
| ------------- | ---------------------------- |
| `mana`        | The amount of mana available |
| `max_mana`    | Maximum amount of mana       |
| `mana_gain`   | Amount of mana to gain       |
| `hit`         | Hit points currently         |
| `max_hit`     | Maximum hit points           |
| `hit_gain`    | Amount of hit points to gain |
| `move`        | Movement points available    |
| `max_move`    | Maximum movement points      |
| `move_gain`   | Movement points to gain      |
| `armor`       | Armor Class                  |
| `gold`        | Amount of gold carried       |
| `hit_roll`    | Hit roll bonus               |
| `damage_roll` | Damage roll bonus            |

## `position`

Current position.

- `DEAD`
- `MORTALLY WOUNDED`
- `INCAPACITATED`
- `STUNNED`
- `SLEEPING`
- `RESTING`
- `SITTING`
- `FIGHTING`
- `STANDING`

## `default_position`

- `DEAD`
- `MORTALLY WOUNDED`
- `INCAPACITATED`
- `STUNNED`
- `SLEEPING`
- `RESTING`
- `SITTING`
- `FIGHTING`
- `STANDING`

## `conditions`

- `drunk`
- `full`
- `thirst`

## `attack_type`

Type of attack the mobile uses.

- `HIT`
- `BLUDGEON`
- `PIERCE`
- `SLASH`
- `WHIP`
- `CLAW`
- `BITE`
- `STING`
- `CRUSH`
- `CLEAVE`
- `STAB`
- `SMASH`
- `SMITE`
- `BLAST`
- `SUFFERING`

## `affected_by`

Current statuses.

- `BLIND`
- `INVISIBLE`
- `DETECT_EVIL`
- `DETECT_INVISIBLE`
- `SENSE_LIFE`
- `LIFE_PROTECTION`
- `SANCTUARY`
- `DRAGON_RIDE`
- `GROWTH`
- `CURSE`
- `FLYING`
- `POISON`
- `TREE_TRAVEL`
- `PARALYSIS`
- `INFRAVISION`
- `WATERBREATH`
- `SLEEP`
- `TRAVELING`
- `SNEAK`
- `HIDE`
- `SILENCE`
- `CHARM`
- `FOLLOW`
- `TRUE_SIGHT`
- `SCRYING`
- `FIRESHIELD`
- `ANIMAL_INVISIBILITY`
- `HEAT_STUFF`
- `SUN_BLIND`
- `FEEDING`
- `BERSERK`

## `act`

Default actions.

- `SENTINEL`
- `SCAVENGER`
- `NICE_THIEF`
- `AGGRESSIVE`
- `STAY_ZONE`
- `WIMPY`
- `ANNOYING`
- `IMMORTAL`
- `DEADLY`
- `ILLUSION`
- `HUGE`
- `GREET`

## `intrinsics`

Intrinsic abilities.

- `BLIND`
- `INVISIBLE`
- `DETECT_EVIL`
- `DETECT_INVISIBLE`
- `SENSE_LIFE`
- `LIFE_PROTECTION`
- `SANCTUARY`
- `DRAGON_RIDE`
- `GROWTH`
- `CURSE`
- `FLYING`
- `POISON`
- `TREE_TRAVEL`
- `PARALYSIS`
- `INFRAVISION`
- `WATERBREATH`
- `SLEEP`
- `TRAVELING`
- `SNEAK`
- `HIDE`
- `SILENCE`
- `CHARM`
- `FOLLOW`
- `TRUE_SIGHT`
- `SCRYING`
- `FIRESHIELD`
- `ANIMAL_INVISIBILITY`
- `HEAT_STUFF`
- `SUN_BLIND`
- `FEEDING`
- `BERSERK`

## `saving_throw`

Saving throws per attribute.

| Attribute   | Apply                       |
| ----------- | --------------------------- |
| `paralysis` | Apply against paralysis     |
| `rod`       | Apply against rod           |
| `petrify`   | Apply against petrification |
| `breath`    | Apply against breath        |
| `spell`     | Apply against spell         |
