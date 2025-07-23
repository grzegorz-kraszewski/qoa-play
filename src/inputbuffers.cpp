#include "inputbuffers.h"
#include "errors.h"


PreloadBuffer::PreloadBuffer(QoaInput &input) : source(input), data(NULL)
{
	datasize = source.size();

	if (data = new UBYTE[datasize])
	{
		D("PreloadBuffer[$%08lx]: data at $%08lx.\n", this, data);
		if (source.read(data, datasize) == datasize)
		{
			D("PreloadBuffer[$%08lx]: %ld bytes loaded.\n", this, datasize);
			ptr = data;
			ready = TRUE;
		}
		else source.FileProblem();
	}
	else Problem(E_APP_OUT_OF_MEMORY);
}


PreloadBuffer::~PreloadBuffer()
{
	if (data) delete data;
}


UBYTE* PreloadBuffer::NextFrame()
{
	UBYTE *frame = ptr;

	if (frame >= data + datasize) return NULL;
	ptr += source.FullFrameSize();
	return frame;
}


StandardBuffer::StandardBuffer(QoaInput &input) : source(input), buf(NULL)
{
	LONG size = source.FullFrameSize() * QOA_FRAMES_PER_BUFFER;

	if (buf = new UBYTE[size])
	{
		D("StandardBuffer[$%08lx]: %ld bytes of buffer at $%08lx.\n", this, size, buf);
		if (Fill()) ready = TRUE;
	}
}


StandardBuffer::~StandardBuffer()
{
	if (buf) delete buf;
}
