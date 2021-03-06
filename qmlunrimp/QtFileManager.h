/*********************************************************\
 * Copyright (c) 2013-2017 Stephan Wezel
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


#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RendererRuntime/Core/File/IFileManager.h>

#include <string>


class QtFileManager : public RendererRuntime::IFileManager
{


//[-------------------------------------------------------]
//[ Friends                                               ]
//[-------------------------------------------------------]
	friend class ExampleApplicationFrontend;	// Manages the instance


//[-------------------------------------------------------]
//[ Public virtual RendererRuntime::IFileManager methods  ]
//[-------------------------------------------------------]
public:
	virtual const char* getAbsoluteLocalDataDirectoryName() const override;
	virtual void createDirectories(const char* directoryName) const override;
	virtual bool doesFileExist(const char* filename) const override;
	virtual RendererRuntime::IFile* openFile(FileMode fileMode, const char* filename) const override;
	virtual void closeFile(RendererRuntime::IFile& file) const override;


//[-------------------------------------------------------]
//[ Protected methods                                     ]
//[-------------------------------------------------------]
protected:
	QtFileManager();
	virtual ~QtFileManager();
	QtFileManager(const QtFileManager&) = delete;
	QtFileManager& operator=(const QtFileManager&) = delete;


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	const std::string mAbsoluteLocalDataDirectoryName;


};
