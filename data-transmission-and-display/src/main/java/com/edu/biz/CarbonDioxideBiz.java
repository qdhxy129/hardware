package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.mapper.CarbonDioxideMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.util.Sqls;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

/**
 * 二氧化碳biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class CarbonDioxideBiz {

    @Resource
    private CarbonDioxideMapper carbonDioxideMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.CarbonDioxide>
     */
    public List<CarbonDioxide> page() {
        Example example = Example.builder(CarbonDioxide.class)
                .orderByDesc("createtime").build();
        return carbonDioxideMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.CarbonDioxide>
     */
    public List<CarbonDioxide> queryDataByTimeType(String timeQueryType) {
        List<CarbonDioxide> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        CarbonDioxide carbonDioxide = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(carbonDioxide.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(carbonDioxide.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(carbonDioxide.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(carbonDioxide.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(carbonDioxide.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(CarbonDioxide.class)
                .where(WeekendSqls.<CarbonDioxide>custom()
                        .andGreaterThanOrEqualTo(CarbonDioxide::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(CarbonDioxide::getCreatetime, carbonDioxide.getCreatetime()))
                .build();
        return carbonDioxideMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public CarbonDioxide getNewData(){
        return carbonDioxideMapper.getNewData();
    }
}
