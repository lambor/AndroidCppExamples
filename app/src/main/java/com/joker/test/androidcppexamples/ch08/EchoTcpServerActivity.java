package com.joker.test.androidcppexamples.ch08;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.View;

import com.joker.test.androidcppexamples.R;

/**
 * Created by lambor on 17-3-1.
 */

public class EchoTcpServerActivity extends AbstractEchoActivity {

    public static void start(Context context) {
        context.startActivity(new Intent(context,EchoTcpServerActivity.class));
    }

    public EchoTcpServerActivity() {
        super(R.layout.activity_ch08_2_echoserver);
    }

    @Override
    protected void onStartButtonClicked() {
        Integer port = getPort();
        if(port != null) {
            ServerTask serverTask = new ServerTask(port);
            serverTask.start();
        }
    }

    private native void nativeStartTcpServer(int port) throws Exception;

    private class ServerTask extends AbstractEchoTask {

        private final int port;

        public ServerTask(int port) {
            this.port = port;
        }

        @Override
        protected void onBackground() {
            logMessage("Starting server.");

            try {
                nativeStartTcpServer(port);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }

            logMessage("Server terminated.");
        }
    }

    @Override
    protected void initOnCreate(@Nullable Bundle savedInstanceState) {
        super.initOnCreate(savedInstanceState);
        findViewById(R.id.start_client).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EchoTcpClientActivity.start(EchoTcpServerActivity.this);
            }
        });
    }
}
