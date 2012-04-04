// CSelectAlgorithm.h: interface for the CSelectAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSELECTALGORITHM_H__E15066CE_982A_4533_A99B_C254CF7DA10E__INCLUDED_)
#define AFX_CSELECTALGORITHM_H__E15066CE_982A_4533_A99B_C254CF7DA10E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSGTable;
class CSGUser;
class CSGManager;

class CSelectAlgorithm  
{
public:
	CSelectAlgorithm();
	virtual ~CSelectAlgorithm();
	CSGManager *GetGameManager();
	BOOL IMustPass(int iCountGrapedCard,int iCountCoocockCard,int iCountGetOutCard,CSGUser *pUser);
	CSGTable *GetTableInstance(){return m_pTable;};
	void SetTable(CSGTable *pTable){m_pTable = pTable;};
	SCardInfo GetNextAction(CSGUser *pUser);

private:
	CSGTable *m_pTable;
	void SetTableInstance(CSGTable *pTable) {m_pTable = pTable;}
	CSGUser *GetUserData();
	void SetUserData();
	
	int CheckPassCard(CSGUser *pUser);
		
	BOOL CheckOtherUserWithProb(CSGUser *pUser, int iProb);
	int FindCardPriorty(int iCardNum,int iType,CSGUser *pUser);
	SCardInfo SelectCard(CSGUser *pUser);
	BOOL CheckProperCard(CSGTable *pTable, CSGUser *pUser);
	BOOL DeterminePassCard(CSGTable *pTable, CSGUser *pUser);

};

#endif // !defined(AFX_CSELECTALGORITHM_H__E15066CE_982A_4533_A99B_C254CF7DA10E__INCLUDED_)
