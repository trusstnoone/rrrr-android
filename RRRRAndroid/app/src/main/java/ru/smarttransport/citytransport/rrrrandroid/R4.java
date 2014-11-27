package ru.smarttransport.citytransport.rrrrandroid;

/**
 * Created by alekse on 26.11.14.
 */
public class R4 {

    static {
        System.loadLibrary("R4"); // "myjni.dll" in Windows, "libmyjni.so" in Unixes
    }

    // A native method that returns a Java String to be displayed on the
    // TextView
    public native String getMessage();

    public native boolean initWithFile(String filePath);



    public native void nativePipeSTDERRToLogcat();

    public native int getStopIndexbyId(String stopId);

    private native String planRoute(int from, int to, boolean arriveby, long time);


}
