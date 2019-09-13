# Trigger data

Triggers are a container with a condition, a function and parameters for these. They are used to cause a change at a specified map-state and are usually checked every secound or less frequent.

## How to create Triggers in-game?
Define the triggers in a json and then:
- add its name to an overmap, furniture, mission or npc definition in the element "Triggers".
- hardcode the creation.

## Trigger definition in json.

"name" :
str: name of the Trigger.

"timing":
str: Ttiming for checking the Trigger. Possible values as defined in Trigger.h/trigger_timing are:
FRAME, SECOUND, MINUTE, HOUR, DAY, SEASON, YEAR

"function":
str: Name of the function called on activation. Has to be mapped in Trigger.cpp/functions_map. Its function must have the blueprint void(Trigger& ) or specify function_data if the arguments differ.

"condition":
str: Name of the condition checked on timing. Names have to be mapped in Trigger.cpp/functions_map. Its function must have the blueprint bool(Trigger& ) or specify condition_data if the arguments differ.

"to_live_checks"
int: number of condition checks the Trigger will perform before destruction. -1 for infinite.

"to_live_act":
int: number of function executions/activations the Trigger will perform before destruction. -1 for infinite.

"condition_data":
str/obj: name or inline of Function data used for the condition.

"function_data":
str/obj: name or inline of Function data used for the function.

"flags":
"UNIQUE" : If the Trigger is active it can't be added again.
"STATIC" : The Trigger isn't saved on saving the game.
"STARTUP" : Add this Trigger on game startup.
"FIFO" : The Trigger is at the end of the que instead of the start.
"COND_TRIGGER_ACCESS" : The condition has access to the Trigger information.
"ACT_TRIGGER_ACCESS" : The function has access to the Trigger information.

## Trigger definition hardcoded.

Use Trigger(func, cond, to_live_checks, to_live_act) to create a Trigger-object with the arguments coresponding to the 2,3,5,6 Elements in the paragraph above.
Use Trigger::bind_function(triggerref, args) and bind_condition(triggerref, args) to set the arguments args on a respective call. triggerref is a bool determining if the 1st argument should be a passed down reference to the Trigger.

## Function Data definition
Data used for binding of function arguments. Members have the same name in json and code.

"types":
array of str: names of the argument types. These have to be predefined in Trigger.cpp/type_map

"values":
array of str: stored values with types in the same order. Included are:
- "creature": str storing the name of the used creature. If the name isn't unique the first in the games creature list is chosen.
- "point": 2-3 ints storing a point seperated with '_'. It is relative to either the character startpiont (on game creation) or the spawnpoint of the Creature or map owning the Trigger.
- basic (int/short): string converted in code.

## Standart applications:

### Trigger:
"ActivationTrigger": Trigger for adding another Trigger on activation. Requires the other trigger in the function_data as str or obj.

### Condition:
"map_distance": Checks the distance between two somethings. condition_data contains:
- "map_obj_1",   "map_obj_2": any - Somethings of which the distance matters. - characterid, montype, itemtype, terraintype, furnituretype
- "type_1",   "type_2": str - types of the somethings.
- "dist": int - The referance distance.
- "operator": str - smaller "<",  bigger ">" or equal "==".

"overmap_distance": like above, but done on the overmap. somethings are player, omtype, horde

