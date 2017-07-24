#include "QtFileManager.h"

#include <RendererRuntime/Core/Platform/PlatformTypes.h>
#include <RendererRuntime/Core/File/IFile.h>

#include <QFile>
#include <QDir>

#include <cassert>
#include <iostream>


namespace detail
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class QtFile : public RendererRuntime::IFile
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit QtFile(QString filename) :
			mQtFile(filename)
		{
			bool result = mQtFile.open(QIODevice::ReadOnly);
			if (result)
			{
				int t = 0;
			}
			else
			{
				std::cerr<<"Could not open file: "<<filename.toUtf8().data()<<'\n';
			}
		}

		virtual ~QtFile()
		{
			// Nothing here
		}

		bool isInvalid() const
		{
			return !mQtFile.isOpen();
		}


	//[-------------------------------------------------------]
	//[ Public virtual RendererRuntime::IFile methods         ]
	//[-------------------------------------------------------]
	public:
		virtual size_t getNumberOfBytes() override
		{
			return static_cast<size_t>(mQtFile.size());
		}

		virtual void read(void* destinationBuffer, size_t numberOfBytes) override
		{
			qint64 readedFiles = mQtFile.read(reinterpret_cast<char*>(destinationBuffer), static_cast<qint64>(numberOfBytes));
			if (readedFiles < 0)
			{
				// Error
				std::cerr<<"Error reading data from file"<<mQtFile.fileName().toUtf8().data()<<'\n';
			}
			if (static_cast<size_t>(readedFiles) != numberOfBytes)
			{
				std::cerr<<"Error reading data from file: "<<mQtFile.fileName().toUtf8().data()<<": size does not match"<<'\n';
				size_t t = getNumberOfBytes();
				int a = 0;
			}
		}

		virtual void skip(size_t numberOfBytes) override
		{
			mQtFile.seek(mQtFile.pos() + static_cast<qint64>(numberOfBytes));
		}

		virtual void write(const void* sourceBuffer, size_t numberOfBytes) override
		{
			// Unsupported
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		QtFile(const QtFile&) = delete;
		QtFile& operator=(const QtFile&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QFile mQtFile;


	};


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
} // detail


//[-------------------------------------------------------]
//[ Public virtual RendererRuntime::IFileManager methods  ]
//[-------------------------------------------------------]
const char* QtFileManager::getAbsoluteLocalDataDirectoryName() const
{
	return mAbsoluteLocalDataDirectoryName.c_str();
}

void QtFileManager::createDirectories(const char* directoryName) const
{
	std::cout<<"Wants to create dir:"<<directoryName<<'\n';
	// Unsupported
}

bool QtFileManager::doesFileExist(const char* filename) const
{
	assert(nullptr != filename);

	QString qFileName(filename);

#ifdef ANDROID
    qFileName.replace("../DataMobile", "assets:/DataMobile");
#else
	qFileName.replace("../DataPc", "DataPc");
#endif
	return QFileInfo::exists(qFileName);
}

RendererRuntime::IFile* QtFileManager::openFile(QtFileManager::FileMode fileMode, const char* filename) const
{
	assert(nullptr != filename);

	QString qFileName(filename);

#ifdef ANDROID
    qFileName.replace("../DataMobile", "assets:/DataMobile");
#else
	qFileName.replace("../DataPc", "DataPc");
#endif
	qFileName.prepend(QString::fromUtf8(mAbsoluteLocalDataDirectoryName.c_str())+'/');

	std::cout<<"try open file: "<<qFileName.toUtf8().data()<<'\n';
	detail::QtFile* file = new detail::QtFile(qFileName);
	if (file->isInvalid())
	{
		std::cerr<<"Failed to open file "<<filename<<'\n';
		delete file;
		file = nullptr;
	}
	return file;
}

void QtFileManager::closeFile(RendererRuntime::IFile& file) const
{
	delete static_cast<detail::QtFile*>(&file);
}


//[-------------------------------------------------------]
//[ Protected methods                                     ]
//[-------------------------------------------------------]
QtFileManager::QtFileManager() :
	mAbsoluteLocalDataDirectoryName(QDir::currentPath().toUtf8().data())
{
	
	// Nothing here
}

QtFileManager::~QtFileManager()
{
	// Nothing here
}
