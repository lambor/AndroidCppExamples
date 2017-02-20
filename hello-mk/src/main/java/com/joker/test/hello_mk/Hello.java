package com.joker.test.hello_mk;

/**
 * Created by lambor on 17-2-15.
 */

public class Hello {
    static {
        try {
            System.loadLibrary("stlport_shared");
            System.loadLibrary("hellomk");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public static native String stringFromJNI();
}
