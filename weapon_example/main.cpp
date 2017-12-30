/**
 * @brief an example befitting the AI of a shooter game
 *
 * @date August 2014
 * @copyright (c) 2014 Prylis Inc. All rights reserved.
 */

#include "Action.h"
#include "Planner.h"
#include "WorldState.h"

#include <map>
#include <string.h>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>

using namespace goap;

class goap_script {
	int n;
	std::map<std::string, int> map;

public:
	std::vector<Action> actions;
	goap_script() :
			n(0)
	{
	}

	void script(const char *str)
	{
		std::stringstream ss;
		ss << str;
		script(ss);
	}

	void script(std::istream &is)
	{
		int line_no = 0;
		// action:250{precondition>effect}
		while (!is.eof())
		{
			std::string line;
			std::getline(is, line);
			if (line.length())
			{
				line_no++;
				bool is_condition = true;
				auto p1 = line.find(':');
				auto p2 = line.find('{', p1);
				auto p_midle = line.find('>');
				auto p_end = line.find('}');
				if (p_end == line.npos)
				{
					std::cerr << "line error in:" << line << std::endl;
					return;
				}
				auto act_name(line.substr(0, p1));
				auto cost = line.substr(p1 + 1, p2 - p1 - 1);
				int costi = atoi(cost.c_str());
				goap::Action act(act_name, costi);
				auto right = p2;
				do
				{
					auto left = right + 1;
					right = line.find(';', left);
					if (right == line.npos)
					{
						break;
					}
					std::string check(line.substr(left, right - left));
					if (check.c_str()[0] == '>')
					{
						left++;
					}
					std::string condition(line.substr(left, right - left));
					std::string real_name(condition);
					if (condition.length() == 0)
					{
						break;
					}
					bool value = true;
					int key = 0;
					if (condition.c_str()[0] == '!')
					{
						value = false;
						std::string cdn(condition.c_str() + 1);
						real_name = cdn;
						// act.setPrecondition(key,false);
					}
					auto it = this->map.insert(std::make_pair(real_name, 0));
					if (it.second)
					{
						std::cout << " n++" << std::endl;
						key = this->n++;
						it.first->second = key;
					} else
					{
						key = it.first->second;
					}
					if (left > p_midle)
					{
						std::cout << "effect:" << real_name << value
						        << std::endl;
						act.setEffect(key, value);
					} else
					{
						act.setPrecondition(key, value);
						std::cout << "condition:" << real_name << value
						        << std::endl;
					}
				} while (1);
				actions.push_back(act);
				std::cout << "line:" << line_no << " " << act_name << ":" << cost << " n=" << n << std::endl;
			}
		}
	}


	void create_state(WorldState &st, const char *str)
	{
		std::stringstream ss;
		ss << str;
		create_state(st, ss);
	}

	void create_state(WorldState &st, std::istream &is)
	{
		int line_no = 0;
		// action:250{precondition>effect}
		while (!is.eof())
		{
			std::string line;
			std::getline(is, line);
			if (line.length())
			{
				bool value = true;
				line_no++;
				bool is_condition = true;
				auto p2 = line.find('{');
				auto p_end = line.find('}');
				if (p2 == line.npos)
				{
					std::cerr << "line error in:" << line << std::endl;
					return;
				}
				if (line.c_str()[0] == '!')
				{
					value = false;
				}
				auto right = p2;
				do
				{
					auto left = right + 1;
					right = line.find(';', left);
					if (right == line.npos)
					{
						break;
					}
					std::string check(line.substr(left, right - left));
					if (check.c_str()[0] == '>')
					{
						std::cerr << "unexpected '>' in line:" << line_no << std::endl;
						break;
					}
					std::string condition(line.substr(left, right - left));
					std::string real_name(condition);
					if (condition.length() == 0)
					{
						break;
					}
					int key = 0;
					auto it = this->map.find(real_name);
					if (it == this->map.end())
					{
						std::cerr << "var '" << real_name << "' not exists." << std::endl;
						break;
					}
					key = it->second;
					st.setVariable(key,value);
					std::cout << "set " << real_name << ":" << value << std::endl;
				} while (1);
			}
		}

	}

};


const char *g_rules = ""
		"scoutStealthily:250{!enemy_sighted;weapon_in_hand;>enemy_sighted;}\n"
		"scoutRunning:150{!enemy_sighted;weapon_in_hand;>enemy_sighted;}\n"
		"closeToGunRange:2{enemy_sighted;!enemy_dead;!enemy_in_range;gun_loaded;>enemy_in_range;}\n"
		"closeToKnifeRange:4{enemy_sighted;!enemy_dead;!enemy_in_close_range;>enemy_in_close_range;}\n"
		"loadGun:2{have_ammo;!gun_loaded;gun_drawn;>gun_loaded;!have_ammo;}\n"
		"drawGun:1{inventory_gun;!weapon_in_hand;!gun_drawn;>gun_drawn;weapon_in_hand;}\n"
		"holsterGun:1{weapon_in_hand;gun_drawn;>!gun_drawn;!weapon_in_hand;}\n"
		"drawKnife:1{inventory_knife;!weapon_in_hand;!knife_drawn;>knife_drawn;weapon_in_hand;}\n"
		"sheathKnife:1{weapon_in_hand;knife_drawn;>!knife_drawn;!weapon_in_hand;}\n"
		"shootEnemy:64{enemy_sighted;!enemy_dead;gun_drawn;gun_loaded;enemy_in_range;>enemy_dead;}\n"
		"knifeEnemy:50{enemy_sighted;!enemy_dead;knife_drawn;enemy_in_close_range;>enemy_dead;}\n"
		"selfDestruct:30{enemy_sighted;enemy_dead;enemy_in_range;>me_dead;}\n"
		;

const char *g_zh_cn =
		"睡觉:3{穿睡衣;脱鞋子;>睡觉;}\n"
		"穿睡衣:3{脱鞋子;有睡衣;>穿睡衣;}\n"
		"买睡衣:3{有钱;>有睡衣;}\n"
		"脱鞋子:4{穿鞋子;>脱鞋子;!穿鞋子;}\n"
		;


int main(void)
{
	goap_script gs;

	gs.script(g_zh_cn);


#if 0
	WorldState st0;
	gs.create_state( st0,
			"!{enemy_dead;enemy_sighted;enemy_in_range;enemy_in_close_range;gun_loaded;gun_drawn;knife_drawn;}\n"
			"!{weapon_in_hand;me_dead;}\n"
			"{have_ammo;inventory_knife;inventory_gun;}\n"
	);
	WorldState st2;
	gs.create_state( st2,
			"{enemy_dead;me_dead;weapon_in_hand;}\n"
	);
	st0.priority_ = 100;
#endif

	WorldState st3;
	gs.create_state( st3,
			"{有钱;有睡衣;脱鞋子;}\n"
	);


	WorldState st4;
	gs.create_state( st4,
			"{睡觉;}\n"
	);


	// Fire up the A* planner
	Planner asa;
	try
	{
		std::vector<Action> the_plan = asa.plan(st3, st4,
		        gs.actions);
		std::cout << "Found a path!\n";
		for (std::vector<Action>::reverse_iterator rit = the_plan.rbegin();
		        rit != the_plan.rend(); ++rit)
		{
			std::cout << rit->name() << std::endl;
		}
	} catch (const std::exception&)
	{
		std::cout << "Sorry, could not find a path!\n";
	}

	std::cout << "\n\nWeapon example running...\n";
	std::vector<Action> actions;

	// Constants for the various states are helpful to keep us from
	// accidentally mistyping a state name.
	const int enemy_sighted = 1;
	const int enemy_dead = 2;
	const int enemy_in_range = 3;
	const int enemy_in_close_range = 4;
	const int inventory_knife = 5;
	const int inventory_gun = 6;
	const int gun_drawn = 7;
	;
	const int gun_loaded = 8;
	const int have_ammo = 9;
	const int knife_drawn = 10;
	const int weapon_in_hand = 11;
	const int me_dead = 12;

	// Now establish all the possible actions for the action pool
	// In this example we're providing the AI some different FPS actions

	Action scout("scoutStealthily", 250);
	scout.setPrecondition(enemy_sighted, false);
	scout.setPrecondition(weapon_in_hand, true);
	scout.setEffect(enemy_sighted, true);
	actions.push_back(scout);

	Action run("scoutRunning", 150);
	run.setPrecondition(enemy_sighted, false);
	run.setPrecondition(weapon_in_hand, true);
	run.setEffect(enemy_sighted, true);
	actions.push_back(run);

	Action approach("closeToGunRange", 2);
	approach.setPrecondition(enemy_sighted, true);
	approach.setPrecondition(enemy_dead, false);
	approach.setPrecondition(enemy_in_range, false);
	approach.setPrecondition(gun_loaded, true);
	approach.setEffect(enemy_in_range, true);
	actions.push_back(approach);

	Action approachClose("closeToKnifeRange", 4);
	approachClose.setPrecondition(enemy_sighted, true);
	approachClose.setPrecondition(enemy_dead, false);
	approachClose.setPrecondition(enemy_in_close_range, false);
	approachClose.setEffect(enemy_in_close_range, true);
	actions.push_back(approachClose);

	Action load("loadGun", 2);
	load.setPrecondition(have_ammo, true);
	load.setPrecondition(gun_loaded, false);
	load.setPrecondition(gun_drawn, true);
	load.setEffect(gun_loaded, true);
	load.setEffect(have_ammo, false);
	actions.push_back(load);

	Action draw("drawGun", 1);
	draw.setPrecondition(inventory_gun, true);
	draw.setPrecondition(weapon_in_hand, false);
	draw.setPrecondition(gun_drawn, false);
	draw.setEffect(gun_drawn, true);
	draw.setEffect(weapon_in_hand, true);
	actions.push_back(draw);

	Action holster("holsterGun", 1);
	holster.setPrecondition(weapon_in_hand, true);
	holster.setPrecondition(gun_drawn, true);
	holster.setEffect(gun_drawn, false);
	holster.setEffect(weapon_in_hand, false);
	actions.push_back(holster);

	Action drawKnife("drawKnife", 1);
	drawKnife.setPrecondition(inventory_knife, true);
	drawKnife.setPrecondition(weapon_in_hand, false);
	drawKnife.setPrecondition(knife_drawn, false);
	drawKnife.setEffect(knife_drawn, true);
	drawKnife.setEffect(weapon_in_hand, true);
	actions.push_back(drawKnife);

	Action sheath("sheathKnife", 1);
	sheath.setPrecondition(weapon_in_hand, true);
	sheath.setPrecondition(knife_drawn, true);
	sheath.setEffect(knife_drawn, false);
	sheath.setEffect(weapon_in_hand, false);
	actions.push_back(sheath);

	Action shoot("shootEnemy", 64);
	shoot.setPrecondition(enemy_sighted, true);
	shoot.setPrecondition(enemy_dead, false);
	shoot.setPrecondition(gun_drawn, true);
	shoot.setPrecondition(gun_loaded, true);
	shoot.setPrecondition(enemy_in_range, true);
	shoot.setEffect(enemy_dead, true);
	actions.push_back(shoot);

	Action knife("knifeEnemy", 50);
	knife.setPrecondition(enemy_sighted, true);
	knife.setPrecondition(enemy_dead, false);
	knife.setPrecondition(knife_drawn, true);
	knife.setPrecondition(enemy_in_close_range, true);
	knife.setEffect(enemy_dead, true);
	actions.push_back(knife);

	Action destruct("selfDestruct", 30);
	destruct.setPrecondition(enemy_sighted, true);
	destruct.setPrecondition(enemy_dead, true);
	destruct.setPrecondition(enemy_in_range, true);
	//destruct.setEffect(enemy_dead, true);
	destruct.setEffect(me_dead, true);
	actions.push_back(destruct);

	// Now establish some goal states and an initial state
	WorldState goal_win;
	goal_win.setVariable(enemy_dead, true);
	goal_win.setVariable(me_dead, true);
	goal_win.setVariable(weapon_in_hand, true);
	goal_win.priority_ = 100;

	// You can tweak these (e.g. have_ammo, the inventory items) to
	// elicit different plans from the AI.
	WorldState initial_state;
	initial_state.setVariable(enemy_dead, false);
	initial_state.setVariable(enemy_sighted, false);
	initial_state.setVariable(enemy_in_range, false);
	initial_state.setVariable(enemy_in_close_range, false);
	initial_state.setVariable(gun_loaded, false);
	initial_state.setVariable(gun_drawn, false);
	initial_state.setVariable(knife_drawn, false);
	initial_state.setVariable(weapon_in_hand, false);
	initial_state.setVariable(me_dead, false);
	initial_state.setVariable(have_ammo, true);
	initial_state.setVariable(inventory_knife, true);
	initial_state.setVariable(inventory_gun, true);

	// Fire up the A* planner
	Planner as;
	try
	{
		std::vector<Action> the_plan = as.plan(initial_state, goal_win,
		        actions);
		std::cout << "Found a path!\n";
		for (std::vector<Action>::reverse_iterator rit = the_plan.rbegin();
		        rit != the_plan.rend(); ++rit)
		{
			std::cout << rit->name() << std::endl;
		}
	} catch (const std::exception&)
	{
		std::cout << "Sorry, could not find a path!\n";
	}

}
