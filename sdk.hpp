#pragma once
#include <d3d9.h>
#include <vector>
#include <algorithm>
#include "imgui/imgui.h"
#include <iostream>
#define M_PI 3.14159265358979323846264338327950288419716939937510

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(double _x, double _y) : x(_x), y(_y) {}
	~Vector2() {}
	double x, y;
};

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}
	double x, y, z;
	inline double dot(Vector3 v) { return x * v.x + y * v.y + z * v.z; }
	inline double distance(Vector3 v) { return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0))); }
	inline double length() { return sqrt(x * x + y * y + z * z); }

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(double number) const
	{
		return Vector3(x * number, y * number, z * number);
	}
};

struct FQuat { double x, y, z, w; };
struct FTransform
{
	FQuat rot;
	Vector3 translation;
	uint8_t pad1c[0x8];
	Vector3 scale;
	uint8_t pad2c[0x8];

	D3DMATRIX to_matrix_with_scale() const
	{
		D3DMATRIX m{};

		Vector3 AdjustedScale
		(
			(scale.x == 0.0) ? 1.0 : scale.x,
			(scale.y == 0.0) ? 1.0 : scale.y,
			(scale.z == 0.0) ? 1.0 : scale.z
		);

		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;
		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;

		m._11 = (1.0f - (yy2 + zz2)) * AdjustedScale.x;
		m._22 = (1.0f - (xx2 + zz2)) * AdjustedScale.y;
		m._33 = (1.0f - (xx2 + yy2)) * AdjustedScale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * AdjustedScale.z;
		m._23 = (yz2 + wx2) * AdjustedScale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * AdjustedScale.y;
		m._12 = (xy2 + wz2) * AdjustedScale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * AdjustedScale.z;
		m._13 = (xz2 - wy2) * AdjustedScale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

D3DMATRIX matrix_multiplication(D3DMATRIX pm1, D3DMATRIX pm2)
{
	D3DMATRIX pout{};
	pout._11 = pm1._11 * pm2._11 + pm1._12 * pm2._21 + pm1._13 * pm2._31 + pm1._14 * pm2._41;
	pout._12 = pm1._11 * pm2._12 + pm1._12 * pm2._22 + pm1._13 * pm2._32 + pm1._14 * pm2._42;
	pout._13 = pm1._11 * pm2._13 + pm1._12 * pm2._23 + pm1._13 * pm2._33 + pm1._14 * pm2._43;
	pout._14 = pm1._11 * pm2._14 + pm1._12 * pm2._24 + pm1._13 * pm2._34 + pm1._14 * pm2._44;
	pout._21 = pm1._21 * pm2._11 + pm1._22 * pm2._21 + pm1._23 * pm2._31 + pm1._24 * pm2._41;
	pout._22 = pm1._21 * pm2._12 + pm1._22 * pm2._22 + pm1._23 * pm2._32 + pm1._24 * pm2._42;
	pout._23 = pm1._21 * pm2._13 + pm1._22 * pm2._23 + pm1._23 * pm2._33 + pm1._24 * pm2._43;
	pout._24 = pm1._21 * pm2._14 + pm1._22 * pm2._24 + pm1._23 * pm2._34 + pm1._24 * pm2._44;
	pout._31 = pm1._31 * pm2._11 + pm1._32 * pm2._21 + pm1._33 * pm2._31 + pm1._34 * pm2._41;
	pout._32 = pm1._31 * pm2._12 + pm1._32 * pm2._22 + pm1._33 * pm2._32 + pm1._34 * pm2._42;
	pout._33 = pm1._31 * pm2._13 + pm1._32 * pm2._23 + pm1._33 * pm2._33 + pm1._34 * pm2._43;
	pout._34 = pm1._31 * pm2._14 + pm1._32 * pm2._24 + pm1._33 * pm2._34 + pm1._34 * pm2._44;
	pout._41 = pm1._41 * pm2._11 + pm1._42 * pm2._21 + pm1._43 * pm2._31 + pm1._44 * pm2._41;
	pout._42 = pm1._41 * pm2._12 + pm1._42 * pm2._22 + pm1._43 * pm2._32 + pm1._44 * pm2._42;
	pout._43 = pm1._41 * pm2._13 + pm1._42 * pm2._23 + pm1._43 * pm2._33 + pm1._44 * pm2._43;
	pout._44 = pm1._41 * pm2._14 + pm1._42 * pm2._24 + pm1._43 * pm2._34 + pm1._44 * pm2._44;
	return pout;
}

D3DMATRIX to_matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radpitch = (rot.x * M_PI / 180);
	float radyaw = (rot.y * M_PI / 180);
	float radroll = (rot.z * M_PI / 180);
	float sp = sinf(radpitch);
	float cp = cosf(radpitch);
	float sy = sinf(radyaw);
	float cy = cosf(radyaw);
	float sr = sinf(radroll);
	float cr = cosf(radroll);
	D3DMATRIX matrix{};
	matrix.m[0][0] = cp * cy;
	matrix.m[0][1] = cp * sy;
	matrix.m[0][2] = sp;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = sr * sp * cy - cr * sy;
	matrix.m[1][1] = sr * sp * sy + cr * cy;
	matrix.m[1][2] = -sr * cp;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(cr * sp * cy + sr * sy);
	matrix.m[2][1] = cy * sr - cr * sp * sy;
	matrix.m[2][2] = cr * cp;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;
	return matrix;
}


enum class EFortRarity : uint8_t
{
	EFortRarity__Common = 0,
	EFortRarity__Uncommon = 1,
	EFortRarity__Rare = 2,
	EFortRarity__Epic = 3,
	EFortRarity__Legendary = 4,
	EFortRarity__Mythic = 5,
	EFortRarity__Transcendent = 6,
	EFortRarity__Unattainable = 7,
	EFortRarity__NumRarityValues = 8,
};

struct Camera
{
	Vector3 location;
	Vector3 rotation;
	float fov;
};

struct FNRot
{
	double a;
	char pad_0008[24];
	double b;
	char pad_0028[424];
	double c;
};

struct FBounds
{
	float top, bottom, left, right;
};

struct FPlane : Vector3
{
	double W;

	FPlane() : W(0) { }
	FPlane(double W) : W(W) { }
};

class FMatrix
{
public:
	double m[4][4];
	FPlane XPlane, YPlane, ZPlane, WPlane;

	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() { }
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane)
		: XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) { }
};

inline double RadiansToDegrees(double dRadians)
{
	return dRadians * (180.0 / M_PI);
}

template< typename t >
class TArray
{
public:
	TArray() : tData(), iCount(), iMaxCount() { }
	TArray(t* data, int count, int max_count) :
		tData(tData), iCount(iCount), iMaxCount(iMaxCount) { }

public:
	auto Get(int idx) -> t
	{
		return read< t >(reinterpret_cast<__int64>(this->tData) + (idx * sizeof(t)));
	}

	auto Size() -> std::uint32_t
	{
		return this->iCount;
	}

	bool IsValid()
	{
		return this->iCount != 0;
	}

	t* tData;
	int iCount;
	int iMaxCount;
};

namespace cache
{
	inline uintptr_t gworld;
	inline uintptr_t game_instance;
	inline uintptr_t local_players;
	inline uintptr_t player_controller;
	inline uintptr_t local_pawn;
	inline uintptr_t player_state;
	inline uintptr_t current_weapon;
	inline uintptr_t root_component;
	inline Vector3 local_relative_location;
	inline char team_id;
	inline float closest_distance;
	inline uintptr_t closest_mesh;
	inline uintptr_t closest_entity;
	inline Camera local_camera;
}

auto GetViewState() -> uintptr_t
{
	TArray<uintptr_t> ViewState = read<TArray<uintptr_t>>(cache::local_players + 0xD0);
	return ViewState.Get(1);
}


Camera get_view_point()
{
	Camera view_point{};
	uintptr_t location_pointer = read<uintptr_t>(cache::gworld + 0x170);
	uintptr_t rotation_pointer = read<uintptr_t>(cache::gworld + 0x180);
	FNRot fnrot{};
	fnrot.a = read<double>(rotation_pointer);
	fnrot.b = read<double>(rotation_pointer + 0x20);
	fnrot.c = read<double>(rotation_pointer + 0x1D0);
	view_point.location = read<Vector3>(location_pointer);
	view_point.rotation.x = asin(fnrot.c) * (180.0 / M_PI);
	view_point.rotation.y = ((atan2(fnrot.a * -1, fnrot.b) * (180.0 / M_PI)) * -1) * -1;
	view_point.fov = read<float>(cache::player_controller + 0x3AC) * 90.0f;
	return view_point;
}

double get_cross_distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

ImVec2 calc_aim(const Vector2& target2D, const ImVec2& screenCenter, int aimSpeed, float screenWidth, float screenHeight)
{
	float targetX = 0.0f;
	float targetY = 0.0f;

	if (target2D.x != 0)
	{
		targetX = (target2D.x > screenCenter.x) ? -(screenCenter.x - target2D.x) / aimSpeed : (target2D.x - screenCenter.x) / aimSpeed;
		if ((target2D.x > screenCenter.x && targetX + screenCenter.x > screenWidth) || (target2D.x < screenCenter.x && targetX + screenCenter.x < 0)) targetX = 0.0f;
	}

	if (target2D.y != 0)
	{
		targetY = (target2D.y > screenCenter.y) ? -(screenCenter.y - target2D.y) / aimSpeed : (target2D.y - screenCenter.y) / aimSpeed;
		if ((target2D.y > screenCenter.y && targetY + screenCenter.y > screenHeight) || (target2D.y < screenCenter.y && targetY + screenCenter.y < 0)) targetY = 0.0f;
	}

	return ImVec2(targetX, targetY);
}

class SDK
{
public:
	Vector2 project_world_to_screen(Vector3 world_location)
	{
		cache::local_camera = get_view_point();
		D3DMATRIX temp_matrix = to_matrix(cache::local_camera.rotation);
		Vector3 vaxisx = Vector3(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
		Vector3 vaxisy = Vector3(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
		Vector3 vaxisz = Vector3(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);
		Vector3 vdelta = world_location - cache::local_camera.location;
		Vector3 vtransformed = Vector3(vdelta.dot(vaxisy), vdelta.dot(vaxisz), vdelta.dot(vaxisx));

		if (vtransformed.z < 1) vtransformed.z = 1;

		return Vector2(settings::screen_center_x + vtransformed.x * ((settings::screen_center_x / tanf(cache::local_camera.fov * (float)M_PI / 360.0f))) / vtransformed.z, settings::screen_center_y - vtransformed.y * ((settings::screen_center_x / tanf(cache::local_camera.fov * (float)M_PI / 360.0f))) / vtransformed.z);
	}

	Vector3 get_entity_bone(uintptr_t mesh, int bone_id)
	{
		uintptr_t bone_array = read<uintptr_t>(mesh + offsets::bone_array);
		if (bone_array == 0) bone_array = read<uintptr_t>(mesh + offsets::bone_cache);
		FTransform bone = read<FTransform>(bone_array + (bone_id * 0x60));
		FTransform component_to_world = read<FTransform>(mesh + offsets::component_to_world);
		D3DMATRIX matrix = matrix_multiplication(bone.to_matrix_with_scale(), component_to_world.to_matrix_with_scale());
		return Vector3(matrix._41, matrix._42, matrix._43);
	}

	FBounds get_actor_bounds(Vector2 head2d, Vector2 bottom2d, float size)
	{
		FBounds a_bounds;
		a_bounds.top = head2d.y;
		a_bounds.bottom = bottom2d.y;
		a_bounds.left = (head2d.x - (size / 2));
		a_bounds.right = bottom2d.x + (size / 2);

		return a_bounds;
	}

	bool is_visible(uintptr_t mesh)
	{
		auto Seconds = read<double>(cache::gworld + 0x190);
		auto LastRenderTime = read<float>(mesh + 0x32C);
		return Seconds - LastRenderTime <= 0.06f;
	}

	bool is_in_fov(const Vector2& target2D, float screenWidth, float screenHeight)
	{
		float distance = get_cross_distance(target2D.x, target2D.y, screenWidth / 2, screenHeight / 2);

		return distance <= settings::aim::fov && target2D.x != 0 && target2D.y != 0;
	}

	bool is_in_screen(Vector2 screen_location)
	{
		if (screen_location.x > 0 && screen_location.x < settings::width && screen_location.y > 0 && screen_location.y < settings::height)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool is_dead(uintptr_t pawn_private)
	{
		return (read<char>(pawn_private + offsets::b_is_dying) >> 5) & 1;
	}

	std::string get_username(uintptr_t playerstate)
	{
		__int64 FString = read<__int64>(playerstate + offsets::p_name_structure);
		int Length = read<int>(FString + 16);
		uintptr_t FText = read<__int64>(FString + 8);

		__int64 v6 = Length;
		if (!v6) return std::string("BOT");

		wchar_t* NameBuffer = new wchar_t[Length];
		huskyud::read_physical(FText, NameBuffer, Length * sizeof(wchar_t));

		char v21;
		int v22;
		int i;
		int v25;
		WORD* v23;

		v21 = v6 - 1;
		if (!(DWORD)v6)
			v21 = 0;
		v22 = 0;
		v23 = (WORD*)NameBuffer;
		for (i = (v21) & 3; ; *v23++ += i & 7)
		{
			v25 = v6 - 1;
			if (!(DWORD)v6)
				v25 = 0;
			if (v22 >= v25)
				break;
			i += 3;
			++v22;
		}

		std::wstring wbuffer{ NameBuffer };

		return std::string(wbuffer.begin(), wbuffer.end());
	}

	std::string get_weapon_name(uintptr_t pawn_private)
	{
		std::string weapon_name;

		uint64_t current_weapon = read<uint64_t>(pawn_private + offsets::current_weapon);
		
		uint64_t weapon_data = read<uint64_t>(current_weapon + offsets::weapon_data);


		uint64_t item_name = read<uint64_t>(weapon_data + 0x40);

		uint64_t FData = read<uint64_t>(item_name + 0x20);
		int FLength = read<int>(item_name + 0x28);

		if (FLength > 0 && FLength < 50)
		{
			wchar_t* WeaponBuffer = new wchar_t[FLength];
			huskyud::read_physical(FData, (PVOID)WeaponBuffer, FLength * sizeof(wchar_t));
			std::wstring wstr_buf(WeaponBuffer);
			weapon_name.append(std::string(wstr_buf.begin(), wstr_buf.end()));

			delete[] WeaponBuffer;
		}

		return weapon_name;
	}

	std::string get_rank_name(int tier)
	{
		switch (tier)
		{
		case 0: return "Bronze I";
		case 1: return "Bronze II";
		case 2: return "Bronze III";
		case 3: return "Silver I";
		case 4: return "Silver II";
		case 5: return "Silver III";
		case 6: return "Gold I";
		case 7: return "Gold II";
		case 8: return "Gold III";
		case 9: return "Platinum I";
		case 10: return "Platinum II";
		case 11: return "Platinum III";
		case 12: return "Diamond I";
		case 13: return "Diamond II";
		case 14: return "Diamond III";
		case 15: return "Elite";
		case 16: return "Champion";
		case 17: return "Unreal";
		default: return "Unranked";
		}
	}

	ImVec4 get_rank_color(int tier)
	{
		switch (tier)
		{
		case 0: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // Unranked
		case 1:
		case 2: return ImVec4(0.902f, 0.580f, 0.227f, 1.0f); // Bronze
		case 3:
		case 4:
		case 5: return ImVec4(0.843f, 0.843f, 0.843f, 1.0f); // Silver
		case 6:
		case 7:
		case 8: return ImVec4(1.0f, 0.871f, 0.0f, 1.0f); // Gold
		case 9:
		case 10:
		case 11: return ImVec4(0.0f, 0.7f, 0.7f, 1.0f);  // Platinum
		case 12:
		case 13:
		case 14: return ImVec4(0.1686f, 0.3294f, 0.8235f, 1.0f); // Diamond
		case 15: return ImVec4(0.7f, 0.7f, 0.7f, 1.0f);   // Elite
		case 16: return ImVec4(1.0f, 0.6f, 0.0f, 1.0f);   // Champion
		case 17: return ImVec4(0.6f, 0.0f, 0.6f, 1.0f);   // Unreal
		default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // Unranked
		}
	}

	ImColor get_loot_color(EFortRarity tier)
	{
		if (tier == EFortRarity::EFortRarity__Common)
		{
			return ImColor(123, 123, 123, 255);
		}
		else if (tier == EFortRarity::EFortRarity__Uncommon)
		{
			return ImColor(58, 121, 19, 255);
		}
		else if (tier == EFortRarity::EFortRarity__Rare)
		{
			return ImColor(18, 88, 162, 255);
		}
		else if (tier == EFortRarity::EFortRarity__Epic)
		{
			return ImColor(189, 63, 250, 255);
		}
		else if (tier == EFortRarity::EFortRarity__Legendary)
		{
			return ImColor(255, 118, 5, 255);
		}
		else if (tier == EFortRarity::EFortRarity__Mythic)
		{
			return ImColor(220, 160, 30, 255);
		}
		else if (tier == EFortRarity::EFortRarity__Transcendent)
		{
			return ImColor(0, 225, 252, 255);
		}

		return ImColor(123, 123, 123, 255);
	}

	void radar_range(float* x, float* y, float range)
	{
		if (fabs((*x)) > range || fabs((*y)) > range)
		{
			if ((*y) > (*x))
			{
				if ((*y) > -(*x))
				{
					(*x) = range * (*x) / (*y);
					(*y) = range;
				}
				else
				{
					(*y) = -range * (*y) / (*x);
					(*x) = -range;
				}
			}
			else
			{
				if ((*y) > -(*x))
				{
					(*y) = range * (*y) / (*x);
					(*x) = range;
				}
				else
				{
					(*x) = -range * (*x) / (*y);
					(*y) = -range;
				}
			}
		}
	}

	void project_world_to_radar(Vector3 vOrigin, int& screenx, int& screeny, ImVec2 pos)
	{
		Vector3 vAngle = cache::local_camera.rotation;
		auto fYaw = vAngle.y * M_PI / 180.0f;
		float dx = vOrigin.x - cache::local_camera.location.x;
		float dy = vOrigin.y - cache::local_camera.location.y;
		float fsin_yaw = sinf(fYaw);
		float fminus_cos_yaw = -cosf(fYaw);
		float x = dy * fminus_cos_yaw + dx * fsin_yaw;
		x = -x;
		float y = dx * fminus_cos_yaw - dy * fsin_yaw;
		float range = (float)(settings::radar::range / 5) * 1000.f;

		radar_range(&x, &y, range);

		ImVec2 DrawPos = ImVec2(pos.x, pos.y);
		ImVec2 DrawSize = ImVec2(200.0f, 200.0f);
		float radarRadius = 200.0f / 2;
		float distance = sqrt(pow(screenx - DrawPos.x, 2) + pow(screeny - DrawPos.y, 2));

		if (distance > radarRadius)
		{
			float angle = atan2(screeny - DrawPos.y, screenx - DrawPos.x);
			screenx = static_cast<int>(DrawPos.x + radarRadius * cos(angle));
			screeny = static_cast<int>(DrawPos.y + radarRadius * sin(angle));
		}

		int rad_x = (int)DrawPos.x;
		int rad_y = (int)DrawPos.y;
		float r_siz_x = DrawSize.x;
		float r_siz_y = DrawSize.y;
		int x_max = (int)r_siz_x + rad_x - 5;
		int y_max = (int)r_siz_y + rad_y - 5;

		screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
		screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

		if (screenx > x_max)
			screenx = x_max;

		if (screenx < rad_x)
			screenx = rad_x;

		if (screeny > y_max)
			screeny = y_max;

		if (screeny < rad_y)
			screeny = rad_y;
	}
};

inline SDK sdk;