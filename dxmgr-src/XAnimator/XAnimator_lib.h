#pragma once
/*
XAnimator_lib.h
XAnimator is a small library to help with loading and rendering of animated .x files
Written by Keith Ditchburn, 2010 - 2011

Your application must be a Windows API application and you should link with one of 
the XAnimator static libraries plus the DirectX libraries as listed below:

Release Configuration
---------------------
Link with: XAnimator.lib
Plus the following DirectX library files: dxguid.lib dxerr.lib d3d9.lib d3dx9.lib

Debug Configuration
-------------------
Link with: XAnimator_debug.lib
Plus the following DirectX library files: dxguid.lib dxerr.lib d3d9.lib d3dx9d.lib

Note: XAnimator uses the DLL versions of the C++ runtime libraries (/MDd or /MD)

Last modified by Keith Ditchburn: 23rd March 2011

Version 0.93 23/3/11
- Fixed a bug where GetAnimationSetName was ignoring the passed in index and just returning the current animation set

Version 0.92 19/11/10
- Fixed a crash when model has no animation data

See ReadmeMe.txt for earlier version changes
*/

// DirectX files
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>

// String header needed to use std::string type
#include <string>

// LoadXFile flags
#define XANIMATOR_OPTIMISE_MESH	0x0001
#define XANIMATOR_CLEAN_MESH	0x0002
#define XANIMATOR_RESERVED2		0x0004
#define XANIMATOR_RESERVED3		0x0008
#define XANIMATOR_RESERVED4		0x0010

// A structure used to return individual bounding boxes per mesh (does not work for skinned mesh)
// Note: if a frame has no name in the model it is named after its parent frame if there is one, otherwise
// it is assigned the "NoName" string. A mesh without a name is also assigned "NoName"
struct TMeshBounds
{
	std::string *frameName;
	std::string *meshName;
	D3DXVECTOR3 corners[8];
};

// Public XAnimator interface
class IXAnimator
{
public:
	// Loads an X file with filename, if successful returns true and sets modelId to a unique identifier
	// that is used in later called. On error returns false.
	// Flags can be used to specify certain actions like OPTIMISE_MESH which optimises the mesh but may take time (see above)
	// rootFrameMat is an optional matrix that if supplied is applied to the root of the model (as a new frame)
	virtual bool LoadXFile(const std::string &filename,int *modelId,DWORD flags=0,const D3DXMATRIX *rootFrameMat=0)=0;		
	// Given the model id returns the number of animation sets in the model
	virtual int GetNumberOfAnimationSets(int modelId) const=0;
	// Given the model id and a new set index, changes the set. The transitionTime is used to merge into the 
	// new animation from the old one. It is in milliseconds.
	virtual bool ChangeAnimationSet(int modelId,int setIndex,float transitionTime=250)=0;
	// Given the model id and an animation set index, fills name with the animation set name.
	virtual bool GetAnimationSetName(int modelId,int setIndex,std::string *name) const=0;
	// Allows the speed of animation to be altered. e.g. 1.0f is normal, 0.5f is half speed etc.
	virtual bool ChangeAnimationPlaybackSpeed(int modelId,float speedRatio)=0;
	// Returns number of frames in currently selected animation set or -1 on error
	virtual int GetNumberOfFramesInCurrentAnimationSet(int modelId) const=0;
	// Allows the current key frame to be obtained (from currently running animation set) or -1 on error
	virtual int GetCurrentAnimationFrame(int modelId) const=0;
	// Allows the current key frame to be set for the currently running animation set
	virtual bool SetAnimationFrame(int modelId, int frameNumber)=0;
	
	// Given the model id, world matrix and the time passed since the last render: updates animation and renders
	// Note: the time is used to advance the animation and is in milliseconds
	virtual bool Render(int modelId,const D3DXMATRIX &mat,float timePassed)=0;	

	// Calculates and then returns a models' bounding shapes in model space 
	// Takes the current (as of last render) animation into account
	// Note: sphere and individual mesh results are optional.
	// modelMinBounds and modelMaxBounds are min, max box bounds aligned in model space
	// sphereCentre and sphereRadius are the sphere centre and size in model space
	// returns an array of TMeshBounds of size numMesh. Each entry is for one mesh in the models' 
	// heirarchy and contains the 8 corners of the model space bounding box plus mesh and frame names
	// NOTE: you need to actually provide a numMesh pointer or the function will just return 0
    /* 8 Corners:
		  6-- ---7
		 /|     /|
		/ |   /  |
		2-----3  |
		|  4--|--5
		| /   | /
		|/    |/
		0-----1
	*/
	virtual TMeshBounds *GetBoundingShapes(int modelId,
		D3DXVECTOR3 *modelMinBounds,D3DXVECTOR3 *modelMaxBounds,
		D3DXVECTOR3* sphereCentre=0,float *sphereRadius=0,
		int *numMesh=0) const=0;

	// Given an .x file on disk add its animation sets to the model with id modelId
	// Note: the models must have the same geometry
	virtual bool AddAnimationsFromXFile(int modelId,const std::string &filename)=0;

	// Save an x file, useful if you have added animations and want to save them all combined
	// Provide the model id and a filename. If no path is supplied saves into the working directory
	virtual bool SaveModel(int modelId,const std::string &filename,bool binary=true,bool compressed=true) const=0;
	
	virtual ~IXAnimator(){};
};

// Direct call to create and return an XAnimator object (only one is needed)
// Requires a valid Direct3D device object to be passed
// You can set verbose to true to turn on XAnimator output messages (errors always show)
IXAnimator* CreateXAnimator(LPDIRECT3DDEVICE9 device, bool verbose=false);
