#include <cstdio>
#include <Windows.h>
#include <string>
#include <thread>
#include <iostream>
#include <fstream>
#include <regex> 
#include <sstream>

// Networking
#include <wininet.h>

#pragma comment(lib, "Wininet.lib") 

std::string url = "https://pastebin.com/raw/tFpk49kq"; 

DWORD GetAddress(const std::string& varName) {
    HINTERNET Internet = InternetOpen("HTTP GET", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (Internet) {
        HINTERNET URL = InternetOpenUrl(Internet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (URL) {
            char buffer[1024];
            DWORD bytesRead = 0;
            std::string Data;

            while (InternetReadFile(URL, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                Data.append(buffer, bytesRead);
            }

            InternetCloseHandle(URL);

            std::regex pattern(varName + R"(\s*=\s*0x([0-9A-Fa-f]+))");
            std::smatch match;

            if (std::regex_search(Data, match, pattern)) {
                DWORD address;
                std::stringstream(match.str(1)) >> std::hex >> address;
                return address;
            }
            else {
                return 0;
            }
        }
        else {
            return 0;
        }

        InternetCloseHandle(Internet);
    }
    else {
        return 0;
    }
    
}

int main() {

    printf("Address 0x%lx\n", GetAddress("Address"));
    printf("NewAddress 0x%lx\n", GetAddress("NewAddress"));
    printf("NULL 0x%lx\n", GetAddress("NULL"));

    return 0;
}