#include "common.hlsl"

Texture2D g_Texture:register(t0);
Texture2D g_TextureNormal:register(t1);
SamplerState g_SamplerState:register(s0);

void main(in PS_IN In, out float4 outDiffuse:SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//�@���}�b�s���O
	float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
	normalMap = (normalMap*2.0f) - 1.0f;
	//normalMap=(-normalMap.x*inTangent)+(-normalMap.y*inBinormal)+

	float4 normal;
	normal.x = -normalMap.x;
	normal.y = normalMap.z;
	normal.z = normalMap.y;
	normal.w = 0.0;

	normal = normalize(normal);


	//�����o�[�g�g�U�Ɩ�
	float light = -dot(normal.xyz, Light.Direction.xyz);
	light = saturate(light);

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb*light;
	outDiffuse.a *= In.Diffuse.a;

	//�X�y�L����(�t�H��)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//�����x�N�g��
	eyev = normalize(eyev);

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);//���˃x�N�g��
	refv = normalize(refv);

	float specular = -dot(eyev, refv);
	specular = saturate(specular);
	specular = pow(specular, 30);

	//�x���x�b�g
	//specular*=saturate(1.0+dot(eyev,normal.xyz));

	outDiffuse.rgb += specular * 0.5;
	outDiffuse.a = 1.0;

	//�������C�e�B���O
	float rim = 1.0 + dot(eyev, normal.xyz);
	rim = pow(rim, 2)*0.6;
	outDiffuse.rgb += rim * 0.6;
}
