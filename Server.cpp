#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/Status.h>
#include <binder/IPCThreadState.h>
#include <com/componolit/example/BnExample.h>

#include "binder_demo.h"

#define UNUSED __attribute__((unused))

using namespace com::componolit::example;
using namespace android;

class Example : public BnExample {

   virtual binder::Status add(int32_t a,
                              int32_t b,
                              int32_t *result)
   {
      printf("add %d + %d\n", a, b);
      *result = a + b;
      return binder::Status::ok();
   }

   virtual binder::Status write(UNUSED const String16& data,
                                UNUSED const base::unique_fd& fd)
   {
      printf("write\n");
      String8 s(data);
      ::write(fd.get(), s.string(), s.size());
      return binder::Status::ok();
   }

   virtual binder::Status callback(UNUSED const sp<ICallback>& cb)
   {
      printf("callback\n");
      return binder::Status::ok();
   }

   virtual binder::Status callback_fd(UNUSED const sp<ICallback>& cb,
                                      UNUSED const String16& data,
                                      UNUSED const base::unique_fd& fd)
   {
      printf("callback_fd\n");
      return binder::Status::ok();
   }
};

extern "C"
int main (UNUSED int    argc,
          UNUSED char **argv)
{
   defaultServiceManager()->addService(String16(SERVICE_NAME), new Example());
   ProcessState::self()->startThreadPool();
   printf("Service \"" SERVICE_NAME "\" ready\n");
   IPCThreadState::self()->joinThreadPool();
}
