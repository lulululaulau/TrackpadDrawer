#include <stdlib.h>
#include <signal.h>
#include "MultitouchSupport.h"


int32_t prevPathIndex = 0;

int trackpadCallback(
    MTDeviceRef device,
    MTTouch *data,
    size_t nFingers,
    double timestamp,
    size_t frame) {
  MTTouch *primaryFinger = 0;

  if (nFingers == 0) {
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

  char *command;
  int size = asprintf(&command, "echo \"%f,%f,%d\" > filename", x, y, pathId); // TODO: determine filename and whether pathId is relevant
  if (size >= 0) {
    system(command);
  }
  free(command);

  prevPathIndex = pathId;
  return 0;
}
    

int main(int argc, char **argv) {
  CFArrayRef deviceList = MTDeviceCreateList();

  while (true) {
    for (CFIndex i = 0; i < CFArrayGetCount(deviceList); i++) {
      MTDeviceRef device = (MTDeviceRef)CFArrayGetValueAtIndex(deviceList, i);
      int familyId;
      MTDeviceGetFamilyID(device, &familyId);
      if (familyId >= 98 && familyId != 112 && familyId != 113) {
        MTRegisterContactFrameCallback(device, (MTFrameCallbackFunction)trackpadCallback);
        MTDeviceStart(device, 0);
      }
    }
  }


}

