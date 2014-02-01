////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
namespace FG
{
	class CameraClass
	{
	public:
		CameraClass();
		CameraClass(const CameraClass&);
		~CameraClass();

		void SetPosition(float, float, float);
		void SetRotation(float, float, float);

		D3DXVECTOR3 GetPosition();
		D3DXVECTOR3 GetRotation();

		void Render();
		void GetViewMatrix(D3DXMATRIX&);
		void SetProjectionMatrix();
	private:
		float m_positionX, m_positionY, m_positionZ;
		float m_rotationX, m_rotationY, m_rotationZ;
		D3DXMATRIX m_viewMatrix;
	};
}
#endif