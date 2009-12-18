/**
 * This source code is a part of Metathrone game project. 
 * (c) Perfect Play 2003.
 *
 * @author Sergey Alekhin
 */

#include "headers.h"
#include "ComponentMgr.h"
#include "CoreImpl.h"
#include "CCorComp.h"


namespace ccor {


std::vector<IComponent*>* gStaticComponents = 0;

void registerStaticComponent(IComponent* component)
{
        if (component == 0) {
                return;
        }

        if (gStaticComponents == 0) {
                gStaticComponents = new std::vector<IComponent*>();
        }
        gStaticComponents->push_back(component);
}


extern "C" __declspec(dllexport) IComponent * __cdecl coreInitEngineComponent();
extern "C" __declspec(dllexport) IComponent * __cdecl coreInitCoreLogicComponent();
extern "C" __declspec(dllexport) IComponent * __cdecl coreInitMainWndComponent();

void ComponentMgr::loadStaticComponents() {
        coreInitEngineComponent();
        coreInitCoreLogicComponent();
        coreInitMainWndComponent();

    ComponentChunk cc;
    cc.name     = "static$CoreComponent";
    cc.instance = NULL;
    cc.comp     = coreInitCoreComponent(SingleCore::getInstance());;
    cc.typeInfo = cc.comp->getTypeInfo();
    addComponent(cc);

    int i;
    int count = gStaticComponents->size();
    for (i = 0; i < count; ++i) {
            IComponent* component = (*gStaticComponents)[i];

            cc.name     = component->getComponentName();
            cc.instance = NULL;
            cc.comp     = component;
            cc.typeInfo = component->getTypeInfo();
            addComponent(cc);
    }
}

}
