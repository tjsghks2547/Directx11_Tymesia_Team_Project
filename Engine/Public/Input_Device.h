#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

BEGIN(Engine)

class CInput_Device : public CBase
{
	

private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)		
	{ 
		return m_bKeyState[byKeyID]; 
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 
	{ 	
		return m_tMouseState.rgbButtons[eMouse]; 	
	}

	// ���� ���콺�� Ư�� �� ��ǥ�� ��ȯ
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	
	{	
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}

	_bool isKeyPressed(_int _iKey);
	_bool isKeyReleased(_int _iKey);
	_bool isKeyEnter(_int _iKey);	
	_bool isAnyEnter();	// �ƹ� Ű�� ������ �� ����
	_bool isMouseEnter(_uint iKey);	
	_bool isMousePressed(_uint _iKey);
	_bool isMouseRelease(_uint _iKey);


	
private:
	deque<MOUSEKEYSTATE> mouseBuffer;  // MouseEvent Ÿ���� �����ϴ� queue
	
	_bool LeftIsDown  = { false };
	_bool rightIsDown = { false };
	_bool mbuttonDown = { false };


public:
	HRESULT		  Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void		  Update_InputDev(void);
	void		  Push_MouseState(MOUSEKEYSTATE eMouseKeyState); 
	MOUSEKEYSTATE Get_PreMouseState();



private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;

private:
	_byte					m_bKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
	_byte					m_bPreKeyState[256];    // Ű������ ���� ���¸� �����ϴ� ��� Ű ��
	DIMOUSESTATE			m_tMouseState;	
	DIMOUSESTATE			m_tPreMouseState;	
	_bool				    m_bPreMouseState = { false };	

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};
END
#endif // InputDev_h__
