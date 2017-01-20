#include "QtFileManager.h"

#include <RendererRuntime/Core/Platform/PlatformTypes.h>
#include <RendererRuntime/Core/File/IFile.h>

#include <QFile>

#include <cassert>


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
			mQtFile.open(QIODevice::ReadOnly);
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
			mQtFile.read(reinterpret_cast<char*>(destinationBuffer), static_cast<qint64>(numberOfBytes));
		}

		virtual void skip(size_t numberOfBytes) override
		{
			mQtFile.seek(mQtFile.pos() + static_cast<qint64>(numberOfBytes));
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
RendererRuntime::IFile* QtFileManager::openFile(const char* filename)
{
	assert(nullptr != filename);
	
	QString qFileName(filename);

#ifdef ANDROID
    qFileName.replace("../DataMobile", "assets:/DataMobile");
#else
	qFileName.replace("../DataPc", "android/assets/DataPc");
#endif
	
	detail::QtFile* file = new detail::QtFile(qFileName);
	if (file->isInvalid())
	{
		RENDERERRUNTIME_OUTPUT_ERROR_PRINTF("Failed to open file %s", filename);
		delete file;
		file = nullptr;
	}
	return file;
}

void QtFileManager::closeFile(RendererRuntime::IFile& file)
{
	delete static_cast<detail::QtFile*>(&file);
}


//[-------------------------------------------------------]
//[ Protected methods                                     ]
//[-------------------------------------------------------]
QtFileManager::QtFileManager()
{
	// Nothing here
}

QtFileManager::~QtFileManager()
{
	// Nothing here
}
