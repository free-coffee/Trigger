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
            Function_serializable* cdata = new Trigger::type_read_map( types ) ( functions_map.at( data.get_str( funcDataName ) ),
                funcDataName == "condition_data" && data.has_flag( "COND_TRIGGER_ACCESS" ) || funcDataName == "function_data" && data.has_flag( "ACT_TRIGGER_ACCESS" ));
            cdata->deserialize( jfuncdata );
        }
    }
    g->Ts.add( *this, string_to_enum( timing_str ), !data.has_flag( "FIFO" ));
}

constexpr std::queue& type_to_queue( std::type_info& type, const std::map<std::type_info, std::queue&>& type_map ){
    return type_map.at( type );
}

template<class... Types >
void Function_serializable::deserialize( JsonObject jdata ){
    JasonArray types = jdata.get_array( "types" );
    JasonArray jarr = jdata.get_array( "values" );
    int iter_n = 0;
    int pos2;
    std::queue<Creature&> creature_temp;
    std::queue<tripoint> point_temp;
    std::queue<int> point_helper;
    std::queue<int> int_temp;
    std::queue<short> short_temp;
    const std::map<std::type_info, std::queue&> type_map
        = {{ typeid(Creature&) : creature_temp}, { typeid(tripoint): point_temp}, { typeid(int): int_temp}, { typeid(short) : short_temp}};
    for( std::string dat : jarr.read_str() ){
        switch( types.get_string( iter_n ) ){
            case "Creature":
                for( Creature& c : g->all_creatures){
                    if( c.name == dat ){
                        creature_temp.push( c );
                    }
                }
                break;
            case "Point":
                for( int pos1 = 0; pos2 != std::string::npos; pos1 = pos2 ){
                    pos2 = dat.find( '_', pos2+1 );
                    point_helper.push( static_cast<int>( dat.substr( pos1, pos2 ) ) );
                }
                if( point_helper.size() == 2 ){
                    point_temp.push( Tripoint( point_helper.pop(), point_helper.pop() ) );
                } else {
                    point_temp.push( Tripoint( point_helper.pop(), point_helper.pop(), point_helper.pop() ) );
                }
                // maybe handle the point it is relative to here?
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
    assign_data( type_to_queue( typeid( Types ) ).pop()... );
}
}

void TriggerSystem::serialize( JsonOut &json ) const
{

}

JsonObject Function_serializable::serialize(){
}

Trigger::store

