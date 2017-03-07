package com.joker.test.androidcppexamples.ch08;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

import com.joker.test.androidcppexamples.R;

/**
 * Created by lambor on 17-3-1.
 */

public abstract class AbstractEchoActivity extends AppCompatActivity implements View.OnClickListener {
    protected EditText portEdit;
    protected Button startButton;
    protected ScrollView logScroll;
    protected TextView logView;
    private final int layoutID;

    public AbstractEchoActivity(int layoutID) {
        this.layoutID = layoutID;
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layoutID);

        portEdit = (EditText) findViewById(R.id.port_edit);
        startButton = (Button) findViewById(R.id.start_button);
        logScroll = (ScrollView) findViewById(R.id.log_scroll);
        logView = (TextView) findViewById(R.id.log_view);

        startButton.setOnClickListener(this);

        initOnCreate(savedInstanceState);
    }

    protected void initOnCreate(@Nullable Bundle savedInstanceState) {

    }

    @Override
    public void onClick(View v) {
        if(v == startButton) {
            onStartButtonClicked();
        }
    }

    protected abstract void onStartButtonClicked();

    protected Integer getPort() {
        Integer port;
        try {
            port = Integer.valueOf(portEdit.getText().toString());
        } catch (NumberFormatException e) {
            port = null;
        }

        return port;
    }

    protected void logMessage(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logMessageDirect(message);
            }
        });
    }

    protected void logMessageDirect(final String message) {
        logView.append(message);
        logView.append("\n");
        logScroll.fullScroll(View.FOCUS_DOWN);
    }

    protected abstract class AbstractEchoTask extends Thread {
        private final Handler handler;

        public AbstractEchoTask() {
            handler = new Handler();
        }

        protected void onPreExecute() {
            startButton.setEnabled(false);
            logView.setText("");
        }

        public synchronized void start() {
            onPreExecute();
            super.start();
        }

        public void run() {
            onBackground();
            handler.post(new Runnable() {
                @Override
                public void run() {
                    onPostExecute();
                }
            });
        }

        protected abstract void onBackground();

        protected void onPostExecute() {
            startButton.setEnabled(true);
        }
    }

    static {
        System.loadLibrary("examplelib");
    }
}
