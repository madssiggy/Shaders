#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "player.h"
#include "input.h"

void CPlayer::Init()
{
	m_Model = new CModel();
	m_Model->Load( "asset\\model\\torus.obj" );

	m_Position = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Scale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	//���܂��Ճe�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), "asset/texture/earthenvmap.png", NULL, NULL, &m_TextureEnv, NULL);
	assert(m_TextureEnv);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "envMappingVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "envMappingPS.cso");

	CRenderer::CreateVertexShader(&m_VertexShader,		&m_VertexLayout, "shadowMappingVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "shadowMappingPS.cso");
}

void CPlayer::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_TextureEnv->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void CPlayer::Update()
{
	if (CInput::GetKeyPress('A'))
		m_Position.x -= 0.1f;

	if (CInput::GetKeyPress('D'))
		m_Position.x += 0.1f;

	if (CInput::GetKeyPress('W'))
		m_Position.z += 0.1f;

	if (CInput::GetKeyPress('S'))
		m_Position.z -= 0.1f;


	if (CInput::GetKeyPress('R'))
		m_Rotation.x -= 0.1f;
	if (CInput::GetKeyPress('F'))
		m_Rotation.x += 0.1f;

	if (CInput::GetKeyPress('Q'))
		m_Rotation.y -= 0.1f;
	if (CInput::GetKeyPress('E'))
		m_Rotation.y += 0.1f;

}

void CPlayer::Draw()
{
	//���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	//���}�b�v�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureEnv);

	//�V���h�E�o�b�t�@�e�N�X�`�����Z�b�g
	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetShadowDepthTexture();//-�ǉ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);//-�ǉ�

	m_Model->Draw();
}


