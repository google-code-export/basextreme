
class MayaMaterial;
class MayaModel;
class MString;


void ExportMayaMaterialAsHMS(FILE* hFile, MString& rsPath, MayaMaterial& rcMaterial);
bool ExportMayaMeshAsHMS(FILE* hFile, MayaModel& rcModel);
