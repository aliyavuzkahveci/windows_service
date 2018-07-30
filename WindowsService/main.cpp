// WindowsService.cpp : Defines the entry point for the console application.
//

#pragma region Includes
#include <csignal>
#include <iostream>
#include "ServiceInstaller.h"
#include "Service.h"
#pragma endregion

// 
// Settings of the service
// 

// Internal name of the service
#define SERVICE_NAME             L"AYK Test Service"

// Displayed name of the service
#define SERVICE_DISPLAY_NAME     L"AYK Test Service"

// Service start options.
#define SERVICE_START_TYPE       SERVICE_AUTO_START

// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     L"Netman\0LanmanServer\0\0"

// The name of the account under which the service should run
#define SERVICE_ACCOUNT          L"LocalSystem" //L"NT AUTHORITY\\LocalService"

// The password to the service account name
#define SERVICE_PASSWORD         NULL

#define INSTALL "install"
#define REMOVE "remove"

using namespace WindowsService;

int main(int argc, char* argv[])
{
	if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/'))))
	{
		if (strcmp(INSTALL, argv[1] + 1) == 0)
		{
			// Install the service when the command is 
			// "-install" or "/install".
			InstallService(
				SERVICE_NAME,               // Name of service
				SERVICE_DISPLAY_NAME,       // Name to display
				SERVICE_START_TYPE,         // Service start type
				SERVICE_DEPENDENCIES,       // Dependencies
				SERVICE_ACCOUNT,            // Service running account
				SERVICE_PASSWORD            // Password of the account
			);
			return 0;
		}
		else if (strcmp(REMOVE, argv[1] + 1) == 0)
		{
			// Uninstall the service when the command is 
			// "-remove" or "/remove".
			UninstallService(SERVICE_NAME);
			return 0;
		}
	}

	wprintf(L"Parameters:\n");
	wprintf(L" -install  to install the service.\n");
	wprintf(L" -remove   to remove the service.\n");

	Service service(SERVICE_NAME);
	if (!Service::Run(service))
	{
		wprintf(L"Service failed to run w/err 0x%08lx\n", GetLastError());
	}

    return 0;
}
