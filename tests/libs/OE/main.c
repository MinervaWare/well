#include <OE/OE.h>
#include <time.h>

void draw() {
	OEDrawObject(OEGetObjectFromName("OECube"));
}

void event() {
}

int main(int argc, char **argv) {
	
	OEInitRenderer(1280, 720, "game", PERSPECTIVE);
	OEEnableDebugInfo();
	OEEnableFXAA();
	OEEnableSSGI(64, 8);
	while(OERendererIsRunning()) {
		OEPollEvents((EVENTFUNC)event);
		OERenderFrame((RENDFUNC)draw, NULL, NULL);
	}
	return 0;
}
