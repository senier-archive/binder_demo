#include <stdio.h>
#include <stdlib.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/Status.h>
#include <binder/IPCThreadState.h>
#include <com/componolit/example/BnExample.h>

#include "binder_demo.h"

using namespace com::componolit::example;
using namespace android;

class Example : public BnExample {
   virtual binder::Status add(int32_t a, int32_t b, int32_t *result) {
      printf("add %d + %d\n", a, b);
      *result = a + b;
      return binder::Status::ok();
   }
};

extern "C"
int main (int    argc __attribute__((unused)),
          char **argv __attribute__((unused)))
{
   defaultServiceManager()->addService(String16(SERVICE_NAME), new Example());
   ProcessState::self()->startThreadPool();
   printf("Service \"" SERVICE_NAME "\" ready\n");
   IPCThreadState::self()->joinThreadPool();
}
