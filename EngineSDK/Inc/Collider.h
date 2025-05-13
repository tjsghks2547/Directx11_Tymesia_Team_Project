//#pragma once
//
//#include "Component.h"
//
//BEGIN(Engine)
//
//class ENGINE_DLL CCollider final : public CComponent	
//{
//public:
//	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END};
//
//private:
//	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
//	CCollider(const CCollider& Prototype);
//	virtual ~CCollider() = default; 
//
//public:
//	virtual HRESULT Initialize_Prototype(TYPE eColliderType);
//	virtual HRESULT Initialize(void* pArg) override; 
//	virtual void Update(_fmatrix WorldMatrix);
//	void    Set_Collider_Name(_char* _pName) { strcpy_s(m_szName, _pName); }	
//
//public:
//	_bool Intersect(CCollider* pTargetCollider);	
//
//#ifdef _DEBUG
//public:
//	virtual HRESULT Render();
//#endif // _DEBUG
//
//private:
//	TYPE					m_eType     = { TYPE_END };
//	class CBounding*		m_pBounding = { nullptr };
//	_bool					m_isColl	= { false };	
//
//
//	_char					m_szName[MAX_PATH] = {};		
//
//
//#ifdef _DEBUG
//private:
//	PrimitiveBatch<DirectX::VertexPositionColor>*		m_pBatch	   = { nullptr };	
//	BasicEffect*										m_pEffect	   = { nullptr };	
//	ID3D11InputLayout*									m_pInputLayout = { nullptr };	
//
//#endif // _DEBUG
//
//
//private:
//	UINT		 m_iID;     // �浹ü ������ ID ��	
//	static UINT  g_iNextID; //�����ɹ��� ��ü���뿡 ���Ծȵ�	
//		
//public:
//
//public:
//	UINT   Get_ID() { return m_iID; }	
//	_char* Get_CollierName() { return m_szName; }	
//	void   Set_Collider_Color(_bool _bool) { m_isColl = _bool; }	
//	_bool  Get_isCollision() { return m_isColl; }	
//
//public:
//	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType);
//	virtual CComponent* Clone(void* pArg) override;
//	virtual void Free() override; 
//
//};
//END
