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

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private native void startEngine();

    private native void tap(boolean b);

    private native void setFrequency(float frequency);

    private native void loadSamples(AssetManager mgr);

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

        SensorManager sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        Sensor rotationSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        sensorManager.registerListener(this, rotationSensor, SensorManager.SENSOR_DELAY_FASTEST);

        mgr = getResources().getAssets();
        active = true;

        loadSamples(mgr);
        startEngine();

    }


    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            active = !active;
            tap(active);
//        } else if (event.getAction() == MotionEvent.ACTION_UP) {
//            tap(false);
        }
        return super.onTouchEvent(event);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        setFrequency(20 + (event.values[0] * 80));
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
