package com.joker.test.androidcppexamples.ch09;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.widget.EditText;

import com.joker.test.androidcppexamples.R;
import com.joker.test.androidcppexamples.ch08.AbstractEchoActivity;

/**
 * Created by lambor on 17-3-6.
 */

public class EchoUdpClientActivity extends AbstractEchoActivity {

    public static void start(Context context) {
        context.startActivity(new Intent(context,EchoUdpClientActivity.class));
    }

    private EditText ipEdit;
    private EditText messageEdit;

    public EchoUdpClientActivity() {
        super(R.layout.activity_ch08_2_echoclient);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ipEdit = (EditText) findViewById(R.id.ip_edit);
        messageEdit = (EditText) findViewById(R.id.message_edit);
    }

    @Override
    protected void onStartButtonClicked() {
        String ip = ipEdit.getText().toString();
        Integer port = getPort();
        String message = messageEdit.getText().toString();

        if(ip.length()!=0 && port != null && message.length()!=0) {
            ClientTask clientTask = new ClientTask(ip,port,message);
            clientTask.start();
        }
    }

    private native void nativeStartUdpClient(String ip, int port, String message) throws Exception;

    private class ClientTask extends AbstractEchoTask {

        private final String ip;
        private final int port;
        private final String message;

        public ClientTask(String ip,int port,String message) {
            this.ip = ip;
            this.port = port;
            this.message = message;
        }

        @Override
        protected void onBackground() {
            logMessage("Starting client.");

            try {
                nativeStartUdpClient(ip,port,message);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }

            logMessage("client terminated.");
        }
    }
}
