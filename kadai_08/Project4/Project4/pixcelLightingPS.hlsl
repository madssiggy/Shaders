
#include "common.hlsl"


Texture2D g_Texture:register(t0);
SamplerState g_SamplerState:register(s0);

void main(in PS_IN In, out float4 outDiffuse:SV_Target) 
{
	float4 normal = normalize(In.Normal);

	//ランバート拡散照明
	float light = -dot(normal.xyz, Light.Direction.xyz);
	light = saturate(light);

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb*light;
	outDiffuse.a *= In.Diffuse.a;

	//スペキュラ(フォン)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//視線ベクトル
	eyev = normalize(eyev);

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);//反射ベクトル
	refv = normalize(refv);

	float specular = -dot(eyev, refv);
	specular = saturate(specular);
	specular = pow(specular, 30);

	//ベルベット
	//specular*=saturate(1.0+dot(eyev,normal.xyz));

	outDiffuse.rgb += specular*0.5;
	outDiffuse.a = 1.0;

	//リムライティング
	float rim = 1.0 + dot(eyev, normal.xyz);
	rim = pow(rim, 2)*0.6;
	outDiffuse.rgb += rim * 0.6;
}
