package ru.smarttransport.citytransport.routemodels;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

import com.google.gson.JsonObject;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Iterator;
import java.util.List;

/**
 * Created by alekse on 25.11.14.
 */
public class Advice implements Serializable
{

    //TRANSPORT MODES
    public static final int WAIT = 1;
    public static final int WALK = 2;
    public static final int BUS = 3;
    public static final int STOP = 4;

    private int fromStopId;

    private int toStopId;

    public   ArrayList<Leg> legs;

    private int transfers;

    private int walkTime;

    private int transitTime;

    private int waitingTime;

    private int walkDistance;

    private long departureTime;

    private long arrivalTime;

    public Advice() {
       super();
       this.fromStopId = 0;
       this.toStopId = 0;
       this.legs = new ArrayList<Leg>();
       this.transfers  = 0;
       this.walkTime = 0;
       this.transitTime = 0;
       this.waitingTime = 0;
       this.walkDistance = 0;
       this.departureTime = 0;
       this.arrivalTime = 0;
    }

    public static ArrayList<Advice> parseAdvice(String s)
    {

        ArrayList<Advice> result = new ArrayList<Advice>();
        try
        {
            JSONObject jsonObject =  new JSONObject(s);
            jsonObject = jsonObject.getJSONObject("plan");
            JSONObject fromObject  = jsonObject.getJSONObject("from");
            JSONObject toObject  = jsonObject.getJSONObject("to");
            JSONArray itineraries = jsonObject.getJSONArray("itineraries");
            for(int i = 0;i < itineraries.length() ; i++)
            {
                JSONObject trip = itineraries.getJSONObject(i);
                Advice newAdvice = new Advice();
                newAdvice.setFromStopId(Integer.parseInt(fromObject.getJSONObject("stopId").getString("id")));
                newAdvice.setToStopId(Integer.parseInt(toObject.getJSONObject("stopId").getString("id")));
                newAdvice.setTransfers(trip.getInt("transfers"));
                newAdvice.setWalkTime(trip.getInt("walkTime"));
                newAdvice.setTransitTime(trip.getInt("transitTime"));
                newAdvice.setWaitingTime(trip.getInt("waitingTime"));
                newAdvice.setWalkDistance(trip.getInt("walkDistance"));
                newAdvice.setDepartureTime(trip.getLong("startTime"));
                newAdvice.setArrivalTime(trip.getLong("endTime"));

                newAdvice.legs = new ArrayList<Leg>();
                JSONArray legsJson = trip.getJSONArray("legs");
                for(int j = 0; j < legsJson.length(); j++)
                {

                    JSONObject legObject = legsJson.getJSONObject(j);
                    Leg newLeg = new Leg();
                    newLeg.setFromStopId(Integer.parseInt(legObject.getJSONObject("from").getJSONObject("stopId").getString("id")));
                    newLeg.setToStopId(Integer.parseInt(legObject.getJSONObject("to").getJSONObject("stopId").getString("id")));
                    newLeg.setFromName(legObject.getJSONObject("from").getString("name"));
                    newLeg.setToName(legObject.getJSONObject("to").getString("name"));

                    newLeg.setDuration((int)(legObject.getLong("endTime") - legObject.getLong("startTime")));
                    String mode = legObject.getString("mode");
                    if(mode.equals("WALK") )
                    {
                        newLeg.setTransportMode(WALK);

                    }else if(mode.equals("WAIT"))
                    {
                        newLeg.setTransportMode(WAIT);
                    }
                    else if(mode.equals("BUS") || mode.equals("TRAM") )
                    {
                        newLeg.setTransportMode(BUS);
                        newLeg.setRouteId(Integer.parseInt(legObject.getString("routeId")));
                        newLeg.setRouteShortName(legObject.getString("routeShortName"));
                    }
                    newAdvice.legs.add(newLeg);



                }

                result.add(newAdvice);



            }

        }catch (Exception ex)
        {
            Log.e("INITDATA", ex.getLocalizedMessage());
        }
        return result;
    }

    public int getFromStopId() {
        return fromStopId;
    }

    public void setFromStopId(int fromStopId) {
        this.fromStopId = fromStopId;
    }

    public int getToStopId() {
        return toStopId;
    }

    public void setToStopId(int toStopId) {
        this.toStopId = toStopId;
    }

    public int getTransfers() {
        return transfers;
    }

    public void setTransfers(int transfers) {
        this.transfers = transfers;
    }

    public int getWalkTime() {
        return walkTime;
    }

    public void setWalkTime(int walkTime) {
        this.walkTime = walkTime;
    }

    public int getTransitTime() {
        return transitTime;
    }

    public void setTransitTime(int transitTime) {
        this.transitTime = transitTime;
    }

    public int getWaitingTime() {
        return waitingTime;
    }

    public void setWaitingTime(int waitingTime) {
        this.waitingTime = waitingTime;
    }

    public int getWalkDistance() {
        return walkDistance;
    }

    public void setWalkDistance(int walkDistance) {
        this.walkDistance = walkDistance;
    }

    public long getDepartureTime() {
        return departureTime;
    }

    public void setDepartureTime(long departureTime) {
        this.departureTime = departureTime;
    }

    public long getArrivalTime() {
        return arrivalTime;
    }

    public void setArrivalTime(long arrivalTime) {
        this.arrivalTime = arrivalTime;
    }


}
