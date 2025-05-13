#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const
	{
		return m_iMaterialIndex;	
	}

public:
	virtual HRESULT Initialize_Prototype(CModel::MODEL eModelType, CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;

public:
	/* 2�� 28�� �߰� */
	HRESULT			Bind_InputAssembler_Instance(ID3D11Buffer* pInstanceBuffer);
	/* 2�� 25�� �߰� */
	HRESULT			Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint _iNumInstance);


public:
	HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pContstantName, const vector<class CBone*>& Bones);
	//void		Compute_BoundingBox(_float3& _vMin, _float3& _vMax);

private:
	_char			   m_szName[MAX_PATH] = {};
	/* �� �޽ÿ� ������ �ִ� ���� ���� */
	_uint			   m_iMaterialIndex = {};

	/* �� �޽ÿ� ������ �ִ� ������ �ε��� */
	_uint			   m_iNumBones = {};

	/* �ε��� : ���� ������ �ִ� ��ü �� ������ �ε����� ���ϴ� ���̴�*/
	vector<_uint>      m_BoneIndices;

	_float4x4          m_BoneMatrices[1500] = {}; /* ���̴� �ڵ��� ���� �Լ��� ���� �������� �غ�*/
	_float4x4		   m_ColMajorBoneMatrices[1500] = {};

	vector<_float4x4>  m_OffsetMatrices;

	/* ���̳ʸ�ȭ ���� �ɹ� ����*/
	D3D11_BUFFER_DESC	  m_MeshBufferdesc = {};	
	VTXMESH*			  m_nonAnimpVertices;	
	VTXANIMMESH*		  m_AnimVertices;	
	CModel::MODEL         m_ModelType;	
	_uint*				  pNewIndices;	

private:
	/*structed Buffer ���� �ڵ�*/
	ID3D11Buffer* m_pStrBuffer = { nullptr };	

private:
	HRESULT  Ready_VertexBuffer_ForNonAnim(const aiMesh* pAIMesh, _fmatrix PreTransforMatrix);
	HRESULT  Ready_VertexBuffer_ForAnim(CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransforMatrix);

public:
	HRESULT Save_Mesh(ostream& os);	
	HRESULT Load_Mesh(istream& is);	


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::MODEL eModelType, CModel* pModel, const aiMesh* pAIMesh,_fmatrix PreTransformMatrix);
	static CMesh* LoadCreate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, istream& is);	
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END