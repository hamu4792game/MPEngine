#include "Line.h"
#include "Engine/Engine.h"
#include "Engine/Manager/TextureManager.h"

decltype(Line::graphicsPipelineState) Line::graphicsPipelineState;
decltype(Line::rootSignature) Line::rootSignature;

Line::Line()
{
	CreateVertexResource();

	D3D12_ROOT_PARAMETER rootParameter[2] = {};
	
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter[0].Descriptor.ShaderRegister = 0;

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[1].Descriptor.ShaderRegister = 1;

	if (!rootSignature) {
		rootSignature = GraphicsPipeline::GetInstance()->CreateRootSignature(rootParameter, 2);
	}
	if (!graphicsPipelineState) {
		graphicsPipelineState = GraphicsPipeline::GetInstance()->CreateGraphicsPipelineLine(rootSignature.Get());
	}
	
}

Line::~Line()
{
	if (vertexResource) {
		vertexResource->Release();
		vertexResource.Reset();
	}
}

void Line::Finalize()
{
	if (graphicsPipelineState) {
		graphicsPipelineState->Release();
		graphicsPipelineState.Reset();
	}
	if (rootSignature) {
		rootSignature->Release();
		rootSignature.Reset();
	}
}

void Line::CreateVertexResource(Vector3 start, Vector3 end)
{
	// 頂点データ
	VertexData vertex[2] = {
		{{start.x,start.y,start.z,1.0f},{0.0f,0.0f}},
		{{end.x,end.y,end.z,1.0f},{1.0f,0.0f}},
	};

	if (!vertexResource) {
		vertexResource = Engine::CreateBufferResource(Engine::GetDevice(), sizeof(vertex));

		vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
		vertexBufferView.SizeInBytes = sizeof(vertex);
		vertexBufferView.StrideInBytes = sizeof(VertexData);
	}
	
	//	
	VertexData* mapData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&mapData));
	for (auto i = 0; i < _countof(vertex); i++)
	{
		mapData[i] = vertex[i];
	}
	//	重要
	vertexResource->Unmap(0, nullptr);
}

void Line::DrawLine(Vector3 start, Vector3 end, const Matrix4x4& viewProjectionMat, uint32_t color)
{
	CreateVertexResource(start, end);
	*cMat = viewProjectionMat;
	*cColor = ChangeColor(color);
	
	Engine::GetList()->SetGraphicsRootSignature(rootSignature.Get());
	Engine::GetList()->SetPipelineState(graphicsPipelineState.Get());

	Engine::GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	Engine::GetList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// CBVをセット（ワールド行列）
	Engine::GetList()->SetGraphicsRootConstantBufferView(0, cMat.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(1, cColor.GetGPUVirtualAddress());

	// 描画コマンド
	Engine::GetList()->DrawInstanced(2, 1, 0, 0);

}
