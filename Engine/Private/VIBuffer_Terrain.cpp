#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
	:CVIBuffer(Prototype)
	, m_iNumverticesX{ Prototype.m_iNumverticesX }
	, m_iNumverticesZ{ Prototype.m_iNumverticesZ }
	, m_VertexPos{ Prototype.m_VertexPos }

{

}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _uint dwCntX, const  _uint dwCntZ, _uint dwVertexItv, const _tchar* path, const _tchar* _HeightPath)
{
	/* ���ؽ����ۿ� �ε��� ������ ����*/
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumVertices = dwCntX * dwCntZ;
	m_iIndexStride = 4;
	m_iNumIndices = (dwCntX - 1) * (dwCntZ - 1) * 2 * 3;


	m_iNumverticesX = dwCntX;
	m_iNumverticesZ = dwCntZ;
	m_iCntX = dwCntX;
	m_iCntY = dwCntZ;

	/* �׷��� ���� �ؾ��� ���� VTXNORTEX�� �ִ� POSITION, NORAML , Texcoord ���� ä���ֱ�*/

	//m_VertexPos = new XMVECTOR[m_iNumVertices];
	m_VertexPos = new XMFLOAT4[m_iNumVertices];
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];

	const _tchar* strFilePath{};
	strFilePath = _HeightPath;

	_ulong  dwByteHegiht = {};

	HANDLE  hFile = CreateFile(strFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Open Terrain File!!");
		return E_FAIL;
	}

	_uint numVertices = {};
	ReadFile(hFile, &numVertices, sizeof(_uint), &dwByteHegiht, nullptr);

	m_pVertices = new VTXNORTEX[numVertices];
	ReadFile(hFile, m_pVertices, sizeof(VTXNORTEX) * numVertices, &dwByteHegiht, nullptr);

	CloseHandle(hFile);

	if (nullptr == path)
	{
		if (nullptr != _HeightPath)
		{
			for (_uint i = 0; i < dwCntZ; i++)
			{
				for (_uint j = 0; j < dwCntX; j++)
				{
					memcpy(pVertices, m_pVertices, sizeof(VTXNORTEX) * m_iNumVertices);
					XMLoadFloat4(&m_VertexPos[dwCntZ * i + j]) = XMLoadFloat3(&pVertices[dwCntZ * i + j].vPosition);
				}
			}
		}
		else
		{
			/* ���ؽ� ���� �־��ֱ� */
			for (_uint i = 0; i < dwCntZ; i++)
			{
				for (_uint j = 0; j < dwCntX; j++)
				{
					pVertices[dwCntZ * i + j].vPosition = _float3(static_cast<_float>(j) * dwVertexItv, 0.f, static_cast<_float>(i));
					pVertices[dwCntZ * i + j].vNormal = _float3(0.f, 1.f, 0.f);
					pVertices[dwCntZ * i + j].vTexcoord = _float2(j * dwVertexItv / (dwCntX - 1.f), i / (dwCntZ - 1.f));
					//m_VertexPos[dwCntZ * i + j] = XMLoadFloat3(&pVertices[dwCntZ * i + j].vPosition);
					XMLoadFloat4(&m_VertexPos[dwCntZ * i + j]) = XMLoadFloat3(&pVertices[dwCntZ * i + j].vPosition);
				}
			}
		}

	

		/* ���� ���ؽ� ������ �ٽ� ���� �׷�����ġ�� �־��ֱ�
		   CPU �� �˰��ִ� �޸� �̹Ƿ� */

		ZeroMemory(&m_InitialData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_InitialData.pSysMem = pVertices;


		/* ���ؽ� ���� �����*/
		m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iVertexStride;


		if (FAILED(__super::Create_Buffer(&m_pVB)))
			return E_FAIL;

		Safe_Delete_Array(pVertices);

		/*���� �ε��� ���� �־��ֱ�*/

		_uint* pIndices = new _uint[m_iNumIndices];
		_ulong index = { 0 };

		/* �ε����� �׷� ��� ���������� �����ϱ�*/

		for (_uint i = 0; i < dwCntZ - 1; i++)
		{
			for (_uint j = 0; j < dwCntX - 1; j++)
			{
				/* 1���� �簢���� �ϼ����Ѿ���.*/
				/*�ε��� 6�� ��ġ*/

				_uint iIndex = i * dwCntZ + j;

				_uint iIndices[4] =
				{
					iIndex + dwCntX,         /* ���� ��*/
					iIndex + dwCntX + 1,	 /* ������ �� */
					iIndex + 1,				 /* ������ �Ʒ�*/
					iIndex,					 /* ���� �Ʒ�*/
				};

				pIndices[index++] = iIndices[0];
				pIndices[index++] = iIndices[1];
				pIndices[index++] = iIndices[2];

				pIndices[index++] = iIndices[0];
				pIndices[index++] = iIndices[2];
				pIndices[index++] = iIndices[3];
			}
		}

		/* �ε��� ���� �����*/
		m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iIndexStride;

		m_InitialData.pSysMem = pIndices;
		if (FAILED(__super::Create_Buffer(&m_pIB)))
			return E_FAIL;

		Safe_Delete_Array(pIndices);
		Safe_Delete_Array(m_pVertices);
	}

	/* ���� ��ΰ� ���� �� */
	else
	{
		_ulong dwByte = {};
		HANDLE hFile = CreateFile(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
		{
			MSG_BOX("Failed to Read : HeightMap");
			return E_FAIL;
		}

		BITMAPFILEHEADER fh{};   /* ���� �̸� �� ���� ������¥�� ������ִ°�*/
		ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

		BITMAPINFOHEADER ih{};   /* �ȼ� ������ ���� �� ����, ���� ���� ������ ����*/
		ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
		/* ReadFile�� �ϴ� ������ ���Ͽ��� ���� Ŀ���� �Ű��ִ� �۾�*/

		_uint* pPixel = new _uint[m_iNumVertices]; /* 32 ��Ʈ�̹Ƿ� _uint�� */
		ReadFile(hFile, pPixel, sizeof(_uint) * m_iNumVertices, &dwByte, nullptr);

		//	A8 R8 G8 B8 
//
//	0b  11111111 10101010 10101010 10101010
//&	0b  00000000 00000000 00000000 11111111
//		00000000 00000000 00000000 10101010
//
//
		/* ���ؽ� ���� �־��ֱ� */
		for (_uint i = 0; i < dwCntZ; i++)
		{
			for (_uint j = 0; j < dwCntX; j++)
			{
				_uint			iIndex = i * dwCntX + j;

				pVertices[iIndex].vPosition = _float3(static_cast<_float>(j) * dwVertexItv, (pPixel[iIndex] & 0x000000ff) / 10.f, static_cast<_float>(i));
				pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
				pVertices[iIndex].vTexcoord = _float2(j * dwVertexItv / (dwCntX - 1.f), i / (dwCntZ - 1.f));
				//m_VertexPos[dwCntZ * i + j] = XMLoadFloat3(&pVertices[dwCntZ * i + j].vPosition);
				XMLoadFloat4(&m_VertexPos[dwCntZ * i + j]) = XMLoadFloat3(&pVertices[dwCntZ * i + j].vPosition);
			}
		}

		/* ���� ���ؽ� ������ �ٽ� ���� �׷�����ġ�� �־��ֱ�
		   CPU �� �˰��ִ� �޸� �̹Ƿ� */



		   /*���� �ε��� ���� �־��ֱ�*/

		_uint* pIndices = new _uint[m_iNumIndices];
		ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);
		_ulong index = { 0 };

		/* �ε����� �׷� ��� ���������� �����ϱ�*/

		for (_uint i = 0; i < dwCntZ - 1; i++)
		{
			for (_uint j = 0; j < dwCntX - 1; j++)
			{
				/* 1���� �簢���� �ϼ����Ѿ���.*/
				/*�ε��� 6�� ��ġ*/

				_uint iIndex = i * dwCntZ + j;

				_uint iIndices[4] =
				{
					iIndex + dwCntX,         /* ���� ��*/
					iIndex + dwCntX + 1,	 /* ������ �� */
					iIndex + 1,				 /* ������ �Ʒ�*/
					iIndex,					 /* ���� �Ʒ�*/
				};

				_vector vSour, vDest, vNormal;

				pIndices[index++] = iIndices[0];
				pIndices[index++] = iIndices[1];
				pIndices[index++] = iIndices[2];

				vSour = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
				vDest = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
				vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

				XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
				XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
				XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

				pIndices[index++] = iIndices[0];
				pIndices[index++] = iIndices[2];
				pIndices[index++] = iIndices[3];

				vSour = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
				vDest = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
				vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

				XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
				XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
				XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);

			}
		}

		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
		}


		/* ���ؽ� ���� ���� �׷��� ī�忡 �Ѱ��ֱ� */
		ZeroMemory(&m_InitialData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_InitialData.pSysMem = pVertices;

		/* ���ؽ� ���� �����*/
		ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
		m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iVertexStride;

		if (FAILED(__super::Create_Buffer(&m_pVB)))
			return E_FAIL;


		/* �ε��� ���� ���� �׷��� ī�忡 �Ѱ��ֱ� */
		ZeroMemory(&m_InitialData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_InitialData.pSysMem = pIndices;

		/* ���ؽ� ���� �����*/
		ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
		m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iIndexStride;

		if (FAILED(__super::Create_Buffer(&m_pIB)))
			return E_FAIL;

		Safe_Delete_Array(pVertices);
		Safe_Delete_Array(pPixel);
		Safe_Delete_Array(pIndices);
		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

_float CVIBuffer_Terrain::Get_Height(const XMFLOAT3& _vPos)
{
	if (!m_VertexPos || m_iNumverticesX <= 1 || m_iNumverticesZ <= 1)
		return 0.0f;

	XMFLOAT3 vLocalPos =
	{
		_vPos.x - m_fTerrainPos.x,
		_vPos.y - m_fTerrainPos.y,
		_vPos.z - m_fTerrainPos.z
	};

	_long uCurrentX = static_cast<_long>(vLocalPos.x);
	_long uCurrentZ = static_cast<_long>(vLocalPos.z);

	if (uCurrentX < 0 || uCurrentZ < 0 || uCurrentX >= m_iNumverticesX - 1 || uCurrentZ >= m_iNumverticesZ - 1)
		return 0.0f;

	_long dwIndex = uCurrentZ * m_iNumverticesX + uCurrentX;

	_float fRatioX = vLocalPos.x - XMVectorGetX(XMLoadFloat4(&m_VertexPos[dwIndex]));
	_float fRatioZ = XMVectorGetZ(XMLoadFloat4(&m_VertexPos[dwIndex])) - vLocalPos.z;

	XMVECTOR v0, v1, v2;

	if (fRatioX > fRatioZ)
	{
		v0 = XMLoadFloat4(&m_VertexPos[dwIndex]);
		v1 = XMLoadFloat4(&m_VertexPos[dwIndex + 1]);
		v2 = XMLoadFloat4(&m_VertexPos[dwIndex + m_iNumverticesX + 1]);
	}
	else
	{
		v0 = XMLoadFloat4(&m_VertexPos[dwIndex]);
		v1 = XMLoadFloat4(&m_VertexPos[dwIndex + m_iNumverticesX]);
		v2 = XMLoadFloat4(&m_VertexPos[dwIndex + m_iNumverticesX + 1]);
	}

	XMVECTOR vDir0 = v1 - v0;
	XMVECTOR vDir1 = v2 - v0;
	XMVECTOR vNormal = XMVector3Normalize(XMVector3Cross(vDir0, vDir1));

	m_vPlane.x = XMVectorGetX(vNormal);
	m_vPlane.y = XMVectorGetY(vNormal);
	m_vPlane.z = XMVectorGetZ(vNormal);
	m_vPlane.w = -(m_vPlane.x * XMVectorGetX(v0) + m_vPlane.y * XMVectorGetY(v0) + m_vPlane.z * XMVectorGetZ(v0));

	return (-m_vPlane.x * vLocalPos.x - m_vPlane.z * vLocalPos.z - m_vPlane.w) / m_vPlane.y;
}


CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iCountX, _uint iCountY, _uint iVertexItv, const _tchar* pPath, const _tchar* _HeightPath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iCountX, iCountY, iVertexItv, pPath, _HeightPath)))
	{
		MSG_BOX("Failed to Created : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_VertexPos);
}
