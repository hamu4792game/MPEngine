#pragma once

#include "math/Vector2.h"
#include "Engine/Base/ConstantBuffer.h"

class MultipathRendering
{
public:
	MultipathRendering() = default;
	~MultipathRendering() = default;

	static MultipathRendering* GetInstance();

	void Initialize();

	struct EffectParameters {
		Vector2 centerPosition; // 中心座標
		float parameterRate = 0.0f; // 半径
		int type = 0; // エフェクトのタイプ
	};
	ConstantBuffer<EffectParameters> cEffectParameters;

private:

};