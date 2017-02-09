package com.joker.test.androidcppexamples.ch03;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;

import com.joker.test.androidcppexamples.R;

/**
 * Created by lambor on 17-2-9.
 */

public class Ch03_5Activity extends AppCompatActivity {
    private static final String TAG = "ch03_5";

    public static void start(Context context) {
        context.startActivity(new Intent(context,Ch03_5Activity.class));
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch03_5);

        Ch03_5 test = new Ch03_5();
        test.accessMethods();
        try {
            test.throwException();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
