#include<functional>
#include<memory>
#include<tuple>
#include<type_traits>

#include "cata_utility.h"
#include "line.h"
#include "Trigger.h"


using Trigger_standard::no_trigger;

std::map<std::string, activation_p_t> Trigger::active_map = {

};

std::map<std::string, activation_p_t> Trigger::cond_map = {
    { "map_dist" : Trigger_standard::map_dist }
};

Trigger::Trigger(activation_p_t func, condition_p_t cond = return_true<Trigger&>, int to_live_checks = -1, int to_live_act = 1) :
function( new std::function( func ) ), condition( new std::function( cond ) ), to_live_checks( to_live_checks ), to_live_act( to_live_act )
{}

Trigger::~Trigger(){
    ~*condition();
    ~*function();
}

template<class Types...>
void Trigger::bind_function(bool triggerref, Types... args){
	bind(func, triggerref, args...);
}

template<class Types...>
void Trigger::bind_condition(bool triggerref, Types... args){
	cond(func, triggerref, args...);
}

extern Trigger& _1;
template<class Types...>
void Trigger::bind(std::function& used, bool triggerref, Types... args){
	if(triggerref){
		used = std::bind(used.target(), _1, args...);
	}else{
		used = std::bind(used.target(), args...);
	}
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
