void TriggerSystem::deserialize( JsonIn &jsin )
{
    JsonObject data = jsin.get_object();
    for( JsonObject trigdata : data.get_array() ){

    }
}

void Trigger::load( JsonObject &data )
{
    std::string timing_str;
    data.read( "timing", timing_str );

    std::string tmp;
    data.read( "function", tmp );
    *function = active_map.get( tmp );

    data.read( "condition", tmp );
    *condition = cond_map.get( tmp );

    data.read("to_live_checks", to_live_checks);
    data.read("to_live_act", to_live_act);

    for( std::string funcDataName : {"condition_data", "function_data"} ){
        if(data.has_member( funcDataName ) ){
            JsonObject jfuncdata;
            if(data.has_string( funcDataName ) ){
                //jfuncdata = ; //add json type FunctionData with searchable Ids.
                realDebugmsg( "not inline FunctionData not implemented" );
            } else {
                jfuncdata = data.get_object( funcDataName );
            }
            std::string types = jfuncdata.ge_array( "types" );
            Trigger::type_read_map( types ) ( jfuncdata );
        }
    }
    g->Ts.add( *this, string_to_enum( timing_str ), false);
}

template<class Types... >
void Trigger::funcDataRead( JsonObject jfuncdata){
    JasonArray jdata = jfuncdata.ge_array( "values" );
    std::tuple<Types... > cdata;
    std::get<std::integer_sequence<>>( cdata ) = jdata.read_next()... ; //c++14, how without integer_sequence to unpack the data?
    switch( funcDataName ){
        case "condition_data":
            ;
            break;
        case "function_data":
            break;
        }
    }
}
void TriggerSystem::serialize( JsonOut &json ) const
{

}

