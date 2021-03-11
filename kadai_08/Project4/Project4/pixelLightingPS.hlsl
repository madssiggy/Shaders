#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{	
	//Normalize_the_normals_of_pixels (ピクセルの法線を正規化)
	float4 normal = normalize(In.Normal);
	

	//明るさを計算
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;


	//テクスチャのピクセル色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	

	//頂点色と明るさを乗算
	outDiffuse.rgb *= In.Diffuse.rgb * light;


	//αに明るさは関係ないので別計算
	outDiffuse.a *= In.Diffuse.a;


	//カメラからピクセルへのベクトル(視線ベクトル)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	/*Case1*/
	////光の反射ベクトルを計算
	//float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	//refv = normalize(refv); //正規化する
	////Calculation_of_specular_reflection
	//float specular = -dot(eyev, refv);
	////Set_mirror_reflection_distance
	//specular = saturate(specular);				//値をサチュレート
	//specular = pow(specular, 30);				//ここでは３０乗してみる
	//Add_specula_as_a_defuse
	//outDiffuse.rgb += specular;					// α値は弄らないように気を付ける

	/*Case2*/
	//ブリンフォンSample
	//ハーフベクトル作成
	float3 halfv = eyev + Light.Direction.xyz;
	halfv = normalize(halfv);
	
	//ハーフベクトルと法線の内積(スペキュラー)計算
	float specular = -dot(halfv, normal.xyz);
	//サチュレートする
	specular = saturate(specular);
	specular = pow(specular, 100);
	//Add_specula_as_a_defuse
	outDiffuse.rgb += specular;					// α値は弄らないように気を付ける

	/*Case3*/
	//リムライティング
	//float rim = 1.0f + dot(eyev, normal.xyz); //視線と法線の内積を明るさに変換する
	//rim = pow(rim, 2) * 10.0f; //スペキュラと同じような処理を適当に行う。
	//rim = saturate(rim); //rimをサチュレートする
	//outDiffuse.gb += rim; //通常の色へ加算する。
	//outDiffuse.a = In.Diffuse.a;

	//float3 halfv = eyev + Light.Direction.xyz;
	//float alfaDel = -dot(halfv, normal.xyz);
	//alfaDel = saturate(alfaDel);
	//outDiffuse.a -= alfaDel;
}