#pragma once

#include "STLWarning.h"
#include "Lock.h"

using namespace std;

template<typename Type> class CDataList
{
private:
	list<Type> m_List;
	char m_szName[128]; 
	LOCK_RESOURCE(m_SyncHandle);	
public:
	typedef typename list<Type>::iterator POSITION;
	CDataList(LPCTSTR szName):m_SyncHandle(szName)	{	}; 
	~CDataList()									{ RemoveAll(); }; 
	inline BOOL Lock()								{ return m_SyncHandle.Lock();} 
	inline void	Unlock()							{ m_SyncHandle.Unlock();} 
	inline POSITION GetHeadPosition		()						{ return m_List.begin();}
	inline POSITION GetNextPosition		(POSITION P)			{ return ++P;}
	inline POSITION GetEndPosition		()						{ return m_List.end();}
	inline int GetSize					()						{ return m_List.size();}
	inline void AddTail					(Type T)				{ Lock(); m_List.push_back(T); Unlock();}
	inline void AddHead					(Type T)				{ Lock(); m_List.push_front(T); Unlock();}
	inline void Remove					(Type T)				{ Lock(); m_List.remove(T); Unlock();}
	inline POSITION Remove				(POSITION P)			{ Lock(); m_List.erase(P); Unlock(); return P;}
	inline void RemoveAll				()						{ Lock(); m_List.clear(); Unlock();}
	inline POSITION	Insert				(POSITION P, Type T)	{ POSITION pos=m_List.insert(P,T); return pos;}

	inline Type GetAndRemoveHead		()						{ Lock();Type T= m_List.front(); m_List.pop_front(); Unlock(); return T;} 
	inline Type	GetHead					()						{ return m_List.front();}
	inline Type	GetType					(POSITION P)			{ return (*P); }
	
	inline void Unique					()						{ Lock(); m_List.unique(); Unlock();};
	inline BOOL	IsThere					(Type T)				
	{
		Lock();
		POSITION pos = m_List.begin();
		for ( ; pos != m_List.end(); ++pos )
		{
			if ( T == (*pos) )
			{
				Unlock();
				return TRUE;
			}
		}
		Unlock();
		return FALSE;
	}

};
