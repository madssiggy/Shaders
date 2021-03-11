#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //�s��v�Z�p
	wvp = mul(World, View);
	wvp = mul(wvp, Projection); //���_�ϊ��s��쐬
	Out.Position = mul(In.Position, wvp); //���_��ϊ����ďo��

	float4 worldNormal, normal; //�@�������[���h�s��ŉ�]
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal); //�ϊ������@���𐳋K��
	Out.Normal = worldNormal; //���[���h�ϊ������@�����o��

	//���̑��o��
	Out.Diffuse = In.Diffuse;
	Out.TexCoord = In.TexCoord;

	//���[���h�ϊ��������_���W���o��
	Out.WorldPosition = mul(In.Position, World);
}