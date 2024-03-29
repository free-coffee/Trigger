#include<functional>
#include<memory>
#include<tuple>
#include<type_traits>

#include "cata_utility.h"
#include "line.h"
#include "Trigger.h"


using Trigger_standard::no_trigger;

extern Trigger& _1;
template<class... Types>
Function_serializable::Function_serializable( std::function func, bool triggerref, Types... data_in ){
    if( triggerref ){
        operator=( std::bind( func, _1, std::ref( std::get<std:integer_sequence<Types...>>( data ) )... ) );
    } else {
        operator=( std::bind( func, std::ref( std::get<std:integer_sequence<Types...>>( data ) )... ) );
    }
    assign_data( triggerref, data_in... );
}

template<class... Types>
void Function_serializable::assign_data( Types... data_in ){
    std::get<std:integer_sequence<Types...>>( data ) = data_in... ;
}

template<int pos, class Type>
void Function_serializable::set_data_element( Type value ){
    std::get<pos>( data ) = value;
}

template<int pos, class Type>
Type Function_serializable::get_data_element(){
    return std::get<pos>( data );
}

std::map<std::string, std::function> Trigger::functions_map = {
    { "map_dist" : Trigger_standard::map_dist }
};

std::map<std::string, std::function<void(JsonObject)>> Trigger::type_read_map = {
    { "{'Creature', 'Creature', 'int', 'short'}" : &Function_serializable<Creature&, Creature&, int, short>},
    { "{'Creature', 'Point', 'int', 'short'}" : &Function_serializable<Creature&, Tripoint, int, short>}
};

Trigger::Trigger(activation_p_t func, condition_p_t cond = return_true<Trigger&>, int to_live_checks = -1, int to_live_act = 1 ) :
function( &func ), condition( &cond ), to_live_checks( to_live_checks ), to_live_act( to_live_act )
{}

// *function = Function_serializable<Types...>( *function.target(), trigerref, args... );
template<class Types...>
void Trigger::bind_function(bool triggerref, Types... args){
	*function = Function_serializable<Types...>( *function.target(), trigerref, args... );
}

template<class Types...>
void Trigger::bind_condition(bool triggerref, Types... args){
	*condition = Function_serializable<Types...>( *condition.target(), trigerref, args... );
}

void TriggerSystem::check(trigger_timing chk) {
	auto que = trig_active.at( chk );
	for( auto tk = que.begin(); tk != que.end(); tk++ ) {
		Trigger &tr = *tk;
		if( tr.to_live_checks >= 0 && ( --tr.to_live_checks == -1 ) ){
			remove( tk, chk );
			continue;
		}
		if( tr.condition( tr ) ) {
			if( tr.to_live_act >= 0 && ( --tr.to_live_act == -1 ) ){
				remove( tk, chk );
				continue;
			}
			(*tr.function)( tr );
		}
	}
}


TriggerSystem::TriggerSystem() : frame_trig_p(no_trigger), min_trig_p(no_trigger), h_trig_p(no_trigger), day_trig_p(no_trigger), season_trig_p(no_trigger), y_trig_p(no_trigger),
	get_trig_handler( {
		{FRAME, TriggerSystem::frame_trig_p},
		{MINUTE, TriggerSystem::min_trig_p},
		{HOUR, TriggerSystem::h_trig_p},
		{DAY, TriggerSystem::day_trig_p},
		{SEASON, TriggerSystem::season_trig_p},
		{YEAR, TriggerSystem::y_trig_p}
	} )
{}


extern TriggerSystem* _1;
void TriggerSystem::add(Trigger trig, trigger_timing chk, bool top=true /*LIFO order*/) {
	auto que = trig_active.at( chk );
	if( que.empty() ) {
		// the action of doing something every trigger_timing timeframe became neccesary
		get_trig_handler.at( chk ) = std::bind( std::mem_fn(&TriggerSystem::check), _1, chk );
	}
	if( top ) {
		que.push_front( trig );
	} else {
		que.push_back( trig );
	}
}

void TriggerSystem::remove(std::deque<Trigger>::iterator tk, trigger_timing chk){
	auto que = trig_active.at( chk );
	int pos = std::distance( que.begin(), tk );
	que.erase( tk );
	tk = que.begin() + std::max(0, pos-1);
	if( que.empty() ) {
		get_trig_handler.at( chk ) = no_trigger;
	}
}


bool TriggerSystem::has_Trigger( std::string name ){
    Trigger& curTrig;
    for( auto const& entry : trig_active ){
        for( curTrig : trig_active.second ){
            if( curTrig.name == name ) {
                return true;
            }
        }
    }
    return false;
}

//better api to avoid external Trigger() construction??
Trigger Trigger_standard::get_activation_Trigger(Trigger later_trig, trigger_timing chk, condition_p_t cond = return_true<Trigger&>, int to_live_checks = -1, int to_live_act = 1) {
	Trigger trig_sup = Trigger( g->Ts.add, cond, to_live_checks, to_live_act );
	trig_sup.bind_function( later_trig, chk, false );
	return trig_sup;
}

bool Trigger_standard::map_dist(Trigger& tr, Creature& c1, Creature& c2, int dist = 1, short operator = -1){
		//cleanup if died: in Creature Destructor with member owned_trigger:
		//owned_trigger.to_live_checks = 0;
	return map_dist(c1.pos(), c2.pos(), dist, operator);
}

bool Trigger_standard::map_dist(Creature& c, Tripoint& tp, int dist = 1, short operator = -1){
	return map_dist(c.pos(), tp, dist, operator);
}

bool Trigger_standard::map_dist(Tripoint& tp1, Tripoint& tp2, int dist = 1, short operator = -1){
	int cur_dist = rl_dist(c1.pos(), tp)
	if( operator < 0 ){
		return cur_dist < dist;
	}else if(operator == 0){
		return cur_dist == dist;
	}else{
		return cur_dist > dist;
	}
}
