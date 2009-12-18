
#include <vector>

#define _BOOL
#include <maya/MString.h>
#include <maya/MFnMesh.h>
#include <maya/MFnLambertShader.h>


class MayaMaterial
{
public:
	MString				m_strName;

	MString				m_strTechnique;

	MString				m_strDiffuseTexture;

	float				m_fRepeatU;

	float				m_fRepeatV;
};


void ExtractMaterials(std::vector<MayaMaterial*>* papcMayaMaterial, MObjectArray& racShaders);
