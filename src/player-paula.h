#include <devices/audio.h>

// Regardless of mono / stereo, I will need two Paula channels (one left, one right), as mono
// files are played on both sides. As I use doublebuffering, 4 IORequests will be needed.
// However I only need two chip-RAM buffers for mono (I can pass the same data to two IORequests)
// and four buffers for stereo. That is why buffers are not allocated here, but in subclasses.

#define ABLOCK_UNUSED   1
#define ABLOCK_PENDING  2
#define ABLOCK_FREE     3


struct AudioBlock
{
	IOAudio *reqL;
	IOAudio *reqR;
	UBYTE statusL;
	UBYTE statusR;
	BYTE *bufL;
	BYTE *bufR;
};


class PlayerPaula
{
	void InitReq0(IOAudio *req);
	void InitReqClone(IOAudio *src, IOAudio *clone, UBYTE side);

	protected:

	UWORD period;
	UBYTE left;                // single channel mask for stereo L
	UBYTE right;               // single channel mask for stereo R
	MsgPort *port;
	AudioBlock blocks[2];
	void AudioProblem(LONG error);
	PlayerPaula(LONG frequency);
	~PlayerPaula();
	BOOL devopen;

	public:

	BOOL ready;
	virtual BYTE* GetBuffer(void) = 0;
	virtual BOOL BufferFilled(LONG samples) = 0;
};
