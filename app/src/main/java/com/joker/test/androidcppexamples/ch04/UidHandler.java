package com.joker.test.androidcppexamples.ch04;

import android.widget.TextView;

import com.joker.test.hello_mk.AsyncUidProvider;


/**
 * Created by lambor on 17-2-21.
 */

public class UidHandler extends AsyncUidProvider {
    private final TextView textView;

    public UidHandler(TextView textView) {
        this.textView = textView;
    }

    @Override
    public void onUid(long uid) {
        textView.setText("Uid:"+uid);
    }
}
