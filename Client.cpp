#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <utils/RefBase.h>
#include <binder/IServiceManager.h>
#include <com/componolit/example/IExample.h>

#include "binder_demo.h"

using namespace com::componolit::example;
using namespace android;

sp<IExample> getService() {
   sp<IServiceManager> sm = defaultServiceManager();
   if (sm == 0) err(1, "Service manager not found");

   sp<IBinder> binder = sm->getService(String16(SERVICE_NAME));
   if (binder == 0) err(1, "Demo service not found");

   sp<IExample> iface = interface_cast<IExample>(binder);
   if (iface == 0) err(1, "Error casting interface");

   return iface;
}

extern "C"
int main (int argc __attribute__((unused)),
          char **argv __attribute__((unused)))
{
   sp<IExample> iface = getService();
   printf("Client started\n");

   int a = 5;
   int b = 13;
   int r;

   iface->add(a, b, &r);
   printf("   #1: %d+%d=%d\n", a, b, r);

   char *tmpfile[100];
   bzero(tmpfile, sizeof(tmpfile));
   strcpy((char *)tmpfile, "/data/local/tmp/test_ipc-XXXXXX");
   base::unique_fd fd(mkstemp((char *)tmpfile));
   if (fd.get() == -1) err(1, "Error opening temp file");

   int rv = write(fd.get(), "PART1", 5);
   if (rv == -1) err(1, "Error writing file");

   iface->write(String16("PART2"), fd);

   off_t off = lseek(fd.get(), 0, SEEK_SET);
   if (off == -1) err(1, "Error resetting file");

   char data[11];
   ssize_t size = read(fd.get(), (void *)&data, 10);
   if (size == -1) err(1, "Error reading file");
   if (size != 10) errx(1, "Invalid content");
   data[10] = 0;

   printf("   #2: %s\n", data);
}
