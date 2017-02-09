package com.joker.test.androidcppexamples.ch03;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import com.joker.test.androidcppexamples.R;

import java.lang.reflect.Field;

public class Ch03_4Activity extends AppCompatActivity {

    private static final String TAG = "ch03_4";

    public static void start(Context context) {
        context.startActivity(new Intent(context,Ch03_4Activity.class));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch03_4);

        Ch03_4 test = new Ch03_4();
        String str = test.getHelloStr();
        test.parseJavaStr(str);

        int[] array = test.getNumberArray();
        if(array != null) {
            test.initNumbersByRegion(array);
            Log.e(TAG,"init array by region: " + content(array));

            test.initNumbersDirectly(array);
            Log.e(TAG,"init array directly: " + content(array));
        }
        else
            Log.e(TAG,"Ch03_4.getNumberArray return null.");


        Log.e(TAG,"test's instance field: "+test.getInstanceField());
        Log.e(TAG,"Ch03_4's static field: "+test.getStaticField());

        Log.e(TAG,"invoke test's instance method: "+test.invokeInstanceMethod());
        Log.e(TAG,"invoke test's static method: "+test.invokeStaticMethod());
    }

    private String content(int[] arr) {
        StringBuilder builder = new StringBuilder();
        builder.append("[");
        for(int i=0;i<arr.length;i++) {
            builder.append(arr[i]);
            if(i!=arr.length-1) builder.append(",");
        }
        builder.append("]");
        return builder.toString();
    }
}
