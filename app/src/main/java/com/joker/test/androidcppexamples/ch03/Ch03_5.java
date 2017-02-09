package com.joker.test.androidcppexamples.ch03;

/**
 * Created by lambor on 17-2-9.
 */

public class Ch03_5 {
    static {
        System.loadLibrary("examplelib");
    }

    private void throwingMethod() throws NullPointerException {
        throw new NullPointerException("Null Pointer");
    }

    public native void accessMethods();

    public native void throwException() throws NullPointerException;
}
