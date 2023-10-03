#include "Particle.h"
#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"


void Particle::Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName, uint16_t num)
{
	//	個数の設定
	kNumInstance = num;

	//	モデルのロードとデスクリプタヒープの生成
	CreateDescriptor(filePath);

	vertexShader = GraphicsPipeline::GetInstance()->CreateVSShader(vsFileName);
	//	ピクセルシェーダーのコンパイルがなぜかできないため、緊急措置を行っている
	pixelShader = ShaderManager::GetInstance()->CompileShader(ConvertString(psFileName), L"ps_6_0");
	//pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader(psFileName);
	GraphicsPipeline::GetInstance()->pixelShader = pixelShader;

	CreateInstancingResource();

	//	頂点データの生成
	CreateVertexResource();

	D3D12_DESCRIPTOR_RANGE range[1] = {};
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 2;	//	必要な数
	range[0].RegisterSpace = 0;
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


	rootSignature = GraphicsPipeline::GetInstance()->CreateRootSignature(rootParameter, 3);
	graphicsPipelineState = GraphicsPipeline::GetInstance()->CreateGraphicsPipeline();
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

void Particle::CreateDescriptor(const std::string& filePath)
{
	//	モデル読み込み
	modelData = TextureManager::LoadObjFile(filePath);

	DirectX::ScratchImage mipImages = TextureManager::LoadTexture("./Resources/" + modelData.material.textureFilePath);
	//DirectX::ScratchImage mipImages = TextureManager::LoadTexture("./Resources/uvChecker.png");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	resource[0] = Engine::CreateTextureResource(Engine::GetDevice(), metaData);
	TextureManager::UploadTextureData(resource[0].Get(), mipImages);

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
	instancingSrvHandleGPU = GetGPUDescriptorHandle(SRVHeap.Get(), Engine::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 1);
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

void Particle::ParticleDraw(WorldTransform& worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Particle* model)
{
	//*worldTransform.cMat = worldTransform.worldMatrix * viewProjectionMat;
	*worldTransform.cColor = ChangeColor(color);

	//	書き込むためのアドレスを取得
	Matrix4x4* instancingData = nullptr;
	model->instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	//	念のため単位行列を書き込んでおく
	for (uint8_t i = 0; i < model->kNumInstance; i++) {
		instancingData[i] = worldTransform.worldMatrix * viewProjectionMat;
	}
	model->instancingResource->Unmap(0, nullptr);

	Engine::GetList()->SetGraphicsRootSignature(model->rootSignature.Get());
	Engine::GetList()->SetPipelineState(model->graphicsPipelineState.Get());
	// インデックスを使わずに四角形以上を書くときは
	// 個々の設定はD3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	// インデックスを使うときは D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	Engine::GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::GetList()->IASetVertexBuffers(0, 1, &model->vertexBufferView);

	Engine::GetList()->SetDescriptorHeaps(1, model->SRVHeap.GetAddressOf());
	Engine::GetList()->SetGraphicsRootDescriptorTable(0, model->textureSrvHandleGPU);
	//Engine::GetList()->SetGraphicsRootDescriptorTable(0, model->instancingSrvHandleGPU);

	//Engine::GetList()->SetGraphicsRootConstantBufferView(1, worldTransform.cMat.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(1, worldTransform.cColor.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(2, worldTransform.cMono.GetGPUVirtualAddress());

	Engine::GetList()->DrawInstanced(UINT(model->modelData.vertices.size()), model->kNumInstance, 0, 0);
}

