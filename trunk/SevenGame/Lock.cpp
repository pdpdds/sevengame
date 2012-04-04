// Lock.cpp: implementation of the CLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLockCritical::CLockCritical(LPCTSTR szName)
{
	_tcscpy_s(m_szName, szName); 
//	InitializeCriticalSection(&m_hCritical);
}

CLockCritical::~CLockCritical()
{
//	DeleteCriticalSection(&m_hCritical);
}

BOOL CLockCritical::Lock(DWORD dwTimeOut, BOOL bWriteLog)
{
//	EnterCriticalSection(&m_hCritical); 
	return TRUE; 
}

void CLockCritical::Unlock()
{
	//LeaveCriticalSection(&m_hCritical); 
}
////////////////////////////////////////////////////////////////////////
CLockEvent::CLockEvent(LPCTSTR szName)
{
//	strcpy(m_szName, szName); 
//	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 
//	ResetEvent(m_hEvent); 
 //	m_dwOwnerThread = 0; 
//	m_iLockCount = 0 ; 
}
 
CLockEvent::~CLockEvent()
{
	ResetEvent(m_hEvent); 
	CloseHandle(m_hEvent);
} 

BOOL CLockEvent::Lock(DWORD dwTimeOut, BOOL bWriteLog)
{
	if ( m_dwOwnerThread == 0 )
	{
		m_dwOwnerThread = GetCurrentThreadId(); 
		m_iLockCount ++; 
		return TRUE;
	}
	else if ( m_dwOwnerThread == GetCurrentThreadId() )
	{
		m_iLockCount ++;
		return TRUE; 
	}

	DWORD dwResult = 0; 
	while ( 1 )
	{
		DWORD dwWaitResult = 0 ; 
		dwWaitResult = ::WaitForSingleObject(m_hEvent, dwTimeOut); 
		if ( dwWaitResult == WAIT_TIMEOUT)
		{			
			return FALSE; 
		}
		else if ( dwWaitResult == WAIT_OBJECT_0)
		{		
			if ( m_dwOwnerThread == NULL )
			{
				m_dwOwnerThread = GetCurrentThreadId(); 
				m_iLockCount++; 
				return TRUE; 
			}			
		}
		else if ( dwWaitResult == WAIT_FAILED )
		{
			return FALSE; 
		}
		else if ( dwWaitResult == WAIT_ABANDONED)
		{
			return FALSE; 			
		}
	}
}

void CLockEvent::Unlock()
{
	if ( m_dwOwnerThread == GetCurrentThreadId() ) 
	{
		m_iLockCount --; 
		if ( m_iLockCount <= 0 )
		{
			m_dwOwnerThread = 0; 
			if ( !SetEvent(m_hEvent) ) 
			{
			}
			return ; 
		}
		else 
		{
			return ; 
		}
	}
	else 
	{		
	}
}
