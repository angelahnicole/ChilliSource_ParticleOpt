//
//  IDrawable.cpp
//  Chilli Source
//  Created by Scott Downie on 18/04/2014.
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

#include <ChilliSource/UI/Drawable/IDrawable.h>

#include <ChilliSource/UI/Drawable/DrawableDesc.h>
#include <ChilliSource/UI/Drawable/NinePatchDrawable.h>
#include <ChilliSource/UI/Drawable/StandardDrawable.h>
#include <ChilliSource/UI/Drawable/ThreePatchDrawable.h>

namespace ChilliSource
{
    namespace UI
    {
        //---------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        IDrawableSPtr IDrawable::Create(const DrawableDesc& in_desc)
        {
            switch(in_desc.GetType())
            {
                case DrawableType::k_standard:
                    return std::make_shared<StandardDrawable>(in_desc);
                case DrawableType::k_ninePatch:
                    return std::make_shared<NinePatchDrawable>(in_desc);
                case DrawableType::k_threePatch:
                    return std::make_shared<ThreePatchDrawable>(in_desc);
                default:
                    CS_LOG_FATAL("Cannot create a widget drawable instance: invalid type.");
                    return nullptr;
            }
        }
    }
}