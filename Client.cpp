#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <utils/RefBase.h>
#include <binder/IServiceManager.h>
#include <com/componolit/example/IExample.h>
#include <com/componolit/example/BnCallback.h>

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

class Cb : public BnCallback {
   int r_ = 0;
   virtual binder::Status result(int r) {
      r_ = r;
      return binder::Status::ok();
   }
public:
   Cb(int value) : r_(value) { };
   int get_r() {
      return r_;
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
   ASSERT_TRUE(status.isOk()) << "Error calling write method";

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

TEST_F(IPCTest, Scenario3) {
   enum { INITIAL_VALUE = 4532, TEST_VALUE = 1234 };
   Cb *cb = new Cb(INITIAL_VALUE);
   ASSERT_EQ(cb->get_r(), INITIAL_VALUE);
   binder::Status status = iface->callback(cb, TEST_VALUE);
   ASSERT_TRUE(status.isOk()) << "Error calling callback method";
   ASSERT_EQ(cb->get_r(), TEST_VALUE);
}

TEST_F(IPCTest, Scenario4) {
   enum { INITIAL_VALUE = 9887, TEST_VALUE = 4834 };
   Cb *cb = new Cb(INITIAL_VALUE);
   ASSERT_EQ(cb->get_r(), INITIAL_VALUE);

   base::unique_fd tf = openTempFile();
   ASSERT_NE(tf.get(), -1) << "Error opening temporary file";

   int rv = write(tf.get(), "ABCDE", 5);
   ASSERT_NE(rv, -1) << "Error writing file";

   binder::Status status = iface->callback_fd(cb, TEST_VALUE, String16("FGHIJ"), tf);
   ASSERT_TRUE(status.isOk()) << "Error calling callback_fd method";

   off_t off = lseek(tf.get(), 0, SEEK_SET);
   ASSERT_NE(off, -1) << "Error resetting file";

   char data[11];
   ssize_t size = read(tf.get(), (void *)&data, 10);
   data[10] = 0;
   ASSERT_NE(size, -1) << "Error reading file";
   ASSERT_EQ(size, 10) << "Invalid content length";

   std::string result(data, 10);
   ASSERT_EQ(result, "ABCDEFGHIJ") << "Invalid content";
   ASSERT_EQ(cb->get_r(), TEST_VALUE);
}

int main (int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
