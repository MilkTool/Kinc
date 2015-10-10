#include "pch.h"
#include "VertexBufferImpl.h"
#include <Kore/Graphics/Graphics.h>
#include "ShaderImpl.h"
#include "ogl.h"

using namespace Kore;

VertexBuffer* VertexBufferImpl::current = nullptr;

VertexBufferImpl::VertexBufferImpl(int count, int instanceDataStepRate) : myCount(count), instanceDataStepRate(instanceDataStepRate) {

}

VertexBuffer::VertexBuffer(int vertexCount, const VertexStructure& structure, int instanceDataStepRate = 0) : VertexBufferImpl(vertexCount, instanceDataStepRate) {
	myStride = 0;
	for (int i = 0; i < structure.size; ++i) {
		VertexElement element = structure.elements[i];
		switch (element.data) {
		case ColorVertexData:
			myStride += 1 * 4;
			break;
		case Float1VertexData:
			myStride += 1 * 4;
			break;
		case Float2VertexData:
			myStride += 2 * 4;
			break;
		case Float3VertexData:
			myStride += 3 * 4;
			break;
		case Float4VertexData:
			myStride += 4 * 4;
			break;
		}
	}
//#if defined SYS_ANDROID || defined SYS_HTML5 || defined SYS_TIZEN
	this->structure = structure;
//#elif defined SYS_IOS
//	glGenVertexArraysOES(1, &arrayId);
//	glBindVertexArrayOES(arrayId);
//#else
//	glGenVertexArrays(1, &arrayId);
//	glBindVertexArray(arrayId);
//#endif
	glGenBuffers(1, &bufferId);
	data = new float[vertexCount * myStride / 4];

	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	int offset = 0;
	int index = 0;
	for (; index < structure.size; ++index) {
		VertexElement element = structure.elements[index];
		glEnableVertexAttribArray(index);
		int size;
		switch (element.data) {
		case ColorVertexData:
			size = 1;
			break;
		case Float1VertexData:
			size = 1;
			break;
		case Float2VertexData:
			size = 2;
			break;
		case Float3VertexData:
			size = 3;
			break;
		case Float4VertexData:
			size = 4;
			break;
		}
		glVertexAttribPointer(index, size, GL_FLOAT, false, myStride, (void*)offset);
		switch (element.data) {
		case ColorVertexData:
			offset += 4 * 1;
			break;
		case Float1VertexData:
			offset += 4 * 1;
			break;
		case Float2VertexData:
			offset += 4 * 2;
			break;
		case Float3VertexData:
			offset += 4 * 3;
			break;
		case Float4VertexData:
			offset += 4 * 4;
			break;
		}
		glVertexAttribDivisor(index, 0);
	}
	for (; index < 10; ++index) glDisableVertexAttribArray(index);
}

VertexBuffer::~VertexBuffer() {
	unset();
	delete[] data;
}

float* VertexBuffer::lock() {
	return data;
}
/*
// TODO: FIXME!
float* VertexBuffer::lock(int start, int count) {
	myCount = count;
	return nullptr;//&buffer[start * 9];
}
*/

void VertexBuffer::unlock() {
/*
#if defined SYS_ANDROID || defined SYS_HTML5 || defined SYS_TIZEN

#elif defined SYS_IOS
	glBindVertexArrayOES(arrayId);
#else
	glBindVertexArray(arrayId);
#endif
*/
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, myStride * myCount, data, GL_STATIC_DRAW);
}

int VertexBuffer::set(int offset) {
//#if defined SYS_ANDROID || defined SYS_HTML5 || defined SYS_TIZEN
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	int internaloffset = 0;
	int index = 0;
	for (; index < structure.size; ++index) {
		VertexElement element = structure.elements[index];
		glEnableVertexAttribArray(index + offset);
		int size;
		switch (element.data) {
		case ColorVertexData:
			size = 1;
			break;
		case Float1VertexData:
			size = 1;
			break;
		case Float2VertexData:
			size = 2;
			break;
		case Float3VertexData:
			size = 3;
			break;
		case Float4VertexData:
			size = 4;
			break;
		}
		glVertexAttribPointer(index + offset, size, GL_FLOAT, false, myStride, (void*)internaloffset);
		switch (element.data) {
		case ColorVertexData:
			internaloffset += 4 * 1;
			break;
		case Float1VertexData:
			internaloffset += 4 * 1;
			break;
		case Float2VertexData:
			internaloffset += 4 * 2;
			break;
		case Float3VertexData:
			internaloffset += 4 * 3;
			break;
		case Float4VertexData:
			internaloffset += 4 * 4;
			break;
		}
		glVertexAttribDivisor(index + offset, instanceDataStepRate);
	}
	for (; index < 10; ++index) glDisableVertexAttribArray(index + offset);

//#elif defined SYS_IOS
//	glBindVertexArrayOES(arrayId);
//#else
//	glBindVertexArray(arrayId);
//#endif
	if (IndexBuffer::current != nullptr) IndexBuffer::current->set();

	return structure.size;
}

void VertexBufferImpl::unset() {
	if ((void*)current == (void*)this) current = nullptr;
}

int VertexBuffer::count() {
	return myCount;
}

int VertexBuffer::stride() {
	return myStride;
}
