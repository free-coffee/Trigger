void TriggerSystem::deserialize( JsonIn &jsin )
{
    JsonObject data = jsin.get_object();
    load( data );
}

void Trigger::load( JsonObject &data )
{
    std::string timing_str;
    data.read( "timing", timing_str );
    type = string_to_enum( sidtmp );

    std::string ftmp;
    data.read( "function", ftmp );
    function = active_map.get( ftmp );

"condition":
str/obj: Name or inline of the condition checked on timing. Names have to be mapped in Trigger.cpp/condition_map and have the blueprint bool(const Trigger&, FunctionData).
Inlines have the folloing elements:
- "algorithm": possibilities in Trigger.cpp/condition_map

"to_live_checks"
int: number of condition checks the Trigger will perform before destruction. -1 for infinite.

"to_live_act":
int: number of function executions/activations the Trigger will perform before destruction. -1 for infinite.

"condition_data":
str/obj: name or inline of Function data used for the condition.

"function_data":
/*
    std::string sidtmp;
    data.read( "typeid", sidtmp );
    type = &mtype_id( sidtmp ).obj();

    data.read( "unique_name", unique_name );
    data.read( "posx", position.x );
    data.read( "posy", position.y );
    if( !data.read( "posz", position.z ) ) {
        position.z = g->get_levz();
    }

    data.read( "wandf", wandf );
    data.read( "wandx", wander_pos.x );
    data.read( "wandy", wander_pos.y );
    if( data.read( "wandz", wander_pos.z ) ) {
        wander_pos.z = position.z;
    }
    data.read( "tied_item", tied_item );
    data.read( "hp", hp );
    data.read( "battery_item", battery_item );*/
}

void TriggerSystem::serialize( JsonOut &json ) const
{

}

