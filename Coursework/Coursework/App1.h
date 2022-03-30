// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"

#include "verManipShader.h"
#include "texShader.h"
#include "LightShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "verEdgeShader.h"
#include "horEdgeShader.h"
#include "verManipDepthShader.h"

#include "myLight.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

	XMFLOAT3 treePos1 = XMFLOAT3(0, 0, 0);
	XMFLOAT3 treePos2;
	XMFLOAT3 treePos3;	
	XMFLOAT3 groundPos;	

	XMFLOAT3 treePos1Def = XMFLOAT3(-27, -11, 30);
	XMFLOAT3 treePos2Def = XMFLOAT3(-44, -11, 38);
	XMFLOAT3 treePos3Def =XMFLOAT3(-50, -10, 14);	
	XMFLOAT3 groundPosDef =XMFLOAT3(-92, -20, -16);

	//modifiable lighting values for imgui
	//point light variables
	XMFLOAT3 pos;
	XMFLOAT4 pointdiffuse;	
	XMFLOAT3 attenu;	
	XMFLOAT4 ambi;	

	//skylight variables
	XMFLOAT3 skyPos;
	XMFLOAT3 direction;	
	XMFLOAT4 skydiffuse;
	XMFLOAT4 skyAmbi;

	//spotlight variables
	XMFLOAT3 spotDirection;
	XMFLOAT4 spotDiffuse;
	XMFLOAT3 spotPos;
	XMFLOAT3 spotAttenu;
	XMFLOAT4 spotAmbi;
	XMFLOAT4 specularColour;
	float specularPower;
	float spotRange;
	float spotCone;

	bool renderSphere;	//variable to control rendering sphere objects at the position of light sources
	bool edgeEnabled;	//variable to toggle post processing effect
	bool showNorms;		//variable to toggle displaying vertex normals

	//default values for modifiable variables
	//point
	XMFLOAT3 posDef = XMFLOAT3(-40.0f, -5.0f, 24.0f);
	XMFLOAT4 pointdiffuseDef = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 ambiDef = XMFLOAT4(0.1f, 0.1f, 0.0f, 0.1f);;
	XMFLOAT3 attenuDef = XMFLOAT3(0.5f, 0.25f, 0.0f);

	//sky
	XMFLOAT4 skydiffuseDef = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f);
	XMFLOAT4 skyAmbiDef = XMFLOAT4(0.0f, 0.1f, 0.5f, 0.5f);
	XMFLOAT3 skyPosDef = XMFLOAT3(-10.0f, 15.0f, 30.0f);
	XMFLOAT3 directionDef = XMFLOAT3(-0.85f, -0.5f, 0.0f);

	//spot
	XMFLOAT4 spotDiffuseDef = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);;
	XMFLOAT3 spotAttenuDef = XMFLOAT3(0.2f, 0.05f, 0.0f);;
	XMFLOAT3 spotPosDef = XMFLOAT3(-43.0f, 25.0f, 25.0f);
	XMFLOAT3 spotDirectionDef = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT4 spotAmbiDef = XMFLOAT4(0.0f, 0.0f, 0.1f, 0.1f);
	XMFLOAT4 specularColourDef = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.2f);
	float specularPowerDef = 0.1f;
	float spotRangeDef = 100.0f;
	float spotConeDef = 1.0f;

	void wireFrameRender();

	void depthRender();
	void firstRender();
	void scndRender();

	void verticalEdge();
	void horizontalEdge();

	void finalPass();

private:
	verManipShader* groundShader = 0;
	texShader* textureShader = 0;
	LightShader* lightShader = 0;
	DepthShader* depthShader = 0;
	ShadowShader* shadowShader =0;
	verEdgeShader* verEdgeShader1 =0;
	horEdgeShader* horEdgeShader1 =0;
	verManipDepthShader* verManipDepthShader1 = 0;

	PlaneMesh* ground = 0;
	PlaneMesh* shadowGround =0 ;
	AModel* model[4] = { 0,0,0,0 };

	SphereMesh* pointLightSphere = 0;
	SphereMesh* spotLightSphere = 0;	
	SphereMesh* skyLightSphere = 0;

	myLight* skylight = 0;
	myLight* pointlight = 0;
	myLight* spotlight = 0;

	OrthoMesh* orthomesh = 0;
	RenderTexture* renderTexture = 0;
	RenderTexture* horizEdgeTexture = 0;
	RenderTexture* vertEdgeTexture = 0;

	ShadowMap* shadowMap = 0;
};

#endif