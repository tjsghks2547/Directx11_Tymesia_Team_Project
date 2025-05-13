#include "ItemMgr.h"
#include "GameInstance.h"

CItemMgr::CItemMgr()
{

}

HRESULT CItemMgr::Initialize()
{
    return S_OK;
}

void CItemMgr::Update(_float _fTimeDelta)
{

}

HRESULT CItemMgr::Add_Item(ITEM_TYPE _eItemType, _uint _iItemCount, CItem* _pGameObject, _bool _bTaken)
{
    if (nullptr == _pGameObject)
        return E_FAIL;

    if (_bTaken)
    {
        // map<ITEM_TYPE, pair<_uint, vector<CItem*>>>
        auto iter = m_mapTakenItems.find(_eItemType);
        if (iter == m_mapTakenItems.end())
            return S_OK;

        ITEM_TYPE eItemType = iter->first;
        _uint iTakenItemCount = iter->second.first;

        // m_mapTakenItems[eItemType].first = iItemCount;


        if (_pGameObject)
        {
            m_mapItems[_eItemType].second.push_back(_pGameObject);
            _pGameObject->Set_ItemCount(iTakenItemCount);
        }

        m_mapItems[_eItemType].first = iTakenItemCount;
    }
    else
    {
        m_eItemType = _eItemType;

        m_eItemCount[(_uint)_eItemType] = _iItemCount;

        m_mapItems[_eItemType].first = m_eItemCount[(_uint)_eItemType];
        m_mapItems[_eItemType].second.push_back(_pGameObject);

        for (auto& pItems : m_mapItems[_eItemType].second)
            pItems->Set_ItemCount(_iItemCount);
    }

   
    return S_OK;
}

HRESULT CItemMgr::Sub_Item(ITEM_TYPE _eItemType, const _wstring& _ItemName)
{
    return S_OK;
}

void CItemMgr::Set_ItemPos(_fvector _vItemWorldPos)
{

}

HRESULT CItemMgr::Drop_Item(ITEM_TYPE _eItemType, _fvector _vDropPosition, class CGameObject* _GameObject, _uint _iDropItemCount)
{
    vector<CItem*>* pVecItems = Find_ItemVector(_eItemType);

    if (nullptr == pVecItems)
        return E_FAIL;

    for (auto& pItems : *pVecItems)
    {
        if (nullptr != pItems)
        {
            pItems->Set_DropItemCount(_iDropItemCount);
            pItems->Set_BeAcquired(false);
            pItems->Set_BeDropping(true);
            pItems->Set_ColliderRender(true);

            _float4 vDropPosition;
            XMStoreFloat4(&vDropPosition, _vDropPosition);
            pItems->Set_BezierPosition(vDropPosition, _GameObject);

            break;
        }
    }
    if (!strcmp(_GameObject->Get_Name(), "PLAYER"))
    {
        m_lDropItem.push_back(_eItemType); // 알림용 버림 아이템 저장 
    }

    return S_OK;
}

HRESULT CItemMgr::Pop_Item(ITEM_TYPE _eItemType, _fvector _vPopPosition, CGameObject* _GameObject, _uint _iPopItemCount)
{
    vector<CItem*>* pVecItems = Find_ItemVector(_eItemType);

    if (nullptr == pVecItems)
        return E_FAIL;

    for (auto& pItems : *pVecItems)
    {
        if (nullptr != pItems)
        {
            pItems->Set_DropItemCount(_iPopItemCount);

            //if (_eItemType == ITEM_TYPE::ITEM_FIELDITEM)
            if (
                _eItemType == ITEM_TYPE::ITEM_BADZOKER ||
                _eItemType == ITEM_TYPE::ITEM_BEOMSEUNG ||
                _eItemType == ITEM_TYPE::ITEM_FAKER ||
                _eItemType == ITEM_TYPE::ITEM_UIN ||
                _eItemType == ITEM_TYPE::ITEM_PLAYERMASTER ||
                _eItemType == ITEM_TYPE::ITEM_YUBHIN
                )
            {
                pItems->Set_BeActivated(true);
                pItems->Set_BeAcquired(false);
                pItems->Set_ColliderRender(true);
            }
            else
                pItems->Set_BeActivated(true);

            pItems->Set_Position(_vPopPosition);

            break;
        }
    }

    return S_OK;
}

void CItemMgr::Clear_ItemInfo()
{
    m_mapTakenItems.clear();

    // map<ITEM_TYPE, pair<_uint, vector<CItem*>>>
    // map 의 second 가 pair 가 되고... pair의 first 가 갯수 자나 얘도 초기화.. 0개로. Pair 가 m_mapItems. 걍시발눈엣가시용임이거는 ㅇㅇ
    // map 으 second 의 second 가 pair 의 second 니까. vector<CItem> 이니까.. 얘 클리어 해서 비워벌힘;;
    for (auto& Pair : m_mapItems)
    {
        ITEM_TYPE eItemType = Pair.first;
        _uint iItemCount = Pair.second.first;

        m_mapTakenItems[eItemType].first = iItemCount;

        Pair.second.second.clear();
        Pair.second.first = 0;
    }

    m_mapItems.clear();
}

HRESULT CItemMgr::Acquire_Item(ITEM_TYPE _eItemType)
{
    vector<CItem*>* pVecItems = Find_ItemVector(_eItemType);

    if (nullptr == pVecItems)
        return E_FAIL;
    _uint iDropCount = {};

    for (auto& pItems : *pVecItems)
    {
        if (_eItemType == ITEM_TYPE::ITEM_DEADBRANCH)
        {
            pItems->Reset_ItemActivate();
            return S_OK;
        }
        else
        {
            pItems->Set_ColliderRender(false);
            pItems->Reset_ItemState();
        }

        pItems->Set_BeAcquired(true);
        iDropCount = pItems->Get_DropItemCount();
    }

    m_lSaveItem.push_back(_eItemType);// 알림용 획득 아이템 저장 
    m_mapItems[_eItemType].first += iDropCount;


    return S_OK;
}

_bool CItemMgr::Use_Item(ITEM_TYPE _eItemType, _uint iNum)
{
    for (auto& ItemBox : m_mapItems)
    {
        if (_eItemType == ItemBox.first)
        {
            ItemBox.second.first -= iNum;
            if (0 >= ItemBox.second.first)
                return true; // 이 아이템이 0개가 되었는지 체크
        }
    }
    return false;
}

vector<CItem*>* CItemMgr::Find_ItemVector(ITEM_TYPE _eItemType)
{
    auto iter = m_mapItems.find(_eItemType);
    if (iter == m_mapItems.end())
        return nullptr;

    return &iter->second.second;
}

CItemMgr* CItemMgr::Create()
{
    CItemMgr* pInstance = new CItemMgr();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : ItemMgr");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemMgr::Free()
{
    __super::Free();
}
