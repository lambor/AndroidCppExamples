package com.joker.test.hello_mk;

/**
 * Created by lambor on 17-2-15.
 */

public class Hello {
    static {
        System.loadLibrary("hellomk");
    }
    public static native String stringFromJNI();
}
