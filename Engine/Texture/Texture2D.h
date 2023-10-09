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
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"

class Texture2D
{
public:
	Texture2D() = default;
	~Texture2D();

	static void Finalize();

private:
	//	ブレンドモード用タイプ変数
	BlendMode blendType = BlendMode::Normal;

	ID3D12Resource* resource[1] = {};
	ID3D12DescriptorHeap* SRVHeap = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource = nullptr;
	// ここにindexBufferViewとindex用のリソースを作る
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	ID3D12Resource* indexResource = nullptr;

public:

	static Microsoft::WRL::ComPtr<IDxcBlob> vertexShader;
	static Microsoft::WRL::ComPtr<IDxcBlob> pixelShader;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState[static_cast<int>(BlendMode::BlendCount)];

	uint32_t textureWidth = 0;
	uint32_t textureHeight = 0;

public:
	enum class AnchorPoint {
		Center,		// 中央
		LeftTop,	// 左上
		RightTop,	// 右上
		LeftBottom,	// 左下
		RightBottom,// 右下
	};

public:

	/// <summary>
	/// textureの読み込みと生成
	/// </summary>
	/// <param name="filePath">Resourceのパス</param>
	/// <param name="vsFileName">頂点シェーダーのパス</param>
	/// <param name="psFileName">ピクセルシェーダーのパス</param>
	void Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName);


	void SetAnchorPoint(AnchorPoint anchor);

private:
	void CreateDescriptor(const std::string& filePath);

	void CreateVertexResource(AnchorPoint anchor);

public:
	//void Draw(Matrix4x4 worldMat, Matrix4x4 viewProjectionMat, uint32_t color);
	void Draw(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color);

	static void TextureDraw(WorldTransform& worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Texture2D* texture);

	//BlendMode blend = BlendMode::Normal;
	//void SetBlend(BlendMode blend_);
	



};

