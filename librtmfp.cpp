#include "librtmfp.h"
#include "Mona/Exceptions.h"
#include "RTMFPConnection.h"
#include "Mona/Logs.h"
#include "Mona/String.h"
#include "Invoker.h"

using namespace Mona;
using namespace std;

extern "C" {

static Invoker GlobalInvoker(0);

unsigned int RTMFP_Connect(const char* host, int port, const char* url, void (__cdecl * onSocketError)(const char*), void (__cdecl * onStatusEvent)(const char*, const char*)) {
	Exception ex;
	// Start Socket Manager if needed
	if(!GlobalInvoker.start(ex)) {
		ERROR(ex.error())
		return 0;
	}

	shared_ptr<RTMFPConnection> pConn(new RTMFPConnection(onSocketError, onStatusEvent));
	unsigned int index = GlobalInvoker.addConnection(pConn);
	if(!pConn->connect(ex,&GlobalInvoker,host,port,url)) {
		ERROR(ex.error())
		return 0;
	}

	return index;
}

void RTMFP_Play(unsigned int RTMFPcontext, const char* streamName) {
	Exception ex;
	shared_ptr<RTMFPConnection> pConn(GlobalInvoker.getConnection(RTMFPcontext));
	pConn->playStream(ex, streamName);
}

void RTMFP_Close(unsigned int RTMFPcontext) {
	shared_ptr<RTMFPConnection> pConn(GlobalInvoker.getConnection(RTMFPcontext));
	pConn->close();
}

void RTMFP_Terminate() {
	GlobalInvoker.stop();
}

}