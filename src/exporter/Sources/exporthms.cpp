
#include <windows.h>

#include <assert.h>
#include <stdio.h>
#include <string>

#define _BOOL
#include "maya/MFileObject.h"
#include "maya/MString.h"

#include "mayamaterial.h"
#include "mayamesh.h"
#include "profiler.h"


void MyCopyFile(MString& rsSource, MString& rsDestination)
{
#ifdef _WIN32
	CopyFile(rsSource.asChar(), rsDestination.asChar(), false);
#else
	#error MyCopyFile is not implemented for this platform
#endif
}


MString PathToFileName(MString &rstrPath)
{
	int iLeft = rstrPath.rindex('\\');
	int iRight = rstrPath.rindex('/');
	int iLastPathSep = iLeft > iRight ? iLeft : iRight;
	return rstrPath.substring(iLastPathSep + 1, rstrPath.length() - 1);
}


void ExportMayaMaterialAsHMS(FILE* hFile, MString& rsPath, MayaMaterial& rcMaterial)
{
	fprintf(hFile,
		"material = {\n"
		"\tname = %s\n"
		"\ttechnique = %s\n"
		"\tdiffusetex = %s\n"
		"}\n",
		rcMaterial.m_strName.asChar(),
		rcMaterial.m_strTechnique.asChar(),
		PathToFileName(rcMaterial.m_strDiffuseTexture).asChar()
	);

	MFileObject cOutputFile;
	cOutputFile.setFullName(rsPath + "/" + PathToFileName(rcMaterial.m_strDiffuseTexture));
	printf("Copying: %s to: %s\n", rcMaterial.m_strDiffuseTexture.asChar(), cOutputFile.resolvedFullName().asChar());
	MyCopyFile(rcMaterial.m_strDiffuseTexture, cOutputFile.resolvedFullName());
}


bool ExportMayaMeshAsHMS(FILE* hFile, MayaModel& rcModel)
{
	PROFILE(ExportMayaMeshAsHMS);

//	ExportShaders(kShaders);

	bool bHasSkin = false;
	bool bHasUVs = true;

	// Start exporting model
//	if (bHasSkin) {
//		fprintf(m_hFile,
//			"skinnedmodel = {\n"
//			"\tname = %s\n"
//			"\tskeleton = unnamed\n",
//			kMeshFn.name().asChar());
//	} else {
		fprintf(hFile,
			"model = {\n"
			"\tname = %s\n",
			rcModel.m_strName.asChar()
		);
//	}

	unsigned int uiMesh;
	unsigned int uiNumMeshes = rcModel.m_apcMeshes.size();

	for (uiMesh = 0; uiMesh < uiNumMeshes; uiMesh++) {
		/// Write mesh header
		MayaMesh* pcMesh = rcModel.m_apcMeshes[uiMesh];
		assert(pcMesh->m_pcMaterial);

		if (bHasSkin) {
			fprintf(hFile, "\tskinnedmesh = {\n");
		} else {
			fprintf(hFile, "\tmesh = {\n");
		}

		fprintf(hFile, 
			"\t\tmaterial = %s\n"
			"\t\tvertexbuffer = {\n"
			"\t\t\tvertices = %d\n"
			"\t\t\tformat = {\n"
			"\t\t\t\tposition = float3\n"
			"\t\t\t\tnormal = float3\n",
			pcMesh->m_pcMaterial->m_strName.asChar(),
			pcMesh->m_acVertices.size());
		if (bHasUVs)
			fprintf(hFile, "\t\t\t\ttexcoord = float2\n");
		fprintf(hFile, 
			"\t\t\t}\n"
			"\t\t\tdata = [\n"
			);

		/// Write vertice data
		unsigned int uiVertex;
		unsigned int uiNumVertices = pcMesh->m_acVertices.size();
		for (uiVertex = 0; uiVertex < uiNumVertices; uiVertex++) {
			MayaVertex &rcVertex = pcMesh->m_acVertices[uiVertex];
//			if (bHasSkin) {
//				assert(rcVertex.m_uiMayaVertexIndex < vpkWeights.size());
//				if (vpkWeights[rcVertex.m_uiMayaVertexIndex]->m_vkWeights.size()) {
//					vpkWeights[rcVertex.m_uiMayaVertexIndex]->m_uiOffset = uiMeshWeights;
//					uiMeshWeights += vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights.size();
//					uiSkinnedVertices++;
//				}
//			}

			fprintf(hFile, "\t\t\t\t%f %f %f", rcVertex.x, rcVertex.y, rcVertex.z);
			fprintf(hFile, " %f %f %f", rcVertex.nx, rcVertex.ny, rcVertex.nz);
			if (bHasUVs)
				fprintf(hFile, " %f %f", rcVertex.u, rcVertex.v);
			fprintf(hFile, "\n");
		}
		fprintf(hFile,
			"\t\t\t]\n"
			"\t\t}\n");

		/// Write out triangles
		fprintf(hFile, 
			"\t\tpolygonbuffer = {\n"
			"\t\t\ttype = triangles\n"
			"\t\t\tpolygons = %d\n"
			"\t\t\tdata = [\n",
			pcMesh->m_acTriangles.size());
		unsigned int uiTriangle;
		unsigned int uiNumTriangles = pcMesh->m_acTriangles.size();
		for (uiTriangle = 0; uiTriangle < uiNumTriangles; uiTriangle++) {
			MayaTriangle &rcTriangle = pcMesh->m_acTriangles[uiTriangle];
			fprintf(hFile, "\t\t\t\t%d %d %d\n", rcTriangle.v[0], rcTriangle.v[1], rcTriangle.v[2]);
		}
		fprintf(hFile,
			"\t\t\t]\n"
			"\t\t}\n");


		fprintf(hFile, "\t}\n");
	}


/*
	PROFILE(Export model meshes);
	//
	// We have one maya mesh, but it can have more than one shader. So we
	// loop through all shaders and export triangles affected by them as
	// separate meshes, who in turn make up a whole model.
	for (uiShader = 0; uiShader < kShaders.length(); uiShader++) {
		// First we need to build vertex and triangle buffers
		std::vector<MayaVertex> vkMeshVertices;
		std::vector<MayaTriangle> vkMeshTriangles;

		if ((vkMeshVertices.size() == 0) || (vkMeshTriangles.size() == 0))
			continue;

		if (bHasSkin) {
			fprintf(hFile, "\tskinnedmesh = {\n");
		} else {
			fprintf(hFile, "\tmesh = {\n");
		}
		
		// Export vertices
		unsigned int uiMeshWeights = 0;
		unsigned int uiSkinnedVertices = 0;
		unsigned int i;
		fprintf(hFile, 
			"\t\tmaterial = hugo\n"
			"\t\tvertexbuffer = {\n"
			"\t\t\tvertices = %d\n"
			"\t\t\tformat = {\n"
			"\t\t\t\tposition = float3\n"
			"\t\t\t\tnormal = float3\n",
			vkMeshVertices.size());
		if (bHasUVs)
			fprintf(m_hFile, "\t\t\t\ttexcoord = float2\n");
		fprintf(hFile, 
			"\t\t\t}\n"
			"\t\t\tdata = [\n"
			);
		for (i = 0; i < vkMeshVertices.size(); i++) {
			MayaVertex &kVertex = vkMeshVertices[i];
			if (bHasSkin) {
				assert(kVertex.m_uiMayaVertexIndex < vpkWeights.size());
				if (vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights.size()) {
					vpkWeights[kVertex.m_uiMayaVertexIndex]->m_uiOffset = uiMeshWeights;
					uiMeshWeights += vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights.size();
					uiSkinnedVertices++;
				}
			}

			fprintf(hFile, "\t\t\t\t%f %f %f", kVertex.x, kVertex.y, kVertex.z);
			fprintf(hFile, " %f %f %f", kVertex.nx, kVertex.ny, kVertex.nz);
			if (bHasUVs)
				fprintf(hFile, " %f %f", kVertex.u, kVertex.v);
			fprintf(hFile, "\n");
		}
		fprintf(hFile,
			"\t\t\t]\n"
			"\t\t}\n");

		// Export polygons
		fprintf(hFile, 
			"\t\tpolygonbuffer = {\n"
			"\t\t\ttype = triangles\n"
			"\t\t\tpolygons = %d\n"
			"\t\t\tdata = [\n",
			vkMeshTriangles.size());
		for (i = 0; i < vkMeshTriangles.size(); i++) {
			MayaTriangle &kTriangle = vkMeshTriangles[i];
			fprintf(hFile, "\t\t\t\t%d %d %d\n", kTriangle.v[0], kTriangle.v[1], kTriangle.v[2]);
		}
		fprintf(hFile,
			"\t\t\t]\n"
			"\t\t}\n");

		
		// Export skin
		if (bHasSkin) {
			fprintf(m_hFile, 
				"\t\tskin = {\n"
				"\t\t\tvertices = %d\n"
				"\t\t\tweights = %d\n", uiSkinnedVertices, uiMeshWeights);

			unsigned int v;
			fprintf(m_hFile, "\t\t\tvertexdata = [\n");
			for (v = 0; v < vkMeshVertices.size(); v++) {
				MayaVertex &kVertex = vkMeshVertices[v];
				if (vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights.size()) {
					fprintf(m_hFile, "\t\t\t\t%d %d %d\n",
						v, // vertex index
						vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights.size(), // num weights
						vpkWeights[kVertex.m_uiMayaVertexIndex]->m_uiOffset); // index
				}
			}
			fprintf(m_hFile, "\t\t\t]\n");

			fprintf(m_hFile, "\t\t\tweightdata = [\n");
			for (v = 0; v < vkMeshVertices.size(); v++) {
				MayaVertex &kVertex = vkMeshVertices[v];
				if (vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights.size()) {
					unsigned int w;
					for (w = 0; w < vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights.size(); w++) {
						fprintf(m_hFile, "\t\t\t\t%d %f %f %f %f\n", 
							vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights[w].m_uiBone,
							vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights[w].m_fWeight,
							vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights[w].x,
							vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights[w].y,
							vpkWeights[kVertex.m_uiMayaVertexIndex]->m_vkWeights[w].z);
					}
				}
			}
			fprintf(m_hFile, "\t\t\t]\n\t\t}\n");
		}
		fprintf(m_hFile, "\t}\n");
		
		
	}
*/
	fprintf(hFile, "}\n");

	return true;
}
