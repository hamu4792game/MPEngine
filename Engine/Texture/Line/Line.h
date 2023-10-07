#pragma once
#include <wrl.h>
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"
#include "Engine/Base/ConstantBuffer.h"

class Line
{
public:
	Line();
	~Line();

	static void Finalize();

private:

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	ConstantBuffer<Matrix4x4> cMat;
	ConstantBuffer<Vector4> cColor;

private:
	void CreateVertexResource(Vector3 start = Vector3(0.0f, 0.0f, 0.0f), Vector3 end = Vector3(0.0f, 0.0f, 0.0f));

public:
	void DrawLine(Vector3 start, Vector3 end, const Matrix4x4& viewProjectionMat, uint32_t color);

};
