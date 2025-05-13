

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
}; 

SamplerState LinearSampler_Clamp
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};


RasterizerState RS_Default
{
    CullMode = BACK;
};

RasterizerState RS_Wireframe
{
    FillMode = WIREFRAME;
};

RasterizerState Rs_Cull_CW
{
    FillMode = SOLID;
    CullMode = FRONT;
};

RasterizerState Rs_Cull_NONE    
{
    FillMode = SOLID;
    CullMode = NONE;    
};



RasterizerState Rs_Depth_Bias
{
    CullMode = BACK;
    
    DepthBias = 100;
    DepthBiasClamp = 0.f;
    SlopeScaledDepthBias = 2.f;
};

DepthStencilState DSS_Default
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;
};

DepthStencilState DSS_SKip_Z
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};


DepthStencilState DSS_Skip_Write // 깊이 비교를 하되 깊이를 작성하지는 않음
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
    DepthFunc = LESS_EQUAL;
};


DepthStencilState DSS_Skip_Always // 깊이 비교를 하되 깊이를 작성하지는 않음
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
    DepthFunc = ALWAYS;
};

DepthStencilState DSS_Skip_ITEMZ
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
    DepthFunc = LESS_EQUAL;
};

DepthStencilState DSS_Thymasia_UI
{
    //DepthEnable = TRUE;
    //DepthWriteMask = ALL;
    //DepthFunc = LESS_EQUAL;
    //StencilEnable = FALSE;
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState DSS_WeightBlend
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
    DepthFunc = LESS_EQUAL;
};

BlendState BS_Default
{
    BlendEnable[0] = FALSE; /* 여기서 0은 RenderTarget의 넘버 */
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = TRUE;

    BlendOp = Add;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;

	/*
	SrcColor(rgba), DestColor(rgba)

	SrcColor.rgb * SrcColor.a + DestColor.rgb * (1.f - SrcColor.a)
	*/		
};

BlendState BS_Blend
{
    BlendEnable[0] = TRUE;
    BlendOp[0] = Add;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;

	/*
	SrcColor(rgba), DestColor(rgba)

	SrcColor.rgb * 1 + DestColor.rgb * 1


	*/
};


BlendState BS_Blend_Second
{
    BlendEnable[0] = TRUE;
    BlendOp[0] = Add;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;

    BlendEnable[1] = TRUE;
    BlendOp[1] = Add;
    SrcBlend[1] = ONE;
    DestBlend[1] = ONE;
};

SamplerComparisonState ShadowSampler
{
    Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    AddressU = wrap;
    AddressV = wrap;
    AddressW = wrap;
    ComparisonFunc = GREATER;
};

BlendState BS_WeightBlend_Client
{
    BlendEnable[0] = TRUE;
    BlendOp[0] = Add;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;
};

BlendState BS_WeightBlend_Engine 
{
    BlendEnable[0] = TRUE;
    BlendOp[0] = Add;
    SrcBlendAlpha[0] = INV_SRC_ALPHA;
    DestBlendAlpha[0] = SRC_ALPHA;
};

BlendState BS_WeightBlend_Dust
{
    BlendEnable[0] = TRUE;
    SrcBlendAlpha[0] = ZERO;
    DestBlendAlpha[0] = INV_SRC_ALPHA;
    BlendOpAlpha[0] = Add;
};

BlendState BS_Blend_Inv
{
    BlendEnable[0] = TRUE;
    BlendOp[0] = Add;
    SrcBlend[0] = DEST_COLOR;
    DestBlend[0] = ZERO;
    SrcBlendAlpha[0] = INV_SRC_ALPHA;
    DestBlendAlpha[0] = SRC_ALPHA;
};