#pragma once

typedef struct items
{
	uintptr_t actor;
	std::string name;
	bool is_pickup;
	float distance;
} items;
std::vector<items> inline items_list;
std::vector<items> items_temp_list;

typedef struct actors
{
	uintptr_t player_state;
	uintptr_t pawn_private;
	uintptr_t mesh;
	uintptr_t root_component;
	Vector3 relative_location;
	std::string username;
	float distance;
	std::string weapon;
	int player_count;
} actors;
std::vector<actors> inline actor_list;
std::vector<actors> actor_temp_list;