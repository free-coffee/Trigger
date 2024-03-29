#include<deque>
#include<functional>
#include<map>
#include<tuple>

struct Trigger;
struct Function_Data;
class Trigger_system;

namespace Trigger_standard{
static void no_trigger(Trigger_system* ) {}
}

typedef std::function<void(Trigger_system* )> do_check_p_t;
typedef std::function<void> activation_p_t;
typedef std::function<bool> condition_p_t;

enum trigger_timing: int {
	FRAME,
	SECOUND,
	MINUTE,
	HOUR,
	DAY,
	SEASON,
	YEAR
};

template<class... Types>
class Function_serializable : std::function{
    std::tuple<Types...> data;
public:
    Function_serializable( std::function func, bool triggerref, Types... data );
    void assign_data( Types... data );

    template<int pos, class Type>
    void set_data_element( Type value );
    template<int pos, class Type>
    Type get_data_element();

    JsonObject serialize();
    void deserialize( JsonObject data );
}

struct Trigger{
	std::unique_ptr<activation_p_t> function;
	std::unique_ptr<condition_p_t> condition;
	int to_live_checks;
	int to_live_act;
	std::string name;

	// If func or cond require arguments diffrent frome (Trigger& ) use the bind members.
	Trigger(activation_p_t func , condition_p_t cond, Function_Data& func_data, Function_Data& cond_data, int to_live_checks, int to_live_act);

	template<class ... Types>
	void bind_function(bool triggerref, Types... args);
	template<class ... Types>
	void bind_condition(bool triggerref, Types... args);

	void load( JsonObject &data );
	void unload( JsonOut &json );
private:
    static std::map<std::string, activation_p_t> active_map;
    static std::map<std::string, condition_p_t> cond_map;
    static std::map<std::string, std::function<void(JsonObject)>> type_read_map;
};

class TriggerSystem{
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
	TriggerSystem();
	void add(Trigger trig, trigger_timing chk, bool top);
	void remove(std::deque<Trigger>::iterator trig_p, trigger_timing chk);

	bool has_Trigger( std::string name );

	void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};


namespace Trigger_standard{
	// Trigger which activates another Trigger on activation
	Trigger get_activation_Trigger(Trigger later_trig, trigger_timing chk, condition_p_t cond, int to_live_checks, int to_live_act);

	bool map_dist(Creature& c1, Creature& c2, int dist, short operator);
	bool map_dist(Creature& c, Tripoint& tp, int dist, short operator);
	bool map_dist(Tripoint& tp1, Tripoint& tp2, int dist, short operator);
}
