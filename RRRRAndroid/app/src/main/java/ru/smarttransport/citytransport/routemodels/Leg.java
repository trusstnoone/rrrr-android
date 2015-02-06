package ru.smarttransport.citytransport.routemodels;

import java.io.Serializable;
import java.util.ArrayList;


/**
 * Created by alekse on 28.11.14.
 */
public class Leg implements Serializable {

    //TRANSPORT MODES
    public static final int WAIT = 1;
    public static final int WALK = 2;
    public static final int BUS = 3;
    public static final int STOP = 4;

    private int fromStopId;

    private int toStopId;

    private String fromName;

    private String toName;

    private int transportMode;

    private int duration;

    //if bus need
    private int routeId;

    public Leg() {
        super();
    }

    private String routeShortName;

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

    public String getFromName() {
        return fromName;
    }

    public void setFromName(String fromName) {
        this.fromName = fromName;
    }

    public String getToName() {
        return toName;
    }

    public void setToName(String toName) {
        this.toName = toName;
    }

    public int getTransportMode() {
        return transportMode;
    }

    public void setTransportMode(int transportMode) {
        this.transportMode = transportMode;
    }

    public int getDuration() {
        return duration;
    }

    public void setDuration(int duration) {
        this.duration = duration;
    }

    public int getRouteId() {
        return routeId;
    }

    public void setRouteId(int routeId) {
        this.routeId = routeId;
    }

    public String getRouteShortName() {
        return routeShortName;
    }

    public void setRouteShortName(String routeShortName) {
        this.routeShortName = routeShortName;
    }



}
