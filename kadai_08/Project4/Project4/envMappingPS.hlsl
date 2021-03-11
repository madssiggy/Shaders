#include "common.hlsl"


Texture2D g_Texture:register(t0);
Texture2D g_TextureEnv:register(t1);
SamplerState g_SamplerState:register(s0);

void main(in PS_IN In, out float4 outDiffuse:SV_Target)
{
	float4 normal = normalize(In.Normal);

	//�����o�[�g�g�U�Ɩ�
	float light = -dot(normal.xyz, Light.Direction.xyz);
	light = saturate(light);

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb*light;
	outDiffuse.a *= In.Diffuse.a;

	//�X�y�L����(�t�H��)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//�����x�N�g��
	eyev = normalize(eyev);

	float3 refv = reflect(eyev, normal.xyz);//���˃x�N�g��
	refv = normalize(refv);

	float specular = -dot(eyev, refv);
	specular = saturate(specular);
	specular = pow(specular, 30);

	//�x���x�b�g
	//specular*=saturate(1.0+dot(eyev,normal.xyz));

	outDiffuse.rgb += specular;

	//�������C�e�B���O
	float rim = 1.0 + dot(eyev, normal.xyz);
	rim = pow(rim, 2)*0.6;
	outDiffuse.rgb += rim;

	//���^�t�H�O
	float dist = distance(In.WorldPosition.xyz, CameraPosition.xyz);
	float fog = saturate(dist / 20.0);

	//���}�b�s���O
	float2 envTexCoord;
	envTexCoord.x = -refv.x*0.3 + 0.5;
	envTexCoord.y = -refv.y*0.3 + 0.5;
	//outDiffuse.rgb += g_TextureEnv.Sample(g_SamplerState, envTexCoord);
	outDiffuse.rgb = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 3.0);
}
