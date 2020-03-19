package com.inf3995.PracticeApp.Utils;

public class ErrorInfo {
    private final static String NOT_IDENTIFIED = "Path parameter \"Id\" value must not be null.";

    public static String convertErrorMessage(String message) {
        if (message.equals(NOT_IDENTIFIED))
            return "You're not identified!";

        return message;
    }
}
