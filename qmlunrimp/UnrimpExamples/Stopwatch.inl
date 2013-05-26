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
//[ Public methods                                        ]
//[-------------------------------------------------------]
inline Stopwatch::Stopwatch() :
	mRunning(false),
	mStart(0),
	mStop(0)
{
	// Nothing to do in here
}

inline Stopwatch::Stopwatch(bool startAtOnce) :
	mRunning(false),
	mStart(0),
	mStop(0)
{
	if (startAtOnce)
	{
		start();
	}
}

inline Stopwatch::~Stopwatch()
{
	// Nothing to do in here
}

inline void Stopwatch::start()
{
	mRunning = true;
	mStart   = getSystemMicroseconds();
}

inline unsigned long Stopwatch::stop()
{
	// Is the stopwatch currently running?
	if (mRunning)
	{
		mStop    = getSystemMicroseconds();
		mRunning = false;
		return mStop - mStart;
	}
	else
	{
		return 0;
	}
}

inline float Stopwatch::getWeeks() const
{
	return getDays() / 7.0f;
}

inline float Stopwatch::getDays() const
{
	return getHours() / 24.0f;
}

inline float Stopwatch::getHours() const
{
	return getMinutes() / 60.0f;
}

inline float Stopwatch::getMinutes() const
{
	return getSeconds() / 60.0f;
}

inline float Stopwatch::getSeconds() const
{
	return getMilliseconds() / 1000.0f;
}

inline float Stopwatch::getMilliseconds() const
{
	return getMicroseconds() / 1000.0f;
}

inline unsigned long Stopwatch::getMicroseconds() const
{
	return mRunning ? (getSystemMicroseconds() - mStart) : (mStop - mStart);
}
