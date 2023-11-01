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
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"
class Particle {
public:
	Particle() = default;
	~Particle();
	static void Finalize();
public:
	//	モデルデータ構造体
	ModelData modelData;
	//	ブレンドモード用タイプ変数
	BlendMode blendType = BlendMode::Normal;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVHeap = nullptr;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState[static_cast<int>(BlendMode::BlendCount)];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

protected:

	Microsoft::WRL::ComPtr<ID3D12Resource> resource[1] = {};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;

	static Microsoft::WRL::ComPtr<IDxcBlob> vertexShader;
	static Microsoft::WRL::ComPtr<IDxcBlob> pixelShader;


	//	depthStencilResourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;
	//	DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;

private:
	uint16_t kNumInstance = 1;
	// Instancing用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
public:
	/// <summary>
	/// モデルデータのロード
	/// </summary>
	/// <param name="filePath">objファイルのパス</param>
	/// <param name="vsFileName">vsShaderのパス</param>
	/// <param name="psFileName">psShaderのパス</param>
	void Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName, const std::string& textureFilePath, uint16_t num = 1);

private:
	void CreateInstancingResource();
	void CreateDescriptor(const std::string& filePath, const std::string& textureFilePath);

	void CreateVertexResource();

	//void CreateDescriptor(const std::string& filePath, const std::string& texturePath);

public:
	static void ParticleDraw(WorldTransform* worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Particle* particle);

};