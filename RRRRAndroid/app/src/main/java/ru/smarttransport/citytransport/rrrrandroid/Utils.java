package ru.smarttransport.citytransport.rrrrandroid;

import android.content.Context;
import android.content.ContextWrapper;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by alekse on 06.02.15.
 */
public class Utils {

    public static void copyFile(String filename, Context context) {
        ContextWrapper contextWrapper = new ContextWrapper(context);
        String pathDest = contextWrapper.getFilesDir().getAbsolutePath()
                + File.separator;

        AssetManager assetManager = context.getAssets();
        String[] files = null;
        try {
            files = assetManager.list("");
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }
        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(filename);

            File fileFolder = new File(pathDest);

            if (!fileFolder.exists())
                fileFolder.mkdirs();

            File outDbFile = new File(pathDest, filename);

            if (!outDbFile.exists())
                outDbFile.createNewFile();
            else {
                outDbFile.delete();
                outDbFile.createNewFile();
            }

            out = new FileOutputStream(outDbFile);
            copyStream(in, out);
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
            Log.d("INITDATA", "sucsecc  copy : " + filename);
        } catch (IOException e) {
            Log.e("INITDATA", "Failed to copy: " + filename, e);
        }

    }

    private static void copyStream(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while ((read = in.read(buffer)) != -1) {
            out.write(buffer, 0, read);
        }
    }
}
