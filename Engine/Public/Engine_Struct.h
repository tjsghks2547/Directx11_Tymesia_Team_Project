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
		_float2 fFogStartDistance; // x 거리 안개 거리 , y 높이 안개 거리, byte = 8

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

	struct UI_TextInfo // 텍스트 정보 저장용
	{
		_uint		iTextID;			// 텍스트 고유 키값
		_wstring	strFontName;		// 사용하는 폰트 이름
		_wstring	srtTextContent;		// 텍스트 내용
		_float2		fTextStartPos;		// 그려지는 시작점(중점X)
		_float2		fTextSize;			// TextContent 길이에 따라 자동으로 구해지는 가로 세로 길이
		_float4     fTextColor;         // 텍스트 색상 저장하자!
	};

	struct UI_Item //아이템 ui 정보 저장용
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
		_float2 g_RippleCenter; // x, z 월드 위치
		_float2 g_Ripplefactor; // x 시간 y 반지름
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
		_float* fSkillCoolTime = {};	        // 전체 스킬 쿨 타임 시간	
		_float* fSkillCurrentCoolTime = {};	    // 현재 스킬 쿨 타임 지난 시간		
		_bool* bSkill_Unlocked = {};	        // 스킬이 해금 되었는 지 안 되었는 지 확인	
		_bool* bSkill_CanUse = {};		        // 스킬이 현재 쿨타임이 지나 사용이 가능 한 지, 안 한지 체크.	
		PLAYER_SKILL* eSkill_Name = {};         // 스킬 이름		
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

	struct ENGINE_DLL VTXPOSTEX_UI	// 티메시아 UI용
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

		/* 이 정점이 어떤 뼈를 따라가야하는지에 대한 뼈 인덱스 최대 4개. */
		XMUINT4			vBlendIndex;

		/* 각 뼈의 상태를 얼마나 적용해야하는가? */
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
		

		float     fTrackPosition;  /* 어떤 시간의 상태인지 */

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

	/* 그리기용 정점과 인스턴싱용 정점을 합한 정보 */
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

	typedef struct ENGINE_DLL vtxNull // 정점 버퍼가 필요없는경우
	{
		const static unsigned int					iNumElements = 1;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNULL;
}


#endif // Engine_Struct_h__
