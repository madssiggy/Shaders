#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "polygon.h"
#include "field.h"
#include "camera.h"
#include "model.h"
#include "player.h"

CPolygon*		g_Polygon;
CField*			g_Field;
CCamera*		g_Camera;
CPlayer*		g_Player;

void CManager::Init()
{
	CRenderer::Init();
	CInput::Init();

	g_Polygon = new CPolygon();
	g_Polygon->Init();

	g_Field = new CField();
	g_Field->Init();

	g_Camera = new CCamera();
	g_Camera->Init();

	g_Player = new CPlayer();
	g_Player->Init();
}

void CManager::Uninit()
{

	g_Polygon->Uninit();
	delete g_Polygon;

	g_Field->Uninit();
	delete g_Field;

	g_Camera->Uninit();
	delete g_Camera;

	g_Player->Uninit();
	delete g_Player;


	CInput::Uninit();
	CRenderer::Uninit();

}

void CManager::Update()
{
	CInput::Update();


	g_Camera->Update();

	g_Field->Update();

	g_Polygon->Update();

	g_Player->Update();

}

void CManager::Draw()
{
	//CRenderer::Begin();

	//g_Camera->Draw();

	//LIGHT light;
	//light.Enable = true;
	//light.Direction = D3DXVECTOR4( 1.0f, -1.0f, 1.0f, 0.0f );
	//D3DXVec4Normalize( &light.Direction, &light.Direction );
	//light.Ambient = D3DXCOLOR( 0.1f, 0.1f, 0.1f, 1.0f );
	//light.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	//CRenderer::SetLight(light);

	//g_Field->Draw();

	//g_Player->Draw();

	//light.Enable = false;
	//CRenderer::SetLight(light);

	//g_Polygon->Draw();

	//CRenderer::End();

	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//-----------ライトをカメラとみなした行列を作成
	D3DXMatrixLookAtLH(&light.ViewMatrix, &D3DXVECTOR3(-10.0f, 10.0f, -10.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//-----------ライト用のプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 30.0f);

	CRenderer::SetLight(light);
	CRenderer::BeginDepth();//追加-シャドウバッファを深度バッファへ設定等
	CRenderer::SetViewMatrix(&light.ViewMatrix);//追加-カメラへライト用行列セット
	CRenderer::SetProjectionMatrix(&light.ProjectionMatrix);//追加-プロジェクションへライト用行列をセット
	g_Field->Draw();//追加-地面のシャドウバッファ作成 カメラから見た距離を作る
	g_Player->Draw();//同上 プレイヤーのシャドウバッファ作成

	CRenderer::Begin();//ここから本来の描画
	g_Camera->Draw();//本来のカメラ＆プロジェクション行列がセットされる
	g_Field->Draw(); //地面描画
	g_Player->Draw();//ドーナッツ描画
	light.Enable = false;
	CRenderer::SetLight(light);
	g_Polygon->Draw();//スプライト描画（深度バッファの内容）
	CRenderer::End();
}


