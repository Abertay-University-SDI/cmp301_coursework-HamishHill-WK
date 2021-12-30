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
	textureMgr->loadTexture(L"snowTexture", L"res/snowTexture.png");
	textureMgr->loadTexture(L"treeTex2", L"res/treeTex2.png");

	model = new AModel(renderer->getDevice(), "res/por_tree.obj");

	ground = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	groundShader = new verManipShader(renderer->getDevice(), hwnd);
	textureShader = new texShader(renderer->getDevice(), hwnd);

	skylight = new Light();
	skylight->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	skylight->setDirection(0.7f, -0.7f, 0.0f);

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
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//XMMATRIX scaleMatrix = XMMatrixScaling(0.75f, 0.75f, 0.75f);
//	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	ground->sendData(renderer->getDeviceContext());
	groundShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"myHeightMap"), textureMgr->getTexture(L"snowTexture"), skylight);
	groundShader->render(renderer->getDeviceContext(), ground->getIndexCount());

	// Render model
	worldMatrix = XMMatrixTranslation(50.f, 10.f, 50.f);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	model->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), skylight);
	textureShader->render(renderer->getDeviceContext(), model->getIndexCount());


	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
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

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

