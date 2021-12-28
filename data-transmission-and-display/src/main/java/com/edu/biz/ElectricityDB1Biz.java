package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.ElectricityDB;
import com.edu.mapper.ElectricityDB1Mapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.util.Sqls;
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
public class ElectricityDB1Biz {

    @Resource
    private ElectricityDB1Mapper electricityDB1Mapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.ElectricityDB>
     */
    public List<ElectricityDB> page() {
        Example example = Example.builder(ElectricityDB.class)
                .andWhere(Sqls.custom()
                        .andEqualTo("epc", "A102F3202108250000000028"))
                .orderByDesc("createtime").build();
        return electricityDB1Mapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.ElectricityDB>
     */
    public List<ElectricityDB> queryDataByTimeType(String timeQueryType) {
        List<ElectricityDB> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        ElectricityDB electricityDB = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(electricityDB.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(electricityDB.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(electricityDB.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(electricityDB.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(electricityDB.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(ElectricityDB.class)
                .where(WeekendSqls.<ElectricityDB>custom()
                        .andEqualTo("epc", "A102F3202108250000000028")
                        .andGreaterThanOrEqualTo(ElectricityDB::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(ElectricityDB::getCreatetime, electricityDB.getCreatetime()))
                .build();
        return electricityDB1Mapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public ElectricityDB getNewData(){
        return electricityDB1Mapper.getNewData();
    }
}
