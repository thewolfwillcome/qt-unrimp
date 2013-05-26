/*********************************************************\
 * Copyright (c) 2012-2013 Christian Ofenberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Stopwatch.h"
#ifdef WIN32
	#include "windows.h"
#elif defined LINUX
	#include <sys/time.h>
#endif


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
unsigned int Stopwatch::getSystemMicroseconds() const
{
	#ifdef WIN32
		// Frequency of the performance counter
		LARGE_INTEGER performanceFrequency;
		::QueryPerformanceFrequency(&performanceFrequency);

		// Get past time
		LARGE_INTEGER curTime;
		::QueryPerformanceCounter(&curTime);
		double newTicks = static_cast<double>(curTime.QuadPart);

		// Scale by 1000000 in order to get microsecond precision
		newTicks *= static_cast<double>(1000000.0)/static_cast<double>(performanceFrequency.QuadPart);

		// Return past time
		return static_cast<unsigned int>(newTicks);
	#elif defined LINUX
		struct timeval now;
		gettimeofday(&now, nullptr);
		return static_cast<unsigned int>(now.tv_sec*1000000 + now.tv_usec);
	#else
		#error "Unsupported platform"
	#endif
}
