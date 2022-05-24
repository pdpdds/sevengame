// CSGManager.cpp: implementation of the CSGManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SGManager.h"
#include "SGUser.h"
#include "SGTable.h"
#include "SelectAlgorithm.h"
#include "DirectXSystem.h"
#include "SGStateManager.h"

CSGManager::CSGManager()
: m_userlist(_T("UserList"))
{
	m_fCardEffectTime = 0.0f;
	m_pStateManager = NULL;
}

CSGManager::~CSGManager()
{
	if(m_pTable)
		delete m_pTable;
	if(m_pMainEngine)
		delete m_pMainEngine;
	
	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();				
	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
		if( (NULL != pUser) )
		{
			delete pUser;		
		}
		pos = m_userlist.GetNextPosition(pos);				
	} 

	if( m_pUserRanking != NULL)
		delete m_pUserRanking;
}

void CSGManager::AllocateObjcet(int iUserNum, int iPassTicket)
{
	m_pStateManager = new SGStateManager(this);

	m_pStateManager->AddState(ENUM_SGINIT);
	m_pStateManager->AddState(ENUM_SGCARDEFFECT);
	m_pStateManager->AddState(ENUM_SGCARDTURN);
	m_pStateManager->AddState(ENUM_SGGAME);
	m_pStateManager->AddState(ENUM_SGSTART);
	m_pStateManager->AddState(ENUM_SGGAMEOVER);
	m_pStateManager->AddState(ENUM_SGCARDPASSEFFECT);

	m_pMainEngine = new CSelectAlgorithm();
    m_pTable = new CSGTable();
	m_pMainEngine->SetTable(m_pTable);
	
	
	for(int i=0 ; i<iUserNum ; i++)
	{	
		CSGUser *pUser = new CSGUser();  
		pUser->SetGameManager(this);
		
		m_userlist.AddTail(pUser);
	}	

	m_iPassTicket = iPassTicket;

	m_pUserRanking = new  int[iUserNum];

	m_iUserNum = iUserNum;
}

void CSGManager::UserRandom()
{
	int iHumanPos = rand() % m_iUserNum;

	int UserOrder[4];

	if(iHumanPos == 0)
	{
		UserOrder[0] = HUMAN_PLAYER;
		UserOrder[1] = SILENCE_PLAYER;
		UserOrder[2] = GENIUS_PLAYER;
		UserOrder[3] = DETECTIVE_PLAYER;
	}
	if(iHumanPos == 1)
	{
		UserOrder[0] = DETECTIVE_PLAYER;
		UserOrder[1] = HUMAN_PLAYER;
		UserOrder[2] = SILENCE_PLAYER;
		UserOrder[3] = GENIUS_PLAYER;
	}

	if(iHumanPos == 2)
	{
		UserOrder[0] = SILENCE_PLAYER;
		UserOrder[1] = GENIUS_PLAYER;
		UserOrder[2] = DETECTIVE_PLAYER;
		UserOrder[3] = HUMAN_PLAYER;
	}
	if(iHumanPos == 3)
	{
		UserOrder[0] = GENIUS_PLAYER;
		UserOrder[1] = DETECTIVE_PLAYER;
		UserOrder[2] = HUMAN_PLAYER;
		UserOrder[3] = SILENCE_PLAYER;
	}

	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();	
	int iCount = 0;
	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	

		pUser->SetID(UserOrder[iCount]);
		if( pUser->GetID() == UserOrder[0] )
			SetCurrentUser(pUser);
		
		pos = m_userlist.GetNextPosition(pos);			

		iCount++;
	} 
}

void CSGManager::InitializeData()
{
	UserRandom();

	m_pTable->InitializeTable();
	m_iCurrentTurn = 0;
	m_iAlivePlayer = m_iUserNum;
	m_iSuccessNum = 1;

	srand( (unsigned)time( NULL ) );

	memset(m_pUserRanking,-1,m_iUserNum * sizeof(int));

	CardDistribue();
	FindSevenAndEliminate();
}

void CSGManager::FindSevenAndEliminate()
{

	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();
	
	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
		if( (NULL != pUser) )
		{
			pUser->SetSevenCardNull();
			pUser->SetPassTicket(m_iPassTicket);
			pUser->SetTotalCard();
			pUser->SetStatus(GAME_RUNNING);
		}
		pos = m_userlist.GetNextPosition(pos);				
	} 
	m_pTable->SetSevenCard();
}

void CSGManager::CardDistribue()
{

	int paCardDistribute[MAX_CARD_NUM*4];
	
	memset(paCardDistribute,-1,MAX_CARD_NUM * 4 * sizeof(int));
	
	int iCardCount = 0;
    while ( iCardCount < MAX_CARD_NUM*4 )
	{
		BOOL bRet = TRUE;
		int iCardNum  = rand() % (MAX_CARD_NUM*4);
		for(int i = 0 ; i< MAX_CARD_NUM*4 ; i++ )
		{
			if(paCardDistribute[i] == iCardNum)
			{
                bRet = FALSE;
				break;
			}
		}
		
		if(bRet == TRUE)
		{
			paCardDistribute[iCardCount] = iCardNum;
			iCardCount++;
		}
	}
    
	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();
	
	iCardCount= 0;
	int iUserIndex = 1;
	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
		if( (NULL != pUser) )
		{
			pUser->Initialize();
			while ( iCardCount < iUserIndex * MAX_CARD_NUM )
			{
				pUser->UpdateUserCard(paCardDistribute[iCardCount]);
				iCardCount++;
			}
			iUserIndex++;
			
		}
		pos = m_userlist.GetNextPosition(pos);				
	} 
}

CSGUser * CSGManager::FindUser(int iUserID)
{
	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();
	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
		
		if( iUserID == pUser->GetID() )
		{
			return pUser;				
		}	
		
		pos = m_userlist.GetNextPosition(pos);				
	} 
	return NULL;
}

CSGUser* CSGManager::GetNextUser(CSGUser *pCurrentUser)
{
	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();
		
	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
		
		if( NULL != pUser && pCurrentUser == pUser )
		{												 				
			pos = m_userlist.GetNextPosition(pos);				
			
			if( pos == pEnd )			
			{
				pos  = m_userlist.GetHeadPosition();
			}
			
			CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
			
			return pUser;
		}					
		pos = m_userlist.GetNextPosition(pos);				
	} 

	return NULL;
}

int CSGManager::ProcessArtificialFriend()
{
	CSGUser *pTurnUser = GetCurrentUser();

	if(pTurnUser->GetStatus() != GAME_END && pTurnUser->GetID() == HUMAN_PLAYER )
		return -1;

	
	int iGameState = GAME_RUNNING;

	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();
		
	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
		
		if( NULL != pUser && pTurnUser == pUser )
		{									
			 if( pUser->GetStatus() == GAME_END )
			 {
				 pos = m_userlist.GetNextPosition(pos);				
				 
				 if( pos == pEnd )			
				 {
					 pos  = m_userlist.GetHeadPosition();
				 }
				 
				 CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
				 SetCurrentUser(pUser);
				 break;
			 }

			SCardInfo SSelectedCard = m_pMainEngine->GetNextAction(pUser);
			
			if( SSelectedCard.iCardNum == -1 )
			{
				pUser->DecreasePassTicketNum();	
				ChangeState(ENUM_SGCARDPASSEFFECT);
			}
			else
			{
				pUser->EliminateCard(SSelectedCard.iCardNum,SSelectedCard.iCardType);
				//m_pTable->UpdateTableState(SSelectedCard.iCardNum,SSelectedCard.iCardType);
				SetEffectUser(pUser);
				SetSpriteCard(SSelectedCard);
				SetCardEffectInfo(&SSelectedCard);

				ChangeState(ENUM_SGCARDEFFECT);
			}

			if(pUser->GetStatus() != GAME_END )			
				EvaluateUser(pUser);			
			
			pos = m_userlist.GetNextPosition(pos);				
			
			if( pos == pEnd )			
			{
				pos  = m_userlist.GetHeadPosition();
			}
			
			CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
			SetCurrentUser(pUser);
							
			break;			
			
		}				
		pos = m_userlist.GetNextPosition(pos);				
	} 
		
	return 1;
}

void CSGManager::EvaluateUser(CSGUser *pUser)
{
		
	if( (NULL != pUser) )
	{
		if ( pUser->GetStatus() == GAME_RUNNING )
		{
			if( pUser->GetPassTicketNum() < 0 )
			{
				pUser->SetStatus(GAME_END);
				pUser->SetRemainCardZero();
				SendUserCardToTable(pUser);
				m_iAlivePlayer--;
				m_pUserRanking[pUser->GetID()] = m_iSuccessNum + m_iAlivePlayer;
				//SetGameState(GAME_USER_OUT);
			}
			else if( pUser->GetRemainCard() == 0 )
			{
				m_pUserRanking[pUser->GetID()] = m_iSuccessNum;
				m_iSuccessNum++;
				m_iAlivePlayer--;
				pUser->SetStatus(GAME_END);
				pUser->SetRemainCardZero();		
				//SetGameState(GAME_USER_SUCCESS);
			}			
		}
	}

	if(m_iAlivePlayer == 1 && m_iSuccessNum == 1) //한명 남았는데 성공한 사람이 없으면 남은 사람이 승리한 것
	{
		FindAndSetRankTop();
	}
}

void CSGManager::SendUserCardToTable(CSGUser *pUser)
{
	pUser->SendUserCardToTable();
}

BOOL CSGManager::FindAndSetRankTop()
{
	CDataList<CSGUser *>::POSITION pEnd = m_userlist.GetEndPosition();	
	CDataList<CSGUser *>::POSITION pos  = m_userlist.GetHeadPosition();

	while ( pos != pEnd) 			
	{
		CSGUser* pUser= (CSGUser*)m_userlist.GetType(pos);	
		if( (NULL != pUser) )
		{
			if(m_pUserRanking[pUser->GetID()] == -1)
			{
				m_pUserRanking[pUser->GetID()] = 1;
				return TRUE;
			}
		}
	
		pos = m_userlist.GetNextPosition(pos);				
	} 

	return FALSE;
}

BOOL CSGManager::CheckGameEnd()
{
	if( m_iAlivePlayer <= 1 )
	{
		if( m_iAlivePlayer <= 1)
		{	
			int i=0;		
			
			for( i=0 ; i<m_iUserNum ;i++)
			{
				if(m_pUserRanking[i] == -1)				
				{
					m_pUserRanking[i] = m_iSuccessNum;				
					break;
				}			
			}
		}	
		return TRUE;
	}

	return FALSE;
}

BOOL CSGManager::MouseInPassButton(int iPosx,int iPosy) 
{
	CSGUser *pUser =GetCurrentUser();

	if( pUser == NULL )
		return FALSE;

	if( pUser->GetID() != HUMAN_PLAYER )
		return FALSE;

	if( pUser->GetStatus() == GAME_END )
		return FALSE;

	if (iPosx > PASSBUTTON_LEFT && 
		iPosx < PASSBUTTON_RIGHT && 
		iPosy > PASSBUTTON_TOP && 
		iPosy < PASSBUTTON_BOTTOM )	
		return TRUE;

	return FALSE;
}

int CSGManager::CheckPassTicket(int iPosx,int iPosy) 
{
	
	CSGUser *pUser =GetCurrentUser();

	if( pUser == NULL )
		return -1;

	if( pUser->GetID() != HUMAN_PLAYER )
		return -1;

	if( pUser->GetStatus() == GAME_END )
		return -1;

	if (iPosx > PASSBUTTON_LEFT && 
		iPosx < PASSBUTTON_RIGHT && 
		iPosy > PASSBUTTON_TOP && 
		iPosy < PASSBUTTON_BOTTOM )			
		pUser->DecreasePassTicketNum();
	else return -1;

	ChangeState(ENUM_SGCARDPASSEFFECT);

	if( pUser->GetPassTicketNum() < 0 )
	{
		EvaluateUser(pUser);			
	}

	pUser = GetNextUser(pUser);
	SetCurrentUser(pUser);

	return 0;
}

int CSGManager::ProcessUserInput(int iPosX, int iPosY)
{
	// TODO: Add extra validation here

	CSGUser *pUser =GetCurrentUser();

	if( pUser == NULL )
		return -1;

	if( pUser->GetID() != HUMAN_PLAYER )
		return -1;

	if( pUser->GetStatus() == GAME_END )
		return -1;

	SCardInfo CardInfo;
	if(UserProcessGame(pUser,iPosX, iPosY, CardInfo) == FALSE)	
		return -1;
	else
	{
		if( pUser->GetRemainCard() <= 0 )
		{			
			EvaluateUser(pUser);	
		}
		
		SetEffectUser(pUser);
		SetSpriteCard(CardInfo);
		SetCardEffectInfo(&CardInfo);

		ChangeState(ENUM_SGCARDEFFECT);

		pUser = GetNextUser(pUser);
		SetCurrentUser(pUser);

	}	
	return -1;
}

BOOL CSGManager::UserProcessGame(CSGUser *pUser,int iPosX, int iPosY, SCardInfo& CardInfo)
{
	int iCardNum = -1;
	int iCardIndex = -1;
	for( int i=0; i<MAX_CARD_NUM; i++ )
	{

		if(m_CARD[i].CurrentRect.top<iPosY)
			if(m_CARD[i].CurrentRect.bottom>iPosY)
				if(m_CARD[i].CurrentRect.left<iPosX)
					if(m_CARD[i].CurrentRect.right>iPosX)
					{
						iCardNum = m_CARD[i].iCol+1;
						iCardIndex = i;

						
					}
	}

	CardInfo.iCardNum = iCardNum;
	CardInfo.iCardType = m_CARD[iCardIndex].iCardType;

	if( iCardNum >= 1 && iCardNum <= 13 && m_CARD[iCardIndex].iCardType == TYPE_SPADE)
	{

		if(pUser->IHaveIt(iCardNum, TYPE_SPADE))
		{
			CSGTable *pTable = GetTable();
			EdgeCard CardLimit = pTable->CheckBoundary(TYPE_SPADE);
			if(iCardNum != CardLimit.High_Card+1 && iCardNum != CardLimit.Low_Card-1 )
				return FALSE;

			pUser->EliminateCard(iCardNum,TYPE_SPADE);
			//pTable->SetSpadeCard(iCardNum,iCardNum);
			//	ResetField();
			return TRUE;
		}

	}

	//m_ctrHeartOut.GetWindowText(str);
	// iCardNum = atoi(str.GetBuffer(0));
	if( iCardNum >= 1 && iCardNum <= 13 && m_CARD[iCardIndex].iCardType == TYPE_HEART)
	{

		if(pUser->IHaveIt(iCardNum, TYPE_HEART))
		{
			CSGTable *pTable = GetTable();
			EdgeCard CardLimit = pTable->CheckBoundary(TYPE_HEART);
			if(iCardNum != CardLimit.High_Card+1 && iCardNum != CardLimit.Low_Card-1 )
				return FALSE;

			pUser->EliminateCard(iCardNum,TYPE_HEART);
			//pTable->SetHeartCard(iCardNum,iCardNum);
			//	ResetField();
			return TRUE;
		}

	}

	//m_ctrDiamondOut.GetWindowText(str);
	//	iCardNum = atoi(str.GetBuffer(0));
	if( iCardNum >= 1 && iCardNum <= 13 && m_CARD[iCardIndex].iCardType == TYPE_DIAMOND)
	{

		if(pUser->IHaveIt(iCardNum, TYPE_DIAMOND))
		{

			CSGTable *pTable = GetTable();
			EdgeCard CardLimit = pTable->CheckBoundary(TYPE_DIAMOND);
			if(iCardNum != CardLimit.High_Card+1 && iCardNum != CardLimit.Low_Card-1 )
				return FALSE;

			pUser->EliminateCard(iCardNum,TYPE_DIAMOND);
			//pTable->SetDiamondCard(iCardNum,iCardNum);
			//	ResetField();
			return TRUE;
		}	
	}

	//	m_ctrCloverOut.GetWindowText(str);
	//	iCardNum = atoi(str.GetBuffer(0));
	if( iCardNum >= 1 && iCardNum <= 13 && m_CARD[iCardIndex].iCardType == TYPE_CLOVER)
	{
		if(pUser->IHaveIt(iCardNum, TYPE_CLOVER))
		{


			CSGTable *pTable = GetTable();
			EdgeCard CardLimit = pTable->CheckBoundary(TYPE_CLOVER);
			if(iCardNum != CardLimit.High_Card+1 && iCardNum != CardLimit.Low_Card-1 )
				return FALSE;

			pUser->EliminateCard(iCardNum,TYPE_CLOVER);
			//pTable->SetCloverCard(iCardNum,iCardNum);
			//ResetField();
			return TRUE;
		}

	}

	return FALSE;
}

void CSGManager::UpdateTableState(SCardInfo* pInfo)
{
	m_pTable->UpdateTableState(pInfo->iCardNum,pInfo->iCardType);
}

void CSGManager::ReArrangeUserCard(CSGUser* pUser,int &iCount,int iCardNum)
{

	int *paArray;
	int i = 0;

	memset(&m_CARD,0,sizeof(SPRITE_CARD_INFO)*13);

	paArray = pUser->GetCloverArray();
	for (i = 1; i <= MAX_CARD_NUM; i++)
	{

		if (paArray[i] == -1)
			continue;
		m_CARD[iCount].iCardType = TYPE_CLOVER;
		m_CARD[iCount].iRow = 0;
		m_CARD[iCount].iCol = i - 1;
		m_CARD[iCount].CurrentRect.top = HUMAN_PLAYER_POS_Y;
		m_CARD[iCount].CurrentRect.left = HUMAN_PLAYER_POS_X + (iCount - ((iCardNum) / 2)) * CARD_LOOKABLE_PART;
		m_CARD[iCount].CurrentRect.bottom = HUMAN_PLAYER_POS_Y + CARD_HEIGHT;
		m_CARD[iCount].CurrentRect.right = HUMAN_PLAYER_POS_X + (iCount - ((iCardNum) / 2))*CARD_LOOKABLE_PART + CARD_WIDTH;
		iCount++;
	}

	paArray = pUser->GetHeartArray();
	for( i=1; i<=MAX_CARD_NUM; i++ )
	{

		if(paArray[i] == -1)
			continue;									
		m_CARD[iCount].iCardType = TYPE_HEART;
		m_CARD[iCount].iRow = 1;
		m_CARD[iCount].iCol = i-1;
		m_CARD[iCount].CurrentRect.top=HUMAN_PLAYER_POS_Y;
		m_CARD[iCount].CurrentRect.left=HUMAN_PLAYER_POS_X+(iCount-((iCardNum)/2)) * CARD_LOOKABLE_PART;				
		m_CARD[iCount].CurrentRect.bottom=HUMAN_PLAYER_POS_Y + CARD_HEIGHT;				
		m_CARD[iCount].CurrentRect.right=HUMAN_PLAYER_POS_X+(iCount-((iCardNum)/2))*CARD_LOOKABLE_PART + CARD_WIDTH;				
		iCount++;
	}

	paArray = pUser->GetDiamondArray();
	for( i=1; i<=MAX_CARD_NUM; i++ )
	{

		if(paArray[i] == -1)
			continue;									
		m_CARD[iCount].iCardType = TYPE_DIAMOND;
		m_CARD[iCount].iRow = 2;
		m_CARD[iCount].iCol = i-1;
		m_CARD[iCount].CurrentRect.top=HUMAN_PLAYER_POS_Y;
		m_CARD[iCount].CurrentRect.left=HUMAN_PLAYER_POS_X+(iCount-((iCardNum)/2)) * CARD_LOOKABLE_PART;				
		m_CARD[iCount].CurrentRect.bottom=HUMAN_PLAYER_POS_Y + CARD_HEIGHT;				
		m_CARD[iCount].CurrentRect.right=HUMAN_PLAYER_POS_X+(iCount-((iCardNum)/2))*CARD_LOOKABLE_PART + CARD_WIDTH;				
		iCount++;
	}

		

	paArray = pUser->GetSpadeArray();
	for (i = 1; i <= MAX_CARD_NUM; i++)
	{

		if (paArray[i] == -1)
			continue;
		m_CARD[iCount].iCardType = TYPE_SPADE;
		m_CARD[iCount].iRow = 3;
		m_CARD[iCount].iCol = i - 1;
		m_CARD[iCount].CurrentRect.top = HUMAN_PLAYER_POS_Y;
		m_CARD[iCount].CurrentRect.left = HUMAN_PLAYER_POS_X + (iCount - ((iCardNum) / 2)) * CARD_LOOKABLE_PART;
		m_CARD[iCount].CurrentRect.bottom = HUMAN_PLAYER_POS_Y + CARD_HEIGHT;
		m_CARD[iCount].CurrentRect.right = HUMAN_PLAYER_POS_X + (iCount - ((iCardNum) / 2))*CARD_LOOKABLE_PART + CARD_WIDTH;
		iCount++;
	}
}


BOOL CSGManager::GetCardImageIndex( SCardInfo* pInfo, int& iCol, int& iRow )
{
	switch(pInfo->iCardType)
	{		
	case TYPE_CLOVER:
		iRow = 0;
		break;
	case TYPE_HEART:
		iRow = 1;
		break;
	case TYPE_DIAMOND:
		iRow = 2;
		break;
	case TYPE_SPADE:
		iRow = 3;
		break;
	}

	iCol = pInfo->iCardNum - 1;
	
	return TRUE;
}

void CSGManager::SetCardEffectInfo( SCardInfo* pInfo )
{
	CSGUser* pUser = GetEffectUser();

	int CardNum = pUser->GetRemainCard();

	switch(pUser->GetID())
	{
	case SILENCE_PLAYER:
		{
			m_CardEffectInfo.iOriX = SILENCE_PLAYER_CARD_POS_X;
			m_CardEffectInfo.iOriY = SILENCE_PLAYER_CARD_POS_Y;
		}
		break;
	case DETECTIVE_PLAYER:
		{
			m_CardEffectInfo.iOriX = DETECTIVE_PLAYER_POS_X;
			m_CardEffectInfo.iOriY = DETECTIVE_PLAYER_POS_Y;
		}
		break;
	case HUMAN_PLAYER:
		{
			m_CardEffectInfo.iOriX = HUMAN_PLAYER_POS_X;
			m_CardEffectInfo.iOriY = HUMAN_PLAYER_POS_Y;
		}
		break;
	case GENIUS_PLAYER:
		{
			m_CardEffectInfo.iOriX = GENIUS_PLAYER_POS_X;
			m_CardEffectInfo.iOriY = GENIUS_PLAYER_POS_Y;
		}
		break;
	}

	switch(pInfo->iCardType)
	{		
	case TYPE_CLOVER:
		m_CardEffectInfo.iDestX = (pInfo->iCardNum - 1)*CARD_LOOKABLE_PART + DEFAULT_TABLE_CARD_POS_X;
		m_CardEffectInfo.iDestY = DEFAULT_TABLE_CARD_POS_Y;
		break;
	case TYPE_HEART:
		m_CardEffectInfo.iDestX = (pInfo->iCardNum - 1)*CARD_LOOKABLE_PART+DEFAULT_TABLE_CARD_POS_X;
		m_CardEffectInfo.iDestY = DEFAULT_TABLE_CARD_POS_Y + CARD_DISTANCE * 1;
		break;
	case TYPE_DIAMOND:
		m_CardEffectInfo.iDestX = (pInfo->iCardNum - 1)*CARD_LOOKABLE_PART+DEFAULT_TABLE_CARD_POS_X;
		m_CardEffectInfo.iDestY = DEFAULT_TABLE_CARD_POS_Y + CARD_DISTANCE * 2;
		break;
	case TYPE_SPADE:
		m_CardEffectInfo.iDestX = (pInfo->iCardNum - 1)*CARD_LOOKABLE_PART + DEFAULT_TABLE_CARD_POS_X;
		m_CardEffectInfo.iDestY = DEFAULT_TABLE_CARD_POS_Y + CARD_DISTANCE * 3;		
		break;
	}
}

BOOL CSGManager::OnRender( float fElapsedTime )
{
	if(m_pStateManager)
		return m_pStateManager->OnRender(fElapsedTime);

	return FALSE;
}

BOOL CSGManager::OnMessage( int iX, int iY )
{
	if(m_pStateManager)
		return m_pStateManager->OnMessage(iX, iY);

	return FALSE;
}

BOOL CSGManager::ChangeState( eSGState state )
{
	if(m_pStateManager)
		m_pStateManager->ChangeState(state);

	return FALSE;
}