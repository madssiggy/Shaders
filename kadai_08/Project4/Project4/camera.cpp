
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"


void CCamera::Init()
{

	m_Position = D3DXVECTOR3( 0.0f, 3.0f, -5.0f );
	m_Target = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

}


void CCamera::Uninit()
{

}


void CCamera::Update()
{

	if (CInput::GetKeyPress('H'))
		m_Position.x -= 0.1f;

	if (CInput::GetKeyPress('K'))
		m_Position.x += 0.1f;

	if (CInput::GetKeyPress('U'))
		m_Position.z += 0.1f;

	if (CInput::GetKeyPress('J'))
		m_Position.z -= 0.1f;

}


void CCamera::Draw()
{

	//ビューマトリクス設定
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CRenderer::SetViewMatrix(&viewMatrix);


	//プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&projectionMatrix);


	CRenderer::SetCameraPosition(m_Position);

}

