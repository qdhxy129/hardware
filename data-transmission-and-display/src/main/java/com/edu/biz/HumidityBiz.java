package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.Humidity;
import com.edu.mapper.HumidityMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

/**
 * 湿度biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class HumidityBiz {

    @Resource
    private HumidityMapper humidityMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.Humidity>
     */
    public List<Humidity> page() {
        Example example = Example.builder(Humidity.class)
                .orderByDesc("createtime").build();
        return humidityMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.Humidity>
     */
    public List<Humidity> queryDataByTimeType(String timeQueryType) {
        List<Humidity> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Humidity humidity = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(humidity.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(humidity.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(humidity.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(humidity.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(humidity.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Humidity.class)
                .where(WeekendSqls.<Humidity>custom()
                        .andGreaterThanOrEqualTo(Humidity::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Humidity::getCreatetime, humidity.getCreatetime()))
                .build();
        return humidityMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public Humidity getNewData(){
        return humidityMapper.getNewData();
    }
}
