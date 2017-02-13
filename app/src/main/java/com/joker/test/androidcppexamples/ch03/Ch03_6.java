package com.joker.test.androidcppexamples.ch03;

/**
 * Created by lambor on 17-2-10.
 */

public class Ch03_6 {
    static {
        System.loadLibrary("examplelib");
    }
    /**
     * 局部引用
     */
    public native void testLocalRef();

    /**
     * 全局引用
     */
    public native int[] makeGlobalRef();

    /**
     * 错误使用
     * @param globalIntArr
     */
//    public native void releaseGlobalRef(int[] globalIntArr);
    /**
     * 正确使用
     */
    public native void releaseGlobalRef();

    /**
     * 弱引用
     */
    public native int[] makeWeakRef();

    /**
     * 错误使用
     * @param weakIntArr
     */
//    public native void releaseWeakRef(int[] weakIntArr);
    /**
     * 正确使用
     */
    public native void releaseWeakRef();
}
