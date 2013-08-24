/*********************************************************\
 * Copyright (c) 2013-2013 Stephan Wezel
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


#include "UnrimpExamples/TextureFromFileLoader.h"
#include <Renderer/Renderer.h>
#include <QImage>

/// Converts the image data of an QImage into a OpenGL compatible image format: GL_RGBA(32Bit) and mirrored on the x-axis.
/// The image data is first converted to ARGB32 (0xAARRGGBB for each pixel on little endian machines).
/// The byte order of GL_RGBA for each pixel is 0xAABBGGRR (on little endian machines)
/// -> the blue and red components must be swapped
/// Note: The returned QImage cannot be used as QImage because the data format isn't compatible anymore to any image drawing/manipulation functionality of Qt
///       (QImage is only used as an data container)
static QImage convertImageDataToOpenGLFormat(const QImage& image)
{
	QImage resultImage(image.size(), QImage::Format_ARGB32);
	// convert the image data to a 32Bit format
	const QImage orig = image.convertToFormat(QImage::Format_ARGB32);
	
	// The data is layouted continousely in format ARGB32 (0xAARRGGBB) for each pixel
	// we mirror the image on the x-axis so we start with the last scanline (lines of pixel) of the original image

	// get the pointer to the first pixelvalue in the last scanline.
	// Note: QImage::scanLine returns a pointer to the first byte of the first pixel value but we want to work on a complete pixel value so we cast the 8bit pointer to a 32Bit pointer (due the continouse layout of the image data this cast is safe)
	const uint32_t *currentImagetData = reinterpret_cast<const uint32_t*>(orig.scanLine(image.height()-1));
	// get the pointer to the first pixel value in the first scanline.
	// Note: QImage::scanLine returns a pointer to the first byte of the first pixel value but we want to work on a complete pixel value so we cast the 8bit pointer to a 32Bit pointer(due the continouse layout of the image data this cast is safe)
	uint32_t *resultImageData = reinterpret_cast<uint32_t*>(resultImage.scanLine(0));
	int width = image.width();
	int height = image.height();

	// [TODO] (sw) BigEndian mode support
	// loop through each scanline
	for(unsigned int i = 0; i < height; ++i) {
		const uint32_t *scanLineEnd = currentImagetData + width;
		// convert each pixel in the scanline and save the result in resultImageData
		while(currentImagetData < scanLineEnd)
		{
			// swap the blue and red component
			//						blue component							red component						alpha and green component (position unchanged)
			*resultImageData = ((*currentImagetData << 16) & 0xff0000) | ((*currentImagetData >> 16) & 0xff) | (*currentImagetData & 0xff00ff00);
			currentImagetData++;
			resultImageData++;
		}
		// move to the start of the previouse scanline.
		// we have to go back 2*width because on the end of the inner loop currentImagetData points at the end of the current scanline
		currentImagetData -= 2 * width;
	}
	return resultImage;
}


Renderer::ITexture2D *loadTextureFromFile(Renderer::IRenderer &renderer, const char *filename)
{
	Renderer::ITexture2D *texture2D = nullptr;
	
	QImage img;
	bool ret = img.load(filename);

	if (ret) {
		QImage oglimg = convertImageDataToOpenGLFormat(img);
		int bytecount = oglimg.byteCount();
		texture2D = renderer.createTexture2D(oglimg.width(), oglimg.height(), Renderer::TextureFormat::R8G8B8A8, oglimg.bits(), Renderer::TextureFlag::MIPMAPS);
	}
	
	return texture2D;
}