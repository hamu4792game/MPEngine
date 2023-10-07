#include "Model.h"
#include "Engine/Manager/TextureManager.h"
#include <numbers>

#include "externals/imgui/imgui.h"

//	便利なtmpみたいなやつ
decltype(Model::rootSignature) Model::rootSignature;
decltype(Model::graphicsPipelineState) Model::graphicsPipelineState;
decltype(Model::vertexShader) Model::vertexShader;
decltype(Model::pixelShader) Model::pixelShader;

Model::~Model() {
	
	if (SRVHeap) {
		SRVHeap->Release();
		SRVHeap.Reset();
	}
	if (depthStencilResource) {
		depthStencilResource->Release();
		depthStencilResource.Reset();
	}
	if (vertexResource) {
		vertexResource->Release();
		vertexResource.Reset();
	}
	if (resource[0]) {
		resource[0]->Release();
		resource[0].Reset();
	}

}

void Model::Finalize()
{
	if (rootSignature) {
		rootSignature->Release();
		rootSignature.Reset();
	}
	for (uint16_t i = 0; i < static_cast<uint16_t>(BlendMode::BlendCount); i++) {
		if (graphicsPipelineState[i]) {
			graphicsPipelineState[i]->Release();
			graphicsPipelineState[i].Reset();
		}
	}
	if (vertexShader) {
		vertexShader->Release();
		vertexShader.Reset();
	}
	if (pixelShader) {
		pixelShader->Release();
		pixelShader.Reset();
	}
}

void Model::Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName)
{
	//	モデルのロードとデスクリプタヒープの生成
	CreateDescriptor(filePath);
	
	//	ピクセルシェーダーのコンパイルがなぜかできないため、緊急措置を行っている
	//pixelShader = ShaderManager::GetInstance()->CompileShader(ConvertString(psFileName), L"ps_6_0");
	vertexShader = GraphicsPipeline::GetInstance()->CreateVSShader(vsFileName);
	pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader(psFileName);

	//	頂点データの生成
	CreateVertexResource();

	D3D12_DESCRIPTOR_RANGE range[1] = {};
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 1;	//	必要な数
	range[0].RegisterSpace = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[4] = {};
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].Descriptor.ShaderRegister = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].DescriptorTable.pDescriptorRanges = range;
	rootParameter[0].DescriptorTable.NumDescriptorRanges = _countof(range);

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter[1].Descriptor.ShaderRegister = 0;

	rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[2].Descriptor.ShaderRegister = 1;

	rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[3].Descriptor.ShaderRegister = 2;


	if (!rootSignature) {
		rootSignature = GraphicsPipeline::GetInstance()->CreateRootSignature(rootParameter, 4);
	}
	for (uint16_t i = 0; i < static_cast<uint16_t>(BlendMode::BlendCount); i++) {
		if (!graphicsPipelineState[i]) {
			graphicsPipelineState[i] = GraphicsPipeline::GetInstance()->CreateGraphicsPipeline(rootSignature.Get(), vertexShader.Get(), pixelShader.Get(), static_cast<BlendMode>(i));
		}
	}
}

void Model::Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName, const std::string& texturePath)
{
	//	モデルのロードとデスクリプタヒープの生成
	CreateDescriptor(filePath, texturePath);

	vertexShader = GraphicsPipeline::GetInstance()->CreateVSShader(vsFileName);
	//	ピクセルシェーダーのコンパイルがなぜかできないため、緊急措置を行っている
	//pixelShader = ShaderManager::GetInstance()->CompileShader(ConvertString(psFileName), L"ps_6_0");
	//pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader(psFileName);
	pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader(psFileName);

	//	頂点データの生成
	CreateVertexResource();

	D3D12_DESCRIPTOR_RANGE range[1] = {};
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 1;	//	必要な数
	range[0].RegisterSpace = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[4] = {};
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].Descriptor.ShaderRegister = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].DescriptorTable.pDescriptorRanges = range;
	rootParameter[0].DescriptorTable.NumDescriptorRanges = _countof(range);

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter[1].Descriptor.ShaderRegister = 0;

	rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[2].Descriptor.ShaderRegister = 1;

	rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[3].Descriptor.ShaderRegister = 2;


	if (!rootSignature) {
		rootSignature = GraphicsPipeline::GetInstance()->CreateRootSignature(rootParameter, 4);
	}
	for (uint16_t i = 0; i < static_cast<uint16_t>(BlendMode::BlendCount); i++) {
		if (!graphicsPipelineState[i]) {
			graphicsPipelineState[i] = GraphicsPipeline::GetInstance()->CreateGraphicsPipeline(rootSignature.Get(), vertexShader.Get(), pixelShader.Get(), static_cast<BlendMode>(i));
		}
	}

}

void Model::CreateDescriptor(const std::string& filePath)
{
	//	モデル読み込み
	modelData = TextureManager::LoadObjFile(filePath);

	DirectX::ScratchImage mipImages = TextureManager::LoadTexture("./Resources/" + modelData.material.textureFilePath);
	//DirectX::ScratchImage mipImages = TextureManager::LoadTexture("./Resources/uvChecker.png");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	resource[0] = Engine::CreateTextureResource(Engine::GetDevice(), metaData);
	TextureManager::UploadTextureData(resource[0].Get(), mipImages);

	// 幅
	//const UINT SRVsize = Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//	デスクリプタヒープを生成
	SRVHeap = CreateDescriptorHeap(Engine::GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10, true);
	
	//	設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = metaData.format;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metaData.mipLevels);
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	
	Engine::GetDevice()->CreateShaderResourceView(resource[0].Get(), &srvDesc, SRVHeap->GetCPUDescriptorHandleForHeapStart());
	textureSrvHandleGPU = GetGPUDescriptorHandle(SRVHeap.Get(), Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 0);
}

void Model::CreateDescriptor(const std::string& filePath, const std::string& texturePath)
{
	//	モデル読み込み
	modelData = TextureManager::LoadObjFile(filePath);

	DirectX::ScratchImage mipImages = TextureManager::LoadTexture("./Resources/" + texturePath);
	//DirectX::ScratchImage mipImages = TextureManager::LoadTexture("./Resources/uvChecker.png");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	resource[0] = Engine::CreateTextureResource(Engine::GetDevice(), metaData);
	TextureManager::UploadTextureData(resource[0].Get(), mipImages);

	// 幅
	//const UINT SRVsize = Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//	デスクリプタヒープを生成
	SRVHeap = CreateDescriptorHeap(Engine::GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10, true);

	//	設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = metaData.format;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metaData.mipLevels);
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	
	Engine::GetDevice()->CreateShaderResourceView(resource[0].Get(), &srvDesc, SRVHeap->GetCPUDescriptorHandleForHeapStart());
	textureSrvHandleGPU = GetGPUDescriptorHandle(SRVHeap.Get(), Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 0);
}

void Model::CreateVertexResource()
{
	//	頂点データ
	
	vertexResource = Engine::CreateBufferResource(Engine::GetDevice(), sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//	
	VertexData* mapData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&mapData));
	std::copy(modelData.vertices.begin(), modelData.vertices.end(), mapData);

	//	重要
	vertexResource->Unmap(0, nullptr);
}

void Model::ModelDraw(WorldTransform& worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Model* model)
{
	*worldTransform.cMat = worldTransform.worldMatrix * viewProjectionMat;
	*worldTransform.cColor = ChangeColor(color);

	//ConstantBuffer<Matrix4x4> cMat;
	//*cMat = worldTransform.worldMatrix * viewProjectionMat;

	Engine::GetList()->SetGraphicsRootSignature(model->rootSignature.Get());
	Engine::GetList()->SetPipelineState(model->graphicsPipelineState[static_cast<int>(model->blendType)].Get());
	// インデックスを使わずに四角形以上を書くときは
	// 個々の設定はD3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	// インデックスを使うときは D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	Engine::GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::GetList()->IASetVertexBuffers(0, 1, &model->vertexBufferView);

	Engine::GetList()->SetDescriptorHeaps(1, model->SRVHeap.GetAddressOf());
	Engine::GetList()->SetGraphicsRootDescriptorTable(0, model->textureSrvHandleGPU);

	Engine::GetList()->SetGraphicsRootConstantBufferView(1, worldTransform.cMat.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(2, worldTransform.cColor.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(3, worldTransform.cMono.GetGPUVirtualAddress());

	Engine::GetList()->DrawInstanced(UINT(model->modelData.vertices.size()), 1, 0, 0);

}
