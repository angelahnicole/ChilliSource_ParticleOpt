//
//  RemoteNotificationSystem.h
//  MoFlow
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_CORE_NOTIFICATIONS_REMOTENOTIFICATIONSYSTEM_H_
#define _MOFLOW_CORE_NOTIFICATIONS_REMOTENOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
    {
        typedef std::function<void(const std::string&)> RemoteTokenReceivedDelegate;
		
        class RemoteNotificationSystem : public ChilliSource::Core::System
        {
        public:
            CS_DECLARE_NAMEDTYPE(RemoteNotificationSystem);
            //---------------------------------------------------------------
            /// Create
            ///
            /// Creates a new platform specfic instance of the remote
            /// notification system.
            //---------------------------------------------------------------
            static RemoteNotificationSystem* Create();
            //---------------------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the passed in interface id.
            //---------------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //-------------------------------------------------------------------------
            /// Request Remote Token
            ///
            /// Request the remote token for this device for use with Push Notifications.
            ///
            /// @param The return delegate.
            //-------------------------------------------------------------------------
            virtual void RequestRemoteToken(const RemoteTokenReceivedDelegate& inDelegate) = 0;
            //-------------------------------------------------------------------------
            /// Get Remote Token
            ///
            /// @return The unique token that the push notification provider uses to
            /// identify the device
            //-------------------------------------------------------------------------
            virtual const std::string& GetRemoteToken() const = 0;
            //-------------------------------------------------------------------------
            /// Get Provider ID
            ///
            /// @return The ID for the Apple remote notification service
            //-------------------------------------------------------------------------
            virtual const std::string& GetProviderID() const = 0;
        };
    }
}

#endif /* _MOFLOW_CORE_NOTIFICATIONS_REMOTENOTIFICATIONSYSTEM_H_ */
