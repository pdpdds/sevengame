#include "StdAfx.h"
#include "SGStateGame.h"
#include "SGManager.h"
#include "DirectXSystem.h"

SGStateGame::SGStateGame( CSGManager* pManager )
: SGState(pManager)
{
}

SGStateGame::~SGStateGame(void)
{
}

BOOL SGStateGame::OnEnter()
{
	GetSGManager()->SetCardEffectTime(0.0f);
	return TRUE;
}

BOOL SGStateGame::OnMessage(int iX, int iY)
{
	CSGManager* pManager = GetSGManager();

	int iGameState = GetSGManager()->CheckPassTicket(iX,iY);
	if(iGameState == -1)
	{
		iGameState = GetSGManager()->ProcessUserInput(iX, iY);
	}

	return TRUE;
}

BOOL SGStateGame::OnRender( float fElapsedTime )
{
	CDirectXSystem* pRenderer = CDirectXSystem::GetInstance();
	CSGManager* pManager = GetSGManager();

	if(pRenderer)
	{
		pRenderer->DrawCard(fElapsedTime);	
		pRenderer->DrawPassButton(fElapsedTime);
	}

	if(pManager->CheckGameEnd() == TRUE )				
	{
		pManager->ChangeState(ENUM_SGGAMEOVER);						
	}
	else
	{			
		if(pManager)									
			pManager->ProcessArtificialFriend();
	}

	return TRUE;
}