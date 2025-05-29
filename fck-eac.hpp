#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <iostream>
#include <windows.h>
#include <wininet.h>
#include <string>
#include <filesystem>
#include <fstream>

#pragma comment(lib, "wininet.lib")

namespace fs = std::filesystem;

bool DownloadFile(const std::string& url, const std::string& filePath);
void SearchAndReplaceSplashScreen(const std::string& downloadUrl);

#endif 