//
//  Screen.mm
//  Chilli Source
//  Created by Ian Copland on 25/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Core/Base/Screen.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        namespace
        {
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @param The orientation of the application.
            ///
            /// @return The iOS device resolution.
            //----------------------------------------------------
            CSCore::Vector2 CalculateResolution(UIInterfaceOrientation in_orientation)
            {
                CGSize size = [[UIScreen mainScreen] bounds].size;
                f32 scale = [UIScreen mainScreen].scale;
                CSCore::Vector2 resolution(size.width * scale, size.height * scale);
                
                if (in_orientation == UIInterfaceOrientationLandscapeLeft || in_orientation == UIInterfaceOrientationLandscapeRight)
                {
                    resolution = CSCore::Vector2(resolution.y, resolution.x);
                }
                
                return resolution;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(Screen);
        //-------------------------------------------------------
        //-------------------------------------------------------
        Screen::Screen()
        {
            @autoreleasepool
            {
                m_resolution = CalculateResolution([UIApplication sharedApplication].keyWindow.rootViewController.interfaceOrientation);
                m_densityScale = [UIScreen mainScreen].scale;
                m_invDensityScale = 1.0f / m_densityScale;
            }
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Screen::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (CSCore::Screen::InterfaceID == in_interfaceId || Screen::InterfaceID == in_interfaceId);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const CSCore::Vector2& Screen::GetResolution() const
        {
            return m_resolution;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        f32 Screen::GetDensityScale() const
        {
            return m_densityScale;
        }
        //----------------------------------------------------------
        //-----------------------------------------------------------
        f32 Screen::GetInverseDensityScale() const
        {
            return m_invDensityScale;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        CSCore::IConnectableEvent<Screen::ResolutionChangedDelegate>& Screen::GetResolutionChangedEvent()
        {
            return m_resolutionChangedEvent;
        }
        //-----------------------------------------------------------
        //------------------------------------------------------------
        void Screen::OnOrientationChanged(UIInterfaceOrientation in_orientation)
        {
            m_resolution = CalculateResolution(in_orientation);
            m_resolutionChangedEvent.NotifyConnections(m_resolution);
        }
    }
}

#endif