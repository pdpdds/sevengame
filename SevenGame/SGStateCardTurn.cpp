#include "StdAfx.h"
#include "SGStateCardTurn.h"
#include "SGManager.h"
#include "DirectXSystem.h"
#include "SGUser.h"

SGStateCardTurn::SGStateCardTurn( CSGManager* pManager )
: SGState(pManager)
{
}

SGStateCardTurn::~SGStateCardTurn(void)
{
}

BOOL SGStateCardTurn::OnRender( float fElapsedTime )
{
	CDirectXSystem* pRenderer = CDirectXSystem::GetInstance();
	CSGManager* pManager = GetSGManager();

	if(pRenderer)
	{
		pRenderer->DrawCard(fElapsedTime);	

		if(pManager->CheckGameEnd())
		{
			pManager->ChangeState(ENUM_SGGAMEOVER);
			return TRUE;
		}

		pRenderer->DrawPassButton(fElapsedTime);

		CSGUser *pUser = pManager->GetCurrentUser();

		if (pUser->GetStatus() == GAME_END)
		{
			pUser = pManager->GetNextUser(pUser);

			while(pUser->GetStatus() == GAME_END)
			{
				pUser = pManager->GetNextUser(pUser);
			}

			pManager->SetCurrentUser(pUser);
		}

		pRenderer->DrawCardTurn(fElapsedTime);
	}

	float fEffectTime = pManager->GetCardEffectTime();

	float fModeTime = 0.5f;

	if(fEffectTime < fModeTime)
	{
		fEffectTime += fElapsedTime;
		pManager->SetCardEffectTime(fEffectTime);
	}
	else
	{
		pManager->ChangeState(ENUM_SGGAME);
		return TRUE;
	}

	return TRUE;
}

BOOL SGStateCardTurn::OnEnter()
{
	GetSGManager()->SetCardEffectTime(0.0f);
	return TRUE;
}