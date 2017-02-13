package com.joker.test.androidcppexamples.ch03;

import android.util.Log;

/**
 * Created by lambor on 17-2-13.
 */

public class Ch03_7 {

    private static final String TAG = "ch03_7";

    static {
        System.loadLibrary("examplelib");
    }

    int count = 0;

    public native void syncMethod_JNI();

    public void syncMethod_JAVA() {
        synchronized (this) {
            count++;
            Log.w(TAG,""+count);
        }
    }


    public native void setJNI_ENV();

    public native void startNativeThread();
}
