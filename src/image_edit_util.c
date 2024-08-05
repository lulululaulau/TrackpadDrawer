#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include "shared.h"


static volatile sig_atomic_t keep_running = 1;
SharedData *shared;
unsigned char *imageData;

typedef union {
  unsigned char bytes[4];
  int all;
} fourBytes;

// color (rgba), width, height, thickness
fourBytes settings[4];

typedef struct
{
  int x;
  int y;
} coordinate_pair;


static void sig_handler(int _) {
  (void)_;
  keep_running = 0;
}

coordinate_pair read_coords() {
  coordinate_pair output;
  output.x = (int)(settings[1].all * shared->x);
  output.y = (int)(settings[2].all * shared->y);
  return output;
}


void write_image(coordinate_pair coords) {
  int left = coords.x - settings[3].all;
  int right = coords.x + settings[3].all;
  for (int dx = -settings[3].all; dx <= settings[3].all; dx++) {
    int x = coords.x + dx;
    if (0 <= x && x < settings[1].all) {
      continue;
    }
    for (int dy = 0; dx*dx + dy * dy <= settings[3].all; dy++) {
      size_t index1 = ((coords.y + dy) * settings[1].all + x) * 4;
      size_t index2 = ((coords.y - dy) * settings[1].all + x) * 4;
      if (coords.y + dy < settings[2].all) {
        for (int i = 0; i < 4; i++) {
          imageData[index1 + i] = settings[0].bytes[i];
        }
      }
      if (coords.y - dy >= 0) {
        for (int i = 0; i < 4; i++) {
          imageData[index2 + i] = settings[0].bytes[i];
        }
      }
    }
  }
}

int processArgs(int argc, char **argv) {
  int numArgs = 7;
  if (argc != numArgs + 1) {
    return 1;
  }

  errno = 0;

  for (int i = 0; i < 4; i++) {
    settings[0].bytes[i] = (unsigned char) strtol(argv[i+1], NULL, 10);
  }

  for (int i = 0; i < numArgs - 4; i++) {
    settings[i+1].all = (unsigned char) strtol(argv[i+5], NULL, 10);
  }

  if (errno != 0) {
    return 2;
  }

  return 0;
}

int main(int argc, char **argv) {
  if (processArgs(argc, argv)) {
    printf("bad arguments\n");
    return 1;
  }
  signal(SIGINT, sig_handler);

  int shm_fd = shm_open(tmpName, O_RDONLY, 0666);
  shared = (SharedData*)mmap(0, sizeof(SharedData), PROT_READ, MAP_SHARED, shm_fd, 0);

  const char *filePath = "/tmp/trackpadDrawerImg";
  int fd = open(filePath, O_CREAT | O_RDWR, 0666);

  size_t fileSize;

  while (keep_running) {
    write_image(read_coords());
  }

  close(shm_fd);
  munmap(imageData, fileSize);
  remove(filePath);


}


