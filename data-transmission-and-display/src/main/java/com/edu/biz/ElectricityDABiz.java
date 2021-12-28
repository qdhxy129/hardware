package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.ElectricityDA;
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
public class ElectricityDABiz {

    @Resource
    private com.edu.mapper.ElectricityDAMapper electricityDAMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.ElectricityDA>
     */
    public List<ElectricityDA> page() {
        Example example = Example.builder(ElectricityDA.class)
                .andWhere(Sqls.custom()
                        .andEqualTo("epc", "A103F2202108250000000001"))
                .orderByDesc("createtime").build();
        return electricityDAMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.ElectricityDA>
     */
    public List<ElectricityDA> queryDataByTimeType(String timeQueryType) {
        List<ElectricityDA> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        ElectricityDA electricityDA = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(electricityDA.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(electricityDA.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(electricityDA.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(electricityDA.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(electricityDA.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(ElectricityDA.class)
                .where(WeekendSqls.<ElectricityDA>custom()
                        .andEqualTo("epc", "A103F2202108250000000001")
                        .andGreaterThanOrEqualTo(ElectricityDA::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(ElectricityDA::getCreatetime, electricityDA.getCreatetime()))
                .build();
        return electricityDAMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public ElectricityDA getNewData(){
        return electricityDAMapper.getNewData();
    }
}
