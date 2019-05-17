#pragma once

#pragma comment(lib,"XAnimator.lib") // XAnimator libs
#include <XAnimator_lib.h> // XAnimator header

class Object
{
	public:
		D3DXVECTOR3 minBounds,maxBounds, m_objectBounds[8], worldBounds[8];
		D3DXMATRIX MoveMat;
		D3DXMATRIX ScaleMat;
		D3DXMATRIX RotMatX, RotMatY, RotMatZ;
		D3DXMATRIX inMat;
		LPDIRECT3DDEVICE9 Device;
		IXAnimator* xAnimator;
		int id,as; // Id of model,Current animation set
		long lastUpdate;

	Object::Object(LPDIRECT3DDEVICE9 MDevice, IXAnimator* xAnim)
	{

		D3DXMatrixRotationX(&RotMatX,0);
		D3DXMatrixRotationX(&RotMatY,0);
		D3DXMatrixRotationX(&RotMatZ,0);
		D3DXMatrixScaling(&ScaleMat,1,1,1);
		D3DXMatrixTranslation(&MoveMat,0,0,0);
		D3DXMatrixIdentity(&MoveMat);
		D3DXMatrixIdentity(&ScaleMat);
		D3DXMatrixIdentity(&RotMatX);
		D3DXMatrixIdentity(&RotMatY);
		D3DXMatrixIdentity(&RotMatZ);
		D3DXMatrixIdentity(&inMat);

		Device = MDevice;
		xAnimator = xAnim;
		lastUpdate=timeGetTime();
		as=0;
	}

	void setMesh(std::string xFilePath)
	{
		xAnimator->LoadXFile(xFilePath,&id);
	}

	int getMesh()
	{
		return id;
	}

	float getHeight(){
		return (maxBounds.y-minBounds.y);
	}

	float getWidth(){
		return (maxBounds.x-minBounds.x);
	}

	float getDepth(){
		return (maxBounds.z-minBounds.z);
	}


	bool CalAABB()
	{
		xAnimator->GetBoundingShapes(id,&minBounds,&maxBounds);
	}

	D3DXMATRIX generateWorldMatrix()
	{
		return ScaleMat*RotMatX*RotMatY*RotMatZ*MoveMat;
	}

	bool Render(D3DXMATRIX doMat){
		long newTime = timeGetTime();
		bool r = xAnimator->Render(id,doMat,newTime-lastUpdate);
		lastUpdate = newTime;

		return r;
	}

	void Run()
	{
		update();
		Render(generateWorldMatrix());
	}

	void setSet(int setIndex){
		xAnimator->ChangeAnimationSet(id,setIndex);
	}

	void nextSet(){
		as++;
		if(xAnimator->GetNumberOfAnimationSets(id)<as)
			as=0;
		xAnimator->ChangeAnimationSet(id,as);
	}

	bool CheckCol(D3DXVECTOR3 *minB,D3DXVECTOR3 *maxB)
	{
		if((maxBounds.x) > (minB->x) && (minBounds.x) < (maxB->x) && (maxBounds.y) > (minB->y) && (minBounds.y) < (maxB->y) && (maxBounds.z) > (minB->z) && (minBounds.z) < (maxB->z))
			return true;
		return false;
	}
 	void Do(D3DXMATRIX* DoAc)
	{
		if(DoAc!=NULL)
			Device->SetTransform(D3DTS_WORLD, DoAc);
	}

	void Move(float x, float y, float z)
	{
		D3DXMatrixTranslation(&this->MoveMat,x,y,z);
	}

	void Rotate(char RotAxis, float By)
	{
		if(RotAxis == 'x' ||RotAxis == 'X')
			D3DXMatrixRotationX(&this->RotMatX,By);
		else if(RotAxis == 'y' || RotAxis == 'Y')
			D3DXMatrixRotationY(&this->RotMatY,By);
		else if(RotAxis == 'z' || RotAxis == 'Z')
			D3DXMatrixRotationZ(&this->RotMatZ,By);
	}

	void Scale(float x, float y, float z)
	{
		D3DXMatrixScaling(&ScaleMat,x,y,z);
	}

	void update() // A normal update
	{
		Do(&(ScaleMat*RotMatX*RotMatY*RotMatZ*MoveMat));
	}
};

LPD3DXBUFFER pD3DXMtrlBuffer;

class Terrain // A static only mesh class
{
public:
	LPDIRECT3DDEVICE9 Device;
	LPD3DXMESH g_pMesh;
	D3DMATERIAL9* g_pMeshMaterials;
	LPDIRECT3DTEXTURE9* g_pMeshTextures;
	DWORD g_dwNumMaterials;
	LPCSTR meshname;

	 Terrain(LPDIRECT3DDEVICE9 MDevice)
	 {
		Device = MDevice;
	 }

	Terrain(LPDIRECT3DDEVICE9 MDevice,LPCSTR Meshname){
		Device = MDevice;
		Load(Meshname);
	}

	float GetHeightBelow(D3DXVECTOR3 ray, D3DXVECTOR3 raydir){
		BOOL  Hit;
		float u, v, Dist;
		DWORD FaceIndex;
		D3DXVECTOR3 tempray; 
		D3DXVECTOR3 tempraydir;
		D3DXMATRIX m_inv;
		D3DXMatrixIdentity(&m_inv);
		D3DXMatrixInverse( &m_inv,NULL,  &m_inv );
		D3DXVec3TransformCoord(&tempray, &ray, &m_inv );
		D3DXVec3TransformNormal( &tempraydir, &raydir, &m_inv );
		D3DXIntersect(g_pMesh,
			&tempray,  
			&tempraydir,     
			&Hit, &FaceIndex, &u, &v, &Dist, NULL, NULL); 
		if(Hit == TRUE) 
			return (ray.y-Dist); 
		return ray.y;
	}

	void Load(LPCSTR Mmeshname)
	{
		g_pMesh = NULL;
		g_pMeshMaterials = NULL;
		g_pMeshTextures = NULL;
		g_dwNumMaterials = 0L;
		meshname = Mmeshname;

		if(FAILED(D3DXLoadMeshFromX(meshname , D3DXMESH_SYSTEMMEM,Device, NULL,&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,&g_pMesh )))
		{
			MessageBox( NULL, "Sorry, we were unable to find some resources. Please try reinstalling.", "Error", MB_OK );
		}
		else
		{
			D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
			g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
			g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];

			for( DWORD i = 0; i < g_dwNumMaterials; i++ )
			{
				// Copy the material
				g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
				// Set the ambient color for the material (D3DX does not do this)
				g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;
				g_pMeshTextures[i] = NULL;
				if( d3dxMaterials[i].pTextureFilename != NULL && lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
				{
					// Create the texture
					if( FAILED( D3DXCreateTextureFromFileA( Device,d3dxMaterials[i].pTextureFilename,&g_pMeshTextures[i] ) ) )
					{
						MessageBox( NULL, "Sorry, we were unable to find some resources. Please try reinstalling.", "Error", MB_OK );
						break;
					}
				}
			}
		}
	}

	void Render() // Render
	{
		if(g_pMesh != NULL)
		{
			for( DWORD i = 0; i < g_dwNumMaterials; i++ )
			{
				// Set the material and texture for this subset
				Device->SetMaterial( &g_pMeshMaterials[i] );
				Device->SetTexture( 0, g_pMeshTextures[i] );
				// Draw the mesh subset
				g_pMesh->DrawSubset( i );
			}
		}
	}
	void Shutdown() // Release
	{

		if(g_pMeshMaterials != NULL)
			delete[] g_pMeshMaterials;

		if(g_pMeshTextures)
		{
			for(DWORD i = 0; i < g_dwNumMaterials; i++)
			{
				if(g_pMeshTextures[i])
				g_pMeshTextures[i]->Release();
			}
			delete[] g_pMeshTextures;
		}

		if(g_pMesh != NULL)
			g_pMesh->Release();

		meshname = NULL;
		delete this;
	}
};

void shutTerrains()
{
	pD3DXMtrlBuffer->Release();
}