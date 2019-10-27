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
Client: 5+13=18
generic_x86_64:/ $
```

Meanwhile the server outputs:

```sh
add 5 + 13
```
