#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"
#include <Engine/Log.h>
#include <Engine/Engine.h>

//	便利なtmpみたいなやつ
//decltype(GraphicsPipeline::rootSignature) GraphicsPipeline::rootSignature;
//decltype(GraphicsPipeline::graphicsPipelineState) GraphicsPipeline::graphicsPipelineState;
//decltype(GraphicsPipeline::SRVHeap) GraphicsPipeline::SRVHeap;

GraphicsPipeline::GraphicsPipeline() {
	//BlendMode type = BlendMode::None;
	//for (uint16_t i = 0; i < static_cast<int>(BlendMode::BlendCount); i++)
	//{
	//	CreateGraphicsPipeline(type);
	//	type = static_cast<BlendMode>(i);
	//}
	
}


GraphicsPipeline* GraphicsPipeline::GetInstance() {
	static GraphicsPipeline Instance;
	return &Instance;
}

ID3D12PipelineState* GraphicsPipeline::CreateGraphicsPipeline(ID3D12RootSignature* rootSignature, IDxcBlob* vertexShader, IDxcBlob* pixelShader, BlendMode blendType) {

	ID3D12PipelineState* graphicsPipelineState = nullptr;

#pragma region InputLayout

	D3D12_INPUT_ELEMENT_DESC inputElementDesc[3] = {};
	inputElementDesc[0].SemanticName = "POSITION";
	inputElementDesc[0].SemanticIndex = 0;
	inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDesc[1].SemanticName = "TEXCOORD";
	inputElementDesc[1].SemanticIndex = 0;
	inputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDesc[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDesc[2].SemanticName = "NORMAL";
	inputElementDesc[2].SemanticIndex = 0;
	inputElementDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDesc[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC layoutDesc{};
	layoutDesc.pInputElementDescs = inputElementDesc;
	layoutDesc.NumElements = _countof(inputElementDesc);

#pragma endregion

#pragma region BlendState

	//	ブレンドモードの設定
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	switch (blendType) {
	case BlendMode::None:
		blendDesc.RenderTarget[0].BlendEnable = false;
		break;
	case BlendMode::Normal:
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	case BlendMode::Add:
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::Subtract:
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::Multily:
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;
	case BlendMode::Screen:
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::Dark:
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_MIN;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::Light:
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	}

#pragma endregion

#pragma region RasterizerState

	//	ラスタライザの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//	裏面を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//	塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.DepthClipEnable = true;

#pragma endregion

#pragma region PSOの生成
	//	PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature;	//	RootSignature
	graphicsPipelineStateDesc.InputLayout = layoutDesc;	//	InputLayout
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleDesc.Quality = 0;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	graphicsPipelineStateDesc.VS = {
		vertexShader->GetBufferPointer(),
		vertexShader->GetBufferSize()
	};
	graphicsPipelineStateDesc.PS = {
		pixelShader->GetBufferPointer(),
		pixelShader->GetBufferSize()
	};

	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//	DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//	Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//	書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//	比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//	DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

#pragma endregion

	//	実際に生成
	HRESULT hr = Engine::GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));

	return graphicsPipelineState;
}

ID3D12RootSignature* GraphicsPipeline::CreateRootSignature(D3D12_ROOT_PARAMETER* rootParameter, UINT num) {
	//	ルートシグネチャーがあれば渡す
	//if (GraphicsPipeline::GetInstance()->rootSignature) {
	//	return GraphicsPipeline::GetInstance()->rootSignature;
	//}

#pragma region RootSignature

	//	ルートシグネチャーの作成
	D3D12_ROOT_SIGNATURE_DESC sigDesc{};
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	sigDesc.pParameters = rootParameter;
	sigDesc.NumParameters = num;

	D3D12_STATIC_SAMPLER_DESC staticSampler[1] = {};
	staticSampler[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSampler[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler[0].ShaderRegister = 0;
	staticSampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	sigDesc.pStaticSamplers = staticSampler;
	sigDesc.NumStaticSamplers = _countof(staticSampler);

	//	シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//	バイナリを元に生成
	ID3D12RootSignature* rootSignature = nullptr;
	hr = Engine::GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
	if (errorBlob)
	{
		errorBlob->Release();
	}
	signatureBlob->Release();

	return rootSignature;

#pragma endregion
}

IDxcBlob* GraphicsPipeline::CreateVSShader(const std::string& vsFileName)
{
	IDxcBlob* vertexShader;
	vertexShader = ShaderManager::GetInstance()->CompileShader(ConvertString(vsFileName), L"vs_6_0");
	return vertexShader;
}

IDxcBlob* GraphicsPipeline::CreatePSShader(const std::string& psFileName)
{
	IDxcBlob* pixelShader;
	pixelShader = ShaderManager::GetInstance()->CompileShader(ConvertString(psFileName), L"ps_6_0");
	return pixelShader;
}

ID3D12PipelineState* GraphicsPipeline::CreateGraphicsPipelineLine(ID3D12RootSignature* rootSignature)
{
	ID3D12PipelineState* graphicsPipelineStateLine = nullptr;

#pragma region InputLayout

	D3D12_INPUT_ELEMENT_DESC inputElementDesc[1] = {};
	inputElementDesc[0].SemanticName = "POSITION";
	inputElementDesc[0].SemanticIndex = 0;
	inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC layoutDesc{};
	layoutDesc.pInputElementDescs = inputElementDesc;
	layoutDesc.NumElements = _countof(inputElementDesc);

#pragma endregion

#pragma region BlendState

	//	ブレンドモードの設定
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

#pragma endregion

#pragma region RasterizerState

	//	ラスタライザの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//	カリング設定
	//	両面を描画する
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//	塗りつぶさない
	//rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	rasterizerDesc.DepthClipEnable = true;

#pragma endregion

#pragma region シェーダーの生成

	IDxcBlob* vertexShader = GraphicsPipeline::GetInstance()->CreateVSShader("./Resources/Shader/Line.VS.hlsl");
	IDxcBlob* pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader("./Resources/Shader/Line.PS.hlsl");

#pragma endregion

#pragma region PSOの生成
	//	PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature;	//	RootSignature
	graphicsPipelineStateDesc.InputLayout = layoutDesc;	//	InputLayout
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleDesc.Quality = 0;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	graphicsPipelineStateDesc.VS = {
		vertexShader->GetBufferPointer(),
		vertexShader->GetBufferSize()
	};
	graphicsPipelineStateDesc.PS = {
		pixelShader->GetBufferPointer(),
		pixelShader->GetBufferSize()
	};

	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//	DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//	Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//	書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//	比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//	DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

#pragma endregion

	//	実際に生成
	HRESULT hr = Engine::GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineStateLine));
	assert(SUCCEEDED(hr));

	return graphicsPipelineStateLine;
}
