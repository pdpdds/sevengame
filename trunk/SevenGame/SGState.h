#pragma once

class CSGManager;

class SGState
{
public:
	SGState(CSGManager* pManager);
	virtual ~SGState(void);

	virtual BOOL OnEnter(){return FALSE;};
	virtual BOOL OnLeave(){return FALSE;};
	virtual BOOL OnMessage(int iX, int iY){return FALSE;};
	virtual BOOL OnRender(float fElpasedTime){return FALSE;};

protected:
	CSGManager* GetSGManager(){return m_pManager;}

private:
	CSGManager* m_pManager;
};