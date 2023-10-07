#pragma once
//	DirextXtex
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

#include <wrl.h>

//	GraphicsPipelineStateDesc
struct PSD {
	D3D12_ROOT_SIGNATURE_DESC sigDesc{};
	D3D12_DESCRIPTOR_RANGE range[2] = {};
	D3D12_ROOT_PARAMETER rootParameter[1] = {};
	D3D12_STATIC_SAMPLER_DESC staticSampler[1] = {};
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[3] = {};


};

// ブレンドモード
enum class BlendMode {
	None,   // ブレンドなし
	Normal, // 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	Add,    // 加算。Src * SrcA + Dest * 1
	Subtract, // 減算。Dest * 1 - Src * SrcA
	Multily,  // 乗算。Src * 0 + Dest * Src
	Screen,   // スクリーン。Src * (1 - Dest) + Dest * 1
	Dark,	// 比較暗
	Light,	// 比較明

	BlendCount,
};

class GraphicsPipeline
{
public:
	GraphicsPipeline();
	~GraphicsPipeline() = default;

	//	シングルトンインスタンス
	static GraphicsPipeline* GetInstance();

public:
	//	GraphicsPipelineの生成とget
	ID3D12PipelineState* CreateGraphicsPipeline(ID3D12RootSignature* rootSignature, IDxcBlob* vertexShader, IDxcBlob* pixelShader, BlendMode blendType = BlendMode::Normal);
	//	RootSignatureの生成とget
	ID3D12RootSignature* CreateRootSignature(D3D12_ROOT_PARAMETER* rootParameter, UINT num);
	//	vertexshaderの生成とget
	IDxcBlob* CreateVSShader(const std::string& vsFileName);
	//	pixelshaderの生成とget
	IDxcBlob* CreatePSShader(const std::string& vsFileName);

	//	線用の生成
	ID3D12PipelineState* CreateGraphicsPipelineLine(ID3D12RootSignature* rootSignature);

};

