//#include "Collider.h"
//#include "Bounding_AABB.h"
//#include "GameInstance.h"   
//
//UINT CCollider::g_iNextID = 0;  
//
//CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//    :CComponent(pDevice, pContext)
//    , m_iID(0)
//{
//}
//
//CCollider::CCollider(const CCollider& Prototype)
//    :CComponent(Prototype)
//    , m_eType(Prototype.m_eType)
//#ifdef _DEBUG
//    , m_pBatch(Prototype.m_pBatch)
//    , m_pEffect(Prototype.m_pEffect)
//    , m_pInputLayout(Prototype.m_pInputLayout)
//#endif // _DEBUG
//    , m_iID(g_iNextID++)
//{
//#ifdef _DEBUG
//    Safe_AddRef(m_pInputLayout);
//#endif // _DEBUG
//}
//
//HRESULT CCollider::Initialize_Prototype(TYPE eColliderType)
//{
//    m_eType = eColliderType;
//
//#ifdef _DEBUG
//    m_pBatch  = new PrimitiveBatch<VertexPositionColor>(m_pContext);
//    m_pEffect = new BasicEffect(m_pDevice);
//
//    m_pEffect->SetVertexColorEnabled(true);
//
//    const void* pShaderByteCode = { nullptr };
//    size_t      iShaderByteCodeLength = {};
//
//    m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);
//    //�� �Լ��� GPU���� ������ ����(���� �����Ϳ� ���̴��� ȣȯ��)�� ���� ���̴� �ڵ��� �޸� �����͸� ��ȯ�մϴ�.
//
//    if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements,  // �Է� ���
//        VertexPositionColor::InputElementCount, // �Է� ��� ���� 
//        pShaderByteCode,  // ���̴� ����Ʈ �ڵ� ���� �ּ� 
//        iShaderByteCodeLength,  // ���̴� ����Ʈ �ڵ� ũ�� 
//        &m_pInputLayout))) // ������ �Է� ���̾ƿ� ���
//        return E_FAIL;
//    
//
//#endif // _DEBUG
//
//    return S_OK;
//}
//
//HRESULT CCollider::Initialize(void* pArg)
//{
//    CBounding::BOUNDING_DESC* pBoundingDesc = static_cast<CBounding::BOUNDING_DESC*>(pArg);
//
//    switch (m_eType)    
//    {
//    case Engine::CCollider::TYPE_AABB:
//        m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pBoundingDesc);
//        break;
//    case Engine::CCollider::TYPE_OBB:
//        break;
//    case Engine::CCollider::TYPE_SPHERE:
//        m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pBoundingDesc);   
//        break;
//    }
//
//    if (nullptr == m_pBounding)
//        return E_FAIL;
//
//
//    return S_OK;
//}
//
//void CCollider::Update(_fmatrix WorldMatrix)
//{
//    m_pBounding->Update(WorldMatrix);
//}
//
//_bool CCollider::Intersect(CCollider* pTargetCollider)
//{
//    m_isColl = m_pBounding->Intersect(pTargetCollider->m_eType, pTargetCollider->m_pBounding);
//
//    return m_isColl;
//}
//
//HRESULT CCollider::Render()
//{
//    if (nullptr == m_pBounding)
//        return E_FAIL;
//
//    m_pEffect->SetWorld(XMMatrixIdentity()); // ���ÿ��� �����Ʈ������ ���ؼ� ���ͼ� ��������� �����ִ°� (���Ļ�)
//    m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
//    m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));
//
//    m_pEffect->Apply(m_pContext);
//
//    m_pContext->IASetInputLayout(m_pInputLayout);
//    
//    m_pBatch->Begin();
//
//    m_pBounding->Render(m_pBatch, true == m_isColl ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));
//
//
//    m_pBatch->End();
//
//    return S_OK;
//}
//
//CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType)
//{
//    CCollider* pInstance = new CCollider(pDevice, pContext);
//
//    if(FAILED(pInstance->Initialize_Prototype(eColliderType)))
//    {
//        MSG_BOX("Failed to Created : CCollider");
//        Safe_Release(pInstance);
//
//    }
//
//    return pInstance;
//}
//
//CComponent* CCollider::Clone(void* pArg)
//{
//    CCollider* pInstance = new CCollider(*this);
//
//    if(FAILED(pInstance->Initialize(pArg)))
//    {
//        MSG_BOX("Failed to Created : CCollider");
//        Safe_Release(pInstance);
//    }
//
//    return pInstance;
//}
//
//void CCollider::Free()
//{
//    __super::Free();
//
//    Safe_Release(m_pBounding);
//
//#ifdef _DEBUG
//    Safe_Release(m_pInputLayout);
//
//    if(false == m_isCloned)
//    {
//        Safe_Delete(m_pBatch);
//        Safe_Delete(m_pEffect);
//    }
//
//#endif // _DEBUG
//
//}
