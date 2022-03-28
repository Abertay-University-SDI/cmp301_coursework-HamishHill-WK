#pragma once

#include "DXF.h"
#include "myLight.h"

using namespace std;
using namespace DirectX;

class verManipShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT3 direction;
		int norms;
		XMFLOAT3 position;
		XMFLOAT3 skypos;
		XMFLOAT3 atten;
		XMFLOAT3 pad;
		XMFLOAT4 ambient;
		XMFLOAT4 skyAmbient;
		XMFLOAT4 diffuse;
		XMFLOAT4 skyDiffuse;
	};

	struct spotLightBuffer
	{
		XMFLOAT3 spotDirection;
		float specPower;
		float range;
		float cone;
		XMFLOAT4 specDiffuse;
		XMFLOAT3 spotPosition;
		XMFLOAT4 spotAmbient;
		XMFLOAT4 spotDiffuse;
		XMFLOAT3 spotAtten;
	};

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
	};

public:
	verManipShader(ID3D11Device* device, HWND hwnd);
	~verManipShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* heightTex, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap, myLight* skylight, myLight* pointlight, myLight* spotlight, bool normals);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleState1;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* spotlightBuffer;
	//ID3D11Buffer* timeBuffer;

};

