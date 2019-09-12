#include<typeinfo>
#include<Trigger.h>

void TriggerSystem::deserialize( JsonIn &jsin )
{
    JsonObject data = jsin.get_object();
    for( JsonObject trigdata : data.read_object() ){
        if( !( trigdata.has_flag( "UNIQUE" ) && g->Tr.has_Trigger( trigdata.get_str( "name" ) ) ) ){
            Trigger().load( trigdata );
        }
    }
}

void Trigger::load( JsonObject &data )
{
    std::string name_str;
    data.read( "name", name_str );

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
            Trigger::type_read_map( types ) ( jfuncdata, funcDataName );
        }
    }
    g->Ts.add( *this, string_to_enum( timing_str ), !data.has_flag( "FIFO" ));
}

constexpr std::queue& type_to_queue( std::type_info& type, const std::map<std::type_info, std::queue&>& type_map ){
    return type_map.at( type );
}

template<class Types... >
void Trigger::funcDataRead( JsonObject jfuncdata, std::string funcDataName ){
    JasonArray types = jfuncdata.get_array( "types" );
    JasonArray jdata = jfuncdata.get_array( "values" );
    std::tuple<Types... > cdata;
    int iter_n = 0;
    int pos;
    std::queue<Creature&> creature_temp;
    std::queue<tripoint> point_temp;
    std::queue<int> int_temp;
    std::queue<short> short_temp;
    const std::map<std::type_info, std::queue&> type_map
        = {{ typeid(Creature&) : creature_temp}, { typeid(tripoint): point_temp}, { typeid(int): int_temp}, { typeid(short) : short_temp}};
    for( std::string dat : jdata.read_str() ){
        switch( types.get_string( iter_n ) ){
            case "Creature":
                for( Creature& c : g->all_creatures){
                    if( c.name == dat ){
                        creature_temp.push( c );
                    }
                }
                break;
            case "Point":
                for( int i = 0; i<3; i++ ){
                    pos =
                }
                break;
            case "int":
                int_temp.push( static_cast<int>( dat ) );
                break;
            case "short":
                short_temp.push( static_cast<short>( dat ) );
                break;
        }
        iter_n++;
    }
    std::get<std::integer_sequence<Types...>>( cdata ) = type_to_queue( typeid( Types ) ).pop()... ;

    switch( funcDataName ){
        case "condition_data":  //refractor since Trigger_access is on Trigger data.
            bind_condition( jfuncdata.has_flag( "COND_TRIGGER_ACCESS" ), std::get<std::integer_sequence<Types...>>( cdata )... ); //use with bind(,Tuple) directly to avoid long code?
            break;
        case "function_data":
            bind_function( jfuncdata.has_flag( "ACT_TRIGGER_ACCESS" ), std::get<std::integer_sequence<Types...>>( cdata )... );
            break;
        }
    }
}
void TriggerSystem::serialize( JsonOut &json ) const
{

}

Trigger::store

