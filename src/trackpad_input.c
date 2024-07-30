#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "MultitouchSupport.h"
#include "shared.h"


static volatile sig_atomic_t keep_running = 1;
int32_t prevPathIndex = 0;
SharedData *shared;


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
    prevPathIndex = primaryFinger->pathIndex;
  }

  shared->x = primaryFinger->normalizedVector.position.x;
  shared->y = primaryFinger->normalizedVector.position.y;
  shared->pathIndex = primaryFinger->pathIndex;


  return 0;
}

int main(int argc, char **argv) {
  CFArrayRef deviceList = MTDeviceCreateList();
  signal(SIGINT, sig_handler);

  int shm_fd = shm_open(tmpName, O_RDWR, 0666); // use O_CREATE | O_RDWR to create shared memory instead of just access
                                                // remember to use shm_unlink(tmpName) to clean up the shared memory
  shared = (SharedData*)mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  while (keep_running) {
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

  close(shm_fd);

}

