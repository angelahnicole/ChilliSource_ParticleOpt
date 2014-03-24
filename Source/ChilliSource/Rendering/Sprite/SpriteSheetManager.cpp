/*
 * File: SpriteManager.cpp
 * Date: 22/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(SpriteSheetManager);
		
		//-----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------------------------------
		SpriteSheetManager::SpriteSheetManager() : mpTextureManager(nullptr)
		{
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool SpriteSheetManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SpriteSheetManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType SpriteSheetManager::GetResourceType() const
		{
			return SpriteSheet::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool SpriteSheetManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SpriteSheet::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType SpriteSheetManager::GetProviderType() const
		{
			return SpriteSheet::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceSPtr SpriteSheetManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			//It's the texture that is passed in so we need to load the binary file
            std::string strSpriteSheetFile;
            std::string strExt;
            Core::StringUtils::SplitBaseFilename(instrFilePath, strSpriteSheetFile, strExt);
            strSpriteSheetFile += ".bin";
            
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(strSpriteSheetFile);
			
			if(pExistingResource == mMapFilenameToResource.end())
			{
				Core::ResourceSPtr pResource(new SpriteSheet());
				
				for(size_t nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, strSpriteSheetFile, pResource))
					{
						//Add it to the cache
						CS_LOG_DEBUG("Loading sprite data " + strSpriteSheetFile);
						mMapFilenameToResource.insert(std::make_pair(strSpriteSheetFile, pResource));
						
						SpriteSheetSPtr pSpriteSheet = std::static_pointer_cast<SpriteSheet>(pResource);
						pSpriteSheet->SetName(strSpriteSheetFile);
						pSpriteSheet->SetFilename(instrFilePath);
						pSpriteSheet->SetStorageLocation(ineStorageLocation);
						pSpriteSheet->SetOwningResourceManager(this);
						pSpriteSheet->SetLoaded(true);
                        
                        if(!mpTextureManager)
                        {
                            mpTextureManager = Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<TextureManager>();
                        }
                        
                        TextureSPtr pTexture = LOAD_RESOURCE(Texture, ineStorageLocation, instrFilePath);
                        pSpriteSheet->SetTexture(pTexture);
                        
						return pSpriteSheet;
					}
				}
			}
			else
			{
				return pExistingResource->second;
			}
			
			//Resource not found
			CS_LOG_ERROR("Cannot find resource for sprite data with path " + instrFilePath);
			return nullptr;
		}
	}
}
