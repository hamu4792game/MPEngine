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


// ブレンドモード
enum BlendMode {
	None,   //!< ブレンドなし
	Normal, //!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	Add,    //!< 加算。Src * SrcA + Dest * 1
	Subtract, //!< 減算。Dest * 1 - Src * SrcA
	Multily,  //!< 乗算。Src * 0 + Dest * Src
	Screen,   //!< スクリーン。Src * (1 - Dest) + Dest * 1
	Dark,	//	比較暗
	Light,	//	比較明
};

class Texture2D
{
public:
	Texture2D() = default;
	~Texture2D() = default;

	void Finalize();

private:
	ID3D12Resource* resource[1] = {};
	ID3D12DescriptorHeap* SRVHeap = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource = nullptr;
	// ここにindexBufferViewとindex用のリソースを作る
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	ID3D12Resource* indexResource = nullptr;

public:

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShader = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

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

	BlendMode blend = BlendMode::Normal;
	void SetBlend(BlendMode blend_);
	



};

