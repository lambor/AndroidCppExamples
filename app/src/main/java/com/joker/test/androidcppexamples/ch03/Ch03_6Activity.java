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

public class Ch03_6Activity extends AppCompatActivity {
    private static final String TAG = "ch03_5";

    public static void start(Context context) {
        context.startActivity(new Intent(context,Ch03_6Activity.class));
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch03_5);

        Ch03_6 test = new Ch03_6();
        test.testLocalRef();
        int[] global = test.makeGlobalRef();
        test.releaseGlobalRef();
        int[] weak = test.makeWeakRef();
        test.releaseWeakRef();
    }
}
