#pragma once
#include "SGState.h"

class SGStateCardTurn : public SGState
{
public:
	SGStateCardTurn( CSGManager* pManager );
	virtual ~SGStateCardTurn(void);

	virtual BOOL OnEnter() override;

	virtual BOOL OnRender(float fElapsedTime) override;
};
