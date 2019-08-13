#include<functional>
#include<memory>
#include<tuple>
#include<type_traits>


#include"Trigger.h"


using Trigger_standard::no_trigger;
extern Trigger_system* _1;

Trigger::Trigger(activation_p_t func, condition_p_t cond = Trigger_standard::allways, int to_live_checks = -1, int to_live_act = 1) :
function(func), condition( cond ), to_live_checks(to_live_checks), to_live_act(to_live_act)
{}

void Trigger_system::check(trigger_timing chk) {
	auto que = trig_active.at( chk );
	for( auto tk = que.begin(); tk != que.end(); tk++ ) {
		Trigger &tr = *tk;
		if( tr.condition( tr ) ) {
			tr.function( tr );
			if( tr.to_live_act > 0 && ( --tr.to_live_act == 0) ){
				remove( tk, chk );
			}
		} else if( tr.to_live_checks > 0 && ( --tr.to_live_checks == 0 ) ){
			remove( tk, chk );
		}
	}
}


Trigger_system::Trigger_system() : frame_trig_p(no_trigger), min_trig_p(no_trigger), h_trig_p(no_trigger), day_trig_p(no_trigger), season_trig_p(no_trigger), y_trig_p(no_trigger), 
	get_trig_handler( { 
		{FRAME, Trigger_system::frame_trig_p},
		{MINUTE, Trigger_system::min_trig_p},
		{HOUR, Trigger_system::h_trig_p},
		{DAY, Trigger_system::day_trig_p},
		{SEASON, Trigger_system::season_trig_p},
		{YEAR, Trigger_system::y_trig_p}
	} )
{}

void Trigger_system::add(Trigger trig, trigger_timing chk, bool top=true /*LIFO order*/) {
	auto que = trig_active.at( chk );
	if( que.empty() ) {
		// the action of doing something every trigger_timing timeframe became neccesary
		get_trig_handler.at( chk ) = std::bind( std::mem_fn(&Trigger_system::check), _1, chk );
	}
	if( top ) {
		que.push_front( trig );
	} else {
		que.push_back( trig );
	}
}

void Trigger_system::remove(std::deque<Trigger>::iterator tk, trigger_timing chk){
	auto que = trig_active.at( chk );
	que.erase( tk );
	if( que.empty() ) {
		get_trig_handler.at( chk ) = no_trigger;
	}
}
/*
void serialize( JsonOut &jsout ) const
{
}

void deserialize( JsonIn &jsin ){
}
*/

Trigger Trigger_standard::get_activation_Trigger(Trigger later_trig, trigger_timing chk, condition_p_t cond = allways, int to_live_checks = -1, int to_live_act = 1) {
	return Trigger( 
		std::bind( g->Ts.add, later_trig, chk, false ),
		cond, to_live_checks, to_live_act
	);
}
