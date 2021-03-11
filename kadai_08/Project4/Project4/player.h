#pragma once

class CPlayer
{

private:
	D3DXVECTOR3	m_Position;
	D3DXVECTOR3	m_Rotation;
	D3DXVECTOR3	m_Scale;

	CModel* m_Model;

	//シェーダー追加に必要！！
	ID3D11VertexShader*		m_VertexShader;
	ID3D11PixelShader*		m_PixelShader;
	ID3D11InputLayout*		m_VertexLayout;

	ID3D11ShaderResourceView* m_TextureEnv = NULL;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};