#pragma once
#include "SGState.h"

class CSGUser;

class SGStateCardPassEffect : public SGState
{
public:
	SGStateCardPassEffect(CSGManager* pManager);
	virtual ~SGStateCardPassEffect(void);

	virtual BOOL OnEnter() override;

	virtual BOOL OnRender(float fElapsedTime) override;

protected:

private:
	CSGUser* m_pCurrentUser;
};
