#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_custom.hpp"
#include "imgui/bytes.hpp"
#include <iostream>
#include <dwmapi.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include <tchar.h>
#include <thread>
#include "mouse/mouse.hpp"
#include "imgui/hotkey.hpp"
#include "sdk.hpp"
#include "../settings/defs.hpp"
#include "offsets.hpp"


#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

IDirect3D9Ex* p_object = NULL;
IDirect3DDevice9Ex* p_device = NULL;
D3DPRESENT_PARAMETERS p_params = { NULL };
MSG messager = { NULL };
HWND my_wnd = NULL;
HWND game_wnd = NULL;
ImFont* logo_font = nullptr;
ImFont* preview_font = nullptr;
ImFont* icon_font = nullptr;

IDirect3DTexture9* character_r = nullptr;
IDirect3DTexture9* skeleton_r = nullptr;

HRESULT directx_init()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_object))) exit(3);
	ZeroMemory(&p_params, sizeof(p_params));
	p_params.Windowed = TRUE;
	p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_params.hDeviceWindow = my_wnd;
	p_params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_params.BackBufferWidth = settings::width;
	p_params.BackBufferHeight = settings::height;
	p_params.EnableAutoDepthStencil = TRUE;
	p_params.AutoDepthStencilFormat = D3DFMT_D16;
	if (settings::misc::vsync)
	{
		p_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; // V-Sync ON
	}
	else
	{
		p_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // V-Sync OFF
	}


	if (FAILED(p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, my_wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, 0, &p_device)))
	{
		p_object->Release();
		exit(4);
	}
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(my_wnd);
	ImGui_ImplDX9_Init(p_device);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;

	ImFontConfig font_config;
	font_config.PixelSnapH = false;
	font_config.FontDataOwnedByAtlas = false;
	font_config.OversampleH = 5;
	font_config.OversampleV = 5;
	font_config.RasterizerMultiply = 1.2f;
	static const ImWchar ranges[] = {

		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};
	font_config.GlyphRanges = ranges;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 12.0f, &font_config, ranges);
	logo_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ebrima.ttf", 25.0f, &font_config, ranges);
	preview_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ebrima.ttf", 15.0f, &font_config, ranges);
	icon_font = io.Fonts->AddFontFromMemoryTTF(&IconFont, sizeof IconFont, 15.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

	if (character_r == nullptr) D3DXCreateTextureFromFileInMemoryEx(p_device, &CharacterImg, sizeof(CharacterImg), 332, 750, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &character_r);
	if (skeleton_r == nullptr) D3DXCreateTextureFromFileInMemoryEx(p_device, &SkeletonImg, sizeof(SkeletonImg), 336, 716, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &skeleton_r);

	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10, 255);
	style->Colors[ImGuiCol_Border] = ImColor(25, 25, 25, 255);
	style->Colors[ImGuiCol_ChildBg] = ImColor(15, 15, 15, 255);
	style->Colors[ImGuiCol_FrameBg] = ImColor(15, 15, 15, 255);
	style->Colors[ImGuiCol_PopupBg] = ImColor(15, 15, 15, 255);
	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_Separator] = ImColor(25, 25, 25, 255);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(25, 25, 25, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(25, 25, 25, 255);
	style->Colors[ImGuiCol_Button] = ImColor(15, 15, 15, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(15, 15, 15, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(15, 15, 15, 255);
	style->WindowRounding = 0.0f;
	style->WindowBorderSize = 1.0f;
	style->ChildBorderSize = 1.0f;
	style->FrameBorderSize = 1.0f;
	style->FramePadding = { 0, 0 };
	style->Alpha = 1.0f;

	p_object->Release();

	return S_OK;
}

void update_rainbow_color()
{
	if (settings::misc::gay_mode)
	{
		static float hue = 0.0f;
		ImVec4 rainbow_color = ImVec4(0, 0, 0, 1.0f);
		ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, rainbow_color.x, rainbow_color.y, rainbow_color.z);
		settings::gui_color = rainbow_color;
		hue += 0.005f;

		if (hue > 1.0f)
		{
			hue -= 1.0f;
		}
	}
	else
	{
		settings::gui_color = ImColor(10, 125, 255, 255);
	}

	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_CheckMark] = ImColor(settings::gui_color);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(settings::gui_color);
}

void create_overlay()
{
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA), 0, DefWindowProcA_Spoofed, 0, 0, nullptr,
		LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW),
		nullptr, nullptr, ("Facebook"), LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA_Spoofed(&wcex);

	my_wnd = CreateWindowExA_Spoofed(NULL, ("Facebook"), ("d - e"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);

	SetWindowLong(my_wnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea_Spoofed(my_wnd, &margin);
	ShowWindow_Spoofed(my_wnd, SW_SHOW);
	SetWindowPos_Spoofed(my_wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetLayeredWindowAttributes_Spoofed(my_wnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	UpdateWindow_Spoofed(my_wnd);
}

bool has_clicked;
std::chrono::steady_clock::time_point tb_begin;
std::chrono::steady_clock::time_point tb_end;
int tb_time_since;
void aimbot(uintptr_t target_mesh, uintptr_t target_entity)
{
	if (!target_mesh || !target_entity) return;
	if (settings::aim::visible_check && !sdk.is_visible(target_mesh)) return;

	Vector3 bone3d = sdk.get_entity_bone(target_mesh, settings::aim::bone);
	Vector2 bone2d = sdk.project_world_to_screen(bone3d);
	ImVec2 screen_center(settings::width / 2, settings::height / 2);
	ImVec2 target = calc_aim(bone2d, screen_center, settings::aim::smoothness, settings::width, settings::height);

	if (!sdk.is_in_fov(bone2d, settings::width, settings::height))
	{
		cache::closest_distance = FLT_MAX;
		cache::closest_mesh = NULL;
		cache::closest_entity = NULL;
		return;
	}

	SonyDriverHelper::api::MouseMove(static_cast<int>(target.x), static_cast<int>(target.y));
}

void draw_box(int x, int y, int w, int h, const ImColor color, ImColor fill_color, int thickness)
{
	if (settings::vis::filled) ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), fill_color, 0, 0);
	if (settings::vis::outline)
	{
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x + 1, y + 1), ImVec2(x + w - 1, y + h - 1), ImColor(0, 0, 0), 0, 0, 1.0f);
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0, 0, 0), 0, 0, 1.0f);
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x + 1, y - 1), ImVec2(x + w - 1, y + h + 1), ImColor(0, 0, 0), 0, 0, 1.0f);
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x - 1, y + 1), ImVec2(x + w + 1, y + h - 1), ImColor(0, 0, 0), 0, 0, 1.0f);
	}
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
}

void draw_corner_box(int x, int y, int w, int h, const ImColor color, ImColor fill_color, int thickness)
{
	if (settings::vis::filled) ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), fill_color, 0, 0);
	if (settings::vis::outline)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x - 1, y - 1), ImVec2(x - 1, y + (h / 3) - 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x - 1, y - 1), ImVec2(x + (w / 3) - 1, y - 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3) + 1, y - 1), ImVec2(x + w + 1, y - 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w + 1, y - 1), ImVec2(x + w + 1, y + (h / 3) - 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x - 1, y + h - (h / 3) + 1), ImVec2(x - 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x - 1, y + h + 1), ImVec2(x + (w / 3) - 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3) + 1, y + h + 1), ImVec2(x + w + 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w + 1, y + h - (h / 3) + 1), ImVec2(x + w + 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + 1, y + 1), ImVec2(x + 1, y + (h / 3) + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + 1, y + 1), ImVec2(x + (w / 3) + 1, y + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3) - 1, y + 1), ImVec2(x + w - 1, y + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - 1, y + 1), ImVec2(x + w - 1, y + (h / 3) + 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + 1, y + h - (h / 3) - 1), ImVec2(x + 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + 1, y + h - 1), ImVec2(x + (w / 3) + 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3) - 1, y + h - 1), ImVec2(x + w - 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - 1, y + h - (h / 3) - 1), ImVec2(x + w - 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
	}
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + (h / 3)), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y), ImVec2(x + (w / 3), y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3), y), ImVec2(x + w, y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + (h / 3)), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y + h - (h / 3)), ImVec2(x, y + h), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + (w / 3), y + h), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3), y + h), ImVec2(x + w, y + h), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w, y + h - (h / 3)), ImVec2(x + w, y + h), color, thickness);
}

void draw_text(ImFont* font, float fontSize, ImVec2 position, ImColor color, const char* text)
{
	ImGui::GetOverlayDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y - 1), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y - 1), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y + 1), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y + 1), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(font, fontSize, position, color, text);
}

void draw_skeleton(uintptr_t mesh, ImColor color, int thickness)
{
	Vector3 neck = sdk.get_entity_bone(mesh, 67);
	Vector2 wts_neck = sdk.project_world_to_screen(neck);
	Vector3 chest = sdk.get_entity_bone(mesh, 7);
	Vector2 wts_chest = sdk.project_world_to_screen(chest);
	Vector3 pelvis = sdk.get_entity_bone(mesh, 2);
	Vector2 wts_pelvis = sdk.project_world_to_screen(pelvis);

	Vector3 right_shoulder = sdk.get_entity_bone(mesh, 9);
	Vector2 wts_right_shoulder = sdk.project_world_to_screen(right_shoulder);
	Vector3 right_elbow = sdk.get_entity_bone(mesh, 10);
	Vector2 wts_right_elbow = sdk.project_world_to_screen(right_elbow);
	Vector3 right_wrist = sdk.get_entity_bone(mesh, 11);
	Vector2 wts_right_wrist = sdk.project_world_to_screen(right_wrist);

	Vector3 left_shoulder = sdk.get_entity_bone(mesh, 38);
	Vector2 wts_left_shoulder = sdk.project_world_to_screen(left_shoulder);
	Vector3 left_elbow = sdk.get_entity_bone(mesh, 39);
	Vector2 wts_left_elbow = sdk.project_world_to_screen(left_elbow);
	Vector3 left_wrist = sdk.get_entity_bone(mesh, 40);
	Vector2 wts_left_wrist = sdk.project_world_to_screen(left_wrist);

	Vector3 right_hip = sdk.get_entity_bone(mesh, 71);
	Vector2 wts_right_hip = sdk.project_world_to_screen(right_hip);
	Vector3 right_knee = sdk.get_entity_bone(mesh, 72);
	Vector2 wts_right_knee = sdk.project_world_to_screen(right_knee);
	Vector3 right_ankle = sdk.get_entity_bone(mesh, 73);
	Vector2 wts_right_ankle = sdk.project_world_to_screen(right_ankle);
	Vector3 right_foot_upper = sdk.get_entity_bone(mesh, 86);
	Vector2 wts_right_foot_upper = sdk.project_world_to_screen(right_foot_upper);

	Vector3 left_hip = sdk.get_entity_bone(mesh, 78);
	Vector2 wts_left_hip = sdk.project_world_to_screen(left_hip);
	Vector3 left_knee = sdk.get_entity_bone(mesh, 79);
	Vector2 wts_left_knee = sdk.project_world_to_screen(left_knee);
	Vector3 left_ankle = sdk.get_entity_bone(mesh, 80);
	Vector2 wts_left_ankle = sdk.project_world_to_screen(left_ankle);
	Vector3 left_foot_upper = sdk.get_entity_bone(mesh, 87);
	Vector2 wts_left_foot_upper = sdk.project_world_to_screen(left_foot_upper);

	if (settings::vis::outline)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_neck.x, wts_neck.y), ImVec2(wts_chest.x, wts_chest.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_chest.x, wts_chest.y), ImVec2(wts_pelvis.x, wts_pelvis.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_chest.x, wts_chest.y), ImVec2(wts_right_shoulder.x, wts_right_shoulder.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_shoulder.x, wts_right_shoulder.y), ImVec2(wts_right_elbow.x, wts_right_elbow.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_elbow.x, wts_right_elbow.y), ImVec2(wts_right_wrist.x, wts_right_wrist.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_chest.x, wts_chest.y), ImVec2(wts_left_shoulder.x, wts_left_shoulder.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_shoulder.x, wts_left_shoulder.y), ImVec2(wts_left_elbow.x, wts_left_elbow.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_elbow.x, wts_left_elbow.y), ImVec2(wts_left_wrist.x, wts_left_wrist.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_pelvis.x, wts_pelvis.y), ImVec2(wts_right_hip.x, wts_right_hip.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_hip.x, wts_right_hip.y), ImVec2(wts_right_knee.x, wts_right_knee.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_knee.x, wts_right_knee.y), ImVec2(wts_right_ankle.x, wts_right_ankle.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_ankle.x, wts_right_ankle.y), ImVec2(wts_right_foot_upper.x, wts_right_foot_upper.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_pelvis.x, wts_pelvis.y), ImVec2(wts_left_hip.x, wts_left_hip.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_hip.x, wts_left_hip.y), ImVec2(wts_left_knee.x, wts_left_knee.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_knee.x, wts_left_knee.y), ImVec2(wts_left_ankle.x, wts_left_ankle.y), ImColor(0, 0, 0), thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_ankle.x, wts_left_ankle.y), ImVec2(wts_left_foot_upper.x, wts_left_foot_upper.y), ImColor(0, 0, 0), thickness + 2.0f);
	}

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_neck.x, wts_neck.y), ImVec2(wts_chest.x, wts_chest.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_chest.x, wts_chest.y), ImVec2(wts_pelvis.x, wts_pelvis.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_chest.x, wts_chest.y), ImVec2(wts_right_shoulder.x, wts_right_shoulder.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_shoulder.x, wts_right_shoulder.y), ImVec2(wts_right_elbow.x, wts_right_elbow.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_elbow.x, wts_right_elbow.y), ImVec2(wts_right_wrist.x, wts_right_wrist.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_chest.x, wts_chest.y), ImVec2(wts_left_shoulder.x, wts_left_shoulder.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_shoulder.x, wts_left_shoulder.y), ImVec2(wts_left_elbow.x, wts_left_elbow.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_elbow.x, wts_left_elbow.y), ImVec2(wts_left_wrist.x, wts_left_wrist.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_pelvis.x, wts_pelvis.y), ImVec2(wts_right_hip.x, wts_right_hip.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_hip.x, wts_right_hip.y), ImVec2(wts_right_knee.x, wts_right_knee.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_knee.x, wts_right_knee.y), ImVec2(wts_right_ankle.x, wts_right_ankle.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_right_ankle.x, wts_right_ankle.y), ImVec2(wts_right_foot_upper.x, wts_right_foot_upper.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_pelvis.x, wts_pelvis.y), ImVec2(wts_left_hip.x, wts_left_hip.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_hip.x, wts_left_hip.y), ImVec2(wts_left_knee.x, wts_left_knee.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_knee.x, wts_left_knee.y), ImVec2(wts_left_ankle.x, wts_left_ankle.y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(wts_left_ankle.x, wts_left_ankle.y), ImVec2(wts_left_foot_upper.x, wts_left_foot_upper.y), color, thickness);
}

void draw_head(ImColor color, Vector3 head_location)
{
	if (!cache::local_pawn)
	{
		Vector2 head_2d = sdk.project_world_to_screen(Vector3(head_location.x, head_location.y, head_location.z + 20));
		Vector3 delta = head_location - cache::local_camera.location;

		float distance = delta.length();
		const float constant_circle_size = 10;
		float circle_radius = constant_circle_size * (settings::height / (2.0f * distance * tanf(cache::local_camera.fov * (float)M_PI / 360.f)));
		float y_offset = +90.0f;
		head_2d.y += y_offset;
		int segments = 50;

		if (settings::vis::outline) ImGui::GetOverlayDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, settings::vis::skeleton_thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, color, segments, settings::vis::skeleton_thickness);
	}
	else
	{
		Vector2 head_2d = sdk.project_world_to_screen(Vector3(head_location.x, head_location.y, head_location.z));
		Vector3 delta = head_location - cache::local_camera.location;

		float distance = delta.length();
		const float constant_circle_size = 10;
		float circle_radius = constant_circle_size * (settings::height / (2.0f * distance * tanf(cache::local_camera.fov * (float)M_PI / 360.f)));
		float y_offset = +12.0f;
		int segments = 50;

		if (settings::vis::outline) ImGui::GetOverlayDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, settings::vis::skeleton_thickness + 2.0f);
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, color, segments, settings::vis::skeleton_thickness);
	}
}

void draw_arrow(float x, float y, float angle, float size, ImColor color)
{
	float arrow_half_size = size / 2.0f;

	ImVec2 points[3];
	points[0] = ImVec2(x + cosf(angle) * size, y + sinf(angle) * size);
	points[1] = ImVec2(x + cosf(angle + 1.5f) * arrow_half_size, y + sinf(angle + 1.5f) * arrow_half_size);
	points[2] = ImVec2(x + cosf(angle - 1.5f) * arrow_half_size, y + sinf(angle - 1.5f) * arrow_half_size);

	if (settings::vis::outline) ImGui::GetOverlayDrawList()->AddLine(points[0], points[1], ImColor(0, 0, 0), 1.0f);
	if (settings::vis::outline) ImGui::GetOverlayDrawList()->AddLine(points[1], points[2], ImColor(0, 0, 0), 1.0f);
	if (settings::vis::outline) ImGui::GetOverlayDrawList()->AddLine(points[2], points[0], ImColor(0, 0, 0), 1.0f);
	ImGui::GetOverlayDrawList()->AddConvexPolyFilled(points, 3, color);
}


void sexspinbot()
{
	if (settings::sexcrosshair::Spinbot)
	{
		auto mesh = read<uint64_t>(cache::local_pawn + offsets::mesh);
		static auto Cached = read<Vector3>(mesh + 0x150);
		write<Vector3>(mesh + 0x150, Vector3(1, rand() % 361, 1));
	}
}

void FOVChanger()
{
	if (settings::sexcrosshair::FOVChanger)
	{
		uintptr_t PlayerCameraManager = read<uintptr_t>(cache::player_controller + PLAYERCAMERAMANAGER);
		write<float>(PlayerCameraManager + DEFAULTFOV + 0x4, settings::sexcrosshair::FOVVALUE);
		write(PlayerCameraManager + BASEFOV, settings::sexcrosshair::FOVVALUE);
	}
}

void sexinvisible()
{
	if (settings::sexcrosshair::invisible)
	{
		auto Mesh = read<uint64_t>(cache::local_pawn + offsets::mesh);
		static auto Cached = read<Vector3>(Mesh + 0x139);
		write<Vector3>(Mesh + 0x139, Vector3(1, rand() % 361, 1));
	}
}


void AdjustPlayerSize()
{
	if (settings::sexcrosshair::Playersize_big_tiny)
	{
		static int bigortiny = 0;
		switch (settings::sexcrosshair::bigortiny)
		{
		case 0: // Big
		{
			auto Mesh = read<uint64_t>(cache::local_pawn + offsets::mesh);
			write<Vector3>(Mesh + 0x168, Vector3(settings::sexcrosshair::Playersizebig, settings::sexcrosshair::Playersizebig, settings::sexcrosshair::Playersizebig));
			break;
		}

		case 1: // Tiny
		{
			auto Mesh = read<uint64_t>(cache::local_pawn + offsets::mesh);
			write<Vector3>(Mesh + 0x168, Vector3(settings::sexcrosshair::Playersizetiny, settings::sexcrosshair::Playersizetiny, settings::sexcrosshair::Playersizetiny));
			break;
		}
		}
	}
}


void actor_cache_loop()
{
	actor_temp_list.clear();
	cache::gworld = read<uintptr_t>(virtualaddy + offsets::gworld);
	cache::game_instance = read<uintptr_t>(cache::gworld + offsets::game_instance);
	cache::local_players = read<uintptr_t>(read<uintptr_t>(cache::game_instance + offsets::local_players));
	cache::player_controller = read<uintptr_t>(cache::local_players + offsets::player_controller);
	cache::local_pawn = read<uintptr_t>(cache::player_controller + offsets::local_pawn);
	cache::player_state = read<uintptr_t>(cache::local_pawn + offsets::player_state);
	cache::team_id = read<char>(cache::player_state + offsets::team_index);
	cache::current_weapon = read<uintptr_t>(cache::local_pawn + offsets::current_weapon);
	cache::root_component = read<uintptr_t>(cache::local_pawn + offsets::root_component);
	cache::local_relative_location = read<Vector3>(cache::root_component + offsets::relative_location);
	cache::closest_distance = FLT_MAX;
	cache::closest_mesh = NULL;
	cache::closest_entity = NULL;

	uintptr_t game_state = read<uintptr_t>(cache::gworld + offsets::game_state);
	uintptr_t player_array = read<uintptr_t>(game_state + offsets::player_array);
	int player_count = read<int>(game_state + (offsets::player_array + sizeof(uintptr_t)));
	for (int i = 0; i < player_count; i++)
	{
		uintptr_t player_state = read<uintptr_t>(player_array + (i * sizeof(uintptr_t)));
		uintptr_t pawn_private = read<uintptr_t>(player_state + offsets::pawn_private);
		if (pawn_private == cache::local_pawn) continue;

		if (sdk.is_dead(pawn_private)) continue;

		uintptr_t mesh = read<uintptr_t>(pawn_private + offsets::mesh);
		if (!mesh) continue;

		auto team_id = read<char>(player_state + offsets::team_index);
		if (settings::vis::ignore_teamates && cache::local_pawn)
		{
			if (team_id == cache::team_id) continue;
		}

		uintptr_t actor_root_component = read<uintptr_t>(pawn_private + offsets::root_component);
		Vector3 actor_relative_location = read<Vector3>(actor_root_component + offsets::relative_location);

		actors cached_player{};
		cached_player.player_state = player_state;
		cached_player.pawn_private = pawn_private;
		cached_player.mesh = mesh;
		cached_player.root_component = actor_root_component;
		cached_player.relative_location = actor_relative_location;
		cached_player.username = sdk.get_username(player_state);
		cached_player.distance = cache::local_relative_location.distance(actor_relative_location) / 100.0f;
		cached_player.weapon = sdk.get_weapon_name(pawn_private);
		cached_player.player_count = player_count;

		actor_temp_list.push_back(cached_player);
	}

	actor_list.clear();
	actor_list = actor_temp_list;

	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}











void actor_loop()
{
	for (actors actor : actor_list)
	{
		Vector3 head3d = sdk.get_entity_bone(actor.mesh, 110);
		Vector2 head2d = sdk.project_world_to_screen(head3d);
		Vector3 bottom3d = sdk.get_entity_bone(actor.mesh, 0);
		Vector2 bottom2d = sdk.project_world_to_screen(bottom3d);
		Vector2 boxHead = sdk.project_world_to_screen(Vector3(head3d.x, head3d.y, head3d.z + 20.0f));
		float box_width = (abs(boxHead.y - bottom2d.y)) * 0.40f;
		FBounds a_bounds = sdk.get_actor_bounds(boxHead, bottom2d, box_width);

		if (actor.distance > settings::vis::render_distance) continue;

		int visible_meshes = 0;
		int not_visible_meshes = 0;

		ImColor esp_col;
		ImColor box_fill_col = ImColor(0, 0, 0, 150);
		if (settings::misc::gay_mode)
		{
			esp_col = settings::gui_color;
		}
		else
		{
			if (sdk.is_visible(actor.mesh))
			{
				esp_col = settings::colors::visible_color;
				//visible_meshes++;
			}
			else
			{
				esp_col = settings::colors::invisible_color;
			}
		}

		//std::string nearby_count_str = std::to_string(actor.player_count) + std::string((" Player(s) Nearby!"));
		//ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(settings::width / 2, 25.0f), ImColor(255, 0, 0, 255), nearby_count_str.c_str());

		//std::string visible_count_str = std::to_string(visible_meshes) + std::string((" Visible Player(s)!"));
		//ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(settings::width / 2, 10.0f), ImColor(0, 255, 0, 255), visible_count_str.c_str());

		if (settings::vis::box)
		{
			switch (settings::vis::box_type)
			{
			case box_type::normal:
			{
				draw_box(a_bounds.left, a_bounds.top, a_bounds.right - a_bounds.left, a_bounds.bottom - a_bounds.top, esp_col, box_fill_col, settings::vis::box_thickness);
				break;
			}
			case box_type::corner:
			{
				draw_corner_box(a_bounds.left, a_bounds.top, a_bounds.right - a_bounds.left, a_bounds.bottom - a_bounds.top, esp_col, box_fill_col, settings::vis::box_thickness);
				break;
			}
			}
		}

		if (settings::vis::skeleton)
		{
			draw_skeleton(actor.mesh, esp_col, settings::vis::skeleton_thickness);
		}

		if (settings::vis::head)
		{
			draw_head(esp_col, head3d);
		}

		if (settings::vis::penis)
		{
			Vector3 penis = sdk.get_entity_bone(actor.mesh, 2);
			Vector2 wts_penis = sdk.project_world_to_screen(penis);

			float shaft_length = 40.0f;
			float shaft_width = 5.0f;
			float ball_radius = 5.0f;

			ImVec2 left_ball_center = ImVec2(wts_penis.x - shaft_width + 5 / 2, wts_penis.y - ball_radius);
			ImGui::GetOverlayDrawList()->AddCircleFilled(left_ball_center, ball_radius, ImColor(255, 219, 172, 255));

			ImVec2 right_ball_center = ImVec2(wts_penis.x + shaft_width + 5 / 2, wts_penis.y - ball_radius);
			ImGui::GetOverlayDrawList()->AddCircleFilled(right_ball_center, ball_radius, ImColor(255, 219, 172, 255));

			ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(wts_penis.x - shaft_width / 2, wts_penis.y), ImVec2(wts_penis.x + shaft_width / 2, wts_penis.y + shaft_length), ImColor(255, 219, 172, 255));
		}


		float yOffsetTop = 10.0f;
		if (settings::vis::username)
		{
			ImVec2 text_size = ImGui::CalcTextSize(actor.username.c_str());
			ImVec2 pos = ImVec2(boxHead.x - text_size.x / 2, boxHead.y - yOffsetTop - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 255), actor.username.c_str());

			yOffsetTop += 15.0f;
		}

		if (settings::vis::platform)
		{
			uintptr_t platform_ptr = read<uintptr_t>(actor.player_state + offsets::platform);
			wchar_t platform_char[64] = { 0 };

			huskyud::read_physical((platform_ptr), reinterpret_cast<uint8_t*>(platform_char), sizeof(platform_char));

			std::wstring platform_wide_str(platform_char);
			std::string platform_str(platform_wide_str.begin(), platform_wide_str.end());

			ImVec2 text_size = ImGui::CalcTextSize(platform_str.c_str());
			ImVec2 pos = ImVec2(boxHead.x - text_size.x / 2, boxHead.y - yOffsetTop - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			ImColor platform_col;
			if (platform_str == xorstr_("XBL") || platform_str == xorstr_("XBL"))
			{
				platform_col = ImColor(0, 255, 0, 255);
			}
			else if (platform_str == xorstr_("PSN") || platform_str == xorstr_("PS5"))
			{
				platform_col = ImColor(0, 0, 255, 255);
			}
			else if (platform_str == xorstr_("SWT"))
			{
				platform_col = ImColor(255, 0, 0, 255);
			}
			else if (platform_str == xorstr_("WIN"))
			{
				platform_col = ImColor(255, 255, 255, 255);
			}

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, platform_col, platform_str.c_str());
		}

		float yOffsetBottom = 10.0f;
		if (settings::vis::distance)
		{
			char dist[64];
			if (cache::local_pawn)
			{
				sprintf_s(dist, "%.fm", actor.distance);
			}
			else
			{
				sprintf_s(dist, "%.fm", 3.0f);
			}

			ImVec2 text_size = ImGui::CalcTextSize(dist);
			ImVec2 pos = ImVec2(bottom2d.x - text_size.x / 2, bottom2d.y + yOffsetBottom - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 255), dist);

			yOffsetBottom += 15.0f;
		}

		if (settings::vis::weapon)
		{
			std::string weapon;
			if (actor.weapon.empty())
			{
				weapon = "Building Plan";
			}
			else
			{
				weapon = actor.weapon;
			}

			ImVec2 text_size = ImGui::CalcTextSize(weapon.c_str());
			ImVec2 pos = ImVec2(bottom2d.x - text_size.x / 2, bottom2d.y + yOffsetBottom - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 255), weapon.c_str());

			yOffsetBottom += 15.0f;
		}

		if (settings::vis::rank)
		{
			uintptr_t habenaro = read<uintptr_t>(actor.player_state + offsets::habenaro_component);
			int32_t ranked_progress = read<int32_t>(habenaro + offsets::ranked_progress + 0x10);
			std::string rank_name = sdk.get_rank_name(ranked_progress);
			ImVec4 rank_color = sdk.get_rank_color(ranked_progress);

			ImVec2 text_size = ImGui::CalcTextSize(rank_name.c_str());
			ImVec2 pos = ImVec2(bottom2d.x - text_size.x / 2, bottom2d.y + yOffsetBottom - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, rank_color, rank_name.c_str());

			yOffsetBottom += 15.0f;
		}

		if (settings::vis::thanked_bus_driver)
		{
			std::string result;

			bool has_thanked = read<bool>(actor.player_state + offsets::b_has_thanked_bus_driver);
			if (has_thanked)
			{
				result = xorstr_("Thanked Bus Driver");
			}
			else
			{
				result = xorstr_("Didnt Thank Bus Driver");
			}

			ImVec2 text_size = ImGui::CalcTextSize(result.c_str());
			ImVec2 pos = ImVec2(bottom2d.x - text_size.x / 2, bottom2d.y + yOffsetBottom - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 255), result.c_str());

			yOffsetBottom += 15.0f;
		}

		if (settings::vis::kill_count)
		{
			int kills = read<int>(actor.player_state + offsets::kill_score);
			std::string k = std::to_string(kills) + xorstr_(" Kills");

			ImVec2 text_size = ImGui::CalcTextSize(k.c_str());
			ImVec2 pos = ImVec2(bottom2d.x - text_size.x / 2, bottom2d.y + yOffsetBottom - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, ImColor(200, 0, 0), k.c_str());

			yOffsetBottom += 15.0f;
		}

		if (settings::vis::season_level)
		{
			int level = read<int>(actor.player_state + offsets::season_level_ui_display);
			std::string l = xorstr_("Level ") + std::to_string(level);

			ImVec2 text_size = ImGui::CalcTextSize(l.c_str());
			ImVec2 pos = ImVec2(bottom2d.x - text_size.x / 2, bottom2d.y + yOffsetBottom - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
			draw_text(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 255), l.c_str());
		}

		if (settings::vis::warning_arrows)
		{
			Vector3 vAngle = cache::local_camera.rotation;
			float fYaw = vAngle.y * M_PI / 180.0f;
			float dx = actor.relative_location.x - cache::local_camera.location.x;
			float dy = actor.relative_location.y - cache::local_camera.location.y;
			float fsin_yaw = sinf(fYaw);
			float fminus_cos_yaw = -cosf(fYaw);
			float x = dy * fminus_cos_yaw + dx * fsin_yaw;
			x = -x;
			float y = dx * fminus_cos_yaw - dy * fsin_yaw;
			float length = sqrtf(x * x + y * y);
			x /= length;
			y /= length;
			float angle = atan2f(y, x);
			float fov_radius = settings::aim::fov + 5.0f;
			float screen_center_x = settings::width / 2.0f;
			float screen_center_y = settings::height / 2.0f;
			Vector2 arrow_pos = { screen_center_x + x * fov_radius, screen_center_y + y * fov_radius };

			draw_arrow(arrow_pos.x, arrow_pos.y, angle, 10.0f, esp_col);
		}

		if (settings::radar::radar)
		{
			ImGui::SetNextWindowSize({ 200, 200 });
			ImGui::SetNextWindowPos({ 10, 35 });
			ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(10, 10, 10, 155));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::Begin("##", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
			{
				ImVec2 winpos = ImVec2(10, 35);
				ImVec2 winsize = ImGui::GetWindowSize();

				if (settings::radar::cross)
				{
					ImGui::GetWindowDrawList()->AddLine(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y), ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y), ImGui::GetColorU32(ImGuiCol_Border), 1.0f);
					ImGui::GetWindowDrawList()->AddLine(ImVec2(winpos.x, winpos.y + winsize.y * 0.5f), ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5f), ImGui::GetColorU32(ImGuiCol_Border), 1.0f);
				}

				if (settings::radar::local_player)
				{
					ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y * 0.5f), 2.0f, ImColor(255, 255, 255, 255), 64);
				}

				int ScreenX, ScreenY = 0;
				sdk.project_world_to_radar(bottom3d, ScreenX, ScreenY, ImGui::GetWindowPos());
				ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(ScreenX, ScreenY), 2.0f, esp_col, 64);
			}
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		double dx = head2d.x - settings::screen_center_x;
		double dy = head2d.y - settings::screen_center_y;
		auto cross_dist = get_cross_distance(head2d.x, head2d.y, settings::screen_center_x, settings::screen_center_y);
		if (cross_dist <= settings::aim::fov && cross_dist <= cache::closest_distance)
		{
			cache::closest_distance = cross_dist;
			cache::closest_mesh = actor.mesh;
			cache::closest_entity = actor.pawn_private;
		}
	}
}








void features()
{
	update_rainbow_color();

	if (settings::aim::mouse_aim && GetAsyncKeyState_Spoofed(settings::aim::aim_key))
	{
		aimbot(cache::closest_mesh, cache::closest_entity);
	}
	if (settings::trig::enable && GetAsyncKeyState_Spoofed(settings::trig::trig_key))
	{
		std::string weapon_name = sdk.get_weapon_name(cache::local_pawn);
		std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), ::tolower);
		if (settings::trig::shotgun_only)
		{
			if (weapon_name.find("shotgun") != std::string::npos)
			{
				if (has_clicked)
				{
					tb_begin = std::chrono::steady_clock::now();
					has_clicked = 0;
				}

				tb_end = std::chrono::steady_clock::now();
				tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(tb_end - tb_begin).count();

				if (tb_time_since >= settings::trig::speed * 10)
				{
					INPUT Input = { 0 };

					Input.type = INPUT_MOUSE;
					Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
					SendInput(1, &Input, sizeof(INPUT));

					ZeroMemory(&Input, sizeof(INPUT));

					Input.type = INPUT_MOUSE;
					Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
					SendInput(1, &Input, sizeof(INPUT));

					has_clicked = 1;
				}
			}
		}
		else
		{
			if (has_clicked)
			{
				tb_begin = std::chrono::steady_clock::now();
				has_clicked = 0;
			}

			tb_end = std::chrono::steady_clock::now();
			tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(tb_end - tb_begin).count();

			if (tb_time_since >= settings::trig::speed * 10)
			{
				INPUT Input = { 0 };

				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				SendInput(1, &Input, sizeof(INPUT));

				ZeroMemory(&Input, sizeof(INPUT));

				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				SendInput(1, &Input, sizeof(INPUT));

				has_clicked = 1;
			}
		}
	}
	if (settings::aim::show_fov)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(settings::screen_center_x, settings::screen_center_y), settings::aim::fov, ImColor(0, 0, 0, 255), 100, 2.0f);
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(settings::screen_center_x, settings::screen_center_y), settings::aim::fov, settings::colors::fov_color, 100, 1.0f);
	}
	if (settings::aim::show_crosshair)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(settings::screen_center_x - 6, settings::screen_center_y), ImVec2(settings::screen_center_x + 6, settings::screen_center_y), ImColor(0, 0, 0), 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(settings::screen_center_x, settings::screen_center_y - 6), ImVec2(settings::screen_center_x, settings::screen_center_y + 6), ImColor(0, 0, 0), 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(settings::screen_center_x - 6, settings::screen_center_y), ImVec2(settings::screen_center_x + 6, settings::screen_center_y), settings::colors::crosshair_color, 1.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(settings::screen_center_x, settings::screen_center_y - 6), ImVec2(settings::screen_center_x, settings::screen_center_y + 6), settings::colors::crosshair_color, 1.0f);
	}

	//world_loop();
	actor_loop();
}

static int current_tab;


void render_menu()
{
	// Define the text strings
	std::string text1 = xorstr_("hitler-hitler-shopcore");
	std::string text2 = xorstr_("updated");
	std::string text3 = xorstr_("| V2 | FPS: ") + std::to_string(static_cast<int>(ImGui::GetIO().Framerate));

	// Calculate the sizes of the individual text components
	ImVec2 text1_size = ImGui::CalcTextSize(text1.c_str());
	ImVec2 text2_size = ImGui::CalcTextSize(text2.c_str());
	ImVec2 text3_size = ImGui::CalcTextSize(text3.c_str());

	// Manually calculate the maximum height
	float max_height = text1_size.y; // Start with the height of text1
	if (text2_size.y > max_height) max_height = text2_size.y; // Compare with text2
	if (text3_size.y > max_height) max_height = text3_size.y; // Compare with text3

	// Calculate the total width for the background
	float total_width = text1_size.x + text2_size.x + text3_size.x + 35.0f; // 25.0f + 40.0f for spacing
	float total_height = max_height + 10.0f; // Add some padding

	// Define the position for the background rectangle
	ImVec2 rect_pos(10, 10);
	ImVec2 rect_size(total_width, total_height); // Add padding to the rectangle

	// Draw the background rectangle
	ImGui::GetOverlayDrawList()->AddRectFilled(rect_pos, ImVec2(rect_pos.x + rect_size.x, rect_pos.y + rect_size.y), ImGui::GetColorU32(ImGuiCol_WindowBg));

	// Draw the individual text components
	ImGui::GetOverlayDrawList()->AddText(rect_pos + ImVec2(10, (rect_size.y - text1_size.y) / 2), ImColor(255, 255, 255, 255), text1.c_str());
	ImGui::GetOverlayDrawList()->AddText(rect_pos + ImVec2(text1_size.x + 19.0f, (rect_size.y - text2_size.y) / 2), ImGui::GetColorU32(ImGuiCol_CheckMark), text2.c_str());
	ImGui::GetOverlayDrawList()->AddText(rect_pos + ImVec2(text1_size.x + text2_size.x + 25.0f, (rect_size.y - text3_size.y) / 2), ImColor(255, 255, 255, 255), text3.c_str());

	if (GetAsyncKeyState_Spoofed(VK_F2) & 1) settings::show_menu = !settings::show_menu;
	if (settings::show_menu)
	{
		ImVec2 pos;
		ImGui::SetNextWindowSize({ 620, 440 });
		ImGui::Begin(xorstr_("##ui"), &settings::show_menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
		{
			pos = ImGui::GetWindowPos();

			ImGui::BeginGroup();
			{
				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
				ImGui::BeginChild(xorstr_("##tabs"), ImVec2(150.0f, 0), true);
				{
					ImGui::PushFont(logo_font);
					ImGui::Text(xorstr_("Shop"));
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_CheckMark));
					ImGui::Text(xorstr_("Core"));
					ImGui::PopStyleColor();
					ImGui::PopFont();
					ImGui::Dummy(ImVec2(0, 5.0f));

					if (imgui_custom->tab(xorstr_("Aim"), xorstr_("1"), current_tab == 0, icon_font)) current_tab = 0;
					if (imgui_custom->tab(xorstr_("Triggerbot"), xorstr_("6"), current_tab == 1, icon_font)) current_tab = 1;
					if (imgui_custom->tab(xorstr_("Visuals"), xorstr_("2"), current_tab == 2, icon_font)) current_tab = 2;
					if (imgui_custom->tab(xorstr_("Exploits"), xorstr_("4"), current_tab == 3, icon_font)) current_tab = 3;
					if (imgui_custom->tab(xorstr_("Radar"), xorstr_("3"), current_tab == 4, icon_font)) current_tab = 4;
					if (imgui_custom->tab(xorstr_("Misc"), xorstr_("5"), current_tab == 5, icon_font)) current_tab = 5;
				}
				ImGui::EndChild();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			{
				float window_width = ImGui::GetContentRegionAvailWidth() - ImGui::GetStyle().WindowPadding.x;
				float window_height = ImGui::GetContentRegionAvail().y - ImGui::GetStyle().WindowPadding.y;

				if (current_tab == 0)
				{
					imgui_custom->begin_child(xorstr_("Aim Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::Checkbox(xorstr_("Enable Aim"), &settings::aim::mouse_aim);
						ImGui::SameLine();
						ImGui::PushID(xorstr_("AIMKEY"));
						hotkey_button(settings::aim::aim_key, ChangeKeyAim, keystatus);
						ImGui::PopID();
						ImGui::Checkbox(xorstr_("Visible Only"), &settings::aim::visible_check);
						ImGui::Checkbox(xorstr_("FOV Circle"), &settings::aim::show_fov);
						ImGui::Checkbox(xorstr_("Crosshair"), &settings::aim::show_crosshair);
						ImGui::Checkbox(xorstr_("Ignore Teamates"), &settings::vis::ignore_teamates);
					}
					imgui_custom->end_child();
					ImGui::SameLine();
					imgui_custom->begin_child(xorstr_("Extra Aim Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::SliderFloat(xorstr_("FOV"), &settings::aim::fov, 50, 400, "%.f");
						ImGui::SliderFloat(xorstr_("Smoothness"), &settings::aim::smoothness, 3, 15, "%.f");
						if (ImGui::BeginCombo(xorstr_("Bone"), get_bone_type_name(settings::aim::bone)))
						{
							for (auto bone_type : bone_values)
							{
								if (ImGui::Selectable(get_bone_type_name(bone_type), settings::aim::bone == bone_type)) settings::aim::bone = bone_type;
							}
							ImGui::EndCombo();
						}
						ImGui::Spacing();
						ImGui::Text(xorstr_("FOV Circle Color"));
						ImGui::ColorEdit4(xorstr_("##FovCircleColor"), reinterpret_cast<float*>(&settings::colors::fov_color));
						ImGui::Spacing();
						ImGui::Text(xorstr_("Crosshair Color"));
						ImGui::ColorEdit4(xorstr_("##CrosshairColor"), reinterpret_cast<float*>(&settings::colors::crosshair_color));
					}
					imgui_custom->end_child();
				}

				if (current_tab == 1)
				{
					imgui_custom->begin_child(xorstr_("Triggerbot Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::Checkbox(xorstr_("Enable Triggerbot"), &settings::trig::enable);
						ImGui::SameLine();
						ImGui::PushID(xorstr_("TRIGBOTKEY"));
						hotkey_button(settings::trig::trig_key, ChangeKeyTrig, keystatus2);
						ImGui::PopID();
						ImGui::Checkbox(xorstr_("Shotgun Only"), &settings::trig::shotgun_only);
					}
					imgui_custom->end_child();
					ImGui::SameLine();
					imgui_custom->begin_child(xorstr_("Extra Triggerbot Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::SliderFloat(xorstr_("Delay"), &settings::trig::speed, 1, 20, "%.f");
					}
					imgui_custom->end_child();
				}

				if (current_tab == 2)
				{
					imgui_custom->begin_child(xorstr_("Visuals Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::Checkbox(xorstr_("Box"), &settings::vis::box);
						ImGui::Checkbox(xorstr_("Skeleton"), &settings::vis::skeleton);
						ImGui::Checkbox(xorstr_("Head Circle"), &settings::vis::head);

						ImGui::Checkbox(xorstr_("Penis"), &settings::vis::penis);
						ImGui::Checkbox(xorstr_("Platform"), &settings::vis::platform);
						ImGui::Checkbox(xorstr_("Username"), &settings::vis::username);
						ImGui::Checkbox(xorstr_("Distance"), &settings::vis::distance);

						ImGui::Checkbox(xorstr_("Weapon"), &settings::vis::weapon);
						ImGui::Checkbox(xorstr_("Rank"), &settings::vis::rank);
						ImGui::Checkbox(xorstr_("Thanked Bus Driver"), &settings::vis::thanked_bus_driver);
						ImGui::Checkbox(xorstr_("Kill Count"), &settings::vis::kill_count);
						ImGui::Checkbox(xorstr_("Season Level"), &settings::vis::season_level);
						ImGui::Checkbox(xorstr_("Warning Arrows"), &settings::vis::warning_arrows);


					}
					imgui_custom->end_child();
					ImGui::SameLine();
					imgui_custom->begin_child(xorstr_("Extra Visuals Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::Checkbox(xorstr_("Filled"), &settings::vis::filled);
						ImGui::Checkbox(xorstr_("Outline"), &settings::vis::outline);
						ImGui::Checkbox(xorstr_("Text Background"), &settings::vis::text_background);
						ImGui::SliderFloat(xorstr_("Max Render Distance"), &settings::vis::render_distance, 50, 2555, "%.f");
						ImGui::SliderFloat(xorstr_("Box Thickness"), &settings::vis::box_thickness, 1, 5, "%.f");
						ImGui::SliderFloat(xorstr_("Skeleton Thickness"), &settings::vis::skeleton_thickness, 1, 5, "%.f");
						if (ImGui::BeginCombo(xorstr_("Box Type"), get_box_type_name(settings::vis::box_type)))
						{
							for (auto box_type : box_values)
							{
								if (ImGui::Selectable(get_box_type_name(box_type), settings::vis::box_type == box_type)) settings::vis::box_type = box_type;
							}
							ImGui::EndCombo();
						}
						ImGui::Spacing();
						ImGui::Text(xorstr_("Visible Color"));
						ImGui::ColorEdit4(xorstr_("##VisibleColor"), reinterpret_cast<float*>(&settings::colors::visible_color));
						ImGui::Spacing();
						ImGui::Text(xorstr_("Invisible Color"));
						ImGui::ColorEdit4(xorstr_("##InvisibleColor"), reinterpret_cast<float*>(&settings::colors::invisible_color));
					}
					imgui_custom->end_child();
				}

				if (current_tab == 3)
				{
					imgui_custom->begin_child(xorstr_("Exploits Configuration"), ImVec2(ImGui::GetContentRegionAvailWidth(), 0));
					{
						ImGui::Text(xorstr_("Remember, using exploits is dangerous and can get you banned quickly."));

						ImGui::Spacing();
						ImGui::Checkbox(xorstr_("Spinbot"), &settings::sexcrosshair::Spinbot);
						ImGui::Checkbox("FOV Changer", &settings::sexcrosshair::FOVChanger);
						if (settings::sexcrosshair::FOVChanger)
						{
							ImGui::SliderInt("##fovvalue", &settings::sexcrosshair::FOVVALUE, 10, 180);
						}

						// space

						ImGui::Checkbox("invisible", &settings::sexcrosshair::invisible);

						// space

						ImGui::Checkbox("Custom Player Size", &settings::sexcrosshair::Playersize_big_tiny);

						// space

						if (settings::sexcrosshair::Playersize_big_tiny)
						{
							ImGui::Combo("##playersize", &settings::sexcrosshair::bigortiny, "Big\0Tiny\0");
						}



					}
					imgui_custom->end_child();
				}

				if (current_tab == 4)
				{
					imgui_custom->begin_child(xorstr_("Radar Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::Checkbox(xorstr_("Radar"), &settings::radar::radar);
						ImGui::Checkbox(xorstr_("Cross"), &settings::radar::cross);
						ImGui::Checkbox(xorstr_("Local Player"), &settings::radar::local_player);
					}
					imgui_custom->end_child();
					ImGui::SameLine();
					imgui_custom->begin_child(xorstr_("Extra Radar Configuration"), ImVec2(window_width / 2, 0));
					{
						ImGui::SliderFloat(xorstr_("Range"), &settings::radar::range, 1, 300, "%.f");
					}





					imgui_custom->end_child();
				}

				if (current_tab == 5)
				{
					imgui_custom->begin_child(xorstr_("Extra Configuration"), ImVec2(ImGui::GetContentRegionAvailWidth(), 0));
					{
						ImGui::Checkbox(xorstr_("Gay Mode"), &settings::misc::gay_mode);
						ImGui::Checkbox(xorstr_("V-Sync"), &settings::misc::vsync);

						ImGui::Spacing();

						if (ImGui::Button("Save Config")) save_cfg("hitler-hitler-shopcore.json");
						ImGui::SameLine();
						if (ImGui::Button("Load Config")) load_cfg("hitler-hitler-shopcore.json");
					}
					imgui_custom->end_child();
				}
			}
			ImGui::EndGroup();
		}
		ImGui::End();

		ImGui::SetNextWindowPos(pos + ImVec2(630, 0));
		ImGui::SetNextWindowSize({ 220, 440 });
		ImGui::Begin(xorstr_("##preview"), &settings::show_menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
		{
			ImDrawList* draw = ImGui::GetWindowDrawList();
			ImVec2 window_pos = ImGui::GetWindowPos();
			ImVec2 window_size = ImGui::GetWindowSize();

			ImGui::PushFont(preview_font);
			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2 - 15);
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(155, 155, 155, 255));
			ImGui::Text(xorstr_("ESP Preview"));
			ImGui::PopStyleColor();
			ImGui::PopFont();
			imgui_custom->gradient_seperator();

			ImVec2 character_pos = window_pos + ImVec2(window_size.x * 0.5f, window_size.y * 0.15f) - ImVec2(60, 0);
			draw->AddImage(character_r, character_pos, character_pos + ImVec2(120, 300));

			if (settings::vis::box)
			{
				ImVec2 box_size(170, 300);
				ImVec2 box_pos = window_pos + (window_size - box_size) * 0.5f;
				if (settings::vis::outline) draw->AddRect(box_pos, box_pos + box_size, ImColor(0, 0, 0, 255), 0, 0, settings::vis::box_thickness + 2);
				draw->AddRect(box_pos, box_pos + box_size, ImColor(255, 0, 0, 255), 0, 0, settings::vis::box_thickness);
			}

			if (settings::vis::skeleton)
			{
				ImVec2 skeleton_pos = window_pos + ImVec2(window_size.x * 0.5f, window_size.y * 0.15f) - ImVec2(70, 0);
				draw->AddImage(skeleton_r, skeleton_pos, skeleton_pos + ImVec2(125, 280));
			}

			if (settings::vis::head)
			{
				float head_radius = 12.0f;
				ImVec2 head_pos = window_pos + ImVec2(window_size.x * 0.5f, window_size.y * 0.20f);
				if (settings::vis::outline) draw->AddCircle(head_pos, head_radius, ImColor(0, 0, 0, 255), 64, settings::vis::skeleton_thickness + 2);
				draw->AddCircle(head_pos, head_radius, ImColor(255, 0, 0, 255), 64, settings::vis::skeleton_thickness);
			}

			float yTopSpace = 5.0f;
			if (settings::vis::username)
			{
				// Define the box size and position
				ImVec2 box_size(170, 300);
				ImVec2 box_pos = window_pos + (window_size - box_size) * 0.5f;

				// Calculate the position for the username text below the platform text
				ImVec2 username_text_size = ImGui::CalcTextSize(xorstr_("hitler-hitler-shopcore"));
				ImVec2 username_pos = ImVec2(box_pos.x + (box_size.x - username_text_size.x) * 0.5f, box_pos.y - username_text_size.y - yTopSpace); // Centered above the box with a little margin

				// Define the background for the username text
				ImVec2 username_background_size = ImVec2(username_text_size.x + 10.0f, username_text_size.y);
				ImVec2 username_background_pos = ImVec2(username_pos.x - 5.0f, username_pos.y);

				// Draw the background if enabled
				if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(username_background_pos, ImVec2(username_background_pos.x + username_background_size.x, username_background_pos.y + username_background_size.y), ImColor(0, 0, 0, 200), 5.0f);

				// Draw the username text
				draw_text(ImGui::GetFont(), 13.0f, username_pos, ImColor(255, 255, 255), xorstr_("hitler-hitler-shopcore"));

				// Increase the top space if platform is enabled
				if (settings::vis::platform) yTopSpace += 15.0f;
			}

			if (settings::vis::platform)
			{
				// Define the box size and position
				ImVec2 box_size(170, 300);
				ImVec2 box_pos = window_pos + (window_size - box_size) * 0.5f;

				// Calculate the position for the platform text above the box
				ImVec2 platform_text_size = ImGui::CalcTextSize(xorstr_("WIN"));
				ImVec2 platform_pos = ImVec2(box_pos.x + (box_size.x - platform_text_size.x) * 0.5f, box_pos.y - platform_text_size.y - yTopSpace); // Centered above the box with a little margin

				// Define the background for the platform text
				ImVec2 platform_background_size = ImVec2(platform_text_size.x + 10.0f, platform_text_size.y);
				ImVec2 platform_background_pos = ImVec2(platform_pos.x - 5.0f, platform_pos.y);

				// Draw the background if enabled
				if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(platform_background_pos, ImVec2(platform_background_pos.x + platform_background_size.x, platform_background_pos.y + platform_background_size.y), ImColor(0, 0, 0, 200), 5.0f);

				// Draw the platform text
				draw_text(ImGui::GetFont(), 13.0f, platform_pos, ImColor(255, 255, 255), xorstr_("WIN"));
			}

			float yBottomSpace = 5.0f;
			if (settings::vis::distance)
			{
				// Define the box size and position
				ImVec2 box_size(170, 300);
				ImVec2 box_pos = window_pos + (window_size - box_size) * 0.5f;

				// Calculate the position for the distance text at the bottom of the box
				ImVec2 distance_text_size = ImGui::CalcTextSize(xorstr_("3m"));
				ImVec2 distance_pos = ImVec2(box_pos.x + (box_size.x - distance_text_size.x) * 0.5f, box_pos.y + box_size.y - distance_text_size.y + (15.0f + yBottomSpace)); // Centered at the bottom of the box

				// Define the background for the distance text
				ImVec2 distance_background_size = ImVec2(distance_text_size.x + 10.0f, distance_text_size.y);
				ImVec2 distance_background_pos = ImVec2(distance_pos.x - 5.0f, distance_pos.y);

				// Draw the background if enabled
				if (settings::vis::text_background) ImGui::GetOverlayDrawList()->AddRectFilled(distance_background_pos, ImVec2(distance_background_pos.x + distance_background_size.x, distance_background_pos.y + distance_background_size.y), ImColor(0, 0, 0, 200), 5.0f);

				// Draw the distance text
				draw_text(ImGui::GetFont(), 13.0f, distance_pos, ImColor(255, 255, 255), xorstr_("3m"));
			}

			if (settings::vis::weapon)
			{
			}

			if (settings::vis::rank)
			{
			}

			if (settings::vis::thanked_bus_driver)
			{
			}

			if (settings::vis::kill_count)
			{
			}

			if (settings::vis::season_level)
			{
			}
		}
		ImGui::End();
	}



	sexspinbot();
	FOVChanger();
	sexinvisible();
	AdjustPlayerSize();










}




WPARAM render_loop()
{
	static RECT old_rc;
	ZeroMemory(&messager, sizeof(MSG));
	while (messager.message != WM_QUIT)
	{
		if (PeekMessage(&messager, my_wnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage_Spoofed(&messager);
			DispatchMessage(&messager);
		}

		HWND hwnd_active = GetForegroundWindow_Spoofed();
		if (hwnd_active == game_wnd)
		{
			HWND hwndxd = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos_Spoofed(my_wnd, hwndxd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT rc;
		POINT xy;
		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect_Spoofed(game_wnd, &rc);
		ClientToScreen_Spoofed(game_wnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;
		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = game_wnd;
		io.DeltaTime = 1.0f / 60.0f;
		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState_Spoofed(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			settings::width = rc.right;
			settings::height = rc.bottom;
			p_params.BackBufferWidth = settings::width;
			p_params.BackBufferHeight = settings::height;
			SetWindowPos_Spoofed(my_wnd, (HWND)0, xy.x, xy.y, settings::width, settings::height, SWP_NOREDRAW);
			p_device->Reset(&p_params);
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			features();
			render_menu();
		}
		ImGui::EndFrame();
		p_device->SetRenderState(D3DRS_ZENABLE, false);
		p_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		p_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		p_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (p_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			p_device->EndScene();
		}
		HRESULT result = p_device->Present(0, 0, 0, 0);
		if (result == D3DERR_DEVICELOST && p_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_device->Reset(&p_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();






	if (p_device != 0)
	{
		p_device->EndScene();
		p_device->Release();
	}

	if (p_object != 0)
	{
		p_object->Release();
	}

	DestroyWindow(my_wnd);


	return messager.wParam;
}