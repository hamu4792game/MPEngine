#include "Particle.h"
#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"
#include "Engine/Manager/TextureManager.h"

//	便利なtmpみたいなやつ
decltype(Particle::rootSignature) Particle::rootSignature;
decltype(Particle::graphicsPipelineState) Particle::graphicsPipelineState;
decltype(Particle::vertexShader) Particle::vertexShader;
decltype(Particle::pixelShader) Particle::pixelShader;

Particle::~Particle()
{
	if (instancingResource) {
		instancingResource->Release();
		instancingResource.Reset();
	}

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

void Particle::Finalize() {
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

void Particle::Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName, const std::string& textureFilePath, uint16_t num)
{
	//	個数の設定
	kNumInstance = num;

	CreateInstancingResource();

	//	モデルのロードとデスクリプタヒープの生成
	CreateDescriptor(filePath, textureFilePath);

	vertexShader = GraphicsPipeline::GetInstance()->CreateVSShader(vsFileName);
	pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader(psFileName);

	//	頂点データの生成
	CreateVertexResource();

	D3D12_DESCRIPTOR_RANGE range[1] = {};
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 2;	//	必要な数
	//range[0].RegisterSpace = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[3] = {};
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].DescriptorTable.pDescriptorRanges = range;
	rootParameter[0].DescriptorTable.NumDescriptorRanges = _countof(range);

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[1].Descriptor.ShaderRegister = 1;

	rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[2].Descriptor.ShaderRegister = 2;


	if (!rootSignature) {
		rootSignature = GraphicsPipeline::GetInstance()->CreateRootSignature(rootParameter, 3);
	}
	for (uint16_t i = 0; i < static_cast<uint16_t>(BlendMode::BlendCount); i++) {
		if (!graphicsPipelineState[i]) {
			graphicsPipelineState[i] = GraphicsPipeline::GetInstance()->CreateGraphicsPipeline(rootSignature.Get(), vertexShader.Get(), pixelShader.Get(), static_cast<BlendMode>(i));
		}
	}
}

void Particle::CreateInstancingResource()
{
	// Instancing用のリソースを作る
	instancingResource = Engine::CreateBufferResource(Engine::GetDevice(), sizeof(Matrix4x4) * kNumInstance);
	//	書き込むためのアドレスを取得
	Matrix4x4* instancingData = nullptr;
	instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	//	念のため単位行列を書き込んでおく
	for (uint8_t i = 0; i < kNumInstance; i++) {
		instancingData[i] = MakeIdentity4x4();
	}
	instancingResource->Unmap(0, nullptr);
}

void Particle::CreateDescriptor(const std::string& filePath,const std::string& textureFilePath)
{
	//	モデル読み込み
	modelData = TextureManager::LoadObjFile(filePath);

	DirectX::ScratchImage mipImages = TextureManager::LoadTexture(textureFilePath);
	//DirectX::ScratchImage mipImages = TextureManager::LoadTexture("./Resources/uvChecker.png");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	resource[0] = Engine::CreateTextureResource(Engine::GetDevice(), metaData);
	TextureManager::UploadTextureData(resource[0].Get(), mipImages);

	//	デスクリプタヒープを生成
	SRVHeap = CreateDescriptorHeap(Engine::GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kNumInstance, true);

	//	設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = metaData.format;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metaData.mipLevels);
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	
	Engine::GetDevice()->CreateShaderResourceView(resource[0].Get(), &srvDesc, SRVHeap->GetCPUDescriptorHandleForHeapStart());
	textureSrvHandleGPU = GetGPUDescriptorHandle(SRVHeap.Get(), Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 0);

	//	設定
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(Matrix4x4);
	//
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU = GetCPUDescriptorHandle(SRVHeap.Get(), Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 1);
	//instancingSrvHandleGPU = GetGPUDescriptorHandle(SRVHeap.Get(), Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 1);
	Engine::GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
}

void Particle::CreateVertexResource()
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

void Particle::ParticleDraw(WorldTransform* worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Particle* particle)
{
	//*worldTransform.cMat = worldTransform.worldMatrix * viewProjectionMat;
	*worldTransform[0].cColor = ChangeColor(color);

	//	書き込むためのアドレスを取得
	Matrix4x4* instancingData = nullptr;
	particle->instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	//	念のため単位行列を書き込んでおく
	for (uint8_t i = 0; i < particle->kNumInstance; i++) {
		instancingData[i] = worldTransform[i].worldMatrix * viewProjectionMat;
	}
	particle->instancingResource->Unmap(0, nullptr);

	Engine::GetList()->SetGraphicsRootSignature(particle->rootSignature.Get());
	//Engine::GetList()->SetPipelineState(GraphicsPipeline::GetInstance()->graphicsPipelineState[static_cast<int>(particle->blendType)].Get());
	Engine::GetList()->SetPipelineState(particle->graphicsPipelineState[static_cast<uint16_t>(particle->blendType)].Get());
	// インデックスを使わずに四角形以上を書くときは
	// 個々の設定はD3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	// インデックスを使うときは D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	Engine::GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::GetList()->IASetVertexBuffers(0, 1, &particle->vertexBufferView);

	Engine::GetList()->SetDescriptorHeaps(1, particle->SRVHeap.GetAddressOf());
	Engine::GetList()->SetGraphicsRootDescriptorTable(0, particle->textureSrvHandleGPU);
	//Engine::GetList()->SetGraphicsRootDescriptorTable(0, particle->instancingSrvHandleGPU);

	//Engine::GetList()->SetGraphicsRootConstantBufferView(1, worldTransform.cMat.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(1, worldTransform[0].cColor.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(2, worldTransform[0].cMono.GetGPUVirtualAddress());

	Engine::GetList()->DrawInstanced(UINT(particle->modelData.vertices.size()), particle->kNumInstance, 0, 0);
}

