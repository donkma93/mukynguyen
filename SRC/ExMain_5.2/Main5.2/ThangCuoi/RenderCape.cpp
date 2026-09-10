#include "Stdafx.h"
#include "RenderCape.h"
#include <ZzzEffect.h>

CapeClassRender gCapeRender;

bool CapeClassRender::IncludeCape(int Type, int Class)
{
	if (Type == MODEL_WING + 155 || Type == MODEL_WING + 160)
	{
		return (Class == 0 || Class == 1);
	}

	if (Type == MODEL_WING + 162)
	{
		return (Class == 1 || Class == 2);
	}

	return false;
}

void CapeClassRender::RenderPointItemDL(int RegisterID, CPhysicsCloth* pCloth, OBJECT* o, bool Enable)
{
	pCloth[2].Create(o, 19, 0.0f, 8.0f, 10.0f, 10, 10, 180.0f, 180.0f, RegisterID, RegisterID, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_HEAVY | PCT_MASK_ALPHA);
	pCloth[2].AddCollisionSphere(-10.f, -10.0f, -10.0f, 25.0f, 17);
	pCloth[2].AddCollisionSphere(10.f, -10.0f, -10.0f, 25.0f, 17);
	pCloth[2].AddCollisionSphere(-10.f, -10.0f, 20.0f, 27.0f, 17);
	pCloth[2].AddCollisionSphere(10.f, -10.0f, 20.0f, 27.0f, 17);

	if (Enable)
	{
		pCloth[4].Create(o, 19, 30.0f, 15.0f, 10.0f, 2, 5, 12.0f, 200.0f, RegisterID + 1, RegisterID + 1, PCT_FLAT | PCT_SHAPE_NORMAL | PCT_COTTON | PCT_MASK_ALPHA);
		pCloth[4].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
		pCloth[4].AddCollisionSphere(0.f, 0.0f, 0.0f, 35.0f, 17);

		pCloth[5].Create(o, 19, -30.0f, 20.0f, 10.0f, 2, 5, 12.0f, 200.0f, RegisterID + 1, RegisterID + 1, PCT_FLAT | PCT_SHAPE_NORMAL | PCT_COTTON | PCT_MASK_ALPHA);
		pCloth[5].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
		pCloth[5].AddCollisionSphere(0.f, 0.0f, 0.0f, 35.0f, 17);
	}
}
void CapeClassRender::RenderPointItemRF(int RegisterID, CPhysicsCloth* pCloth, OBJECT* o, bool Enable)
{
	pCloth[0].Create(o, 19, 0.0f, 15.0f, 5.0f, 10, 10, 180.0f, 170.0f, RegisterID, RegisterID, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_HEAVY | PCT_MASK_ALPHA);
	pCloth[0].AddCollisionSphere(-10.f, -10.0f, -10.0f, 35.0f, 17);
	pCloth[0].AddCollisionSphere(10.f, -10.0f, -10.0f, 35.0f, 17);
	pCloth[0].AddCollisionSphere(-10.f, -10.0f, 20.0f, 37.0f, 17);
	pCloth[0].AddCollisionSphere(10.f, -10.0f, 20.0f, 37.0f, 17);

	if (Enable)
	{
		pCloth[1].Create(o, 19, 25.0f, 15.0f, 2.0f, 2, 5, 12.0f, 180.0f, RegisterID + 1, RegisterID + 1, PCT_FLAT | PCT_SHAPE_NORMAL | PCT_COTTON | PCT_ELASTIC_RAGE_L | PCT_MASK_ALPHA);
		pCloth[1].AddCollisionSphere(0.0f, -15.0f, -20.0f, 35.0f, 2);
		pCloth[1].AddCollisionSphere(0.0f, 0.0f, 0.0f, 45.0f, 17);

		pCloth[2].Create(o, 19, -25.0f, 15.0f, 2.0f, 2, 5, 12.0f, 180.0f, RegisterID + 1, RegisterID + 1, PCT_FLAT | PCT_SHAPE_NORMAL | PCT_COTTON | PCT_ELASTIC_RAGE_R | PCT_MASK_ALPHA);
		pCloth[2].AddCollisionSphere(0.0f, -15.0f, -20.0f, 35.0f, 2);
		pCloth[2].AddCollisionSphere(0.0f, 0.0f, 0.0f, 50.0f, 17);
	}
}

void CapeClassRender::RenderModelCapeDL(int Type, CPhysicsCloth* pCloth, OBJECT* o)
{
	if (Type == MODEL_WING + 155)
	{
		RenderPointItemDL(BITMAP_START_SPK_BIT + 3, pCloth, o, true);
	}

	if (Type == MODEL_WING + 160)
	{
		RenderPointItemDL(BITMAP_START_SPK_BIT, pCloth, o, true);
	}
}

void CapeClassRender::RenderModelCapeRF(int Type, CPhysicsCloth* pCloth, OBJECT* o)
{
	if (Type == MODEL_WING + 162)
	{
		RenderPointItemRF(BITMAP_START_SPK_BIT + 2, pCloth, o);
	}
}
//them static wing tai day //ACuoi wing
void CapeClassRender::RenderEffectWingAndCape(int Type, BMD* b, OBJECT* o)
{
	if (Type == MODEL_WING + 150)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((1.0f + Luminosity) / 2.f, (0.0f + Luminosity) / 2.f, (0.0f + Luminosity) / 2.f, Light);
		int bones[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23 };

		for (int i = 0; i < sizeof(bones) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[bones[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
		}
	}

	if (Type == MODEL_WING + 151)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((1.5f + Luminosity) / 2.f, (1.0f + Luminosity) / 2.f, (0.0f + Luminosity) / 2.f, Light);
		int bones[] = { 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23 };

		for (int i = 0; i < sizeof(bones) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[bones[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
		}

		Vector((1.0f + Luminosity) / 2.f, (0.0f + Luminosity) / 2.f, (0.0f + Luminosity) / 2.f, Light);
		b->TransformPosition(BoneTransform[8], p, Position, true); CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
		b->TransformPosition(BoneTransform[16], p, Position, true); CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);

	}

	if (Type == MODEL_WING + 152)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((1.0f + Luminosity) / 2.f, (0.3f + Luminosity) / 2.f, (0.3f + Luminosity) / 2.f, Light);
		int redBones[] = { 14, 15, 16, 17, 4, 5, 6, 7 };
		for (int i = 0; i < sizeof(redBones) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[redBones[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
		}

		Vector((0.3f + Luminosity) / 2.f, (1.0f + Luminosity) / 2.f, (0.3f + Luminosity) / 2.f, Light);
		int greenBones[] = { 11, 12, 13, 14 };
		for (int i = 0; i < sizeof(greenBones) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[greenBones[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
		}

		Vector((0.3f + Luminosity) / 2.f, (0.3f + Luminosity) / 2.f, (1.0f + Luminosity) / 2.f, Light);
		int blueBones[] = { 24, 25, 26, 27, 34, 35, 36, 37 };
		for (int i = 0; i < sizeof(blueBones) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[blueBones[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
		}
	}

	if (Type == MODEL_WING + 153)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((1.00f + Luminosity) / 2.f, (0.50f + Luminosity) / 2.f, (0.00f + Luminosity) / 2.f, Light);
		struct { int bone; float scaleOffset; } lightGroup1[] = {
			{7, 1.1f}, {15, 1.1f}, {23, 1.5f}, {24, 0.8f},
			{30, 1.1f}, {38, 1.1f}, {46, 1.5f}, {47, 0.8f},
			{58, 0.5f}, {51, 0.5f}
		};
		for (auto& l : lightGroup1)
		{
			b->TransformPosition(BoneTransform[l.bone], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + l.scaleOffset, Light, o);
		}

		Vector((0.50f + Luminosity) / 2.f, (0.05f + Luminosity) / 2.f, (0.00f + Luminosity) / 2.f, Light);
		struct { int bone; float scaleOffset; } lightGroup2[] = {
			{28, 1.8f}, {37, 1.5f}, {42, 1.5f}, {5, 1.8f},
			{14, 1.5f}, {19, 1.5f}, {57, 1.8f}, {60, 1.3f},
			{61, 1.5f}, {50, 1.8f}, {53, 1.3f}, {54, 1.5f}
		};
		for (auto& l : lightGroup2)
		{
			b->TransformPosition(BoneTransform[l.bone], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + l.scaleOffset, Light, o);
		}
	}

	if (Type == MODEL_WING + 154)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;
		Vector((0.53f + Luminosity) / 2.f, (0.36f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		int group1[] = { 3, 13, 15, 25, 27 };
		for (int i = 0; i < sizeof(group1) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[group1[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.0f, Light, o);
		}

		Vector((0.56f + Luminosity) / 2.f, (0.45f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		int group2[] = { 39, 43, 44, 45, 46, 58, 62, 63, 64, 65 };
		for (int i = 0; i < sizeof(group2) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[group2[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 2.0f, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (0.79f + Luminosity) / 2.f, (0.60f + Luminosity) / 2.f, Light);
		struct { int bone; float scaleOffset; } group3[] = {
			{40, 2.0f}, {41, 1.5f}, {42, 0.8f},
			{59, 2.0f}, {61, 1.5f}, {61, 0.8f}
		};
		for (auto& l : group3)
		{
			b->TransformPosition(BoneTransform[l.bone], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + l.scaleOffset, Light, o);
		}
	}

	if (Type == MODEL_WING + 156)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;
		Vector((1.00f + Luminosity) / 2.f, (0.72f + Luminosity) / 2.f, (0.25f + Luminosity) / 2.f, Light);
		struct { int bone; float scaleOffset; } group1[] = {
			{13, 2.0f}, {14, 1.0f}, {16, 2.0f}, {23, 2.0f},
			{33, 2.0f}, {38, 1.0f}, {39, 2.0f}, {46, 2.0f}
		};
		for (auto& l : group1)
		{
			b->TransformPosition(BoneTransform[l.bone], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + l.scaleOffset, Light, o);
		}

		Vector((0.14f + Luminosity) / 2.f, (0.14f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		int group2[] = { 10, 12, 35, 37 };
		for (int i = 0; i < sizeof(group2) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[group2[i]], p, Position, true);
			CreateSprite(BITMAP_START_SPK_BIT + 5, Position, Scale + 0.5f, Light, o);
		}
	}
	if (Type == MODEL_WING + 157)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((0.30f + Luminosity) / 2.f, (0.30f + Luminosity) / 2.f, (0.30f + Luminosity) / 2.f, Light);
		struct { int bone; float scale; } redFlare[] = {
			{7, 0.7f}, {8, 0.7f}, {9, 0.7f}, {77, 0.7f}, {78, 0.7f}, {79, 0.7f}, {17, 0.7f}, {87, 0.7f}, {22, 0.7f}, {23, 0.7f},
			{24, 0.7f}, {25, 0.5f}, {92, 0.7f}, {93, 0.7f}, {94, 0.7f}, {95, 0.5f}, {30, 0.5f}, {31, 0.7f}, {32, 0.7f},
			{33, 0.7f}, {34, 0.7f}, {35, 0.7f}, {36, 0.7f}, {37, 0.7f}, {38, 0.7f}, {100, 0.5f}, {101, 0.7f}, {102, 0.7f},
			{103, 0.7f}, {104, 0.7f}, {105, 0.7f}, {106, 0.7f}, {107, 0.7f}, {108, 0.7f},
			{45, 0.35f}, {46, 0.35f}, {47, 0.35f}, {53, 0.35f}, {54, 0.35f}, {55, 0.35f}, {56, 0.35f}, {60, 0.35f},
			{61, 0.35f}, {62, 0.35f}, {66, 0.35f}, {67, 0.35f}, {68, 0.35f}, {115, 0.35f}, {116, 0.35f}, {117, 0.35f},
			{123, 0.35f}, {124, 0.35f}, {125, 0.35f}, {118, 0.35f}, {130, 0.35f}, {131, 0.35f}, {132, 0.35f},
			{136, 0.35f}, {137, 0.35f}, {138, 0.35f}
		};
		for (auto& l : redFlare)
		{
			b->TransformPosition(BoneTransform[l.bone], p, Position, true);
			CreateSprite(BITMAP_FLARE_RED, Position, Scale + l.scale, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		struct { int bone; float scale; } whiteFlare[] = {
			{11, 0.9f}, {12, 0.7f}, {81, 0.9f}, {82, 0.7f}
		};
		for (auto& l : whiteFlare)
		{
			b->TransformPosition(BoneTransform[l.bone], p, Position, true);
			CreateSprite(BITMAP_FLARE_RED, Position, Scale + l.scale, Light, o);
		}
	}
	if (Type == MODEL_WING + 158)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((1.00f + Luminosity) / 2.f, (0.70f + Luminosity) / 2.f, (0.20f + Luminosity) / 2.f, Light);
		struct { int bone; float scale; int bitmap; } effects[] = {
			{9, 1.0f, BITMAP_LIGHT}, {23, 1.0f, BITMAP_LIGHT}, {65, 1.0f, BITMAP_LIGHT}, {93, 1.0f, BITMAP_LIGHT},
			{38, 0.7f, BITMAP_LIGHT}, {80, 0.7f, BITMAP_LIGHT}, {62, 0.4f, BITMAP_LIGHT}, {6, 0.4f, BITMAP_LIGHT},
			{9, 0.6f, BITMAP_LIGHT}, {23, 0.6f, BITMAP_LIGHT}, {65, 0.6f, BITMAP_LIGHT}, {93, 0.6f, BITMAP_LIGHT},
			{38, 0.6f, BITMAP_LIGHT}, {80, 0.6f, BITMAP_LIGHT},
			{23, 0.4f, BITMAP_START_SPK_BIT + 5}, {38, 0.25f, BITMAP_START_SPK_BIT + 5},
			{9, 0.2f, BITMAP_START_SPK_BIT + 5}, {65, 0.2f, BITMAP_START_SPK_BIT + 5},
			{80, 0.25f, BITMAP_START_SPK_BIT + 5}, {93, 0.4f, BITMAP_START_SPK_BIT + 5}
		};

		for (auto& e : effects)
		{
			b->TransformPosition(BoneTransform[e.bone], p, Position, true);
			CreateSprite(e.bitmap, Position, Scale + e.scale, Light, o);
		}
	}
	if (Type == MODEL_WING + 159)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((1.00f + Luminosity) / 2.f, (0.72f + Luminosity) / 2.f, (0.25f + Luminosity) / 2.f, Light);
		int bonesA[] = { 4, 12, 20, 28, 36, 44, 52, 60 };
		for (int i = 0; i < sizeof(bonesA) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[bonesA[i]], p, Position, true);
			CreateSprite(BITMAP_START_SPK_BIT + 5, Position, Scale + 0.3f, Light, o);
			b->TransformPosition(BoneTransform[bonesA[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 0.8f, Light, o);
		}

		Vector((0.10f + Luminosity) / 2.f, (0.33f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		int bonesB[] = { 9, 17, 25, 33, 41, 49, 57, 65 };
		for (int i = 0; i < sizeof(bonesB) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[bonesB[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
		}

		Vector((0.05f + Luminosity) / 2.f, (0.12f + Luminosity) / 2.f, (0.50f + Luminosity) / 2.f, Light);
		int bonesC[] = { 17, 25, 33, 41, 49, 57, 65 };
		for (int i = 0; i < sizeof(bonesC) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[bonesC[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.0f, Light, o);
		}

		Vector((0.16f + Luminosity) / 2.f, (0.69f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		int bonesD[] = { 72, 79 };
		for (int i = 0; i < sizeof(bonesD) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[bonesD[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 0.5f, Light, o);
		}
	}
	if (Type == MODEL_WING + 160)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((0.32f + Luminosity) / 2.f, (0.88f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		int group1[] = { 25, 26, 27, 28, 29, 30, 31, 32 };
		for (int i = 0; i < sizeof(group1) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[group1[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 0.6f, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (0.72f + Luminosity) / 2.f, (0.25f + Luminosity) / 2.f, Light);
		struct { int bone; float scale; int bitmap; } group2[] = {
			{33, 1.0f, BITMAP_LIGHT}, {34, 1.0f, BITMAP_LIGHT},
			{35, 0.3f, BITMAP_LIGHT}, {36, 0.5f, BITMAP_LIGHT},
			{33, 0.3f, BITMAP_START_SPK_BIT + 5}, {34, 0.3f, BITMAP_START_SPK_BIT + 5}
		};
		for (auto& e : group2)
		{
			b->TransformPosition(BoneTransform[e.bone], p, Position, true);
			CreateSprite(e.bitmap, Position, Scale + e.scale, Light, o);
		}

		Vector((0.85f + Luminosity) / 2.f, (0.82f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		int group3[] = { 25, 26, 27, 28, 29, 30, 31, 32, 35 };
		for (int i = 0; i < sizeof(group3) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[group3[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 0.5f, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (0.95f + Luminosity) / 2.f, (0.49f + Luminosity) / 2.f, Light);
		int group4[] = { 33, 34, 35 };
		for (int i = 0; i < sizeof(group4) / sizeof(int); ++i)
		{
			b->TransformPosition(BoneTransform[group4[i]], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 1.0f, Light, o);
		}
	}

	if (Type == MODEL_WING + 161)
	{
		o->BlendMesh = 1;

		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;
		Vector((1.00f + Luminosity) / 2.f, (0.43f + Luminosity) / 2.f, (0.21f + Luminosity) / 2.f, Light);
		int bones1[] = { 7, 108 };
		for (int i : bones1)
		{
			b->TransformPosition(BoneTransform[i], p, Position, true);
			CreateSprite(BITMAP_SWORD_EFFECT_MONO, Position, Scale + 0.5f, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (0.40f + Luminosity) / 2.f, (0.89f + Luminosity) / 2.f, Light);
		struct { int bone; float scale; int bitmap; } group2[] = {
			{7, 0.6f, BITMAP_SWORD_EFFECT_MONO}, {7, 0.35f, BITMAP_START_SPK_BIT + 5},
			{108, 0.6f, BITMAP_SWORD_EFFECT_MONO}, {108, 0.35f, BITMAP_START_SPK_BIT + 5},
			{186, 0.3f, BITMAP_SWORD_EFFECT_MONO}, {187, 0.3f, BITMAP_SWORD_EFFECT_MONO}, {188, 0.3f, BITMAP_SWORD_EFFECT_MONO},
			{186, 0.2f, BITMAP_START_SPK_BIT + 5}, {188, 0.2f, BITMAP_START_SPK_BIT + 5}
		};
		for (auto& e : group2)
		{
			b->TransformPosition(BoneTransform[e.bone], p, Position, true);
			CreateSprite(e.bitmap, Position, Scale + e.scale, Light, o);
		}

		Vector((0.72f + Luminosity) / 2.f, (0.18f + Luminosity) / 2.f, (0.26f + Luminosity) / 2.f, Light);
		int bones3[] = {
			46,47,48,49,50,51,52,53,54,55,56,57,58,59,
			60,61,62,63,64,65,66,67,68,69,70,71,72,73,
			138,139,140,141,142,143,144,145,146,147,148,149,
			150,151,152,153,154,155,156,157,158,159,160,161,
			162,163,164,165
		};
		for (int i : bones3)
		{
			b->TransformPosition(BoneTransform[i], p, Position, true);
			CreateSprite(BITMAP_SWORD_EFFECT_MONO, Position, Scale + 0.3f, Light, o);
		}
	}
	if (Type == MODEL_WING + 162)
	{
		vec3_t p, Position, Light;
		Vector(0.f, 0.f, 0.f, p);
		float Scale = absf(sinf(WorldTime * 0.002f)) * 0.2f;
		float Luminosity = absf(sinf(WorldTime * 0.002f)) * 0.4f;

		Vector((1.00f + Luminosity) / 2.f, (0.39f + Luminosity) / 2.f, (0.15f + Luminosity) / 2.f, Light);
		int group1[] = { 8, 16 };
		for (int i : group1)
		{
			b->TransformPosition(BoneTransform[i], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 0.5f, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, (1.00f + Luminosity) / 2.f, Light);
		struct { int bone; float scale; } group2[] = {
			{17, 0.5f}, {18, 0.15f}, {19, 0.1f}, {21, 0.1f}, {22, 0.15f}, {23, 0.1f}, {24, 0.1f}, {20, 0.4f}, {25, 0.4f}
		};
		for (auto& e : group2)
		{
			b->TransformPosition(BoneTransform[e.bone], p, Position, true);
			CreateSprite(BITMAP_FLARE_RED, Position, Scale + e.scale, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (0.09f + Luminosity) / 2.f, (0.00f + Luminosity) / 2.f, Light);
		int group3[] = { 20, 25, 20, 25 };
		for (int i : group3)
		{
			b->TransformPosition(BoneTransform[i], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, Scale + 0.8f, Light, o);
		}
		int group3b[] = { 18, 22 };
		for (int i : group3b)
		{
			b->TransformPosition(BoneTransform[i], p, Position, true);
			CreateSprite(BITMAP_START_SPK_BIT + 5, Position, Scale + 0.1f, Light, o);
		}

		Vector((1.00f + Luminosity) / 2.f, (0.89f + Luminosity) / 2.f, (0.29f + Luminosity) / 2.f, Light);
		struct { int bone; float scale; } group4[] = {
			{20, 0.15f}, {25, 0.15f}, {17, 0.2f}
		};
		for (auto& e : group4)
		{
			b->TransformPosition(BoneTransform[e.bone], p, Position, true);
			CreateSprite(BITMAP_START_SPK_BIT + 5, Position, Scale + e.scale, Light, o);
		}

		Vector((0.50f + Luminosity) / 2.f, (0.50f + Luminosity) / 2.f, (0.50f + Luminosity) / 2.f, Light);
		int group5[] = { 20, 25 };
		for (int i : group5)
		{
			b->TransformPosition(BoneTransform[i], p, Position, true);
			CreateSprite(BITMAP_SWORD_EFFECT_MONO, Position, Scale + 0.15f, Light, o);
		}

		/*Vector((1.00f + Luminosity) / 2.f, (0.39f + Luminosity) / 2.f, (0.15f + Luminosity) / 2.f, Light);
		b->TransformPosition(BoneTransform[8], p, Position, true); CreateSprite(BITMAP_LIGHT, Position, Scale + 0.5f, Light, o);*/
	}
}

void CapeClassRender::CreateRenderCape(BMD* b, OBJECT* o, std::vector<int> a)
{
	if (b->BodyLight[0] == 1 && b->BodyLight[1] == 1 && b->BodyLight[2] == 1)
	{
		Vector(1.f, 1.f, 1.f, b->BodyLight);
		glColor3fv(b->BodyLight);
		b->RenderBody(RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else
	{
		Vector(1.f, 1.f, 1.f, b->BodyLight);
		glColor3fv(b->BodyLight);

		for (int index : a)
		{
			b->RenderMesh(index, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
}

bool CapeClassRender::RenderModelCapeMesh(int Type, BMD* b, OBJECT* o)
{
	if (Type == MODEL_WING + 155)
	{
		std::vector<int> meshParts = { 0 };
		CreateRenderCape(b, o, meshParts);
		return true;
	}
	else if (Type == MODEL_WING + 160)
	{
		std::vector<int> meshParts = { 0, 3, 4, 5 };
		CreateRenderCape(b, o, meshParts);
		return true;
	}
	else if (Type == MODEL_WING + 162)
	{
		std::vector<int> meshParts = { 1, 2, 3};
		CreateRenderCape(b, o, meshParts);
		return true;
	}
	return false;
}
