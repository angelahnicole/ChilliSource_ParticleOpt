/*
 *  SkeletalAnimationManager.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.SkinnedAnimation
 *
 */

#include <ChilliSource/Rendering/Model/SkinnedAnimationManager.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Base/Application.h>


namespace ChilliSource
{
	namespace Rendering
	{
		
		CS_DEFINE_NAMEDTYPE(SkinnedAnimationManager);
		
		//-----------------------------------------------------------------
		/// Constructor
		//-----------------------------------------------------------------
		SkinnedAnimationManager::SkinnedAnimationManager()
		{
		}
		//----------------------------------------------------------------
		/// Is A
		//----------------------------------------------------------------
		bool SkinnedAnimationManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SkinnedAnimationManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		//----------------------------------------------------------------
		Core::InterfaceIDType SkinnedAnimationManager::GetResourceType() const
		{
			return SkinnedAnimation::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		//----------------------------------------------------------------
		bool SkinnedAnimationManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SkinnedAnimation::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		//----------------------------------------------------------------
		Core::InterfaceIDType SkinnedAnimationManager::GetProviderType() const
		{
			return SkinnedAnimation::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourceSPtr SkinnedAnimationManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetSkinnedAnimationFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourceSPtr SkinnedAnimationManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetSkinnedAnimationFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Get Skinned Animation From File
		//-----------------------------------------------------------------
		SkinnedAnimationSPtr SkinnedAnimationManager::GetSkinnedAnimationFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceSPtr pResource(new SkinnedAnimation());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::StringUtils::SplitBaseFilename(instrFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource)) 
						{
							//Add it to the cache
							CS_LOG_DEBUG("Loading skinned animation " + instrFilePath);
							mMapFilenameToResource.insert(std::make_pair(instrFilePath , pResource));
							pResource->SetName(instrFilePath);
							pResource->SetFilename(instrFilePath);
							pResource->SetStorageLocation(ineStorageLocation);
							pResource->SetOwningResourceManager(static_cast<Core::ResourceManager*>(this));
							pResource->SetLoaded(true);
							
							return std::static_pointer_cast<SkinnedAnimation>(pResource);
						}
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<SkinnedAnimation>(pExistingResource->second);
			}
			
			//Resource not found
			CS_LOG_ERROR("Cannot find resource for skinned animation with path " + instrFilePath);
			return SkinnedAnimationSPtr();
		}
		//-----------------------------------------------------------------
		/// Async Get Skinned Animation From File
		//-----------------------------------------------------------------
		SkinnedAnimationSPtr SkinnedAnimationManager::AsyncGetSkinnedAnimationFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceSPtr pResource(new SkinnedAnimation());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::StringUtils::SplitBaseFilename(instrFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						
						CS_LOG_DEBUG("Loading skinned animation " + instrFilePath);
						
						pResource->SetName(instrFilePath);
						pResource->SetFilename(instrFilePath);
						pResource->SetStorageLocation(ineStorageLocation);
						pResource->SetOwningResourceManager(this);
						pResource->SetLoaded(false);
						
						if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, instrFilePath, pResource)) 
						{
							//Add it to the cache
							mMapFilenameToResource.insert(std::make_pair(instrFilePath, pResource));
							return std::static_pointer_cast<SkinnedAnimation>(pResource);
						}
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<SkinnedAnimation>(pExistingResource->second);
			}
			
			//Resource not found
			CS_LOG_ERROR("Cannot find resource for skinned animation with path " + instrFilePath);
			return SkinnedAnimationSPtr();
		}
		//-----------------------------------------------------------------
		/// Destructor
		///
		//-----------------------------------------------------------------
		SkinnedAnimationManager::~SkinnedAnimationManager()
		{
		}
	}
}
