/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.joker.test.hello_mk;

public class Unix implements UnixConstants {
  public static long getuid() {
    return UnixJNI.getuid();
  }

  public static void setCounter(int value) {
    UnixJNI.counter_set(value);
  }

  public static int getCounter() {
    return UnixJNI.counter_get();
  }

  public static void inc_counter() {
    UnixJNI.inc_counter();
  }

  public static int getReadOnly() {
    return UnixJNI.readOnly_get();
  }

  public static void setReadWrite(int value) {
    UnixJNI.readWrite_set(value);
  }

  public static int getReadWrite() {
    return UnixJNI.readWrite_get();
  }

}
