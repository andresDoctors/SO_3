#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define N 128
#define TIMES 32
#define MINTICKS 250
#define OPSIZE 16
#define DEFAULT_TIME 10000

static float a[N][N];
static float b[N][N];
static float c[N][N];

static void init(void) {
  int x, y;
  for (y = 0; y < N; ++y) {
    for (x = 0; x < N; ++x) {
      a[y][x] = y - x;
      b[y][x] = x - y;
      c[y][x] = 0.0f;
    }
  }
}

static void matmul(float beta) {
  int x, y, k;
  for (y = 0; y < N; ++y) {
    for (x = 0; x < N; ++x) {
      for (k = 0; k < N; ++k) {
        c[y][x] += beta * a[y][k] * b[k][x];
      }
    }
  }
}

int
cpubench(void)
{
  int pid = getpid();
  float beta = 1.0f;

  init();
  int ops = 0;
  int uptime_0 = uptime();
  while(uptime() - uptime_0 < DEFAULT_TIME * 100) {

    printf(1,      "CPU: pid=%d ops=%d of=%d time=%d\n", pid, ops, uptime() - uptime_0);
    //printf(fd_out, "CPU: pid=%d ops=%d time=%d\n", pid, ops, uptime() - uptime_0);

    for(int i = 0; i < TIMES; ++i) {
        matmul(beta);
        beta = -beta;
        ops++;
        //ops += 3 * N * N * N;
    }
  }

  printf(1, "%x\n", c[0][0]);
  return 0;
}


static char path[] = "12iops";
static char data[OPSIZE];

int
iobench(void)
{
  int rfd, wfd;
  int pid = getpid();
  int i;

  path[0] = '0' + (pid / 10);
  path[1] = '0' + (pid % 10);

  memset(data, 'a', sizeof(data));

  int ops = 0;
  int uptime_0 = uptime();
  while(uptime() - uptime_0 < DEFAULT_TIME * 100) {
    
    printf(1,      "\t\t\t\t\tIO: pid=%d ops=%d time=%d\n", pid, ops, uptime() - uptime_0);
    //printf(fd_out, "\t\t\t\t\tIO: pid=%d ops=%d time=%d\n", pid, ops, uptime() - uptime_0);

    wfd = open(path, O_CREATE | O_WRONLY);
    rfd = open(path, O_RDONLY);

    for(i = 0; i < TIMES; ++i) {
      write(wfd, data, OPSIZE);
    }
    for(i = 0; i < TIMES; ++i) {
      read(rfd, data, OPSIZE);
    }
    close(wfd);
    close(rfd);

    ops += 2 * TIMES;
  }
  
  return 0;
}

char ntest[6] = "testX";

void
test(int ncpu, int nio)
{
    ntest[4] = '0' + ncpu * 3 + nio - 1;
    printf(1, "\n\n\ntest%d\n\n", ncpu * 3 + nio - 1);
    
    int i;

    for (i = 0; i < ncpu; i++) {
        if (fork() == 0) {
            cpubench();
            exit();
        }
    }
    
    for (i = 0; i < nio; i++) {
        if (fork() == 0) {
            iobench();
            exit();
        }
    }
    

    //close(fd_out);
    while (wait() != -1) ;

    return;
}

int
main(int argc, char const *argv[])
{
    int i, j;    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            test(i, j);
        }    
    }

    return 0;
}
