Android IPC demo.

# Building

In a full Android AOSP tree do:

```sh
$ git clone https://github.com/senier/binder_demo external/binder_demo
$ . build/envsetup.sh
$ lunch <target>
$ mmm external/binder_demo
...
```

# Installation

Install resulting binaries to device or emulator:

```sh
$ adb push out/target/product/generic_x86_64/system/bin/ipc_server out/target/product/generic_x86_64/testcases/ipc_client/x86_64/ipc_client /data/local/tmp/
```

# Running server

```sh
$ adb shell
generic_x86_64:/ $ su
generic_x86_64:/ # /data/local/tmp/ipc_server
Service "binder_demo" ready
```

# Running client

Keep the server running and open a second shell using adb:

```sh
$ adb shell
generic_x86_64:/ $ /data/local/tmp/ipc_client
[==========] Running 4 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 4 tests from IPCTest
[ RUN      ] IPCTest.Scenario1
[       OK ] IPCTest.Scenario1 (2 ms)
[ RUN      ] IPCTest.Scenario2
[       OK ] IPCTest.Scenario2 (2 ms)
[ RUN      ] IPCTest.Scenario3
[       OK ] IPCTest.Scenario3 (1 ms)
[ RUN      ] IPCTest.Scenario4
[       OK ] IPCTest.Scenario4 (1 ms)
[----------] 4 tests from IPCTest (6 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test case ran. (7 ms total)
[  PASSED  ] 4 tests.
```

Meanwhile the server outputs:

```sh
add 5 + 13
write
callback
callback_fd
write
callback
```
