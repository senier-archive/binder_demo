#include <stdio.h>
#include <stdlib.h>
#include <utils/RefBase.h>
#include <binder/IServiceManager.h>
#include <com/componolit/example/IExample.h>

#include "binder_demo.h"

using namespace com::componolit::example;
using namespace android;

void die(const char *message) {
   fprintf(stderr, "%s\n", message);
   exit(1);
}

sp<IExample> getService() {
   sp<IServiceManager> sm = defaultServiceManager();
   if (sm == 0) die("Service manager not found");

   sp<IBinder> binder = sm->getService(String16(SERVICE_NAME));
   if (binder == 0) die("Demo service not found");

   sp<IExample> iface = interface_cast<IExample>(binder);
   if (iface == 0) die("Error casting interface");

   return iface;
}

extern "C"
int main (int argc __attribute__((unused)),
          char **argv __attribute__((unused)))
{
   sp<IExample> iface = getService();
   int a = 5;
   int b = 13;
   int r;

   iface->add(a, b, &r)
   printf("Client: %d+%d=%d\n", a, b, r);
}
