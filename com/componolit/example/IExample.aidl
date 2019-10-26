package com.componolit.example;

import com.componolit.example.ICallback;

interface IExample {

   /* Scenario #1: Add two numbers */
   int add(int a, int b);

   /* Scenario #2: Write string to open file descriptor */
   void write(String data, FileDescriptor fd);

   /* Scenario #3: Callback */
   void callback(ICallback cb);

   /* Scenario #4: Callback + file descriptor */
   void callback_fd(ICallback cb, String data, FileDescriptor fd);
}
