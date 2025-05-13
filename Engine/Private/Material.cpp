#include "Material.h"
#include "Shader.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice)
	,m_pContext(pContext)
{

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Initialize(const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	for (_uint i = 0; i<AI_TEXTURE_TYPE_MAX; i++)	
	{
		/* Ư�� �Ӽ��� ǥ���ϴ� �ؽ��İ� �����ΰ�? (ex) Diffuse �ؽ�ó�� ��������?)*/
		_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(i));

		m_MtrlTextures[i].reserve(iNumTextures);

		/* ������ Diffuse �ؽ�ó�� �ε��Ѵ�.*/
		/* �ش� ������ ������ �׳� Skip �ϴ� ���� */
		for(_uint j = 0; j < iNumTextures; j++)
		{
			aiString   strTextureFilePath;
			ID3D11ShaderResourceView* pSRV = { nullptr };

			/*GetTexture���� ��θ� ��ȯ���ִµ� ���� ������ �������� ���(����fbx�� ���� ����)�� ��θ� �������ذ�*/
			/*���� �츮 ���� ��η� ����ְ� ���⿡���� �����̸��� Ȯ���ڸ� ��������.*/
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(i), j, &strTextureFilePath)))
				return E_FAIL;

			_char   szDrive[MAX_PATH] = "";
			_char   szDir[MAX_PATH] = "";
			_char   szFileName[MAX_PATH] = "";
			_char   szEXT[MAX_PATH] = "";

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			_char   szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);	

			_tchar  szLastPath[MAX_PATH] = {};

			/* _char ���� multiByte�� �ƽ�Ű�ڵ带 ���Ϲ���Ʈ�� �ٲٴ� �Լ� _char->_tchar��*/
			/*  szFullPath�� szLastPath�� ���Ϲ���Ʈ�� �ٲ�*/
			MultiByteToWideChar(CP_ACP, 0, szFullPath, static_cast<_int>(strlen(szFullPath)), szLastPath, MAX_PATH);

			 
			HRESULT hr = {};

			if (false == strcmp(szEXT, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);

			/* �ٷ� ��ġ�� tga ������ �� �б� ����*/
			else if (false == strcmp(szEXT, ".tga"))
				return E_FAIL;


			else
				hr = CreateWICTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);	


			if (FAILED(hr))
				return E_FAIL;

			/* i�� aiTextureType�� �ǹ�!*/
			/* ���⼭ i�� ai���� ������ �ڷ����� Diffuse�� �ش��Ұ���.*/
			m_MtrlTextures[i].push_back(pSRV);	

		}
	}
	return S_OK;
}

HRESULT CMaterial::Bind_Material(CShader* pShader, aiTextureType eType, const _char* pConstantName, _uint iTextureIndex)
{
	/* iTextureIndex�� Diffuse���� Specluar���� ����Ű�� int ( ����� Diffuse �� 1 �̴�.)*/
	if (iTextureIndex >= m_MtrlTextures[eType].size())
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_MtrlTextures[eType][iTextureIndex]);

}


HRESULT CMaterial::Save_Material(ostream& os, const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	for (_uint i = 1; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		m_iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(i));

		//os.write((char*)&m_iNumTextures, sizeof(_uint));	
		os.write((char*)&m_iNumTextures, sizeof(_uint));

		m_ForSaveMtrlTextures[i].reserve(m_iNumTextures);

		for (_uint j = 0; j < m_iNumTextures; j++)
		{
			/* ShaderResoureView �ڷ������� ������ֱ�*/
			aiString                    strTextureFilePath;
			ID3D11ShaderResourceView* pSRV = { nullptr };

			/* strTextureFilePath�� ���� ���� ��ÿ�  ����ߴ� �ؽ�ó ��ζ� �ٸ�! ���� �̰�
			   �� Ŭ���̾�Ʈ ���� �ȿ��ִ� resource�� ��θ� �ٲ��ֱ� ���� ��ó�� �۾��� ����*/
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(i), j, &strTextureFilePath)))
				return E_FAIL;

			_char  szDrive[MAX_PATH] = "";
			_char  szDir[MAX_PATH] = "";
			_char  szFileName[MAX_PATH] = "";
			_char  szEXT[MAX_PATH] = "";

			/* ���� ���� �� ����� ����̺� ��ο� ���丮 ��α��� �и��ؼ� �־���*/
			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			_char szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);

			_tchar szLastPath[MAX_PATH] = {};

			MultiByteToWideChar(CP_ACP, 0, szFullPath, static_cast<_int>(strlen(szFullPath)), szLastPath, MAX_PATH);

			HRESULT hr = {};

			if (false == strcmp(szEXT, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);

			else if (false == strcmp(szEXT, ".tga"))
				return E_FAIL;

			else
			{
				hr = CreateWICTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);
			}

			if (FAILED(hr))
				return E_FAIL;


			m_ForSaveMtrlTextures[i].push_back(pSRV);

			//os.write((char*)&m_iNumTextures, sizeof(_uint));	
			os.write((char*)&szLastPath, sizeof(_tchar) * MAX_PATH);

		}
	}

	return S_OK;
}

HRESULT CMaterial::Load_Material(istream& os)
{
	for (size_t i = 1; i < AI_TEXTURE_TYPE_MAX; i++)
	{

		os.read((char*)&m_iNumTextures, sizeof(_uint));
		//os.read((char*)&m_szLastPath,   sizeof(_tchar) * MAX_PATH);	
		//m_MtrlTextures[i].reserve(os.read((char*)&m_iNumTextures, sizeof(_uint))

		m_MtrlTextures[i].reserve(m_iNumTextures);

		for (size_t j = 0; j < m_iNumTextures; j++)
		{
			os.read((char*)&m_szLastPath, sizeof(_tchar) * MAX_PATH);

			ID3D11ShaderResourceView* pSRV = { nullptr };

			HRESULT hr = {};
			_char  szEXT[MAX_PATH] = "";
			_char  szFullPath[MAX_PATH] = "";


			WideCharToMultiByte(CP_ACP, 0, m_szLastPath, static_cast<_int>(wcslen(m_szLastPath)), szFullPath, MAX_PATH, nullptr, nullptr);

			_splitpath_s(szFullPath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

			if (false == strcmp(szEXT, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, m_szLastPath, nullptr, &pSRV);

			else if (false == strcmp(szEXT, ".tga"))
				return E_FAIL;

			else
			{
				hr = CreateWICTextureFromFile(m_pDevice, m_szLastPath, nullptr, &pSRV);
			}

			if (FAILED(hr))
				return E_FAIL;


			m_MtrlTextures[i].push_back(pSRV);
		}

	}

	return S_OK;
}

CMaterial* CMaterial::LoadCreate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, istream& os)
{
	CMaterial* pInstance = new CMaterial(pDevice, pContext);

	if (FAILED(pInstance->Load_Material(os)))
	{
		MSG_BOX("Failed to LoadCreate : Material");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	CMaterial* pInstance = new CMaterial(pDevice,pContext);
	
	if (FAILED(pInstance->Initialize(pAIMaterial, pModelFilePath)))
	{
		MSG_BOX("Failed to Created : Material");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CMaterial::Free()
{
	__super::Free();

	for (auto& vectorSRV : m_MtrlTextures)
	{
		for (auto& SRV : vectorSRV)
			Safe_Release(SRV);

		vectorSRV.clear();
	}

	for (auto& vectorSRV2 : m_ForSaveMtrlTextures)
	{
		for (auto& SRV : vectorSRV2)
			Safe_Release(SRV);

		vectorSRV2.clear();
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
