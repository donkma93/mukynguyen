#pragma once

class CCamera
{
public:
	CCamera();
	virtual ~CCamera();
	void Toggle();
	void Restore();
	void Update();
	int	m_CameraOnOff;
	float	CameraZoom;
	float	AngleY3D;
	float	AngleZ3D;
private:
	DWORD LastPress;
	int m_CursorX;
	int m_CursorY;
};

extern CCamera gCamera;