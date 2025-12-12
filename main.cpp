#include <iostream>
#include "network_tools.h"

int main() {

    std::wcout << L"Network Scanner Starting...\n";
    std::wcout << L'\n';

    ShowActiveAdapter();
    
    return 0;
}