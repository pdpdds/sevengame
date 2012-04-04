#pragma once
#include "Define.h"

class CSGUser;
class CSGTable;
class CSelectAlgorithm;
class CGameView;
class CGameDoc;
class SGStateManager;

class CSGManager  
{
public:
	CSGManager();
	virtual ~CSGManager();

	CDataList<CSGUser *>  m_userlist;
	void AllocateObjcet(int iUserNum, int iPassTicket);
    void InitializeData();
	void UserRandom();
  
	void SendUserCardToTable(CSGUser *pUser);		
	CSGTable *GetTable(){return m_pTable;};
	CSGUser * FindUser(int iUserID);
	
	int GetUserNum(){return m_iUserNum;};
	BOOL CheckGameEnd();
	CSGUser* GetNextUser(CSGUser *pCurrentUser);

	int CheckPassTicket(int iPosx,int iPosy);
	BOOL MouseInPassButton(int iPosx,int iPosy);

	void EvaluateUser(CSGUser *pUser);		
	int ProcessArtificialFriend();		
	CSGUser *GetCurrentUser(){return m_pCurrentUser;};
	void SetCurrentUser(CSGUser *pUser){m_pCurrentUser = pUser;}

	CSGUser *GetEffectUser(){return m_pEffectUser;}
	void SetEffectUser(CSGUser *pUser){m_pEffectUser = pUser;}

	BOOL ChangeState(eSGState state);

	int ProcessUserInput(int iPosX, int iPosY);
	BOOL UserProcessGame(CSGUser *pUser,int iPosX, int iPosY, SCardInfo& CardInfo);
	void ReArrangeUserCard(CSGUser* pUser,int &iCount,int iCardNum);
	void UpdateTableState(SCardInfo* pInfo);

	float GetCardEffectTime(){return m_fCardEffectTime;}
	void SetCardEffectTime(float fTime){m_fCardEffectTime = fTime;}

	SPRITE_CARD_INFO* GetUserCard(){return m_CARD;}

	SCardInfo* GetSpriteCard(){return &m_SelectedCard;}
	VOID SetSpriteCard(SCardInfo& Card){m_SelectedCard = Card;}


	CARD_EFFECT_INFO* GetCardEffectInfo(){return &m_CardEffectInfo;}
	void SetCardEffectInfo(SCardInfo* pInfo);

	BOOL GetCardImageIndex(SCardInfo* pInfo, int& iCol, int& iRow);

	BOOL FindAndSetRankTop();

	int *m_pUserRanking;

	BOOL OnRender(float fElapsedTime);
	BOOL OnMessage(int iX, int iY);

private:
	void CardDistribue();
	void FindSevenAndEliminate();
	CSelectAlgorithm *m_pMainEngine;

    CSGTable *m_pTable;
		
	
	int m_iUserNum;
	int m_iCurrentTurn ;
	int m_iPassTicket;
	int m_iAlivePlayer;
	int m_iSuccessNum;
	CSGUser *m_pCurrentUser;
	CSGUser *m_pEffectUser;

	int m_iGameState;
	BOOL m_bVictory;

	SPRITE_CARD_INFO m_CARD[13];

	int m_iCardWidth;
	int m_iCardHeight;

	float m_fCardEffectTime;
	CARD_EFFECT_INFO m_CardEffectInfo;

	SCardInfo m_SelectedCard;

	SGStateManager* m_pStateManager;
};