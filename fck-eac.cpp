#include "fck-eac.hpp"
#include <Windows.h>
#include <iostream>
#include <sapi.h>
#include <fstream>
#include <filesystem>
#include <thread>
#include "function_spoofer.h"

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <sapi.h>

void EnableAnsiColors() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hConsole, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, dwMode);
    }
}

void textToSpeech3(const std::string& text) {
    ISpVoice* pVoice = nullptr;
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
        if (SUCCEEDED(hr) && pVoice) {
            std::wstring wText(text.begin(), text.end());
            pVoice->Speak(wText.c_str(), SPF_DEFAULT, NULL);
            pVoice->Release();
        }
        CoUninitialize();
    }
}

// Main function with default param
void typeAndSpeak3(const std::string& text, int delay, bool showTimestamp = true) {
    std::thread tSpeak(textToSpeech3, text);

    if (showTimestamp) {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &now_time_t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");

        std::cout << "[" << oss.str() << "] ";
    }

    int startR = 10, startG = 125, startB = 255;
    int endR = 0, endG = 0, endB = 255;
    int length = (int)text.length();

    for (int i = 0; i < length; ++i) {
        int r = startR + (endR - startR) * i / length;
        int g = startG + (endG - startG) * i / length;
        int b = startB + (endB - startB) * i / length;

        // ANSI escape for 24-bit color
        std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m";
        std::cout << text[i] << std::flush;
        Sleep(delay);
    }
    std::cout << "\033[0m\n";

    tSpeak.join();
}

// Overload to fix linker error - calls main function with default showTimestamp=true
void typeAndSpeak3(const std::string& text, int delay) {
    typeAndSpeak3(text, delay, true);
}

namespace fs = std::filesystem;

bool DownloadFile(const std::string& url, const std::string& filePath) {
    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    char buffer[1024];

    hInternet = InternetOpenW(L"HTTP Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        return false;
    }

    hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return false;
    }

    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile.is_open()) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        outFile.write(buffer, bytesRead);
    }

    outFile.close();
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return true;
}

void SearchAndReplaceSplashScreen(const std::string& downloadUrl) {

    DWORD drives = GetLogicalDrives();

    for (char drive = 'A'; drive <= 'Z'; drive++) {
        if (drives & (1 << (drive - 'A'))) {
            std::string drivePath(1, drive);
            drivePath += ":\\";

            system("cls");

            try {
                for (const auto& entry : fs::recursive_directory_iterator(drivePath)) {
                    if (entry.is_directory() && entry.path().filename() == "EasyAntiCheat") {
                        std::string splashscreenPath = entry.path().string() + "\\Splashscreen.png";
                        if (fs::exists(splashscreenPath)) {
                            if (DownloadFile(downloadUrl, splashscreenPath)) {

                            }
                            else {

                            }
                            return;
                        }
                    }
                }
            }
            catch (const fs::filesystem_error& e) {

            }
        }
    }
}
