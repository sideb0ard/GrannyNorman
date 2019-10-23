package com.theb0ardside.grannynorman;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private native void startEngine(AssetManager mgr);

    private native void setGrainsPerSecond(float val);

    private native void setGrainDuration(float val);

    private native void setGrainSpray(float val);

    private native void setGrainFudge(float val);

    private native void setGranularMode(int mode);

    private native void setGrainIndex(int val);

    private AssetManager mgr;

    private boolean active;

    SeekBar seekbar_grain_index; // 0-100
    private TextView grain_index_value;

    SeekBar seekbar_grains_per_second; // 10 - 200?
    private TextView grains_per_second_value;

    SeekBar seekbar_grain_duration; // ms. 10ms - 200?
    private TextView grain_duration_value;

    SeekBar seekbar_spray; // random start offset from playback idx in ms
    private TextView spray_value;

    SeekBar seekbar_fudge; // random diff added to duration
    private TextView fudge_value;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mgr = getResources().getAssets();
        active = true;

        seekbar_grain_index
                = (SeekBar) findViewById(R.id.seekBar5);
        grain_index_value = (TextView) findViewById(R.id.textView9);

        seekbar_grain_index
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainIndex(val);
                                grain_index_value.setText(String.valueOf(val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_grains_per_second
                = (SeekBar) findViewById(R.id.seekBar);
        grains_per_second_value = (TextView) findViewById(R.id.gps_value);

        seekbar_grains_per_second
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainsPerSecond(val);
                                grains_per_second_value.setText(String.valueOf(val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_grain_duration = (SeekBar) findViewById(R.id.seekBar2);
        grain_duration_value = (TextView) findViewById(R.id.textView4);
        seekbar_grain_duration
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainDuration(val);
                                grain_duration_value.setText(String.valueOf(val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_spray = (SeekBar) findViewById(R.id.seekBar3);
        spray_value = (TextView) findViewById(R.id.textView6);
        seekbar_spray
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainSpray(val);
                                spray_value.setText(String.valueOf(val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_fudge = (SeekBar) findViewById(R.id.seekBar4);
        fudge_value = (TextView) findViewById(R.id.textView8);
        seekbar_fudge
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainFudge(val);
                                fudge_value.setText(String.valueOf(val));

                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });


        startEngine(mgr);


    }


}
