/**
 * CFacebookJavaInterface.cpp
 * moFlow
 *
 * Created by Robert Henning on 01/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#include <ChilliSource/Platform/Android/Social/Facebook/FacebookJavaInterface.h>
#include <ChilliSource/Platform/Android/Social/Facebook/FacebookAuthentication.h>
#include <ChilliSource/Platform/Android/Social/Facebook/FacebookPostSystem.h>

#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceManager.h>

#include <ChilliSource/Core/TaskScheduler.h>

#include <jni.h>

moFlo::AndroidPlatform::CFacebookAuthenticationSystem* gpAndroidAuthSystem = NULL;
moFlo::AndroidPlatform::CFacebookPostSystem* gpAndroidPostSystem = NULL;

//function definitions
extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnAuthenticationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
	void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnReadAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
	void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnWriteAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
	void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnPostToFeedComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
	void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnPostRequestComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
}

//------------------------------------------------------------
// (Java) OnAuthenticationComplete
//
// Calls when authentication has been achieved either through login or an existing session
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authentication
//------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnAuthenticationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidAuthSystem)
	{
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<bool>(gpAndroidAuthSystem, &moFlo::AndroidPlatform::CFacebookAuthenticationSystem::OnAuthenticationComplete, inbSuccess));
	}
}
//------------------------------------------------------------
// (Java) OnReadAuthorisationComplete
//
// Calls when the read permissions have been authorised via facebook
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authorised
//------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnReadAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidAuthSystem)
	{
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<bool>(gpAndroidAuthSystem, &moFlo::AndroidPlatform::CFacebookAuthenticationSystem::OnAuthoriseReadPermissionsComplete, inbSuccess));
	}
}
//------------------------------------------------------------
// (Java) OnWriteAuthorisationComplete
//
// Calls when the write permissions have been authorised via facebook
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authorised
//------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnWriteAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidAuthSystem)
	{
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<bool>(gpAndroidAuthSystem, &moFlo::AndroidPlatform::CFacebookAuthenticationSystem::OnAuthoriseWritePermissionsComplete, inbSuccess));
	}
}
//------------------------------------------------------------
// (Java) OnPostToFeedComplete
//
// Calls when the results of the attempt to post to feed are received
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authorised
//------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnPostToFeedComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidPostSystem)
	{
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<bool>(gpAndroidPostSystem, &moFlo::AndroidPlatform::CFacebookPostSystem::OnPostToFeedComplete, inbSuccess));
	}
}
//------------------------------------------------------------
// (Java) OnPostRequestComplete
//
// Calls when the results of the attempt to post a request are received
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authorised
//------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CFacebookNativeInterface_OnPostRequestComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidPostSystem)
	{
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<bool>(gpAndroidPostSystem, &moFlo::AndroidPlatform::CFacebookPostSystem::OnPostRequestComplete, inbSuccess));
	}
}


namespace moFlo
{
	namespace AndroidPlatform
	{
		DEFINE_NAMED_INTERFACE(CFacebookJavaInterface);

		CFacebookJavaInterface::CFacebookJavaInterface()
		{
			CreateNativeInterface("com/taggames/moflow/nativeinterface/CFacebookNativeInterface");

			CreateMethodReference("Authenticate", "([Ljava/lang/String;)V");
			CreateMethodReference("IsSignedIn", "()Z");
			CreateMethodReference("GetActiveToken", "()Ljava/lang/String;");
			CreateMethodReference("HasPermission", "(Ljava/lang/String;)Z");
			CreateMethodReference("AuthoriseReadPermissions", "([Ljava/lang/String;)V");
			CreateMethodReference("AuthoriseWritePermissions", "([Ljava/lang/String;)V");
			CreateMethodReference("SignOut", "()V");
			CreateMethodReference("MakePostToFeedRequest", "(Ljava/lang/String;[Ljava/lang/String;)V");
			CreateMethodReference("PublishInstall", "()V");
			CreateMethodReference("MakeRequestToUser", "([Ljava/lang/String;)V");
		}

		bool CFacebookJavaInterface::IsA(moCore::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CFacebookJavaInterface::InterfaceID;
		}

		//--------------------------------------------------------------------------------------
		/// Set Authentication System
		///
		/// Used to callback into the auth system
		///
		/// @param Auth system
		//--------------------------------------------------------------------------------------
		void CFacebookJavaInterface::SetAuthenticationSystem(CFacebookAuthenticationSystem* inpAuthSystem)
		{
			gpAndroidAuthSystem = inpAuthSystem;
		}
		//--------------------------------------------------------------------------------------
		/// Set Post System
		///
		/// Used to callback into the post system
		///
		/// @param Post system
		//--------------------------------------------------------------------------------------
		void CFacebookJavaInterface::SetPostSystem(CFacebookPostSystem* inpPostSystem)
		{
			gpAndroidPostSystem = inpPostSystem;
		}
		//--------------------------------------------------------------------------------------
		/// Authenticate
		///
		/// Attempt to open an existing session or prompt the user to login if no
		/// session exists
		//--------------------------------------------------------------------------------------
		void CFacebookJavaInterface::Authenticate(const DYNAMIC_ARRAY<std::string>& inaReadPerms)
		{
			JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaPermissions = pEnv->NewObjectArray(inaReadPerms.size(), jStringClass, jstrEmptyString);
	    	u32 udwPermissionCount = 0;
	    	for(DYNAMIC_ARRAY<std::string>::const_iterator it = inaReadPerms.begin(); it != inaReadPerms.end(); ++it)
	    	{
	    		jstring jstrPermission = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaPermissions, udwPermissionCount, jstrPermission);
	    		pEnv->DeleteLocalRef(jstrPermission);
	    		++udwPermissionCount;
	    	}

	    	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Authenticate"), jaPermissions);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jaPermissions);
		}
		//--------------------------------------------------------------------------------------
		/// Is Signed In
		///
		/// @return Whether a session exists and is open
		//--------------------------------------------------------------------------------------
		bool CFacebookJavaInterface::IsSignedIn()
		{
			JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsSignedIn"));
		}
		//--------------------------------------------------------------------------------------
		/// Get Active Token
		///
		/// @return The access token of the active session
		//--------------------------------------------------------------------------------------
		std::string CFacebookJavaInterface::GetActiveToken()
		{
			JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrToken = (jstring)pEnv->CallObjectMethod(GetJavaObject(), GetMethodID("GetActiveToken"));
			std::string strToken = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(jstrToken);
			pEnv->DeleteLocalRef(jstrToken);
			return strToken;
		}
		//--------------------------------------------------------------------------------------
		/// Has Permission
		///
		/// @param Permission ID
	    /// @return Whether the current session user has given the permission
		//--------------------------------------------------------------------------------------
	    bool CFacebookJavaInterface::HasPermission(const std::string& instrPermission)
	    {
			JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrPermission = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateJStringFromSTDString(instrPermission);
			bool bHasPermission = pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("HasPermission"), jstrPermission);
			pEnv->DeleteLocalRef(jstrPermission);
			return bHasPermission;
	    }
		//--------------------------------------------------------------------------------------
		/// Authorise Read Permissions
		///
	    /// Prompt Facebook to ask the users to grant the given permissions.
	    /// Calls into native with the result
	    ///
		/// @param List of read permissions IDs
		//--------------------------------------------------------------------------------------
	    void CFacebookJavaInterface::AuthoriseReadPermissions(const DYNAMIC_ARRAY<std::string>& inaReadPerms)
	    {
	    	JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaPermissions = pEnv->NewObjectArray(inaReadPerms.size(), jStringClass, jstrEmptyString);
	    	u32 udwPermissionCount = 0;
	    	for(DYNAMIC_ARRAY<std::string>::const_iterator it = inaReadPerms.begin(); it != inaReadPerms.end(); ++it)
	    	{
	    		jstring jstrPermission = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaPermissions, udwPermissionCount, jstrPermission);
	    		pEnv->DeleteLocalRef(jstrPermission);
	    		++udwPermissionCount;
	    	}

	    	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("AuthoriseReadPermissions"), jaPermissions);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jaPermissions);
	    }
		//--------------------------------------------------------------------------------------
		/// Authorise Write Permissions
		///
	    /// Prompt Facebook to ask the users to grant the given permissions.
	    /// Calls into native with the result
	    ///
		/// @param List of write permissions IDs
		//--------------------------------------------------------------------------------------
	    void CFacebookJavaInterface::AuthoriseWritePermissions(const DYNAMIC_ARRAY<std::string>& inaWritePerms)
	    {
	    	JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaPermissions = pEnv->NewObjectArray(inaWritePerms.size(), jStringClass, jstrEmptyString);
	    	u32 udwPermissionCount = 0;
	    	for(DYNAMIC_ARRAY<std::string>::const_iterator it = inaWritePerms.begin(); it != inaWritePerms.end(); ++it)
	    	{
	    		jstring jstrPermission = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaPermissions, udwPermissionCount, jstrPermission);
	    		pEnv->DeleteLocalRef(jstrPermission);
	    		++udwPermissionCount;
	    	}

	    	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("AuthoriseWritePermissions"), jaPermissions);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jaPermissions);
	    }
		//--------------------------------------------------------------------------------------
		/// Sign Out
		///
	    /// If a session exists log the user out and destroy any cached tokens
		//--------------------------------------------------------------------------------------
	    void CFacebookJavaInterface::SignOut()
	    {
			JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SignOut"));
	    }
		//--------------------------------------------------------------------------------------
		/// Try Post To Feed
		///
	    /// Attempt to make a Graph API request to post to feed with the following
	    /// path and key value array
		//--------------------------------------------------------------------------------------
	    void CFacebookJavaInterface::TryPostToFeed(const std::string& instrGraphPath, const DYNAMIC_ARRAY<std::string>& inaKeyValues)
	    {
	    	JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaKeyValues = pEnv->NewObjectArray(inaKeyValues.size(), jStringClass, jstrEmptyString);
	    	u32 i = 0;
	    	for(DYNAMIC_ARRAY<std::string>::const_iterator it = inaKeyValues.begin(); it != inaKeyValues.end(); ++it, ++i)
	    	{
	    		jstring jstrElement = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaKeyValues, i, jstrElement);
	    		pEnv->DeleteLocalRef(jstrElement);
	    	}

    		jstring jstrGraphPath = JavaInterfaceUtils::CreateJStringFromSTDString(instrGraphPath);
    		pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("MakePostToFeedRequest"), jstrGraphPath, jaKeyValues);
	    	pEnv->DeleteLocalRef(jaKeyValues);
	    	pEnv->DeleteLocalRef(jstrGraphPath);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    }
		//--------------------------------------------------------------------------------------
		/// Try Post To Feed
		///
	    /// Attempt to make a Graph API request to post to feed with the following
	    /// path and key value array
		//--------------------------------------------------------------------------------------
	    void CFacebookJavaInterface::TryPostRequest(const DYNAMIC_ARRAY<std::string>& inaKeyValues)
	    {
	    	JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaKeyValues = pEnv->NewObjectArray(inaKeyValues.size(), jStringClass, jstrEmptyString);
	    	u32 i = 0;
	    	for(DYNAMIC_ARRAY<std::string>::const_iterator it = inaKeyValues.begin(); it != inaKeyValues.end(); ++it, ++i)
	    	{
	    		jstring jstrElement = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaKeyValues, i, jstrElement);
	    		pEnv->DeleteLocalRef(jstrElement);
	    	}

    		pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("MakeRequestToUser"), jaKeyValues);
	    	pEnv->DeleteLocalRef(jaKeyValues);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    }
		//--------------------------------------------------------------------------------------
		/// Publish Install
		///
	    /// Inform FB about the app launch for the marketing SDK
		//--------------------------------------------------------------------------------------
	    void CFacebookJavaInterface::PublishInstall()
	    {
			JNIEnv* pEnv = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("PublishInstall"));
	    }
	}
}
