package com.theb0ardside.grannynorman;

public class Utilz {

    public static double scale(final double valueIn, final double baseMin, final double baseMax, final double limitMin, final double limitMax) {
        double divvy = baseMax - baseMin;
        if (divvy != 0) {
            return ((limitMax - limitMin) * (valueIn - baseMin) / divvy) + limitMin;
        } else {
            return valueIn;
        }
    }
}
