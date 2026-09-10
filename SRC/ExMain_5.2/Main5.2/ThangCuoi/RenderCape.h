#pragma once
#include <ZzzBMD.h>
#include"../PhysicsManager.h"
class CapeClassRender
{
	public:
	bool IncludeCape(int Type, int Class = 1);
	void CreateRenderCape(BMD* b, OBJECT* o, std::vector<int> a);
	void RenderModelCapeDL(int Type, CPhysicsCloth* pCloth, OBJECT* o);
	void RenderModelCapeRF(int Type, CPhysicsCloth* pCloth, OBJECT* o);
	bool RenderModelCapeMesh(int Type, BMD* b, OBJECT* o);
	void RenderEffectWingAndCape(int Type, BMD* b, OBJECT* o);
	void RenderPointItemDL(int RegisterID, CPhysicsCloth* pCloth, OBJECT* o, bool Enable = false);
	void RenderPointItemRF(int RegisterID, CPhysicsCloth* pCloth, OBJECT* o, bool Enable = false);
};
extern CapeClassRender gCapeRender;