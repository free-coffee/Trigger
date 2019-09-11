# Trigger data

Triggers are a container with a condition, a function and parameters for these. They are used to cause a change at a specified map-state and are usually checked every secound or less frequent.

## How to create Triggers in-game?
Define the triggers in a json and then:
- add its name to an overmap, furniture, mission or npc definition in the element "Triggers".
- hardcode the creation.

## Trigger definition in json.

"timing":
str: Ttiming for checking the Trigger. Possible values as defined in Trigger.h/trigger_timing are:
FRAME, SECOUND, MINUTE, HOUR, DAY, SEASON, YEAR

"function":
str: Name of the function called on activation. Has to be mapped in Trigger.cpp/active_map and have the blueprint void(Trigger, FunctionData).

"condition":
str/obj: Name or inline of the condition checked on timing. Names have to be mapped in Trigger.cpp/condition_map and have the blueprint bool(const Trigger&, FunctionData).
Inlines have the folloing elements:
- "algorithm": possibilities:

"to_live_checks"
int: number of condition checks the Trigger will perform before destruction. -1 for infinite.

"to_live_act":
int: number of function executions/activations the Trigger will perform before destruction. -1 for infinite.

"condition_data":
str/obj: name or inline of Function data used for the condition.

"function_data":
str/obj: name or inline of Function data used for the function.

## Trigger definition hardcoded.

Use Trigger(func, cond, to_live_checks, to_live_act) to create a Trigger-object with the arguments coresponding to the 2,3,5,6 Elements in the paragraph above.
Use Trigger::bind_function(triggerref, args) and bind_condition(triggerref, args) to set the arguments args on a respective call. triggerref is a bool determining if the 1st argument should be a passed down reference to the Trigger.

## Function Data definition
Data used for binding of function arguments. Members have the same name in json and code.

"types":
str: names of the argument types in their keywords seperated with '_'. These have to be predefined in Trigger.cpp/FuncDataTypes

"defaults":
array of any: used if not specified on creation.


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

