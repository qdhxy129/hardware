package com.edu.unit;

import java.util.Calendar;
import java.util.Date;

/**
 * 获取最近时间的工具类
 *
 * @author 姜振炜
 * @date 2021/8/11 19:53
 */
public class RecentTime {

    /**
     * 获取一小时前的时间
     * @param date 指定时间
     * @return java.util.Date
     */
    public static Date getTheTimeOneHourAgo(Date date) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        calendar.add(Calendar.HOUR, -1);
        return calendar.getTime();
    }

    /**
     * 获取一天前的时间
     * @param date 指定时间
     * @return java.util.Date
     */
    public static Date getTheTimeOneDayAgo(Date date) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        calendar.add(Calendar.DATE, -1);
        return calendar.getTime();
    }

    /**
     * 获取一周前的时间
     * @param date 指定时间
     * @return java.util.Date
     */
    public static Date getTheTimeOneWeekAgo(Date date) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        calendar.add(Calendar.DAY_OF_WEEK, -7);
        return calendar.getTime();
    }

    /**
     * 获取一个月前的时间
     * @param date 指定时间
     * @return java.util.Date
     */
    public static Date getTheTimeOneMonthAgo(Date date) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        calendar.add(Calendar.MONTH, -1);
        return calendar.getTime();
    }

    /**
     * 获取一年前的时间
     * @param date 指定时间
     * @return java.util.Date
     */
    public static Date getTheTimeOneYearAgo(Date date) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        calendar.add(Calendar.YEAR, -1);
        return calendar.getTime();
    }
}
