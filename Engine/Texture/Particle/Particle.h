#pragma once
#include "Engine/Texture/Model.h"

class Particle : public Model {
public:
	Particle() = default;
	~Particle();
	static void Finalize();
private:
	uint16_t kNumInstance = 1;
	// Instancing用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

public:
	/// <summary>
	/// モデルデータのロード
	/// </summary>
	/// <param name="filePath">objファイルのパス</param>
	/// <param name="vsFileName">vsShaderのパス</param>
	/// <param name="psFileName">psShaderのパス</param>
	void Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName, uint16_t num = 1);

private:
	void CreateInstancingResource();
	void CreateDescriptor(const std::string& filePath);

	void CreateVertexResource();

	//void CreateDescriptor(const std::string& filePath, const std::string& texturePath);

public:
	static void ParticleDraw(WorldTransform* worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Particle* particle);

};