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
		float pad;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 position;
		int type;
		XMFLOAT3 atten;
		float pad1;
	};

public:
	verManipShader(ID3D11Device* device, HWND hwnd);
	~verManipShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* heightTex, ID3D11ShaderResourceView* texture, Light* light, myLight* spotlight);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;	
	ID3D11SamplerState* sampleState1;
	ID3D11Buffer* lightBuffer;
	//ID3D11Buffer* timeBuffer;

};

