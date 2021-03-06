/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.8
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.joker.test.hello_mk;

public final class NumbersSafe {
  public final static NumbersSafe ONE_SAFE = new NumbersSafe("ONE_SAFE", UnixJNI.ONE_SAFE_get());
  public final static NumbersSafe TWO_SAFE = new NumbersSafe("TWO_SAFE", UnixJNI.TWO_SAFE_get());
  public final static NumbersSafe THREE_SAFE = new NumbersSafe("THREE_SAFE");
  public final static NumbersSafe FOUR_SAFE = new NumbersSafe("FOUR_SAFE");

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static NumbersSafe swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + NumbersSafe.class + " with value " + swigValue);
  }

  private NumbersSafe(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private NumbersSafe(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private NumbersSafe(String swigName, NumbersSafe swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static NumbersSafe[] swigValues = { ONE_SAFE, TWO_SAFE, THREE_SAFE, FOUR_SAFE };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

