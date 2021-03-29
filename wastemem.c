#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef _WIN32
void zzzzz(int seconds) {
    Sleep(seconds * 1000);
}

#else

#include <unistd.h>
void zzzzz(int seconds) {
    sleep(seconds);
}

#endif

int waste_block(uint64_t blocksize) {
  unsigned char *block;
  uint64_t c;

  block = malloc(blocksize);
  if (block == NULL) {
    printf("Malloc failed, aborting.\n");
    return 1;
  }

  for (c = 0; c < blocksize; c++) {
    block[c] = (unsigned char)(c & 0xff);
  }
  return 0;
}

int waste_blocks(uint64_t num_blocks, uint64_t blocksize) {
  uint64_t i, rv;
  uint64_t wasted_so_far = 0;
  for (i = 0; i < num_blocks; i++) {
      rv = waste_block(blocksize);
      if (rv != 0) {
          printf("ERROR, waste_block failed\n");
          return 6;
      }

      wasted_so_far += blocksize;
      printf("Wasted %" PRIu64 " bytes so far\n", wasted_so_far);
  }
  return 0;
}

int main (int argc, char **argv) {
  int mb_to_waste, rv, sleep_seconds;
  uint64_t block_size_bytes, bytes_to_waste, num_blocks;

  if (sizeof(uint64_t) != 8) {
    printf("ERROR: uint64_t is %d bytes\n", (int)sizeof(uint64_t));
    return 1;
  }

  if (sizeof(char*) != 8) {
    printf("ERROR: pointer is %d bytes\n", (int)sizeof(char*));
    return 1;  
  }
   
  if (argc != 4) {
      printf("Usage: %s <MiB to waste> <block size bytes> <sleep seconds>", argv[0]);
      return 1;
  }

  rv = sscanf(argv[1], "%d", &mb_to_waste);
  if (rv != 1) {
    printf("ERROR: failed to parse %s Mib to waste.\n", argv[1]);
    return 1;
  }
  rv = sscanf(argv[2], "%" PRIu64, &block_size_bytes);
  if (rv != 1) {
    printf("ERROR: failed to parse %s as a block size\n", argv[2]);
    return 1;
  }
  rv = sscanf(argv[3], "%d", &sleep_seconds);
  if (rv != 1) {
    printf("ERROR: failed to parse %s as a number of seconds\n", argv[3]);
    return 1;
  }

  bytes_to_waste = mb_to_waste * 1024ULL * 1024ULL;
 
  num_blocks = bytes_to_waste / block_size_bytes;
 
  printf("mb_to_waste=%d bytes_to_waste=%" PRIu64 " block_size_bytes=%" PRIu64 "\n", mb_to_waste, bytes_to_waste, block_size_bytes);
  printf("Will allocate %" PRIu64 " blocks of size %" PRIu64 " bytes\n", num_blocks, block_size_bytes);
  rv = waste_blocks(num_blocks, block_size_bytes);
  if (rv != 0) {
    printf("ERROR, waste_blocks failed\n");
    // Don't exit here, to support the case where we want to waste memory until it fails
  }
 
  printf("Sleeping for %d seconds\n", sleep_seconds);  
  fflush(stdout);
  fflush(stderr);
 
  zzzzz(sleep_seconds);
  printf("Waking up and exiting\n");
  return 0;
}

