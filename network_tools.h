#pragma once // Prevents including this file twice
#include <winsock2.h>
#include <iphlpapi.h>
#include <string>

// If using Visual Studio, keep this. If using Makefile, use -lws2_32
// #pragma comment(lib, "ws2_32.lib") 
// #pragma comment(lib, "iphlpapi.lib")

// Function Promises
void PrintIpAddress(sockaddr* sa, const wchar_t* label);
void ShowActiveAdapter();