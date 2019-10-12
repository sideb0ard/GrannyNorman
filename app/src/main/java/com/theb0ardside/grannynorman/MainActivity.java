package com.theb0ardside.grannynorman;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private native void startEngine();
    private native void tap(boolean b);
    private native void setFrequency(float frequency);

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            tap(true);
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            tap(false);
        }
        return super.onTouchEvent(event);
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        startEngine();


    }
}
