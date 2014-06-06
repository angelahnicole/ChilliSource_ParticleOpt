//
//  UnifiedCoordinates.cpp
//  moFloTest
//
//  Created by Scott Downie on 22/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Math/UnifiedCoordinates.h>

namespace ChilliSource
{
    namespace Core
    {
		const UnifiedVector2 UnifiedVector2::ZERO(Core::Vector2::k_zero, Core::Vector2::k_zero);
        
        //------------------------------------------------------
        /// Get Absolute
        ///
        /// @return Absolute vector
        //------------------------------------------------------
        const Core::Vector2& UnifiedVector2::GetAbsolute() const
        {
            return vAbsolute;
        }
        //------------------------------------------------------
        /// Get Relative
        ///
        /// @return Relative vector
        //------------------------------------------------------
        const Core::Vector2& UnifiedVector2::GetRelative() const
        {
            return vRelative;
        }
    }
}
