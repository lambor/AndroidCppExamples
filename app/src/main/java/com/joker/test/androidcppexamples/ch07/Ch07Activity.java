package com.joker.test.androidcppexamples.ch07;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.joker.test.androidcppexamples.R;

import org.w3c.dom.Text;

/**
 * Created by lambor on 17-2-27.
 */

public class Ch07Activity extends AppCompatActivity {

    private static final String EXTRA_NATIVETHREAD = "native_thread";

    public static void start(Context context,boolean useNativeThread) {
        Intent intent = new Intent(context,Ch07Activity.class);
        intent.putExtra(EXTRA_NATIVETHREAD,useNativeThread);
        context.startActivity(intent);
    }

    boolean useNativeThread = false;
    private EditText threadsEdit;
    private EditText iterationsEdit;
    private Button startButton;
    private TextView logView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch07);

        nativeInit();

        useNativeThread = getIntent().getBooleanExtra(EXTRA_NATIVETHREAD,false);
        threadsEdit = (EditText) findViewById(R.id.threads_edit);
        iterationsEdit = (EditText) findViewById(R.id.iterations_edit);
        startButton = (Button) findViewById(R.id.start_button);
        logView = (TextView) findViewById(R.id.log_view);

        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int threads = getNumber(threadsEdit,0);
                int iterations = getNumber(iterationsEdit,0);
                if(threads > 0 && iterations > 0) {
                    startThreads(threads,iterations);
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        nativeFree();
        super.onDestroy();
    }

    private void onNativeMessage(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logView.append(message);
                logView.append("\n");
            }
        });
    }

    private static int getNumber(EditText editText,int defaultValue) {
        int value;
        try {
            value = Integer.parseInt(editText.getText().toString());
        } catch (NumberFormatException e) {
            value = defaultValue;
        }
        return value;
    }

    private native void nativeInit();

    private native void nativeFree();

    private native void nativeWorker(int id,int iterations);

    static {
        System.loadLibrary("examplelib");
    }

    private void startThreads(int threads, final int iterations) {
        if(useNativeThread) {
            posixThread(threads,iterations);
        } else {
            javaThreads(threads, iterations);
        }
    }

    private void javaThreads(int threads, final int iterations) {
        for(int i=0;i<threads;i++) {
            final int id = i;
            Thread thread = new Thread() {
                public void run() {
                    nativeWorker(id,iterations);
                }
            };
            thread.start();
        }
    }

    private native void posixThread(int threads, int iterations);
}
