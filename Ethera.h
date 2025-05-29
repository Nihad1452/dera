#ifndef ETHERA_PROTECT
#define ETHERA_PROTECT

#include <string>
#include <vector>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "ntdll.lib")

#define Hide_Function() try { int* __INVALID_ALLOC__ = new int[(std::size_t)std::numeric_limits<std::size_t>::max]; } catch ( const std::bad_alloc& except ) { 
#define Hide_Function_End() } 

std::string Ethera_License = "Ethera-0iOV-ISJu-sF6n-6Smq";
std::string Ethera_Webhook = "0";

bool Ethera_Logs = true;
bool Ethera_Bluescreen = true;
bool Ethera_Imgui_Support = false;
bool Ethera_Block_All_Console_Windows = false;

extern "C"
{
	class Protection
	{
	public:
		void Protect(std::string client_license, std::string webhook, bool discord_logs, bool bluescreen, bool imgui_support, bool block_all_console_windows);
		void Check();
		void Print(const std::string text);
		void Wait(int milliseconds);
		void Authswap();
		void Map_Driver(std::vector<uint8_t> data);
		std::string Getinput(); // example string License = Ethera_Getinput();
	}; Protection Ethera;
}
#endif
