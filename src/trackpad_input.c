#include "MultitouchSupport.h"





int trackpadCallback(
    MTDeviceRef device,
    MTTouch *data,
    size_t nFingers,
    double timestamp,
    size_t frame) {
  // do stuff
  
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

