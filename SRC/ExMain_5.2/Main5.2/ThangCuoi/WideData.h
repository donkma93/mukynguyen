#pragma once

#ifndef POS_CALCULATIONS_H
#define POS_CALCULATIONS_H

#include "ZzzOpenglUtil.h"
void mPosScreen(int SPK);
extern float IsScaleWideX;
class GetPosWide
{
	public:
	float x_fScreenRate_x;
	float x_fScreenRate_y;
    int x_GetAddPos;
	void SetupPosScreen(float a);
};
extern GetPosWide gPosWide;

inline float GetWindowsX() {
    return WindowWidth / gPosWide.x_fScreenRate_x;
}

inline float GetWindowsY() {
    return WindowHeight / gPosWide.x_fScreenRate_y;
}

inline float setPosCenterX(float x) {
    return (GetWindowsX() / 2.f) - (x / 2.f);
}

inline float setPosCenterY(float x) {
    return (GetWindowsY() / 2.f) - (x / 2.f) - 51.f;
}

inline float setPosRight(float x) {
    return GetWindowsX() - 640.f + x;
}

inline float setPosDown(float x) {
    return GetWindowsY() - 480.f + x;
}
inline float setPosRightXShop(float x) {
    return GetWindowsX() - (640.f + x);
}

#endif
