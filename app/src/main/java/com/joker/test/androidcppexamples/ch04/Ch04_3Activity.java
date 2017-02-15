package com.joker.test.androidcppexamples.ch04;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.joker.test.androidcppexamples.R;
import com.joker.test.androidcppexamples.ch03.Ch03_7;
import com.joker.test.hello_mk.Hello;

/**
 * Created by lambor on 17-2-9.
 */

public class Ch04_3Activity extends AppCompatActivity {
    private static final String TAG = "ch03_5";

    public static void start(Context context) {
        context.startActivity(new Intent(context,Ch04_3Activity.class));
        Log.e(TAG, Hello.stringFromJNI());
    }

    private TextView mUId;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch04_3);
        mUId = (TextView) findViewById(R.id.uid);
        mUId.setText("uid: "+Unix.getuid());
    }
}
