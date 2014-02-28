//
//  SceneProvider.h
//  moFlo
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneProvider_h
#define MoshiMonsters_SceneProvider_h

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource 
{
    namespace Core
    {
        class SceneProvider : public ChilliSource::Core::ResourceProvider
        {
        public:
            CS_DECLARE_NAMEDTYPE(SceneProvider);
            
            bool IsA(InterfaceIDType inInterface) const override;
            
            bool CanCreateResourceOfKind(InterfaceIDType inInterfaceID) const override;
            bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const override;
            
            //---Filepath is relative to the resources directory - either the documents or the package
            bool CreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource) override;
            bool AsyncCreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource) override;
            
        private:
            bool LoadMoScene(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource);
            void LoadAsyncMoScene(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource);
            
        };
    }
}






#endif
