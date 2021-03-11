#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{	
	//Normalize_the_normals_of_pixels (�s�N�Z���̖@���𐳋K��)
	float4 normal = normalize(In.Normal);
	

	//���邳���v�Z
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;


	//�e�N�X�`���̃s�N�Z���F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	

	//���_�F�Ɩ��邳����Z
	outDiffuse.rgb *= In.Diffuse.rgb * light;


	//���ɖ��邳�͊֌W�Ȃ��̂ŕʌv�Z
	outDiffuse.a *= In.Diffuse.a;


	//�J��������s�N�Z���ւ̃x�N�g��(�����x�N�g��)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	/*Case1*/
	////���̔��˃x�N�g�����v�Z
	//float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	//refv = normalize(refv); //���K������
	////Calculation_of_specular_reflection
	//float specular = -dot(eyev, refv);
	////Set_mirror_reflection_distance
	//specular = saturate(specular);				//�l���T�`�����[�g
	//specular = pow(specular, 30);				//�����ł͂R�O�悵�Ă݂�
	//Add_specula_as_a_defuse
	//outDiffuse.rgb += specular;					// ���l�͘M��Ȃ��悤�ɋC��t����

	/*Case2*/
	//�u�����t�H��Sample
	//�n�[�t�x�N�g���쐬
	float3 halfv = eyev + Light.Direction.xyz;
	halfv = normalize(halfv);
	
	//�n�[�t�x�N�g���Ɩ@���̓���(�X�y�L�����[)�v�Z
	float specular = -dot(halfv, normal.xyz);
	//�T�`�����[�g����
	specular = saturate(specular);
	specular = pow(specular, 100);
	//Add_specula_as_a_defuse
	outDiffuse.rgb += specular;					// ���l�͘M��Ȃ��悤�ɋC��t����

	/*Case3*/
	//�������C�e�B���O
	//float rim = 1.0f + dot(eyev, normal.xyz); //�����Ɩ@���̓��ς𖾂邳�ɕϊ�����
	//rim = pow(rim, 2) * 10.0f; //�X�y�L�����Ɠ����悤�ȏ�����K���ɍs���B
	//rim = saturate(rim); //rim���T�`�����[�g����
	//outDiffuse.gb += rim; //�ʏ�̐F�։��Z����B
	//outDiffuse.a = In.Diffuse.a;

	//float3 halfv = eyev + Light.Direction.xyz;
	//float alfaDel = -dot(halfv, normal.xyz);
	//alfaDel = saturate(alfaDel);
	//outDiffuse.a -= alfaDel;
}