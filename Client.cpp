#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <utils/RefBase.h>
#include <binder/IServiceManager.h>
#include <com/componolit/example/IExample.h>

#include <gtest/gtest.h>

#include "binder_demo.h"

using namespace com::componolit::example;
using namespace android;

class IPCTest : public ::testing::Test {
public:
   sp<IExample> iface;
   virtual void SetUp() {
      getService();
   }
   virtual void TearDown() {
   }
protected:
   base::unique_fd openTempFile() {
      char *tmpfile[100];
      strcpy((char *)tmpfile, "/data/local/tmp/test_ipc-XXXXXX");
      return base::unique_fd(mkstemp((char *)tmpfile));
   }
private:
   void getService() {
      sp<IServiceManager> sm = defaultServiceManager();
      ASSERT_NE(sm, nullptr) << "Service manager not found";

      sp<IBinder> binder = sm->getService(String16(SERVICE_NAME));
      ASSERT_NE(binder, nullptr) << "Demo service not found";

      iface = interface_cast<IExample>(binder);
      ASSERT_NE(iface, nullptr) << "Error casting interface";
   }
};

TEST_F(IPCTest, Scenario1) {
   int r;
   iface->add(5, 13, &r);
   ASSERT_EQ(r, 18);
}

TEST_F(IPCTest, Scenario2) {
   base::unique_fd tf = openTempFile();
   ASSERT_NE(tf.get(), -1) << "Error opening temporary file";

   int rv = write(tf.get(), "PART1", 5);
   ASSERT_NE(rv, -1) << "Error writing file";

   binder::Status status = iface->write(String16("PART2"), tf);
   ASSERT_TRUE(status.isOk()) << "Error calling write interface";

   off_t off = lseek(tf.get(), 0, SEEK_SET);
   ASSERT_NE(off, -1) << "Error resetting file";

   char data[11];
   ssize_t size = read(tf.get(), (void *)&data, 10);
   data[10] = 0;
   ASSERT_NE(size, -1) << "Error reading file";
   ASSERT_EQ(size, 10) << "Invalid content length";

   std::string result(data, 10);
   ASSERT_EQ(result, "PART1PART2") << "Invalid content";
}

int main (int argc, char **argv)
{
   printf("Client test started\n");
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
