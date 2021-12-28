package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.Light;
import com.edu.mapper.LightMapper;
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
 * 光照biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class LightBiz {

    @Resource
    private LightMapper lightMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.Light>
     */
    public List<Light> page() {
        Example example = Example.builder(Light.class)
                .orderByDesc("createtime")
                .build();
        return lightMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.Light>
     */
    public List<Light> queryDataByTimeType(String timeQueryType) {
        List<Light> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Light light = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(light.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(light.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(light.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(light.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(light.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Light.class)
                .where(WeekendSqls.<Light>custom()
                        .andGreaterThanOrEqualTo(Light::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Light::getCreatetime, light.getCreatetime()))
                .build();
        return lightMapper.selectByExample(example);
    }

    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public Light getNewData(){
        return lightMapper.getNewData();
    }
}
