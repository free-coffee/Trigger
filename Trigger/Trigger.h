#include<deque>
#include<functional>
#include<map>
#include<memory>
#include<tuple>

struct Trigger;
class Trigger_system;

namespace Trigger_standard{ //slow ?
static void no_trigger(Trigger_system* ) {}
static bool allways(const Trigger& ) { return true; }
}

typedef std::function<void(Trigger_system* )> do_check_p_t;
typedef std::function<void(Trigger)> activation_p_t;
typedef std::function<bool(const Trigger&)> condition_p_t;

enum trigger_timing: int {
	FRAME,
	MINUTE,
	HOUR,
	DAY,
	SEASON,
	YEAR
};

struct Trigger{
	activation_p_t function;
	condition_p_t condition;
	int to_live_checks;
	int to_live_act;

	Trigger(activation_p_t func, condition_p_t cond, int to_live_checks, int to_live_act);
	// No Cleanup required for std::function.
};

class Trigger_system{
	//storing
	std::map<trigger_timing, std::deque<Trigger>> trig_active;
	std::map<trigger_timing, do_check_p_t> get_trig_handler;
 public:
	//activation
	void check(trigger_timing chk);
	do_check_p_t frame_trig_p;
	do_check_p_t min_trig_p;
	do_check_p_t h_trig_p;
	do_check_p_t day_trig_p;
	do_check_p_t season_trig_p;
	do_check_p_t y_trig_p;
	
	//administration
	Trigger_system();
	void add(Trigger trig, trigger_timing chk, bool top);
	void remove(std::deque<Trigger>::iterator trig_p, trigger_timing chk);
	/*
	void serialize( JsonOut &jsout ) const;
        void deserialize( JsonIn &jsin );*/
};


namespace Trigger_standard{
	// Trigger which activates another Trigger on activation 
	Trigger get_activation_Trigger(Trigger later_trig, trigger_timing chk, condition_p_t cond, int to_live_checks, int to_live_act);
}
