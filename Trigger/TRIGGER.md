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

Use Trigger(func, cond, func_data, cond_data, to_live_checks, to_live_act) to create a Trigger-object with the arguments coresponding to the 2-6 Elements in the paragraph above, using function symbols with the right blueprints for func and cond.

## Function Data definition
Data used for binding of function arguments. Members have the same name in json and code.

"types":
array of str: names the argument types in their keywords. So far int, bool and pointers are allowed?

"defaults":
array of any: used if not specified on creation. 


## Standart applications:

### Trigger:
"ActivationTrigger": Trigger for adding another Trigger on activation. Requires the other trigger in the function_data as str or obj.

### Condition:
"map_distance": Checks the distance between two somethings. condition_data contains:
- "something_1",   "something_2": any - Somethings of which the distance matters. - player, npc, montype, itemtype, terraintype, furnituretype
- "type_1",   "type_2": str - types of the somethings.
- "dist": int - The referance distance.
- "operator": str - smaller "<",  bigger ">" or equal "==".

"overmap_distance": like above, but done on the overmap. somethings are player, omtype, horde

