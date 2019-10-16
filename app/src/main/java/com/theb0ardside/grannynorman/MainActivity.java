package com.theb0ardside.grannynorman;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private native void startEngine(AssetManager mgr);

    private AssetManager mgr;

    private boolean active;


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView textView = (TextView) findViewById(R.id.text_view);
        textView.setText("++++//JIMMMMEMEM++++++++");

        mgr = getResources().getAssets();
        active = true;

        startEngine(mgr);

    }


}
