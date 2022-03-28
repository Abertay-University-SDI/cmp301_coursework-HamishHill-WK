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
		XMFLOAT3 direction[2]; 
		float specPower;
		float range;
		float cone;			
		XMFLOAT4 specDiffuse;	
		XMFLOAT3 position[3];
		XMFLOAT4 ambient[3];	
		XMFLOAT4 diffuse[3];	
		XMFLOAT3 atten[2];		
	};

public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, myLight* light, myLight* skylight, myLight* spotlight);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

