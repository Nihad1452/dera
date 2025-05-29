#pragma once
#include <fstream>
#include "imgui/imgui.h"
#include "../libs/json/single_include/nlohmann/json.hpp"


enum bone_type : int
{
	head = 110,
	neck = 67,
	chest = 7,
	pelvis = 69,
};
inline int bone_values[] = {
	bone_type::head,
	bone_type::neck,
	bone_type::chest,
	bone_type::pelvis,
};
inline const char* get_bone_type_name(int type)
{
	switch (type)
	{
	case bone_type::head: return "Head";
	case bone_type::neck: return "Neck";
	case bone_type::chest: return "Chest";
	case bone_type::pelvis: return "Pelvis";
	default: return "Unknown";
	}
}

enum box_type : int
{
	normal = 0,
	corner = 1,
};
inline int box_values[] = {
	box_type::normal,
	box_type::corner,
};
inline const char* get_box_type_name(int type)
{
	switch (type)
	{
	case box_type::normal: return "Normal";
	case box_type::corner: return "Corner";
	default: return "Unknown";
	}
}

namespace settings
{
	inline int width = GetSystemMetrics(SM_CXSCREEN);
	inline int height = GetSystemMetrics(SM_CYSCREEN);
	inline int screen_center_x = width / 2;
	inline int screen_center_y = height / 2;
	inline bool show_menu = true;
	inline ImColor gui_color = ImColor(10, 125, 255, 255);

	namespace caching {
		uintptr_t base_address = 0;
	}


	namespace aim
	{
		inline bool mouse_aim = false;
		inline bool visible_check = false;
		inline bool show_fov = false;
		inline bool show_crosshair = false;
		inline float fov = 150;
		inline float smoothness = 5;
		inline int aim_key = 2;
		inline int bone = bone_type::head;
	}
	namespace trig
	{
		inline bool enable = false;
		inline int trig_key = 5;
		inline bool shotgun_only = false;
		inline float speed = 1.0f;
	}
	namespace vis
	{
		inline bool box = false;
		inline bool skeleton = false;
		inline bool head = false;
		inline bool penis = false;
		inline bool platform = false;
		inline bool username = false;
		inline bool distance = false;
		inline bool weapon = false;
		inline bool rank = false;
		inline bool thanked_bus_driver = false;
		inline bool kill_count = false;
		inline bool season_level = false;
		inline bool warning_arrows = false;
		inline bool filled = false;
		inline bool outline = false;
		inline bool text_background = false;
		inline float render_distance = 2555.0f;
		inline float box_thickness = 1.0f;
		inline float skeleton_thickness = 1.0f;
		inline int box_type = box_type::normal;
		inline bool ignore_teamates = false;
		inline bool show_nearby_players = false;
		inline bool show_visible_players = false;
	}
	namespace world
	{
		inline bool uncommon_loot = false;
		inline bool common_loot = false;
		inline bool rare_loot = false;
		inline bool epic_loot = false;
		inline bool legendary_loot = false;
		inline bool mythic_loot = false;
		inline bool chests = false;
		inline bool llamas = false;
	}

	namespace sexcrosshair
	{

		inline bool Spinbot = false;

		inline bool FOVChanger = false;
		static int FOVVALUE = 100;
		inline bool invisible = false;
		inline bool Crosshair = true;
		inline float Playersizetiny = (0.5f, 1.8f, 0.5f);
		inline float Playersizebig = (1.5f, 2.5f, 1.5f);
		static int bigortiny = 0;
		static float menuAlpha = 0.0f;
		inline bool carfly = false;
		inline bool Playersize_big_tiny = false;
	}



	namespace radar
	{
		inline bool radar = false;
		inline bool cross = false;
		inline bool local_player = false;
		inline float range = 50.0f;
	}
	namespace misc
	{
		inline bool gay_mode = false;
		inline bool vsync = false;
	}
	namespace colors
	{
		ImColor fov_color = ImColor(255, 255, 255, 255);
		ImColor crosshair_color = ImColor(255, 255, 255, 255);
		ImColor visible_color = ImColor(0, 255, 0, 255);
		ImColor invisible_color = ImColor(255, 0, 0, 255);
	}
}

void save_cfg(const std::string& file_name)
{
    nlohmann::json j;

    j["aim"] = {
        { "mouse_aim", settings::aim::mouse_aim },
        { "visible_check", settings::aim::visible_check },
        { "show_fov", settings::aim::show_fov },
        { "show_crosshair", settings::aim::show_crosshair },
        { "fov", settings::aim::fov },
        { "smoothness", settings::aim::smoothness },
        { "aim_key", settings::aim::aim_key },
        { "bone", settings::aim::bone }
    };

    j["trig"] = {
        { "enable", settings::trig::enable },
        { "trig_key", settings::trig::trig_key },
        { "shotgun_only", settings::trig::shotgun_only },
        { "speed", settings::trig::speed }
    };

    j["vis"] = {
        { "box", settings::vis::box },
        { "skeleton", settings::vis::skeleton },
        { "head", settings::vis::head },
        { "penis", settings::vis::penis }, // you had this setting, so save it too
        { "platform", settings::vis::platform },
        { "username", settings::vis::username },
        { "distance", settings::vis::distance },
        { "weapon", settings::vis::weapon },
        { "rank", settings::vis::rank },
        { "thanked_bus_driver", settings::vis::thanked_bus_driver },
        { "kill_count", settings::vis::kill_count },
        { "season_level", settings::vis::season_level },
        { "warning_arrows", settings::vis::warning_arrows },
        { "filled", settings::vis::filled },
        { "outline", settings::vis::outline },
        { "text_background", settings::vis::text_background },
        { "render_distance", settings::vis::render_distance },
        { "box_thickness", settings::vis::box_thickness }, // fixed here
        { "skeleton_thickness", settings::vis::skeleton_thickness },
        { "box_type", settings::vis::box_type },
        { "ignore_teamates", settings::vis::ignore_teamates },
        { "show_nearby_players", settings::vis::show_nearby_players },
        { "show_visible_players", settings::vis::show_visible_players }
    };

    j["world"] = {
        { "uncommon_loot", settings::world::uncommon_loot },
        { "common_loot", settings::world::common_loot },
        { "rare_loot", settings::world::rare_loot },
        { "epic_loot", settings::world::epic_loot },
        { "legendary_loot", settings::world::legendary_loot },
        { "mythic_loot", settings::world::mythic_loot },
        { "chests", settings::world::chests },
        { "llamas", settings::world::llamas }
    };

    j["sexcrosshair"] = {
        { "Spinbot", settings::sexcrosshair::Spinbot },
        { "FOVChanger", settings::sexcrosshair::FOVChanger },
        { "FOVVALUE", settings::sexcrosshair::FOVVALUE },
        { "invisible", settings::sexcrosshair::invisible },
        { "Crosshair", settings::sexcrosshair::Crosshair },
        { "bigortiny", settings::sexcrosshair::bigortiny },
        { "menuAlpha", settings::sexcrosshair::menuAlpha },
        { "carfly", settings::sexcrosshair::carfly },
        { "Playersize_big_tiny", settings::sexcrosshair::Playersize_big_tiny }
    };

    j["radar"] = {
        { "radar_enabled", settings::radar::radar },
        { "cross", settings::radar::cross },
        { "local_player", settings::radar::local_player },
        { "range", settings::radar::range }
    };

    j["misc"] = {
        { "gay_mode", settings::misc::gay_mode },
        { "vsync", settings::misc::vsync }
    };

    j["colors"] = {
        { "fov_color", { settings::colors::fov_color.Value.x * 255, settings::colors::fov_color.Value.y * 255, settings::colors::fov_color.Value.z * 255, settings::colors::fov_color.Value.w * 255 } },
        { "crosshair_color", { settings::colors::crosshair_color.Value.x * 255, settings::colors::crosshair_color.Value.y * 255, settings::colors::crosshair_color.Value.z * 255, settings::colors::crosshair_color.Value.w * 255 } },
        { "visible_color", { settings::colors::visible_color.Value.x * 255, settings::colors::visible_color.Value.y * 255, settings::colors::visible_color.Value.z * 255, settings::colors::visible_color.Value.w * 255 } },
        { "invisible_color", { settings::colors::invisible_color.Value.x * 255, settings::colors::invisible_color.Value.y * 255, settings::colors::invisible_color.Value.z * 255, settings::colors::invisible_color.Value.w * 255 } }
    };

    std::ofstream file(file_name);
    if (file.is_open())
    {
        file << j.dump(5);
        file.close();
    }
}

void load_cfg(const std::string& file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) return;

    nlohmann::json j;
    file >> j;

    // aim
    if (j.contains("aim"))
    {
        auto& aim = j["aim"];
        settings::aim::mouse_aim = aim.value("mouse_aim", settings::aim::mouse_aim);
        settings::aim::visible_check = aim.value("visible_check", settings::aim::visible_check);
        settings::aim::show_fov = aim.value("show_fov", settings::aim::show_fov);
        settings::aim::show_crosshair = aim.value("show_crosshair", settings::aim::show_crosshair);
        settings::aim::fov = aim.value("fov", settings::aim::fov);
        settings::aim::smoothness = aim.value("smoothness", settings::aim::smoothness);
        settings::aim::aim_key = aim.value("aim_key", settings::aim::aim_key);
        settings::aim::bone = aim.value("bone", settings::aim::bone);
    }

    // trig
    if (j.contains("trig"))
    {
        auto& trig = j["trig"];
        settings::trig::enable = trig.value("enable", settings::trig::enable);
        settings::trig::trig_key = trig.value("trig_key", settings::trig::trig_key);
        settings::trig::shotgun_only = trig.value("shotgun_only", settings::trig::shotgun_only);
        settings::trig::speed = trig.value("speed", settings::trig::speed);
    }

    // vis
    if (j.contains("vis"))
    {
        auto& vis = j["vis"];
        settings::vis::box = vis.value("box", settings::vis::box);
        settings::vis::skeleton = vis.value("skeleton", settings::vis::skeleton);
        settings::vis::head = vis.value("head", settings::vis::head);
        settings::vis::penis = vis.value("penis", settings::vis::penis);
        settings::vis::platform = vis.value("platform", settings::vis::platform);
        settings::vis::username = vis.value("username", settings::vis::username);
        settings::vis::distance = vis.value("distance", settings::vis::distance);
        settings::vis::weapon = vis.value("weapon", settings::vis::weapon);
        settings::vis::rank = vis.value("rank", settings::vis::rank);
        settings::vis::thanked_bus_driver = vis.value("thanked_bus_driver", settings::vis::thanked_bus_driver);
        settings::vis::kill_count = vis.value("kill_count", settings::vis::kill_count);
        settings::vis::season_level = vis.value("season_level", settings::vis::season_level);
        settings::vis::warning_arrows = vis.value("warning_arrows", settings::vis::warning_arrows);
        settings::vis::filled = vis.value("filled", settings::vis::filled);
        settings::vis::outline = vis.value("outline", settings::vis::outline);
        settings::vis::text_background = vis.value("text_background", settings::vis::text_background);
        settings::vis::render_distance = vis.value("render_distance", settings::vis::render_distance);
        settings::vis::box_thickness = vis.value("box_thickness", settings::vis::box_thickness);
        settings::vis::skeleton_thickness = vis.value("skeleton_thickness", settings::vis::skeleton_thickness);
        settings::vis::box_type = vis.value("box_type", settings::vis::box_type);
        settings::vis::ignore_teamates = vis.value("ignore_teamates", settings::vis::ignore_teamates);
        settings::vis::show_nearby_players = vis.value("show_nearby_players", settings::vis::show_nearby_players);
        settings::vis::show_visible_players = vis.value("show_visible_players", settings::vis::show_visible_players);
    }

    // world
    if (j.contains("world"))
    {
        auto& world = j["world"];
        settings::world::uncommon_loot = world.value("uncommon_loot", settings::world::uncommon_loot);
        settings::world::common_loot = world.value("common_loot", settings::world::common_loot);
        settings::world::rare_loot = world.value("rare_loot", settings::world::rare_loot);
        settings::world::epic_loot = world.value("epic_loot", settings::world::epic_loot);
        settings::world::legendary_loot = world.value("legendary_loot", settings::world::legendary_loot);
        settings::world::mythic_loot = world.value("mythic_loot", settings::world::mythic_loot);
        settings::world::chests = world.value("chests", settings::world::chests);
        settings::world::llamas = world.value("llamas", settings::world::llamas);
    }

    // sexcrosshair
    if (j.contains("sexcrosshair"))
    {
        auto& sc = j["sexcrosshair"];
        settings::sexcrosshair::Spinbot = sc.value("Spinbot", settings::sexcrosshair::Spinbot);
        settings::sexcrosshair::FOVChanger = sc.value("FOVChanger", settings::sexcrosshair::FOVChanger);
        settings::sexcrosshair::FOVVALUE = sc.value("FOVVALUE", settings::sexcrosshair::FOVVALUE);
        settings::sexcrosshair::invisible = sc.value("invisible", settings::sexcrosshair::invisible);
        settings::sexcrosshair::Crosshair = sc.value("Crosshair", settings::sexcrosshair::Crosshair);

        settings::sexcrosshair::bigortiny = sc.value("bigortiny", settings::sexcrosshair::bigortiny);
        settings::sexcrosshair::menuAlpha = sc.value("menuAlpha", settings::sexcrosshair::menuAlpha);
        settings::sexcrosshair::carfly = sc.value("carfly", settings::sexcrosshair::carfly);
        settings::sexcrosshair::Playersize_big_tiny = sc.value("Playersize_big_tiny", settings::sexcrosshair::Playersize_big_tiny);
    }

    // radar
    if (j.contains("radar"))
    {
        auto& radar = j["radar"];
        settings::radar::radar = radar.value("radar_enabled", settings::radar::radar);
        settings::radar::cross = radar.value("cross", settings::radar::cross);
        settings::radar::local_player = radar.value("local_player", settings::radar::local_player);
        settings::radar::range = radar.value("range", settings::radar::range);
    }

    // misc
    if (j.contains("misc"))
    {
        auto& misc = j["misc"];
        settings::misc::gay_mode = misc.value("gay_mode", settings::misc::gay_mode);
        settings::misc::vsync = misc.value("vsync", settings::misc::vsync);
    }

    // colors
    if (j.contains("colors"))
    {
        auto& colors = j["colors"];

        auto toFloatColor = [](const std::vector<int>& v) -> ImVec4 {
            if (v.size() == 4)
                return ImVec4(v[0] / 255.f, v[1] / 255.f, v[2] / 255.f, v[3] / 255.f);
            return ImVec4(1, 1, 1, 1);
            };

        settings::colors::fov_color.Value = toFloatColor(colors.value("fov_color", std::vector<int>{255, 255, 255, 255}));
        settings::colors::crosshair_color.Value = toFloatColor(colors.value("crosshair_color", std::vector<int>{255, 255, 255, 255}));
        settings::colors::visible_color.Value = toFloatColor(colors.value("visible_color", std::vector<int>{255, 255, 255, 255}));
        settings::colors::invisible_color.Value = toFloatColor(colors.value("invisible_color", std::vector<int>{255, 255, 255, 255}));
    }
}