#include <vector>
#include <map>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "GameObjects.h"
#include "VerticeShapes.h"
#include "Camera.h"

#include <Keyboard.h>
#include "imgui\\imgui.h"
#include "imgui\\imgui_impl_dx11.h"
#include "imgui\\imgui_impl_win32.h"
#include <windows.h>
#include <chrono>
#include "resource.h"

HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;

struct ConstantBuffer
{
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mWorldInverse;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;

	XMFLOAT4 vCameraPos;
	float numOfLights;
	float time;
	XMFLOAT2 padding;
};

class MainRender
{
private:	
	map<wstring, ID3D11VertexShader*>			mVertexShaders;
	map<wstring, ID3D11PixelShader*>			mPixelShaders;
	map<MeshType, ID3D11Buffer*>				    mVertexBuffers;
	map<MeshType, ID3D11Buffer*>				    mIndexBuffers;
	map<string, ID3D11Buffer*>				mInstanceBuffers;
	map<wstring, ID3D11ShaderResourceView*>	mTextures;
	map<wstring, ID3D11InputLayout*>			mLayouts;
	int													mWidth;
	int													mHeight;
	D3D_DRIVER_TYPE										mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL									mFeatureLevel = D3D_FEATURE_LEVEL_11_1;
	HWND												mWnd = nullptr;
	ID3D11Device*										mD3dDevice = nullptr;
	ID3D11Device1*										mD3dDevice1 = nullptr;
	ID3D11DeviceContext*								mImmediateContext = nullptr;
	ID3D11DeviceContext1*								mImmediateContext1 = nullptr;
	IDXGISwapChain*										mSwapChain = nullptr;
	IDXGISwapChain1*									mSwapChain1 = nullptr;
	ID3D11RenderTargetView*								mRenderTargetView = nullptr;
	ID3D11Texture2D*									mDepthStencil = nullptr;
	ID3D11DepthStencilView*								mDepthStencilView = nullptr;
	ID3D11DepthStencilState*							mDepthStencilLessThanEqual = nullptr;
	ID3D11RasterizerState*								mRasteriserStateNoCull = nullptr;
	ID3D11RasterizerState*								mRasteriserStateBackCull = nullptr;
	ID3D11BlendState*									mBlendStateAlpha = nullptr;
	ID3D11BlendState*									mBlendStateDisable = nullptr;
	ID3D11Buffer*										mConstantBuffer = nullptr;
	D3D11_BUFFER_DESC									mBufferDesc;
	ID3D11SamplerState*									mSampler = nullptr;
	ConstantBuffer										cb1;


	void SetCamera(const Camera* camera);
	HRESULT VertexIndex(const VerticeShapes& shape);
	
	HRESULT Shader(const VerticeShapes& shape);
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT CompileShaderFromFile(const WCHAR* const szFileName, const LPCSTR szEntryPoint, const LPCSTR szShaderModel, ID3DBlob** const ppBlobOut) const;
	bool Resetclicked ;


	vector<Camera> cameras;
	vector<GameObject> entities;
	float timescale;
	
	int width;
	int height;
	bool isExit;
	bool isRocketLaunched;
	bool isEngineOn;
	bool isReset;
	
	unique_ptr<Keyboard> keyboard{};

	Camera* activeCam;
	

	
	GameObject* terrain;
	

	void CheckInput(const float& dt);
	void CreateCameras();
	void CreateVoxels();
	
	
	
	void SetEntityPointers();
	

public:
	int cam = 0;
	MainRender();
	MainRender(const int& width, const int& height);
	~MainRender();

	const int& Width() const;
	const int& Height() const;

	void SetWindow(const HWND& Wnd);
	HRESULT InitDXDevice();
	HRESULT Render(const vector<GameObject>& entities, const Camera* const camera, const float& time);
	void CleanUpDevice();

	MainRender& operator=(const MainRender&) = delete;
	MainRender(const MainRender&) = delete;

	

	void setReset(bool _st) 
	{
		Resetclicked = _st;
	}
	bool getClicked()
	{
		return Resetclicked;
	}
	

	void InitScene();
	void ResetScene();
	void Update(const float& dt);
	const Camera* const ActiveCamera() const;
	const vector<GameObject>& Entities() const;
	const bool& ExitApplication() const;
	void camsetting(int ch);

	
};