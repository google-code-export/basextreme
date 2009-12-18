
#include "engine/asset.h"
#include "engine/bsp.h"
#include "ccor/Core.h"
#include "shared/engine.h"
#include "mayamaterial.h"
#include "mayamesh.h"
#include "profiler.h"
#include "exporter.h"


using namespace engine;


class ExporterAsset: public Asset
{
	std::vector<MayaMaterial*> mMaterials;
	std::vector<MayaModel*> mModels;


        bool                    GatherMayaModels();
        bool                    ExportMayaModels();
                
public:
        virtual void __stdcall release(void) { delete this; };
        virtual void __stdcall serialize(void) { };

        ExporterAsset();
};


bool ExporterAsset::GatherMayaModels()
{
	PROFILE(SceneExporter::ExportModels);

	MStatus kStatus;

	printf("---------------------------------------------\n");
	printf("Gathering models\n");

	// Get iterator for meshes
	MItDag kDagIter(MItDag::kDepthFirst, MFn::kMesh, &kStatus);

	if (kStatus != MStatus::kSuccess) {
		printf("Error: Unable to get DAG iterator\n");
		return false;
	}

	// Iterate through all meshes and extract their materials and meshes
	for(; !kDagIter.isDone(); kDagIter.next()) {
		MDagPath kDagPath;
		if (kDagIter.getPath(kDagPath) != MStatus::kSuccess) {
			printf("Error: Unable to get path to mesh node\n");
			return false;
		}

		MayaModel* pcModel = new MayaModel();
		pcModel->m_strName = kDagPath.partialPathName();

		ExtractMayaMesh(&mMaterials, pcModel, kDagPath);
		mModels.push_back(pcModel);
	}

	//unsigned int i;
	//unsigned int n;
	//
	//// Export materials
	//n = apcMaterials.size();
	//for (i = 0; i < n; i++) {
	//	ExportMayaMaterialAsHMS(m_hFile, rsPath, *apcMaterials[i]);
	//}

	//// Export meshes
	//n = apcModels.size();
	//for (i = 0; i < n; i++) {
	//	ExportMayaMeshAsHMS(m_hFile, *apcModels[i]);
	//}

	return true;
}


bool ExporterAsset::GatherMayaModels()
{
	unsigned int i;
	unsigned int n;
	
	//// Export materials
	//n = apcMaterials.size();
	//for (i = 0; i < n; i++) {
	//	ExportMayaMaterialAsHMS(m_hFile, rsPath, *apcMaterials[i]);
	//}

	// Export meshes
	n = apcModels.size();
	for (i = 0; i < n; i++) {
		MayaModel* model = mModels[i];
                
	}
}


ExporterAsset::ExporterAsset()
{
        GatherMayaModels();
        ExportMayaModels();

        engine::IEngine* iEngine;

        if(askInterfaceT("Engine", &iEngine)) {
                BSP* bsp = dynamic_cast<BSP*>(iEngine->createBSP("Land", Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f)));

                _bsps.push_back(bsp);
        }
}



void ExportBa(const char* fileName)
{
        int rcode = CoreInitEngine(0, 0, "", 0);
        if (rcode == 0) {
                engine::IEngine* iEngine;

                if(askInterfaceT("Engine", &iEngine)) {
                        IAsset* asset = iEngine->createAsset(atBinary, fileName);

                        ExporterAsset exporterAsset;

                        asset->seize(&exporterAsset);

                        asset->serialize();
                        asset->release();
                } else {
                        printf("Couldn't get engine interface.");
                }
        }
        rcode = CoreShutdownEngine(0);
}

void main() {
                ExportBa("test.ba");
}
