
#include "engine/asset.h"
#include "engine/bsp.h"
#include "ccor/Core.h"
#include "shared/engine.h"
#include "profiler.h"
#include "exporter.h"

#include <maya/MFnDagNode.h>
#include <maya/MMatrix.h>

#include "mayamaterial.h"
#include "mayalight.h"
#include "mayamesh.h"
#include "mayanode.h"


using namespace engine;


engine::IEngine* gEngine;



class ExporterAsset: public Asset
{
	std::vector<MayaMaterial*>      mMaterials;

        MayaNode*               mRootNode;

        AssetObjectM            mAssetObjects;

        void                    TraverseMayaScene();
        bool                    TraverseMayaSceneRecurse(MayaNode* parentNode, MDagPath& parentPath);

        void                    ExportScene();
        void                    ExportSceneRecursively(MayaNode* parentNode, Clump* clump, Frame* parentFrame);
        bool                    ExportModel(Clump* clump, Frame* frame, MayaModel* model);
        void                    ExportMesh(Geometry* geometry, MayaMesh* mesh, int materialId, int* vertexOffset, int* triangleOffset);
//        Geometry*               ExportMesh(MayaMesh* mesh);
        bool                    ExportLight(Clump* clump, Frame* frame, MayaLight* mayaLight);
        Shader*                 ExportMayaMaterial(MayaMaterial* material);

public:
        virtual void __stdcall release(void) { delete this; };
        virtual void __stdcall serialize(void) { };

        ExporterAsset();
};


Matrix4f MayaToBaMatrix(MMatrix matrix) {
        Matrix4f m;

        int x, y;
        for (y = 0; y < 4; ++y) {
                for (x = 0; x < 4; ++x) {
                        m[x][y] = matrix.matrix[x][y];
                }
        }

        return m;
}


void ExporterAsset::ExportScene()
{
        AABB boundingBox( wrap( Vector3f(-1000000.0f, -1000000.0f, -1000000.0f) ), wrap( Vector3f(1000000.0f, 1000000.0f, 1000000.0f) ) );

        BSP* bsp = new BSP( "blabsp", boundingBox, 0 );
        _bsps.push_back(bsp);
        BSPSector* rootSector = new BSPSector( bsp, NULL, boundingBox, NULL );

        int i;
        int count = mRootNode->mChilds.size();
        for (i = 0; i < count; ++i) {
                MayaNode* node = mRootNode->mChilds[i];

                Clump* clump = new Clump(node->mName.asChar());
                Frame* frame = new Frame(node->mName.asChar());

                Matrix4f transform(MayaToBaMatrix(node->mTransform));
                frame->setMatrix(transform);

                AssetObjectT assetObjectT((auid)frame, frame);
                mAssetObjects.insert(assetObjectT);

                clump->setFrame(frame);
                
                _clumps.push_back(clump);

                ExportSceneRecursively(node, clump, frame);
        }
}

void ExporterAsset::ExportSceneRecursively(MayaNode* parentNode, Clump* clump, Frame* parentFrame) {
        int i;
        int count = parentNode->mChilds.size();
        for (i = 0; i < count; ++i) {
                MayaNode* node = parentNode->mChilds[i];

                Frame* frame = new Frame(node->mName.asChar());

                Matrix4f transform(MayaToBaMatrix(node->mTransform));
                frame->setMatrix(transform);

                AssetObjectT assetObjectT((auid)frame, frame);
                mAssetObjects.insert(assetObjectT);

                frame->setParent(parentFrame);

                ExportSceneRecursively(node, clump, frame);
        }

        if (parentNode->mModel) {
                ExportModel(clump, parentFrame, parentNode->mModel);
        }

        if (parentNode->mLight) {
                ExportLight(clump, parentFrame, parentNode->mLight);
        }
}


bool ExporterAsset::ExportModel(Clump* clump, Frame* frame, MayaModel* model)
{
        int vertexCount = 0;
        int triangleCount = 0;
        int uvCount = 1;
        int shaderCount = model->m_apcMeshes.size();

        int i;
        int meshCount = model->m_apcMeshes.size();
        for (i = 0; i < meshCount; ++i) {
                MayaMesh* mesh = model->m_apcMeshes[i];
                vertexCount += mesh->m_acVertices.size();
                triangleCount += mesh->m_acTriangles.size();
        }

        Geometry* geometry = new Geometry(vertexCount, triangleCount, uvCount, shaderCount, 0, false, model->m_strName.asChar());

        int vertexOffset = 0;
        int triangleOffset = 0;

        for (i = 0; i < meshCount; ++i) {
                MayaMesh* mesh = model->m_apcMeshes[i];

                ExportMesh(geometry, mesh, i, &vertexOffset, &triangleOffset);
        }

        Atomic* atomic = new Atomic();
        atomic->setFrame(frame);
        atomic->setGeometry(geometry);
        clump->add(atomic);
        return true;
}


void ExporterAsset::ExportMesh(Geometry* geometry, MayaMesh* mesh, int materialId, int* vertexOffset, int* triangleOffset)
{
        int vertexCount = mesh->m_acVertices.size();
        int triangleCount = mesh->m_acTriangles.size();

        // Fill vertex data
        int i;
        Vector* vertices = geometry->getVertices() + *vertexOffset;
        Vector* normals = geometry->getNormals() + *vertexOffset;
        Flector* uvs = geometry->getUVSet(0) + *vertexOffset;
        for (i = 0; i < vertexCount; ++i) {
                vertices[i].x = mesh->m_acVertices[i].x;
                vertices[i].y = mesh->m_acVertices[i].y;
                vertices[i].z = mesh->m_acVertices[i].z;

                normals[i].x = mesh->m_acVertices[i].nx;
                normals[i].y = mesh->m_acVertices[i].ny;
                normals[i].z = mesh->m_acVertices[i].nz;

                uvs[i].x = mesh->m_acVertices[i].u;
                uvs[i].y = mesh->m_acVertices[i].v;
        }

        // Fill triangle data
        Triangle* triangles = geometry->getTriangles() + *triangleOffset;
        for (i = 0; i < triangleCount; i++) {
                const MayaTriangle& triangle = mesh->m_acTriangles[i];
                triangles[i].set(
                        triangle.v[0] + *vertexOffset, 
                        triangle.v[1] + *vertexOffset, 
                        triangle.v[2] + *vertexOffset, 
                        materialId
                );
        }

        *vertexOffset = *vertexOffset + vertexCount;
        *triangleOffset = *triangleOffset + triangleCount;

        // Setup shaders
        Shader* shader = ExportMayaMaterial(mesh->m_pcMaterial);
        geometry->setShader(materialId, shader);
}


bool ExporterAsset::ExportLight(Clump* clump, Frame* frame, MayaLight* mayaLight)
{
        engine::LightType lightType;
        switch (mayaLight->mType) {
                case MayaLightType::Ambient:    lightType = engine::ltAmbient; break;
                case MayaLightType::Directional: lightType = engine::ltDirectional; break;
                case MayaLightType::Point:      lightType = engine::ltPoint; break;
                case MayaLightType::Spot:       lightType = engine::ltSpot; break;
        }

        Light* light = new Light(lightType);

        Vector3x diffuseLight(mayaLight->mRed, mayaLight->mGreen, mayaLight->mBlue);
        diffuseLight *= mayaLight->mIntensity;
        light->setDiffuseColor(Vector4f(diffuseLight.x, diffuseLight.y, diffuseLight.z, 1.0f));
        light->setAttenuation(Vector3f(mayaLight->mAttenuation[0], mayaLight->mAttenuation[1], mayaLight->mAttenuation[2]));
        light->setRange(10000000.0f);

        clump->add(light);
        light->setFrame(frame);

        return true;
}


void ExporterAsset::TraverseMayaScene()
{
	PROFILE(ExporterAsset::TraverseMayaScene);

	MStatus kStatus;

	printf("---------------------------------------------\n");
	printf("Traversing scene.\n");

        MItDag kDagIter(MItDag::kBreadthFirst, MFn::kInvalid, &kStatus);

	if (kStatus != MStatus::kSuccess) {
		printf("Error: Unable to get DAG iterator\n");
		return;
	}

        mRootNode = new MayaNode();

	// Iterate through all nodes
	for(; !kDagIter.isDone(); kDagIter.next()) {
		MDagPath path;
		if (kDagIter.getPath(path) != MStatus::kSuccess) {
			printf("Error: Unable to get path to mesh node\n");
			return;
		}

                MFnDagNode dagNode(path);

                if (dagNode.isIntermediateObject() || dagNode.isDefaultNode()) {
                        continue;
                }

                if (kDagIter.depth() != 1) {
                        continue;
                }

                bool exp = false;
                if (path.hasFn(MFn::kMesh)) exp = true;
                if (path.hasFn(MFn::kLight)) exp = true;

                TraverseMayaSceneRecurse(mRootNode, path);

                fflush(stdout);
	}
}


bool ExporterAsset::TraverseMayaSceneRecurse(MayaNode* parentNode, MDagPath& path) {
        printf("Checking: %s\n", path.fullPathName().asChar());

        bool hasSomething = false;

        if (path.hasFn(MFn::kTransform)) {
                MayaNode* node = new MayaNode();
                MFnTransform transform(path);
                node->mName = path.partialPathName();
                node->mTransform = transform.transformationMatrix();

                int i;
                for (i = 0; i < path.childCount(); ++i) {
                        MObject child = path.child(i);
                        path.push(child);
                        if (TraverseMayaSceneRecurse(node, path)) {
                                hasSomething = true;
                        }
                        path.pop();
                }

                if (hasSomething) {
                        printf("Node: %s\n", path.partialPathName().asChar());
                        parentNode->mChilds.push_back(node);
                } else {
                        delete node;
                }
        } else if (path.hasFn(MFn::kMesh)) {
                printf("Mesh: %s\n", path.partialPathName().asChar());
                hasSomething = true;

                MayaModel* model = new MayaModel();
                if (ExtractMayaMesh(&mMaterials, model, path)) {
                        model->m_strName = parentNode->mName;
                        parentNode->mModel = model;
                } else {
                        delete model;
                }
        } else if (path.hasFn(MFn::kLight)) {
                printf("Light: %s\n", path.partialPathName().asChar());
                hasSomething = true;

                MayaLight* light = new MayaLight();

                if (ExtractMayaLight(light, path)) {
		        light->mName = path.partialPathName().asChar();
                        parentNode->mLight = light;
                } else {
                        delete light;
                }
        }

        return hasSomething;
}




Shader* ExporterAsset::ExportMayaMaterial(MayaMaterial* material)
{
        Shader* shader = new Shader(material->mTextures.size(), material->m_strName.asChar());

        int i;
        int textureCount = material->mTextures.size();
        for (i = 0; i < textureCount; ++i) {
                ITexture* texture = gEngine->getTexture(Texture::getTextureNameFromFilePath(material->mTextures[i]->mName.asChar()).c_str());
                if (texture == 0) {
                        texture = gEngine->createTexture(material->mTextures[i]->mName.asChar());
                }
                shader->setLayerTexture(i, texture);
        }

        return shader;
}


ExporterAsset::ExporterAsset()
{
        TraverseMayaScene();
        ExportScene();

        fflush(stdout);
}



void ExportBa(const char* fileName)
{
        printf("Starting export -------------------\n");

        int rcode = CoreInitEngine(0, 0, "", 0);
        if (rcode == 0) {
                if(askInterfaceT("Engine", &gEngine)) {
                        IAsset* asset = gEngine->createAsset(atBinary, fileName);

                        ExporterAsset exporterAsset;

                        asset->seize(&exporterAsset);

                        asset->serialize();
                        asset->release();
                } else {
                        printf("Couldn't get engine interface.\n");
                }
        }
        rcode = CoreShutdownEngine(rcode);
        gEngine = 0;

        printf("Export finished -------------------\n");
        fflush(stdout);
}

void main() {
        printf("Starting export -------------------");
        ExportBa("test.ba");
        printf("Export finished -------------------");
        fflush(stdout);
}
