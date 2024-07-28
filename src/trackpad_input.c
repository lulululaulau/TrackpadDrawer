#include <signal.h>
#include "MultitouchSupport.h"


static volatile sig_atomic_t keep_running = 1;
FILE *fptr;
size_t prevFingers = 0; // might remove (could be unnecessary)
int32_t prevPathIndex = 0;

static void sig_handler(int _) {
  (void)_;
  keep_running = 0;
}

int trackpadCallback(
    MTDeviceRef device,
    MTTouch *data,
    size_t nFingers,
    double timestamp,
    size_t frame) {
  MTTouch *primaryFinger = 0;

  if (nFingers == 0) {
    prevFingers = 0;
    return 0;
  }
  
  if (nFingers == 1) {
    prevPathIndex = data->pathIndex;
    primaryFinger = data;
  } else {
    for (int i = 0; i < nFingers; i++) {
      if (data[i].pathIndex == prevPathIndex) {
        primaryFinger = data+i;
      }
    }
  }

  if (primaryFinger == 0) {
    primaryFinger = data;
  }

  float x, y;
  x = primaryFinger->normalizedVector.position.x;
  y = primaryFinger->normalizedVector.position.y;
  int32_t pathId = primaryFinger->pathIndex;
  fprintf(fptr, "%f,%f,%d\n", x, y, pathId); // TODO: determine if pathIndex is necessary

  prevFingers = nFingers;
  prevPathIndex = pathId;
  return 0;
}
    

int main(int argc, char **argv) {
  CFArrayRef deviceList = MTDeviceCreateList();
  fptr = fopen("filename", "w"); // TODO: figure out what file to write to
  signal(SIGINT, sig_handler);

  while (keep_running) {
    for (CFIndex i = 0; i < CFArrayGetCount(deviceList); i++) {
      MTDeviceRef device = (MTDeviceRef)CFArrayGetValueAtIndex(deviceList, i);
      int familyId;
      MTDeviceGetFamilytID(device, &familyId);
      if (familyId >= 98 && familyId != 112 && familyId != 113) {
        MTRegisterContactFrameCallback(device, (MTFrameCallbackFunction)trackpadCallback);
        MTDeviceStart(device, 0);
      }
    }
  }

  fclose(fptr);

}

