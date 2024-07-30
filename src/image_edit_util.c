#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "shared.h"


static volatile sig_atomic_t keep_running = 1;
SharedData *shared;
unsigned char *imageData;

struct settings {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
  int thickness;
  int width;
  int height;
} settings;

typedef struct
{
  float x;
  float y;
} coordinate_pair;


static void sig_handler(int _) {
  (void)_;
  keep_running = 0;
}

coordinate_pair read_coords() {

}

void write_image(coordinate_pair coords) {

}

int processArgs(int argc, char **argv) {

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


