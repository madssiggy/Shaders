
#include "main.h"
#include "renderer.h"
#include "field.h"



void CField::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3( -10.0f, 0.0f, 10.0f );
	vertex[0].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[0].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[0].TexCoord = D3DXVECTOR2( 0.0f, 0.0f );

	vertex[1].Position = D3DXVECTOR3( 10.0f, 0.0f, 10.0f );
	vertex[1].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[1].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = D3DXVECTOR2( 10.0f, 0.0f );

	vertex[2].Position = D3DXVECTOR3( -10.0f, 0.0f, -10.0f );
	vertex[2].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[2].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[2].TexCoord = D3DXVECTOR2( 0.0f, 10.0f );

	vertex[3].Position = D3DXVECTOR3( 10.0f, 0.0f, -10.0f );
	vertex[3].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[3].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[3].TexCoord = D3DXVECTOR2( 10.0f, 10.0f );


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );


	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
											"asset/texture/field004.jpg",
											NULL,
											NULL,
											&m_Texture,
											NULL);
	assert( m_Texture );

	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
											"asset/texture/normalmap.png",
											NULL,
											NULL,
											&m_TextureNormal,
											NULL);



	m_Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Scale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "normalMappingVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "normalMappingPS.cso");

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shadowMappingVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "shadowMappingPS.cso");
}


void CField::Uninit()
{

	m_VertexBuffer->Release();
	m_Texture->Release();
	m_TextureNormal->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void CField::Update()
{

}


void CField::Draw()
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

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	CRenderer::SetMaterial( material );

	// �e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureNormal);

	//�V���h�E�o�b�t�@���Z�b�g
	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetShadowDepthTexture();//-�ǉ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);//-�ǉ�

	// �v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// �|���S���`��
	CRenderer::GetDeviceContext()->Draw( 4, 0 );

}