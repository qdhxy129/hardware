package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.Smoke;
import com.edu.mapper.SmokeMapper;
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
 * 烟雾biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class SmokeBiz {

    @Resource
    private SmokeMapper smokeMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.Smoke>
     */
    public List<Smoke> page() {
        Example example = Example.builder(Smoke.class)
                .orderByDesc("createtime")
                .build();
        return smokeMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.Temperature>
     */
    public List<Smoke> queryDataByTimeType(String timeQueryType) {
        List<Smoke> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Smoke smoke = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(smoke.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(smoke.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(smoke.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(smoke.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(smoke.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Smoke.class)
                .where(WeekendSqls.<Smoke>custom()
                        .andGreaterThanOrEqualTo(Smoke::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Smoke::getCreatetime, smoke.getCreatetime()))
                .build();
        return smokeMapper.selectByExample(example);
    }

    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public Smoke getNewData(){
        return smokeMapper.getNewData();
    }

    public List<Smoke> getSmokeList() {
        return smokeMapper.getSmokeList();
    }
}
