#include "sysfile.h"


class QoaInput : public SysFile
{
	LONG fullFrameSize;
	LONG ExpectedFileSize();
	BOOL FileSizeCheck(LONG realFileSize);
	BOOL HeaderCheck();
	BOOL FirstFrameCheck();
	BOOL ProbeFirstFrame();
	void PrintAudioInfo();

	public:

	BOOL ready;
	ULONG samples;
	ULONG channels;
	ULONG sampleRate;
	QoaInput(STRPTR filename);
	~QoaInput();
	ULONG* GetFrame() {}
	ULONG FullFrameSize() { return fullFrameSize; }
	LONG QoaFrameSize(LONG samples, LONG channels);
};
