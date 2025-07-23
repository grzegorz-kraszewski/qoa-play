#include "main.h"
#include "qoainput.h"

#include <proto/exec.h>
#include <proto/dos.h>

#define QOA_FRAMES_PER_BUFFER     16    // used by StandardBuffer


// Abstract buffer class

class InputBuffer
{
	public:

	BOOL ready;
	InputBuffer() : ready(FALSE) {}
	virtual UBYTE* NextFrame() = 0;
};


// PreloadBuffer loads all the QOA file in memory

class PreloadBuffer : public InputBuffer
{
	UBYTE *data;
	UBYTE *ptr;
	LONG datasize;
	QoaInput &source;

	public:

	PreloadBuffer(QoaInput &input);
	~PreloadBuffer();
	virtual UBYTE* NextFrame();
};


// StandardBuffer reads the QOA file in chunks of 16 frames

class StandardBuffer : public InputBuffer
{
	UBYTE *buf;
	QoaInput &source;
	BOOL Fill();

	public:

	StandardBuffer(QoaInput &input);
	~StandardBuffer();
	virtual UBYTE* NextFrame();
};

