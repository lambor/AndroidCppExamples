package com.joker.test.androidcppexamples;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.joker.test.androidcppexamples.ch03.Ch03_4Activity;
import com.joker.test.androidcppexamples.ch03.Ch03_5Activity;
import com.joker.test.androidcppexamples.ch03.Ch03_6Activity;
import com.joker.test.androidcppexamples.ch03.Ch03_7Activity;
import com.joker.test.androidcppexamples.ch04.Ch04_3Activity;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.ch03_4).setOnClickListener(this);
        findViewById(R.id.ch03_5).setOnClickListener(this);
        findViewById(R.id.ch03_6).setOnClickListener(this);
        findViewById(R.id.ch03_7).setOnClickListener(this);
        findViewById(R.id.ch04_3).setOnClickListener(this);
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.ch03_4:
                Ch03_4Activity.start(this);
                break;
            case R.id.ch03_5:
                Ch03_5Activity.start(this);
                break;
            case R.id.ch03_6:
                Ch03_6Activity.start(this);
                break;
            case R.id.ch03_7:
                Ch03_7Activity.start(this);
                break;
            case R.id.ch04_3:
                Ch04_3Activity.start(this);
                break;
        }
    }
}
