// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Initalise scene variables.
	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	//textureMgr->loadTexture(L"height", L"res/height.png");
	textureMgr->loadTexture(L"myHeightMap", L"res/myHeightMap.png");	
	textureMgr->loadTexture(L"height", L"res/height.png");
	textureMgr->loadTexture(L"snowTexture", L"res/snowTexture.png");
	textureMgr->loadTexture(L"treeTex2", L"res/treeTex2.png");

	for(int i = 0; i < 4; i++)
	model[i] = new AModel(renderer->getDevice(), "res/por_tree.obj");

	ground = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	pointLightSphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	groundShader = new verManipShader(renderer->getDevice(), hwnd);
	textureShader = new texShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);

	orthomesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth , screenHeight);
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	renderSphere = false;

	skylight = new myLight();
	//skylight->setType(0);
	skylight->setDiffuseColour(0.0f, 0.35f, 0.6f, 0.2f);
//	skylight->setAmbientColour(0.0f, 0.0f, 0.1f, 0.1f);
	skylight->setDirection(0.7f, -0.7f, 0.0f);

	pointlight = new myLight();
	//pointlight->setType(2);
	pointlight->setAmbientColour(0.1f, 0.1f, 0.0f, 0.1f);
	pointlight->setDiffuseColour(1.0f, 0.75f, 0.0f, 1.0f);
	pointlight->setPosition(25.0f, 10.0f, 25.0f);
	pointlight->setAtten(0.5f, 0.25f, 0.0f);

	pos.x = 50.0f;
	pos.y = 11.0f;
	pos.z = 50.0f;

	direction.x = 0.5;
	direction.y = -0.7;
	direction.z = 0.0f;
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (ground)
	{
		delete ground;
		ground = 0;
	}

	if (renderTexture)
	{
		delete renderTexture;
		renderTexture = 0;
	}

	if (orthomesh)
	{
		delete orthomesh;
		orthomesh = 0;
	}

	if (skylight)
	{
		delete skylight;
		skylight = 0;
	}

	if (pointlight)
	{
		delete  pointlight;
		pointlight = 0;
	}

	//if (model[4])
	//{
	//	delete model[4];
	//	model[4] = 0;
	//}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}

	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}

	if (groundShader)
	{
		delete groundShader;
		groundShader = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	pointlight->setPosition(pos.x, pos.y, pos.z);
	skylight->setDirection(direction.x, direction.y, direction.z);

	// Clear the scene. (default blue colour)
	firstRender();

	scndRender();

	return true;
}

void App1::firstRender()
{
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//XMMATRIX scaleMatrix = XMMatrixScaling(0.75f, 0.75f, 0.75f);
//	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	//worldMatrix = XMMatrixRotationZ(45);
	ground->sendData(renderer->getDeviceContext());
	groundShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"myHeightMap"), textureMgr->getTexture(L"snowTexture"), skylight, pointlight);
	groundShader->render(renderer->getDeviceContext(), ground->getIndexCount());
	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), pointlight);
	//lightShader->render(renderer->getDeviceContext(), ground->getIndexCount());

	// Render model
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	worldMatrix = XMMatrixTranslation(30, 5, 30);
	model[0]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), pointlight, skylight);
	lightShader->render(renderer->getDeviceContext(), model[0]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-30, 0, -30);

	worldMatrix = XMMatrixTranslation(45, 0, 30);
	model[1]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), pointlight, skylight);
	lightShader->render(renderer->getDeviceContext(), model[1]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-45, 0, -30);

	worldMatrix = XMMatrixTranslation(30, 0, 45);
	model[0]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), pointlight, skylight);
	lightShader->render(renderer->getDeviceContext(), model[2]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-30, 0, -45);

	worldMatrix = XMMatrixTranslation(60, 0, 60);
	model[3]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), pointlight, skylight);
	lightShader->render(renderer->getDeviceContext(), model[3]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-60, -5, -60);

	XMMATRIX scaleMatrix1 = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix1);

	if (renderSphere)
	{
		worldMatrix = XMMatrixTranslation(pointlight->getPosition().x, pointlight->getPosition().y, pointlight->getPosition().z);
		pointLightSphere->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), pointlight, skylight);
		lightShader->render(renderer->getDeviceContext(), pointLightSphere->getIndexCount());
	}

	// Present the rendered scene to the screen.
	renderer->setBackBufferRenderTarget();

}

void App1::scndRender()
{
	//renderTexture->setRenderTarget(renderer->getDeviceContext());
	//renderTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 0.0f, 0.0f, 1.0f);

	// Get matrices
	camera->update();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();




	// Render normal scene, with light shader set.
	
	//ground->sendData(renderer->getDeviceContext());
	//groundShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"myHeightMap")); // , textureMgr->getTexture(L"snowTexture"), skylight);
	//groundShader->render(renderer->getDeviceContext(), ground->getIndexCount());

	//greyShader1->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bunny"), light, input->getMouseX(), input->getMouseY());
	//greyShader1->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	renderer->setBackBufferRenderTarget();

	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);

	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	orthomesh->sendData(renderer->getDeviceContext());

	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthomesh->getIndexCount());

	renderer->setZBuffer(true);

	// Render GUI
	gui();

	//renderer->wo

	// Present the rendered scene to the screen.
	renderer->endScene();
}



void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	ImGui::Checkbox("Show Light Sources", &renderSphere);

	ImGui::SliderFloat("x pos", &pos.x, 1.0f, 100.0f);
	ImGui::SliderFloat("y pos", &pos.y, 1.0f, 100.0f);
	ImGui::SliderFloat("z pos", &pos.z, 1.0f, 100.0f);	
	
	ImGui::SliderFloat("Direction x", &direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Direction y", &direction.y, -1.0f, 1.0f);
	ImGui::SliderFloat("Direction z", &direction.z, -1.0f, 1.0f);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

