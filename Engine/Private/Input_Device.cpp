#include "Input_Device.h"

Engine::CInput_Device::CInput_Device(void)
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));		
	ZeroMemory(m_bPreKeyState, sizeof(m_bPreKeyState));		
}


_bool CInput_Device::isKeyPressed(_int _iKey)
{
	if ((m_bKeyState[_iKey] & 0x80) && (m_bPreKeyState[_iKey] & 0x80)) // Ű���忡�� ��ư�� Ǯ�������?��� �ؾ��ϳ� ����;;		
		return true;

	else //  ó�� �۾� Ű�� �Է� �Ǿ��ٸ�.( * ���� �����Ӱ� ���ؼ� )	
		return false;
}

_bool CInput_Device::isKeyReleased(_int _iKey)
{		
	if (!(m_bKeyState[_iKey] & 0x80) && (m_bPreKeyState[_iKey] & 0x80)) // Ű���忡�� ��ư�� Ǯ�������?��� �ؾ��ϳ� ����;;		
		return true;	

	else //  ó�� �۾� Ű�� �Է� �Ǿ��ٸ�.( * ���� �����Ӱ� ���ؼ� )	
		return false;	
}

_bool CInput_Device::isKeyEnter(_int _iKey)
{
	if ((m_bKeyState[_iKey] & 0x80) && !(m_bPreKeyState[_iKey] & 0x80)) // ó�� ������ �� 		
		return true;

	else //  ó�� �۾� Ű�� �Է� �Ǿ��ٸ�.( * ���� �����Ӱ� ���ؼ� )	
		return false;		
}

_bool CInput_Device::isAnyEnter()
{
	for (int i = 0; i < 256; i++)
	{
		if (m_bKeyState[i] & 0x80)
			return true;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_tMouseState.rgbButtons[i] & 0x80)
		{
			return true;
		}
	}

	return false;
}

_bool CInput_Device::isMouseEnter(_uint _iKey)		
{
	if ((m_tMouseState.rgbButtons[_iKey] & 0x80) && !(m_tPreMouseState.rgbButtons[_iKey] & 0x80))	
		return true;

	else
		return false; 
	 
	
	//bool bCurMouseState = Get_DIMouseState(DIM_LB) & 0x80;	
	//	
	//if (m_bPreMouseState != bCurMouseState)
	//{
	//	m_bPreMouseState = bCurMouseState;
	//	return true;
	//}

}

_bool CInput_Device::isMousePressed(_uint _iKey)
{
	if ((m_tMouseState.rgbButtons[_iKey] & 0x80) && (m_tPreMouseState.rgbButtons[_iKey] & 0x80))
		return true;

	else
		return false;
}

_bool CInput_Device::isMouseRelease(_uint _iKey)
{
	if (!(m_tMouseState.rgbButtons[_iKey] & 0x80) && (m_tPreMouseState.rgbButtons[_iKey] & 0x80))
		return true;

	else
		return false;
}



HRESULT Engine::CInput_Device::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput �İ�ü�� �����ϴ� �Լ�
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK, nullptr)))
		return E_FAIL;
											

	// Ű���� ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInput_Device::Update_InputDev(void)
{
	memcpy(m_bPreKeyState, m_bKeyState, sizeof(_byte) * VK_MAX);
	m_pKeyBoard->GetDeviceState(256, m_bKeyState);	
	
	memcpy(&m_tPreMouseState, &m_tMouseState, sizeof(DIMOUSESTATE));		
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);	
	
}

void CInput_Device::Push_MouseState(MOUSEKEYSTATE eMouseKeyState)
{
	mouseBuffer.push_front(eMouseKeyState);	
}

MOUSEKEYSTATE CInput_Device::Get_PreMouseState()
{
	return mouseBuffer.back();	
}

CInput_Device * CInput_Device::Create(HINSTANCE hInst, HWND hWnd)
{
	CInput_Device*		pInstance = new CInput_Device();

	if (FAILED(pInstance->Ready_InputDev(hInst, hWnd)))
	{
		MSG_BOX("Failed to Created : CInput_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CInput_Device::Free(void)
{
	__super::Free();

	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

