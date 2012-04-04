// Lock.h: interface for the CLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCK_H__8F901270_E27E_43D8_B117_4A316C1CA009__INCLUDED_)
#define AFX_LOCK_H__8F901270_E27E_43D8_B117_4A316C1CA009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLockCritical
{
private:
	TCHAR m_szName[128]; 
	CRITICAL_SECTION m_hCritical; 
public:
	BOOL Lock(DWORD dwTimeOut=LOCK_TIME_OUT, BOOL bWriteLog=TRUE);
	void Unlock(); 
public:
	CLockCritical(LPCTSTR szName); 
	virtual ~CLockCritical(); 
};

class CLockEvent
{
private:
	DWORD	m_dwOwnerThread; 
	char	m_szName[128]; 
	int		m_iLockCount; 
	HANDLE	m_hEvent; 
public:
	BOOL	Lock(DWORD dwTimeOut=LOCK_TIME_OUT, BOOL bWriteLog=TRUE); 
	void	Unlock(); 
public:
	CLockEvent(LPCTSTR szName);
	virtual ~CLockEvent();
};

#endif // !defined(AFX_LOCK_H__8F901270_E27E_43D8_B117_4A316C1CA009__INCLUDED_)
