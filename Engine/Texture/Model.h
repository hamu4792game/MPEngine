#pragma once

//	DirextXtex
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include <vector>

#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include "math/Vector4.h"
#include "Engine/Base/ConstantBuffer.h"
#include "math/Matrix4x4.h"
#include "Engine/Manager/TextureManager.h"
#include <wrl.h>
#include <Engine/WorldTransform/WorldTransform.h>

class Model
{
public:
	Model() = default;
	~Model() = default;

public:

	ModelData modelData;

	ID3D12DescriptorHeap* SRVHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> resource[1] = {};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShader = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShader = nullptr;


	//	depthStencilResourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;
	//	DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;

private:

public:
	
	/// <summary>
	/// モデルデータのロード
	/// </summary>
	/// <param name="filePath">objファイルのパス</param>
	/// <param name="vsFileName">vsShaderのパス</param>
	/// <param name="psFileName">psShaderのパス</param>
	void Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName);

	void Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName, const std::string& texturePath);

private:
	
	void CreateDescriptor(const std::string& filePath);

	void CreateDescriptor(const std::string& filePath, const std::string& texturePath);

	void CreateVertexResource();

public:
	static void ModelDraw(WorldTransform& worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Model* model);

};
