package ru.smarttransport.citytransport.rrrrandroid;

import android.app.Activity;
import android.content.ContextWrapper;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import java.io.File;
import java.util.ArrayList;
import java.util.Random;

import ru.smarttransport.citytransport.routemodels.Advice;


public class MainActivity extends Activity {

    R4 r4;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        r4 = new R4();
        new Thread() {
            public void run() {
                r4.nativePipeSTDERRToLogcat();
            }
        }.start();
        copyTimetable();
        testR4();
    }

    public void testR4()
    {
        try {
            Random r = new Random();
            int success = 0;

            boolean result = r4.initWithFile(new ContextWrapper(this).getFilesDir().getAbsolutePath()
                    + File.separator + "timetable4.dat");

                int fromIdx = 84;
                int toIdx = 90;

                Log.e("router", "from " + String.valueOf(fromIdx));
                Log.e("router", "to" + String.valueOf(toIdx));


                String planRoute = r4.planRoute(fromIdx, toIdx, false, java.util.Calendar.getInstance().getTime().getTime());
                ArrayList<Advice> advices = Advice.parseAdvice(planRoute);

                if (advices.size() > 0)
                    success++;

                Log.e("router", " plan :" + planRoute);


//            for (int i = 1; i < 500; i++) {
//                int fromIdx = i;
//                int toIdx = 500 - i;
//
//                Log.e("router", "from " + String.valueOf(fromIdx));
//                Log.e("router", "to" + String.valueOf(toIdx));
//
//
//                String planRoute = r4.planRoute(fromIdx, toIdx, false, java.util.Calendar.getInstance().getTime().getTime());
//                ArrayList<Advice> advices = Advice.parseAdvice(planRoute);
//
//                if (advices.size() > 0)
//                    success++;
//
//                Log.e("router", " plan :" + planRoute);
//            }

            Log.e("router", "count find routes" + String.valueOf(success));
        }catch (Exception ex)
        {

        }

    }

    public void copyTimetable()
    {
        Utils.copyFile("timetable.dat",this);
        Utils.copyFile("timetable4.dat",this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


    //HackString for generate R4.header
    // javah -classpath /Users/alekse/Documents/GitProject/its/rrrr/RRRRAndroid/app/build/intermediates/classes/debug:/Users/alekse/Documents/AndroidSDK/sdk/platforms/android-21/android.jar ru.smarttransport.citytransport.rrrrandroid.R4
}
