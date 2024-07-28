#include "MultitouchSupport.h"


size_t prevFingers = 0;
int32_t prevPathIndex = 0;


int trackpadCallback(
    MTDeviceRef device,
    MTTouch *data,
    size_t nFingers,
    double timestamp,
    size_t frame) {
  MTTouch primaryFinger;

  if (nFingers == 0) {
    prevFingers = 0;
    return 0;
  }
  
  if (nFingers == 1) {
    prevPathIndex = data->pathIndex;
    primaryFinger = *data;
  } else {
    for (int i = 0; i < nFingers; i++) {
      if (data[i].pathIndex == prevPathIndex) {
        primaryFinger = data[i];
      }
    }
  }

  // do stuff with primary finger

  prevFingers = nFingers;
  prevPathIndex = primaryFinger.pathIndex;
  return 0;
}
    

int main(int argc, char** argv) {
  CFArrayRef deviceList = MTDeviceCreateList();
  while (true) {
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


}

