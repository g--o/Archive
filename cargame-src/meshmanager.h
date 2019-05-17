

LPD3DXBUFFER pD3DXMtrlBuffer;
class MeshManager
{
public:
D3DXMATRIX MoveMat;
D3DXMATRIX ScaleMat;
D3DXMATRIX RotMatX, RotMatY, RotMatZ;
D3DXMATRIX inMat;
LPD3DXMESH g_pMesh;
D3DMATERIAL9* g_pMeshMaterials;
LPDIRECT3DTEXTURE9* g_pMeshTextures;
DWORD g_dwNumMaterials;
LPCSTR meshname;
LPDIRECT3DDEVICE9 Device;
D3DXVECTOR3 minBounds,maxBounds, m_objectBounds[8], worldBounds[8];
 MeshManager::MeshManager(LPDIRECT3DDEVICE9 MDevice)
{
Device = MDevice;
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
}

float GetHeightBelow(D3DXVECTOR3 ray, D3DXVECTOR3 raydir){
	BOOL  Hit;
	float u, v, Dist;
	DWORD FaceIndex;
	D3DXVECTOR3 tempray; 
	D3DXVECTOR3 tempraydir;
	D3DXMATRIX m_inv;
	D3DXMatrixInverse( &m_inv,NULL,  &(ScaleMat*RotMatX*RotMatY*RotMatZ*MoveMat) );
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

float getHeight(){
	return (maxBounds.y-minBounds.y);
}

float getWidth(){
	return (maxBounds.x-minBounds.x);
}

float getDepth(){
	return (maxBounds.z-minBounds.z);
}


bool CalBB()
{
	BYTE* pVertices=NULL;


if (FAILED(g_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices)))
{
       return false;
}
D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, g_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(g_pMesh->GetFVF()), &minBounds, &maxBounds);
g_pMesh->UnlockVertexBuffer();

// We have min and max values, use these to get the 8 corners of the bounding box
m_objectBounds[0] = D3DXVECTOR3( minBounds.x, minBounds.y, minBounds.z ); // xyz
m_objectBounds[1] = D3DXVECTOR3( maxBounds.x, minBounds.y, minBounds.z ); // Xyz
m_objectBounds[2] = D3DXVECTOR3( minBounds.x, maxBounds.y, minBounds.z ); // xYz
m_objectBounds[3] = D3DXVECTOR3( maxBounds.x, maxBounds.y, minBounds.z ); // XYz
m_objectBounds[4] = D3DXVECTOR3( minBounds.x, minBounds.y, maxBounds.z ); // xyZ
m_objectBounds[5] = D3DXVECTOR3( maxBounds.x, minBounds.y, maxBounds.z ); // XyZ
m_objectBounds[6] = D3DXVECTOR3( minBounds.x, maxBounds.y, maxBounds.z ); // xYZ
m_objectBounds[7] = D3DXVECTOR3( maxBounds.x, maxBounds.y, maxBounds.z ); // XYZ
}

void UpdateBB()
{
	for( int i = 0; i < 8; i++ )
	{
      D3DXVec3TransformCoord( &worldBounds[i], &m_objectBounds[i], &(ScaleMat*MoveMat) );
	}

assert(minBounds);
assert(maxBounds);
assert(worldBounds);

minBounds.x = maxBounds.x = worldBounds[0].x;
minBounds.y = maxBounds.y = worldBounds[0].y;
minBounds.z = maxBounds.z = worldBounds[0].z;

for (int i=1;i<8;i++)
{
       if (worldBounds[i].x < minBounds.x) minBounds.x = worldBounds[i].x;
       if (worldBounds[i].x > maxBounds.x) maxBounds.x = worldBounds[i].x;
       if (worldBounds[i].y < minBounds.y) minBounds.y = worldBounds[i].y;
       if (worldBounds[i].y > maxBounds.y) maxBounds.y = worldBounds[i].y;
       if (worldBounds[i].z < minBounds.z) minBounds.z = worldBounds[i].z;
       if (worldBounds[i].z > maxBounds.z) maxBounds.z = worldBounds[i].z;
}

}

bool CheckCol(D3DXVECTOR3 *minB,D3DXVECTOR3 *maxB)
{
if((maxBounds.x) > (minB->x) && (minBounds.x) < (maxB->x) && (maxBounds.y) > (minB->y) && (minBounds.y) < (maxB->y) && (maxBounds.z) > (minB->z) && (minBounds.z) < (maxB->z))
{
return true;
}
return false;
}

void Load(LPCSTR Mmeshname, bool alpha)
{
g_pMesh = NULL;
g_pMeshMaterials = NULL;
g_pMeshTextures = NULL;
g_dwNumMaterials = 0L;
meshname = Mmeshname;

		if( FAILED(D3DXLoadMeshFromX(meshname , D3DXMESH_SYSTEMMEM,Device, NULL,&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,&g_pMesh )))
		{
			                    MessageBox( NULL, "מצטערים, התוכנית קרסה.", "שגיאה", MB_OK );
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
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
			if(alpha == true){
				//z-buffer disabled
Device->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

//alpha blending enabled
Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
//source blend factor
Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
//dest blend factor
Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
//alpha from texture
Device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			}
            if( FAILED( D3DXCreateTextureFromFileA( Device,
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &g_pMeshTextures[i] ) ) )
                {
                    MessageBox( NULL, "Sorry but we're gonna crash.", "Error", MB_OK );
			}
		}
	}
}
Device->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
}


void Update(){
	Do(&(ScaleMat*RotMatX*RotMatY*RotMatZ*inMat*MoveMat));
}

void Render()
{
	Update();

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
		void Shutdown()
		{
			g_pMesh->Release();
			g_pMesh = NULL;
			g_pMeshMaterials = NULL;
			g_pMeshTextures = NULL;
			g_dwNumMaterials = 0L;
			delete this;
		}
		void Do(D3DXMATRIX* DoAc)
		{

if(DoAc!=NULL)
{
Device->SetTransform(D3DTS_WORLD, DoAc);
}
			}

void Move(float x, float y, float z)
{
	D3DXMatrixTranslation(&this->MoveMat,x,y,z);
}
void Rotate(char RotAxis, float By)
{
if(RotAxis == 'x' ||RotAxis == 'X')
{
	D3DXMatrixRotationX(&this->RotMatX,By);
}
else if(RotAxis == 'y' || RotAxis == 'Y')
{
	D3DXMatrixRotationY(&this->RotMatY,By);
}
else if(RotAxis == 'z' || RotAxis == 'Z')
{
	D3DXMatrixRotationZ(&this->RotMatZ,By);
}
else
{
}

}
void Scale(float x, float y, float z)
{
D3DXMatrixScaling(&ScaleMat,x,y,z);
}

};

void ShutMeshManager()
{
	pD3DXMtrlBuffer->Release();
}