package com.joker.test.androidcppexamples.ch04;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.joker.test.androidcppexamples.R;
import com.joker.test.hello_mk.Hello;
import com.joker.test.hello_mk.Unix;

/**
 * Created by lambor on 17-2-9.
 */

/**
 * build by ndk-build with Android.mk
 */

public class Ch04_4Activity extends AppCompatActivity {
    private static final String TAG = "ch04_3MK";

    public static void start(Context context) {
        context.startActivity(new Intent(context,Ch04_4Activity.class));
        Log.e(TAG, Hello.stringFromJNI());
    }

    private TextView mUId;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch04_3);
        mUId = (TextView) findViewById(R.id.uid);
        mUId.setText("uid: "+ Unix.getuid());

        Log.e(TAG,""+ Unix.getCounter());
        Unix.inc_counter();
        Log.e(TAG,""+Unix.getCounter());
    }
}
