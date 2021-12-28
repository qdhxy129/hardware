package com.saier.socket.util;



import java.util.Date;

/**
 * DA和DB间隔时间保存类
 *
 * @author 姜振炜
 * @date 2021/8/18 19:32
 */

public class IntervalsDate {

    private static Date responseTimeDa = null;

    /**
     * DA,DB之间的最大时间间隔
     */
    private static final Long JUDGMENT_INTERVAL = 11L;


    public static void init() {
        responseTimeDa = new Date();
    }

    /**
     * 判断是否符合条件的da
     *
     * @return boolean
     */
    public static boolean isProcessing() {
        if (responseTimeDa == null) {
            return false;
        }
        Date date = new Date();
        long actualInterval = (date.getTime() - responseTimeDa.getTime())/1000;
        close();
        return JUDGMENT_INTERVAL > actualInterval;
    }

    public static void close() {
        responseTimeDa = null;
    }

}
