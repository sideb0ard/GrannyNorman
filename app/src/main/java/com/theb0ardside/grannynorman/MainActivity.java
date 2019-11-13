package com.theb0ardside.grannynorman;


import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private native void startEngine(AssetManager mgr);

    private native void setGrainsPerSecond(float val);

    private native float getGrainsPerSecond();

    private native void setGrainDuration(float val);

    private native float getGrainDuration();

    private native void setGrainSpray(float val);

    private native float getGrainSpray();

    private native void setGrainFudge(float val);

    private native float getGrainFudge();

    private native void setGranularMode(int mode);

    private native void setGrainIndex(int val);

    private native int getGrainIndex();

    private native void toggleGranularMode();

    private native void toggleOnOffMode();

    private native void setEnvelopeMode(int val);

    private native void reset();
    private native void scramble();
    private native void stutter();

    private AssetManager mgr;

    private boolean active;

    SeekBar seekbar_grain_index; // 0-100 % of length
    private TextView grain_index_value;

    SeekBar seekbar_grains_per_second;
    private int seekbar_grains_per_second_min = 1;
    private int seekbar_grains_per_second_max = 50;
    private TextView grains_per_second_value;

    SeekBar grain_duration_seekbar;
    private int seekbar_grain_duration_min = 10; // ms
    private int seekbar_grain_duration_max = 200; // ms
    private TextView grain_duration_value;

    SeekBar seekbar_spray; // random start offset from playback idx in ms
    private int seekbar_spray_min = 0; // ms
    private int seekbar_spray_max = 500; // ms
    private TextView spray_value;

    SeekBar seekbar_fudge; // random diff added to duration
    private int seekbar_fudge_min = 0;
    private int seekbar_fudge_max = 500;
    private TextView fudge_value;

    private TextView sample_display_num;

    Button modeButton;
    Button onOffButton;
    Button resetButton;
    Button stutterButton;
    Button scrambleButton;

    Spinner envelope_spinner;

    private int sample_number = 0; // 0-3 for drums, bass, strings, thingie


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    protected void updateInterface() {

        grain_index_value.setText(String.valueOf(0));
        seekbar_grain_index.setProgress(0);

        float grains_per_sec = getGrainsPerSecond();
        grains_per_second_value.setText(String.valueOf(grains_per_sec));
        seekbar_grains_per_second.setProgress((int) Utilz.scale(grains_per_sec, seekbar_grains_per_second_min, seekbar_grains_per_second_max, 0, 100));

        float grain_duration = getGrainDuration();
        grain_duration_value.setText(String.valueOf(grain_duration));
        grain_duration_seekbar.setProgress((int) Utilz.scale(grain_duration, seekbar_grain_duration_min, seekbar_grain_duration_max, 0, 100));

        float grain_spray = getGrainSpray();
        spray_value.setText(String.valueOf(grain_spray));
        seekbar_spray.setProgress((int) Utilz.scale(grain_spray, seekbar_spray_min, seekbar_spray_max, 0, 100));

        float grain_fudge = getGrainFudge();
        fudge_value.setText(String.valueOf(grain_fudge));
        seekbar_fudge.setProgress((int) Utilz.scale(grain_fudge, seekbar_fudge_min, seekbar_fudge_max, 0, 100));

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        Log.v("JAVAME", "Starting UP, YO!\n\n");

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mgr = getResources().getAssets();
        active = true;

        seekbar_grain_index
                = (SeekBar) findViewById(R.id.seekBar5);
        grain_index_value = (TextView) findViewById(R.id.textView9);

        sample_display_num = (TextView) findViewById(R.id.SampleNum);

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

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_grains_per_second_min, seekbar_grains_per_second_max);
                                setGrainsPerSecond((float) scaled_val);
                                grains_per_second_value.setText(String.valueOf(scaled_val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        grain_duration_seekbar = (SeekBar) findViewById(R.id.seekBar2);
        grain_duration_value = (TextView) findViewById(R.id.textView4);
        grain_duration_seekbar
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_grain_duration_min, seekbar_grain_duration_max);
                                setGrainDuration((float) scaled_val);
                                grain_duration_value.setText(String.valueOf(scaled_val));
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

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_spray_min, seekbar_spray_max);
                                setGrainSpray((float) scaled_val);
                                spray_value.setText(String.valueOf(scaled_val));
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

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_fudge_min, seekbar_fudge_max);
                                setGrainFudge((float) scaled_val);
                                fudge_value.setText(String.valueOf(scaled_val));

                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        modeButton = findViewById(R.id.button);
        modeButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                toggleGranularMode();
                // Code here executes on main thread after user presses button
            }
        });

        onOffButton = findViewById(R.id.button2);
        onOffButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                toggleOnOffMode();
            }
        });

        resetButton = findViewById(R.id.button3);
        resetButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                reset();
                updateInterface();
            }
        });


        scrambleButton = findViewById(R.id.button4);
        scrambleButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                scramble();
            }
        });

        stutterButton = findViewById(R.id.button5);
        stutterButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                stutter();
            }
        });


        startEngine(mgr);

        updateInterface();

        envelope_spinner = (Spinner) findViewById(R.id.spinner);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.envelope_types, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        envelope_spinner.setAdapter(adapter);
        envelope_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                setEnvelopeMode(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });


    }

    public void onRadioButtonClicked(View view) {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch(view.getId()) {
            case R.id.radio_drum:
                if (checked)
                    sample_number = 0;
                    break;
            case R.id.radio_bass:
                if (checked)
                    sample_number = 1;
                    break;
            case R.id.radio_strings:
                if (checked)
                    sample_number = 2;
                    break;
            case R.id.radio_thingie:
                if (checked)
                    sample_number = 3;
                    break;
        }

        sample_display_num.setText(String.valueOf(sample_number));
    }


}
