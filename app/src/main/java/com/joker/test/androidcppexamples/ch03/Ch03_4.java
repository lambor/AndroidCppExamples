package com.joker.test.androidcppexamples.ch03;

/**
 * Created by lambor on 17-2-8.
 */

public class Ch03_4 {

    static {
        System.loadLibrary("examplelib");
    }

    public String instanceField = "Instance Field";
    public static String staticField = "Static Field";

    /**
     * 字符串 操作
     */
    public native String getHelloStr();

    public native void parseJavaStr(String str);

    /**
     * 数组 操作
     */
    public native int[] getNumberArray();

    public native void initNumbersByRegion(int[] array);

    public native void initNumbersDirectly(int[] array);

    /**
     * NIO
     */
    //TODO

    /**
     * 访问域
     */
    public native String getInstanceField();

    public native String getStaticField();

    /**
     * 调用方法
     */
    private String instanceMethod() {
        return "Instance Method";
    }

    public native String invokeInstanceMethod();

    private static String staticMethod() {
        return "Static Method";
    }

    public native String invokeStaticMethod();

}
