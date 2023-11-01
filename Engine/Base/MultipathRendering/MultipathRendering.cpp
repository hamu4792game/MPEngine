#include "MultipathRendering.h"

MultipathRendering* MultipathRendering::GetInstance() {
	static MultipathRendering instance;
	return &instance;
}

void MultipathRendering::Initialize() {
	cEffectParameters->centerPosition = Vector2(0.0f, 0.0f);
	cEffectParameters->parameterRate = 0.0f;
	cEffectParameters->type = 0;
}