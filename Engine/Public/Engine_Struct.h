#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	struct ENGINE_DESC	
	{ 
		HINSTANCE		hInstance;
		HWND			hWnd;
		unsigned int	iNumLevels;
		unsigned int	iNumUIScenes;
		bool			isWindowed;
		unsigned int	iViewportWidth;
		unsigned int	iViewportHeight;
	};

	struct LIGHT_DESC
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOT, TYPE_END };

		TYPE	eType;

		XMFLOAT4		vDirection;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vPosition;

		FLOAT			fRange;
		FLOAT			fSpot;

		_uint			iCurrentLevel;
	};

	typedef struct MonsterSpawnInfo
	{
		_float4 vMonsterPos = {};
		_int	iMonsterIndex = {};
	}MONSTERSPAWNINFO;

	typedef struct FogParams
	{
		_float4x4 g_ProjMatrixInv; // 64
		_float4x4 g_ViewMatrixInv;// 64

		_float2 fHeightNoiseFactor;	//8
		_float2 fFogStartDistance; // x �Ÿ� �Ȱ� �Ÿ� , y ���� �Ȱ� �Ÿ�, byte = 8

		_float4 g_vCamPosition; // 16
		_float4 g_FogColor;		// 16
		_float4 fFogFactor;		// 12

		float g_fTime; //4
		float Padding[3];
	}FOGPARAMS;



	struct MATERIAL_DESC	
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
		float			fPower;
		XMFLOAT4		vEmissive;
	};

	struct UI_TextInfo // �ؽ�Ʈ ���� �����
	{
		_uint		iTextID;			// �ؽ�Ʈ ���� Ű��
		_wstring	strFontName;		// ����ϴ� ��Ʈ �̸�
		_wstring	srtTextContent;		// �ؽ�Ʈ ����
		_float2		fTextStartPos;		// �׷����� ������(����X)
		_float2		fTextSize;			// TextContent ���̿� ���� �ڵ����� �������� ���� ���� ����
		_float4     fTextColor;         // �ؽ�Ʈ ���� ��������!
	};

	struct UI_Item //������ ui ���� �����
	{
		//_uint ItemID = {};
		ITEM_TYPE ItemType = {};
		_uint ItemIconNum = {};
		_tchar* ItemName = {};
		_tchar* ItemDesc = {};
		_uint ItemCount = {};
	};
	
	typedef struct RippleInfo
	{
		_float2 g_RippleCenter; // x, z ���� ��ġ
		_float2 g_Ripplefactor; // x �ð� y ������
	}RIPPLEINFO;


	struct LIGHT_RANGE_DESC
	{
		class CLight* pDesc;

		FLOAT		fMAXRange;
		FLOAT		fRangeSpeed;
	};


	struct LIGHT_RANGE_DESC_MINUS
	{
		class CLight* pDesc;

		FLOAT		fMINRange;
		FLOAT		fRangeSpeed;
	};

	struct ENGINE_DLL Skill_Information
	{
		_float* fSkillCoolTime = {};	        // ��ü ��ų �� Ÿ�� �ð�	
		_float* fSkillCurrentCoolTime = {};	    // ���� ��ų �� Ÿ�� ���� �ð�		
		_bool* bSkill_Unlocked = {};	        // ��ų�� �ر� �Ǿ��� �� �� �Ǿ��� �� Ȯ��	
		_bool* bSkill_CanUse = {};		        // ��ų�� ���� ��Ÿ���� ���� ����� ���� �� ��, �� ���� üũ.	
		PLAYER_SKILL* eSkill_Name = {};         // ��ų �̸�		
	};


	struct ENGINE_DLL VTXPOS	
	{
		XMFLOAT3  vPosition; 

		const static unsigned int					iNumElements = 1; 
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};

	struct ENGINE_DLL VTXPOSTEX	
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		const static unsigned int					iNumElements = 2;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];		
	};

	struct ENGINE_DLL VTXPOSTEX_UI	// Ƽ�޽þ� UI��
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		const static unsigned int					iNumElements = 2;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};

	struct ENGINE_DLL VTXPOSTEXAGE
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;
		float			fAge; 

		const static unsigned int					iNumElements = 3;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};

	struct ENGINE_DLL VTXCUBE
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		const static unsigned int					iNumElements = 2;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};

	struct ENGINE_DLL VTXPOINT
	{
		XMFLOAT3		vPosition;
		float			fPSize;

		const static unsigned int					iNumElements = 2;
		const static D3D11_INPUT_ELEMENT_DESC       Elements[iNumElements];
	};

	struct ENGINE_DLL VTXNORTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		const static unsigned int					iNumElements = 3;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};


	struct ENGINE_DLL VTXMESH	
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		const static unsigned int					iNumElements = 4;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};


	struct ENGINE_DLL VTXANIMMESH	
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		/* �� ������ � ���� ���󰡾��ϴ����� ���� �� �ε��� �ִ� 4��. */
		XMUINT4			vBlendIndex;

		/* �� ���� ���¸� �󸶳� �����ؾ��ϴ°�? */
		/* x + y + z + w = 1 */
		XMFLOAT4		vBlendWeight;

		const static unsigned int					iNumElements = 6;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};
	
	struct KEYFRAME
	{
		XMFLOAT3  vScale; 
		XMFLOAT4  vRotation; 
		XMFLOAT3  vTranslation;
		

		float     fTrackPosition;  /* � �ð��� �������� */

	};


	struct ENGINE_DLL VTX_PARTICLE_INSTANCE 
	{
		XMFLOAT4	   vRight; 
		XMFLOAT4       vUp; 
		XMFLOAT4	   vLook; 
		XMFLOAT4       vTranslation;
		XMFLOAT2       vLifeTime;
		XMFLOAT2       vBornTime;
		XMFLOAT2       vTexcoord;
		unsigned int   iFrame; 
		float          fDurationTime; 
	};

	struct ENGINE_DLL COMPUTE_PARTICLE_INSTANCE
	{
		XMFLOAT4	   vRight;
		XMFLOAT4       vUp;
		XMFLOAT4	   vLook;
		XMFLOAT4       vTranslation;
		XMFLOAT2       vLifeTime;
		XMFLOAT3	   vSpeed;
		XMFLOAT3	   vPivot;
		XMFLOAT3	   vScale;
		float		   fDelayTime;
	};

	struct ENGINE_DLL ANIMEVENT	
	{
		_bool isEventActivate = false;
		_bool isPlay = false;	
		_float fStartTime;
		_float fEndTime;	
		EVENT_FRAME_TYPE eType;
		char szName[MAX_PATH];	
	};

	/* �׸���� ������ �ν��Ͻ̿� ������ ���� ���� */
	struct ENGINE_DLL VTX_RECT_INSTANCE	
	{
		const static unsigned int					iNumElements = 7;	
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];	
	};	


	struct ENGINE_DLL VTX_POINT_INSTANCE	
	{
		const static unsigned int					iNumElements = 9;	
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];	
	};	


	struct ENGINE_DLL VTX_MODEL_INSTANCE
	{
		XMFLOAT4 InstanceMatrix[4];

		const static unsigned int					iNumElements = 8;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};

	struct ENGINE_DLL COMPUTE_POINT_INSTANCE
	{
		const static unsigned int					iNumElements = 8;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};

	typedef struct ENGINE_DLL VtxSwordTrail
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		const static unsigned int					iNumElements = 2;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXST;

	typedef struct ENGINE_DLL vtxNull // ���� ���۰� �ʿ���°��
	{
		const static unsigned int					iNumElements = 1;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNULL;
}


#endif // Engine_Struct_h__
