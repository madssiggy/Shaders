//マトリクスバッファ
cbuffer WorldBuffer:register(b0) 
{
	matrix World;
}

cbuffer ViewBuffer : register(b1)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}


struct VS_IN 
{
	float4 Position			:POSITION0;
	float4 Normal			:NORMAL0;
	float4 Diffuse			:COLOR0;
	float2 TexCoord			:TEXCOORD0;
};


struct PS_IN 
{
	float4 Position			:SV_POSITION;
	float4 WorldPosition	:POSITION0;
	float4 Normal			:NORMAL0;
	float4 Diffuse			:COLOR0;
	float2 TexCoord			:TEXCOORD0;

	float4 ShadowPosition : POSITION1;

};
//マテリアルバッファへ
struct MATERIAL 
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float  Shininess;
	float3 Dummy;//16byte境界用(バイトアライン)
};

cbuffer MaterialBuffer:register(b3) 
{
	MATERIAL Material;
}

//ライトバッファ
struct LIGHT 
{
	bool	Enable;
	bool3	Dummy;//16byte境界用(バイトアライン)
	float4  Direction;
	float4  Diffuse;
	float4  Ambient;

	matrix View;
	matrix Projection;
};

cbuffer LightBuffer:register(b4) 
{
	LIGHT Light;
}

cbuffer CameraBuffer : register(b5) 
{
	float4 CameraPosition;
}

cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter;
}

