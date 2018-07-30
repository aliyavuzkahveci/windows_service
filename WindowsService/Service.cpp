#pragma region Includes
#include <thread>
#include <chrono>

#include "Service.h"
#include "ServiceThread.h"
#pragma endregion

namespace WindowsService
{
	Service::Service(PWSTR pszServiceName,
		BOOL fCanStop,
		BOOL fCanShutdown,
		BOOL fCanPauseContinue)
		: ServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
	{
		m_fPaused = FALSE;
		m_fStopping = FALSE;

		// Create a manual-reset event that is not signaled at first to indicate 
		// the stopped signal of the service.
		m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_hStoppedEvent == NULL)
		{
			throw GetLastError();
		}
	}


	Service::~Service(void)
	{
		if (m_hStoppedEvent)
		{
			CloseHandle(m_hStoppedEvent);
			m_hStoppedEvent = NULL;
		}
	}

	//
	//   FUNCTION: Service::OnStart(DWORD, LPWSTR *)
	//
	//   PURPOSE: The function is executed when a Start command is sent to the 
	//   service by the SCM or when the operating system starts (for a service 
	//   that starts automatically). It specifies actions to take when the 
	//   service starts. In this code sample, OnStart logs a service-start 
	//   message to the Application log, and queues the main service function for 
	//   execution in a thread pool worker thread.
	//
	//   PARAMETERS:
	//   * dwArgc   - number of command line arguments
	//   * lpszArgv - array of command line arguments
	//
	//   NOTE: A service application is designed to be long running. Therefore, 
	//   it usually polls or monitors something in the system. The monitoring is 
	//   set up in the OnStart method. However, OnStart does not actually do the 
	//   monitoring. The OnStart method must return to the operating system after 
	//   the service's operation has begun. It must not loop forever or block. To 
	//   set up a simple monitoring mechanism, one general solution is to create 
	//   a timer in OnStart. The timer would then raise events in your code 
	//   periodically, at which time your service could do its monitoring. The 
	//   other solution is to spawn a new thread to perform the main service 
	//   functions, which is demonstrated in this code sample.
	//
	void Service::OnStart(DWORD dwArgc, LPWSTR *lpszArgv)
	{
		// Log a service start message to the Application log.
		WriteEventLogEntry(L"AYK Test Service in OnStart", EVENTLOG_INFORMATION_TYPE);

		// Queue the main service function for execution in a worker thread.
		ServiceThread::QueueUserWorkItem(&Service::ServiceWorkerThread, this);
	}


	//
	//   FUNCTION: Service::ServiceWorkerThread(void)
	//
	//   PURPOSE: The method performs the main function of the service. 
	//   It runs on a thread pool worker thread.
	//
	void Service::ServiceWorkerThread(void)
	{
		WriteEventLogEntry(L"Entry point of 'AYK Test Service'...", EVENTLOG_INFORMATION_TYPE);

		while (!m_fStopping)
		{
			WriteEventLogEntry(L"'AYK Test Service' is running...", EVENTLOG_INFORMATION_TYPE);

			std::this_thread::sleep_for(std::chrono::seconds(5));

			while (m_fPaused && !m_fStopping)
				std::this_thread::sleep_for(std::chrono::seconds(5)); //keep sleeping since the service is STOPPED!
		}
		
		WriteEventLogEntry(L"'AYK Test Service' is shutting down...", EVENTLOG_INFORMATION_TYPE);
		return;
	}


	//
	//   FUNCTION: Service::OnStop(void)
	//
	//   PURPOSE: The function is executed when a Stop command is sent to the 
	//   service by SCM. It specifies actions to take when a service stops 
	//   running. In this code sample, OnStop logs a service-stop message to the 
	//   Application log, and waits for the finish of the main service function.
	//
	//   COMMENTS:
	//   Be sure to periodically call ReportServiceStatus() with 
	//   SERVICE_STOP_PENDING if the procedure is going to take long time. 
	//
	void Service::OnStop()
	{
		// Log a service stop message to the Application log.
		WriteEventLogEntry(L"AYK Test Service in OnStop", EVENTLOG_INFORMATION_TYPE);

		// Indicate that the service is stopping and wait for the finish of the 
		// main service function (ServiceWorkerThread).
		m_fStopping = TRUE;
		if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
		{
			throw GetLastError();
		}
	}

	void Service::OnPause()
	{
		WriteEventLogEntry(L"AYK Test Service in OnPause", EVENTLOG_INFORMATION_TYPE);

		m_fPaused = TRUE;
	}

	void Service::OnContinue()
	{
		WriteEventLogEntry(L"AYK Test Service in OnContinue", EVENTLOG_INFORMATION_TYPE);

		m_fPaused = FALSE;
	}

}
