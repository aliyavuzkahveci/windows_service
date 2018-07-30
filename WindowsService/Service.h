#ifndef _SERVICE_H_
#define _SERVICE_H_

/*@author  Ali Yavuz Kahveci aliyavuz.kahveci@tav.aero
* @version 1.0
* @since   23-07-2018
* @Purpose: Provides a sample service class that derives from the service base class -
* ServiceBase. The sample service logs the service start and stop
* information to the Application event log, and shows how to run the main
* function of the service in a thread pool worker thread.
Taken from: https://code.msdn.microsoft.com/windowsapps/CppWindowsService-cacf4948#content
*/
#pragma once

#include "ServiceBase.h"

namespace WindowsService
{
	class Service : public ServiceBase
	{
	public:
		Service(PWSTR pszServiceName,
			BOOL fCanStop = TRUE,
			BOOL fCanShutdown = TRUE,
			BOOL fCanPauseContinue = TRUE);

		virtual ~Service(void);

	protected:
		virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);
		virtual void OnStop();
		virtual void OnPause();
		virtual void OnContinue();

		void ServiceWorkerThread(void);

	private:
		BOOL m_fPaused;
		BOOL m_fStopping;
		HANDLE m_hStoppedEvent;
	};
}

#endif
