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

class GraphicsPipeline
{
public:
	GraphicsPipeline() = default;
	~GraphicsPipeline() = default;

	//	シングルトンインスタンス
	static GraphicsPipeline* GetInstance();

public:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShader = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShader = nullptr;

	static ID3D12DescriptorHeap* SRVHeap;

public:
	//	GraphicsPipelineの生成とget
	Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateGraphicsPipeline();

	//	RootSignatureの生成とget
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature(D3D12_ROOT_PARAMETER* rootParameter, UINT num);
	//	vertexshaderの生成とget
	Microsoft::WRL::ComPtr<IDxcBlob> CreateVSShader(const std::string& vsFileName);
	//	pixelshaderの生成とget
	Microsoft::WRL::ComPtr<IDxcBlob> CreatePSShader(const std::string& vsFileName);

};

