//
//  Component.h
//  Chilli Source
//  Created by Ian Copland on 14/11/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_COMPONENT_H_
#define _CHILLISOURCE_UI_BASE_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/UI/Base/PropertyAccessor.h>

#include <unordered_map>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------
        /// A component which can be attached to a widget to provide additional
        /// functionality. Components are attached to a widget on construction
        /// and live for the entire life of the widget. A component def is used
        /// to create an instance of a widget.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        class Component : public Core::QueryableInterface
        {
        public:
            CS_DECLARE_NAMEDTYPE(Component);
            CS_DECLARE_NOCOPY(Component);
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the component instance.
            //----------------------------------------------------------------
            const std::string& GetName() const;
            //----------------------------------------------------------------
            /// Allows querying of whether or not the component has a property
            /// with the given name.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the property. This is case insensitive.
            ///
            /// @return Whether or not the property exists.
            //----------------------------------------------------------------
            bool HasProperty(const std::string& in_propertyName) const;
            //----------------------------------------------------------------
            /// Allows querying for the value of properties in the component
            /// using a string name. If there is no property with the given
            /// name or it is not of the requested type the app is considered
            /// to be in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the property to get. This is case insensitive.
            ///
            /// @return The value of the requested property.
            //----------------------------------------------------------------
            template <typename TPropertyType> TPropertyType GetProperty(const std::string& in_propertyName) const;
            //----------------------------------------------------------------
            /// Allows setting of properties in the component using a string
            /// name. If there is no property with the given name or it is not
            /// of the requested type the app is considered to be in an
            /// irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the property. This is case insensitive.
            /// @param The value of the property.
            //----------------------------------------------------------------
            template <typename TPropertyType> void SetProperty(const std::string& in_propertyName, TPropertyType&& in_propertyValue);
            //----------------------------------------------------------------
            /// Allows setting of properties in the component using a string
            /// name. If there is no property with the given name or it is not
            /// of the requested type the app is considered to be in an
            /// irrecoverable state and will terminate.
            ///
            /// This is an overload of the templated version of SetProperty
            /// which will convert a string literal to a std::string.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the property. This is case insensitive.
            /// @param The value of the property.
            //----------------------------------------------------------------
            void SetProperty(const std::string& in_propertyName, const char* in_propertyValue);
            //----------------------------------------------------------------
            /// Destructor
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual ~Component(){}
        protected:
            //----------------------------------------------------------------
            /// Constructor
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the component instance.
            //----------------------------------------------------------------
            Component(const std::string& in_name);
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A pointer to the widget that owns this component.
            //----------------------------------------------------------------
            Widget* GetWidget();
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A constant pointer to the widget that owns this
            /// component.
            //----------------------------------------------------------------
            const Widget* GetWidget() const;
            //----------------------------------------------------------------
            /// This registers a property such that it can be called using the
            /// set property method. This takes two function pointers, one
            /// for getting the value of a property and one for setting the
            /// value of a property. Property names must be unqiue; if a
            /// duplicate property name is registered the app is considered to
            /// be in an irrecoverable state and will terminate.
            ///
            /// This should only be called during the register properties
            /// lifecycle event.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the registered property. This is case
            /// insensitive.
            /// @param The getter delegate for the property.
            /// @param The setter delegate for the property.
            //----------------------------------------------------------------
            template <typename TPropertyType> void RegisterProperty(const std::string& in_name, const std::function<TPropertyType()>& in_getter, const std::function<void(TPropertyType&&)> in_setter);
            //----------------------------------------------------------------
            /// This is called prior to the On Init lifecycle event. This
            /// should be used to register all component properties.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnRegisterProperties() {}
            //----------------------------------------------------------------
            /// A method which is called when all components owned by the parent
            /// widget have been created and added. Inheriting classes should use
            /// this for any required initialisation.
            ///
            /// All registration of properties should occur at this stage.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnInit() {}
            //----------------------------------------------------------------
            /// This is called when the owning widget is added to the canvas.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnAddedToCanvas() {}
            //----------------------------------------------------------------
            /// This is called when the application is resumed while the owning
            /// widget is attached to the canvas. This is also also called
            /// when added to the canvas if the application is already active.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnResume() {}
            //----------------------------------------------------------------
            /// This is called when the application is foregrounded while the
            /// owning widget is attached to the canvas. This is also called
            /// when added to the canvas if the application is already
            /// foregrounded.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnForeground() {}
            //----------------------------------------------------------------
            /// This is called every frame while the application is active
            /// and the owning widget is on the canvas. This should be used
            /// for any update logic in the component.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time since the last update.
            //----------------------------------------------------------------
            virtual void OnUpdate(f32 in_deltaTime) {}
            //----------------------------------------------------------------
            /// This is called during the draw event whenever the application
            /// is active and the owning widget in on the canvas. This should
            /// be used for any rendering.
            ///
            /// @author Ian Copland
            ///
            /// @param The canvas renderer.
            //----------------------------------------------------------------
            virtual void Draw(Rendering::CanvasRenderer* in_renderer) {}
            //----------------------------------------------------------------
            /// This is called when the application is backgrounded while the
            /// owning widget is on the canvas. This will also be called when
            /// the owning widget is removed from the canvas if the application
            /// is foregrounded.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnBackground() {}
            //----------------------------------------------------------------
            /// This is called when the application is suspended while the
            /// owning widget is on the canvas. This will also be called when
            /// the owning widget is removed from the canvas if the application
            /// is active.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnSuspend() {}
            //----------------------------------------------------------------
            /// This is called when the owning widget is removed from the
            /// canvas.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnRemovedFromCanvas() {}
            //----------------------------------------------------------------
            /// This is called when the component--and the owning widget--is
            /// about to be destroyed. Components are destroyed in reverse
            /// initialisation order.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnDestroy() {}
        private:
            friend class Widget;
            //----------------------------------------------------------------
            /// Sets the owning widget. This should be called by the owning
            /// widget when it is first added, prior to calling OnInit().
            ///
            /// @author Ian Copland
            ///
            /// @param The owning widget.
            //----------------------------------------------------------------
            void SetWidget(Widget* in_widget);
            //----------------------------------------------------------------
            /// Enables property registration then calls down to the subclass
            /// to register its properties then again disables registation.
            /// This can only be called once and should be called between setting
            /// the owning widget and calling OnInit.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning widget.
            //----------------------------------------------------------------
            void RegisterProperties();

            bool m_propertyRegistrationEnabled = false;
            std::unordered_map<std::string, IPropertyAccessorUPtr> m_properties;
            Widget* m_widget = nullptr;
            std::string m_name;
        };
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        template <typename TPropertyType> TPropertyType Component::GetProperty(const std::string& in_propertyName) const
        {
            std::string lowerPropertyName = in_propertyName;
            Core::StringUtils::ToLowerCase(lowerPropertyName);
            
            auto it = m_properties.find(lowerPropertyName);
            if(it == m_properties.end())
            {
                CS_LOG_FATAL("Cannot find property with name '" + in_propertyName + "' in UI::Component.");
            }
            
            auto accessor = CS_SMARTCAST(PropertyAccessor<TPropertyType>*, it->second.get());
            return accessor->Get();
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        template <typename TPropertyType> void Component::SetProperty(const std::string& in_propertyName, TPropertyType&& in_propertyValue)
        {
            std::string lowerPropertyName = in_propertyName;
            Core::StringUtils::ToLowerCase(lowerPropertyName);
            
            auto it = m_properties.find(lowerPropertyName);
            if(it == m_properties.end())
            {
                CS_LOG_FATAL("Cannot find property with name '" + in_propertyName + "' in UI::Component.");
            }
            
            auto accessor = CS_SMARTCAST(PropertyAccessor<TPropertyType>*, it->second.get());
            accessor->Set(std::forward<TPropertyType>(in_propertyValue));
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        template <typename TPropertyType> void Component::RegisterProperty(const std::string& in_name, const std::function<TPropertyType()>& in_getter, const std::function<void(TPropertyType&&)> in_setter)
        {
            CS_ASSERT(m_propertyRegistrationEnabled == false, "UI::Component properties can only be registered during the OnRegisterProperties lifecycle event.");
            
            std::string lowerPropertyName = in_name;
            Core::StringUtils::ToLowerCase(lowerPropertyName);
            
            CS_ASSERT(m_properties.find(lowerPropertyName) == m_properties.end(), "Cannot register duplicate property name '" + in_name + "' in a UI::Component.");
            
            m_properties.emplace(lowerPropertyName, IPropertyAccessorUPtr(new PropertyAccessor<TPropertyType>(in_setter, in_getter)));
        }
    }
}

#endif
