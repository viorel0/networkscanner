#include "network_tools.h"
#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h> 
#include <memory>

void PrintIpAddress(sockaddr* sa, const wchar_t* label) {
    wchar_t ipString[INET6_ADDRSTRLEN] = { 0 };

    if (sa->sa_family == AF_INET) {
        sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(sa);
        InetNtopW(AF_INET, &(ipv4->sin_addr), ipString, INET6_ADDRSTRLEN);
        std::wcout << label << ipString << L"\n";
    }
    else if (sa->sa_family == AF_INET6) {
        sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(sa);
        InetNtopW(AF_INET6, &(ipv6->sin6_addr), ipString, INET6_ADDRSTRLEN);
        std::wcout << label << ipString << L"\n";
    }
}


void ShowActiveAdapter(){
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS ; //| GAA_FLAG_INCLUDE_ALL_INTERFACES;
    ULONG family = AF_UNSPEC;
    ULONG outBufLen = 0;
    
    bool foundInternet = false;

    GetAdaptersAddresses(family, flags, NULL, nullptr, &outBufLen);
    std::unique_ptr<char[]> pBuffer(new char[outBufLen]); 
    PIP_ADAPTER_ADDRESSES pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(pBuffer.get());
    GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

    
    PIP_ADAPTER_ADDRESSES pCurr = pAddresses;

    while (pCurr) {
        
        PWCHAR adapterName = pCurr->FriendlyName;
        
        IF_OPER_STATUS operStatus = pCurr->OperStatus;
        PIP_ADAPTER_GATEWAY_ADDRESS pDefGateway = pCurr->FirstGatewayAddress;
        PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurr->FirstUnicastAddress;

        if (operStatus == IfOperStatusUp && pDefGateway != nullptr) {
            foundInternet = true;
            std::wcout << L"--------------------------------\n";
            std::wcout << L"Adapter: " << adapterName << L"\n";
            std::wcout << L"   Status: Up\n";
            
            //Loop through all IP addresses assigned to this adapter
            while (pUnicast != nullptr) {
                PrintIpAddress(pUnicast->Address.lpSockaddr, L"   My IP:   ");
                // Move to next IP address
                pUnicast = pUnicast->Next;
            }
    
            while(pDefGateway != nullptr){
                PrintIpAddress(pDefGateway->Address.lpSockaddr, L"   Gateway:   ");
                //Move to next gateway address
                pDefGateway = pDefGateway->Next;
            }
            NET_IF_CONNECTION_TYPE connType = pCurr->ConnectionType;
            std::wcout << L"   Connection Type: " << connType << L"\n";
        }
        // ULONG64 transmitspeed = pCurr->TransmitLinkSpeed;
        // ULONG64 receivespeed = pCurr->ReceiveLinkSpeed;
        // std::wcout << L"   Transmit Speed: " << transmitspeed / 1000000 << L" Mbps\n";
        // std::wcout << L"   Receive Speed: " << receivespeed / 1000000 << L" Mbps\n";
        
        pCurr = pCurr->Next;
    }

    if (!foundInternet) {
        std::wcout << L"No active internet connection found.\n";
    }

}