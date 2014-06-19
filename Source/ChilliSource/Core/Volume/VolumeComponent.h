/*
 *  VolumeComponent.h
 *  MoFlow
 *
 *  Created by Ian Copland on 09/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_CORE_VOLUME_COMPONENT_H_
#define _CHILLISOURCE_CORE_VOLUME_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
    namespace Core
    {
        //====================================================
        /// Description:
        ///
        /// Any object that has a AABB, OOBB or Bounding
        /// Sphere.
        //====================================================
        class VolumeComponent : public Core::Component
        {
        public:
            CS_DECLARE_NAMEDTYPE(VolumeComponent);
            VolumeComponent() : mfQueryIntersectionValue(0.0f){};
            virtual ~VolumeComponent(){}

            //----------------------------------------------------
            /// Get Axis Aligned Bounding Box
            ///
            /// All render components have a box used for culling
            /// and coarse intersections. This is cached and
            /// recomputed when required.
            /// @return Axis aligned bounding box
            //----------------------------------------------------
            virtual const Core::AABB& GetAABB() = 0;
            //----------------------------------------------------
            /// Get Object Oriented Bounding Box
            ///
            /// All render objects have an OOBB for
            /// picking. This is cached and
            /// recomputed when required.
            /// @return OOBB
            //----------------------------------------------------
            virtual const Core::OOBB& GetOOBB() = 0;
            //----------------------------------------------------
            /// Get Bounding Sphere
            ///
            /// All render objects have an bounding sphere for
            /// culling. This is cached and
            /// recomputed when required.
            /// @return bounding sphere
            //----------------------------------------------------
            virtual const Core::Sphere& GetBoundingSphere() = 0;
            //----------------------------------------------------
            /// Is Visible
            ///
            /// @return Whether or not to render
            //----------------------------------------------------
            virtual bool IsVisible() const = 0;

        public:
            f32 mfQueryIntersectionValue;
        };
    }
}

#endif
