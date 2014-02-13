//
//  SceneAnimDescManager.h
//  MoshiMonsters
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneAnimDescManager_h
#define MoshiMonsters_SceneAnimDescManager_h

#include <ChilliSource/Core/ResourceManager.h>
#include <ChilliSource/Core/SceneAnimation/SceneAnimation.h>

namespace  moFlo 
{
    namespace Core
    {
        class CSceneAnimDescManager : public IResourceManager
        {
        public:
            DECLARE_NAMED_INTERFACE(CSceneAnimDescManager);
            
            bool IsA(InterfaceIDType inInterfaceID) const;
            
            InterfaceIDType GetResourceType() const;
            InterfaceIDType GetProviderType() const;
            
            bool ManagesResourceOfType(InterfaceIDType inInterfaceID) const;
            
            SceneAnimationPtr GetSceneAnimationFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
            
            SceneAnimationPtr AsyncGetSceneAnimationFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
            
            ResourcePtr GetResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
            
            ResourcePtr AsyncGetResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
            
        };
    }
}

#endif
