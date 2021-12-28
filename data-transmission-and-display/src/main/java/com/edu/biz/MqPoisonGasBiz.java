package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.MqPoisonGas;
import com.edu.mapper.MqPoisonGasMapper;
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
 * Mq有毒气体 biz
 *
 * @author yixiaowei
 */
@Service
public class MqPoisonGasBiz {

    @Resource
    private MqPoisonGasMapper mqPoisonGasMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.MqPoisonGas>
     */
    public List<MqPoisonGas> page() {
        Example example = Example.builder(MqPoisonGas.class)
                .orderByDesc("createtime").build();
        return mqPoisonGasMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.MqPoisonGas>
     */
    public List<MqPoisonGas> queryDataByTimeType(String timeQueryType) {
        List<MqPoisonGas> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        MqPoisonGas mqPoisonGas = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(mqPoisonGas.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(mqPoisonGas.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(mqPoisonGas.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(mqPoisonGas.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(mqPoisonGas.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(MqPoisonGas.class)
                .where(WeekendSqls.<MqPoisonGas>custom()
                        .andGreaterThanOrEqualTo(MqPoisonGas::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(MqPoisonGas::getCreatetime, mqPoisonGas.getCreatetime()))
                .build();
        return mqPoisonGasMapper.selectByExample(example);
    }

    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public MqPoisonGas getNewData(){
        return mqPoisonGasMapper.getNewData();
    }
}
