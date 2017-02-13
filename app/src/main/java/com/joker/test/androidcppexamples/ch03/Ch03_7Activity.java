package com.joker.test.androidcppexamples.ch03;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;

import com.joker.test.androidcppexamples.R;

/**
 * Created by lambor on 17-2-9.
 */

public class Ch03_7Activity extends AppCompatActivity {
    private static final String TAG = "ch03_5";

    public static void start(Context context) {
        context.startActivity(new Intent(context,Ch03_7Activity.class));
    }

    private Ch03_7 test;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch03_5);

        test = new Ch03_7();
        test.setJNI_ENV(); //init env for starting native thread.
        test.startNativeThread();

        new Thread(new Runnable() {
            @Override
            public void run() {
                for(int i=0;i<500;i++) {
                    test.syncMethod_JAVA();
                    Thread.yield();
                }
            }
        }).start();

        new Thread(new Runnable() {
            @Override
            public void run() {
                for(int i=0;i<500;i++) {
                    test.syncMethod_JNI();
                }
            }
        }).start();
    }
}
