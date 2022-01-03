#pragma once

#include "DXF.h"
#include "myLight.h"

using namespace std;
using namespace DirectX;

class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
/*		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT3 position[2];
		float padding;
		XMFLOAT4 atten[2];	*/	
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
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, myLight* light, myLight* skylight);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

