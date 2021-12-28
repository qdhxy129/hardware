package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.CarbonMonoxide;
import com.edu.mapper.CarbonMonoxideMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * CO biz
 *
 * @author yixiaowei
 */
@Service
public class CarbonMonoxideBiz {

    @Resource
    private CarbonMonoxideMapper carbonMonoxideMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.CarbonMonoxide>
     */
    public List<CarbonMonoxide> page() {
        Example example = Example.builder(CarbonMonoxide.class)
                .orderByDesc("createtime").build();
        return carbonMonoxideMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.CarbonMonoxide>
     */
    public List<CarbonMonoxide> queryDataByTimeType(String timeQueryType) {
        List<CarbonMonoxide> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        CarbonMonoxide carbonMonoxide = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(carbonMonoxide.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(carbonMonoxide.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(carbonMonoxide.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(carbonMonoxide.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(carbonMonoxide.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(CarbonMonoxide.class)
                .where(WeekendSqls.<CarbonMonoxide>custom()
                        .andGreaterThanOrEqualTo(CarbonMonoxide::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(CarbonMonoxide::getCreatetime, carbonMonoxide.getCreatetime()))
                .build();
        return carbonMonoxideMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public CarbonMonoxide getNewData(){
        return carbonMonoxideMapper.getNewData();
    }
}
