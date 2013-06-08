/*
 *
 */

#include "ExampleBase.h"

ExampleBase::ExampleBase()
: mWidth(0), mHeigth(0)
{

}

ExampleBase::~ExampleBase()
{

}

void ExampleBase::setSize(int width, int height)
{
	if (mWidth != width || mHeigth != height) {
		mWidth = width;
		mHeigth = height;
		onSizeChanged();
	}
}
