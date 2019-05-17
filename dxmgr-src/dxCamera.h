#pragma once

#include<d3dx9.h>


class dxCamera // Just a camera struct
{
public:
	D3DXVECTOR3 camera_position; // Holding the camera position
	D3DXVECTOR3 camera_lootat; // Holding the camera looking direction
	D3DXVECTOR3 camera_upvector; // Holding the up-vector
	D3DXMATRIX camera_Projection; // The projection matrix
	D3DXMATRIXA16 camera_view; // The view matrix

	// Init our camera vars
	dxCamera()
	{
		 camera_position = D3DXVECTOR3( 0.0f, 1.0f,5.0f ); // Setting deafult position
		 camera_lootat = D3DXVECTOR3( 0.0f, 1.0f, 1.0f ); // Setting deafult looking direction
		 camera_upvector = D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // Setting deafult up-vector
	}

	/********************************************************************
	* createCamera
	* Utility function used to define a camera
	********************************************************************/
	void createCamera(float nearClip, float farClip, LPDIRECT3DDEVICE9 lpDevice)
	{
		// Setting the camera position, looking direction and up-vector..
		D3DXMatrixLookAtLH(&camera_view, &camera_position,		//Camera Position
									 &camera_lootat,		//Look At Position
									 &camera_upvector);		//Up Direction
		//Here we specify the field of view, aspect ration and near and far clipping planes.
		D3DXMatrixPerspectiveFovLH(&camera_Projection, D3DX_PI/4, screen_width/screen_height, nearClip, farClip);
		lpDevice->SetTransform(D3DTS_VIEW, &camera_view);
		lpDevice->SetTransform(D3DTS_PROJECTION, &camera_Projection);
	}

	void rotate()
	{
			
	}

};