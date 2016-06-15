//
//  ForwardDeclarations.h
//  CSProfiling
//  Created by Angela Gross 31/03/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CSPROFILING_FORWARDDECLARATIONS_H_
#define _CSPROFILING_FORWARDDECLARATIONS_H_

#include <ChilliSource/ChilliSource.h>

namespace CSProfiling
{
	CS_FORWARDDECLARE_CLASS(MetricsSystem);
	
}

//------------------------------------------------------------
/// Android: std::to_string and std::put_time is not defined
/// so we have to define it
//------------------------------------------------------------
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
//------------------------------------------------------------
#if defined(CS_TARGETPLATFORM_ANDROID)
//------------------------------------------------------------
#define TO_STRING to_stringAndroid
template <typename T>
inline std::string to_stringAndroid(T in_value)
{
    std::ostringstream os;
    os << in_value;
    return os.str();
}
#define PUT_TIME put_timeAndroid
inline std::string put_timeAndroid(const struct tm* in_tm, const char* in_fmt)
{
	auto tmb = mktime(in_tm);
    std::ostringstream os;
    char buffer[100];
    if (std::strftime(buffer, sizeof(buffer), in_fmt, std::localtime(tmb)))
    {
        os << buffer;
    }
    else
    {
        os << "NULL";
    }
    return os.str();
}
//------------------------------------------------------------
#else
#define TO_STRING std::to_string
#define PUT_TIME std::put_time
#endif
//------------------------------------------------------------

#endif
