#ifndef _SERVICE_THREAD_H_
#define _SERVICE_THREAD_H_
/*@author  Ali Yavuz Kahveci aliyavuzkahveci@gmail.com
* @version 1.0
* @since   23-07-2018
* @Purpose: Provides the ability to queue
* simple member functions of a class to the Windows thread pool.
Taken from: https://code.msdn.microsoft.com/windowsapps/CppWindowsService-cacf4948#content
*/
#pragma once

#include <memory>

namespace WindowsService
{
	class ServiceThread
	{
	public:
		template <typename T>
		static void QueueUserWorkItem(void (T::*function)(void),
			T *object, ULONG flags = WT_EXECUTELONGFUNCTION)
		{
			typedef std::pair<void (T::*)(), T *> CallbackType;
			std::auto_ptr<CallbackType> p(new CallbackType(function, object));

			if (::QueueUserWorkItem(ThreadProc<T>, p.get(), flags))
			{
				// The ThreadProc now has the responsibility of deleting the pair.
				p.release();
			}
			else
			{
				throw GetLastError();
			}
		}

	private:

		template <typename T>
		static DWORD WINAPI ThreadProc(PVOID context)
		{
			typedef std::pair<void (T::*)(), T *> CallbackType;

			std::auto_ptr<CallbackType> p(static_cast<CallbackType *>(context));

			(p->second->*p->first)();
			return 0;
		}
	};
}

#endif
