#ifndef _Scene_h
#define _Scene_h


#include <Sprite3d/AutoPtr.h>


#include <string>

#include <Sprite3d/Sprite3dObj.h>


**************************************
class Scene : public Sprite3dObj
{
protected:
	int   cHardness;
	float cInterval;
	float cH;
	float cV;	
	std::string cName;

public:
	Scene( const char* pName, int pHardness, float  pInterval, float pH, float pV );
	virtual~Scene(){std::cout << "~Scene " << cName << std::endl;}

	virtual GLboolean animate();
	virtual GLboolean leaveWorld() { return GL_FALSE;}



	virtual const char*  getStrName() { return cName.c_str(); }
	std::string& getName() { return cName; }
	int          getHardness() { return cHardness; }
	float        getInterval() { return cInterval; }
*************************************

#endif