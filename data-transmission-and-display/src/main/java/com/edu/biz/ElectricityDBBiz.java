package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.ElectricityDB;
import com.edu.mapper.ElectricityDB1Mapper;
import com.edu.mapper.ElectricityDBMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.util.Sqls;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.math.BigDecimal;
import java.util.*;

/**
 * CO biz
 *
 * @author yixiaowei
 */
@Service
public class ElectricityDBBiz {

    @Resource
    private ElectricityDBMapper electricityDBMapper;

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
                        .andEqualTo("epc", "A103F2202108250000000001"))
                .orderByDesc("createtime").build();
        return electricityDBMapper.selectByExample(example);
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
                        .andEqualTo("epc", "A103F2202108250000000001")
                        .andGreaterThanOrEqualTo(ElectricityDB::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(ElectricityDB::getCreatetime, electricityDB.getCreatetime()))
                .build();
        return electricityDBMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public ElectricityDB getNewData(){
        return electricityDBMapper.getNewData();
    }


    /**
     * 获取耗电信息
     * @author yixiaowei
     * @date 20:41 2021/12/26
     * @return com.edu.entity.ElectricityDB
     */
    public Map getElectricityList() {
        List<ElectricityDB> electrityDBList = electricityDBMapper.getElectrityDBList();

        List<ElectricityDB> electrityDB1List = electricityDB1Mapper.getElectrityDB1List();

        Map map = new HashMap();

        // 定义总耗电电量
        BigDecimal totalPowerData = new BigDecimal("0");

        // 定义当天消耗总电量
        BigDecimal todayPowerData = new BigDecimal("0");

        // 总电量相加
        for (ElectricityDB item : electrityDBList) {
            for (ElectricityDB item2 : electrityDB1List) {
                todayPowerData = todayPowerData.add(new BigDecimal(item.getTotalBattery()).add(new BigDecimal(item2.getTotalBattery())));
            }
        }

        // 相加总耗电量
        for (ElectricityDB elItem1 : electricityDBMapper.selectAll()) {
            totalPowerData = totalPowerData.add(new BigDecimal(elItem1.getTotalBattery()));
        }

        for (ElectricityDB elItem2 : electricityDB1Mapper.selectAll()) {
            totalPowerData = totalPowerData.add(new BigDecimal(elItem2.getTotalBattery()));
        }



        // 实际总电量（全部数据相加）
        map.put("totalPowerData", totalPowerData);
        map.put("todayPowerData", todayPowerData);
        // 两个设备的前五条数据
        map.put("electrityDBList", electrityDBList);
        map.put("electrityDB1List", electrityDB1List);

        return map;
    }


}
