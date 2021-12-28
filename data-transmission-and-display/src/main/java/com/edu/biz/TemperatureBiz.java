package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.dto.TemHumDTO;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.Temperature;
import com.edu.mapper.HumidityMapper;
import com.edu.mapper.TemperatureMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.util.Sqls;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.stream.Collectors;

/**
 * 温度biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:56
 */
@Service
public class TemperatureBiz {

    @Resource
    private TemperatureMapper temperatureMapper;

    @Resource
    private HumidityMapper humidityMapper;

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.Temperature>
     */
    public List<Temperature> queryDataByTimeType(String timeQueryType) {
        List<Temperature> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Temperature temperature = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(temperature.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(temperature.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(temperature.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(temperature.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(temperature.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Temperature.class)
                .where(WeekendSqls.<Temperature>custom()
                        .andGreaterThanOrEqualTo(Temperature::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Temperature::getCreatetime, temperature.getCreatetime()))
                .build();
        return temperatureMapper.selectByExample(example);
    }

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.Temperature>
     */
    public List<Temperature> page() {
        Example example = Example.builder(Temperature.class)
                .orderByDesc("createtime").build();
        return temperatureMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public Temperature getNewData(){
        return temperatureMapper.getNewData();
    }



    /**
     * 获取温湿度数据
     * @author yixiaowei
     * @date 21:34 2021/12/26
     * @return java.util.Map
     */
    public Map getTemHumDataList(){
        // 返回结果
        Map resultMap = new HashMap();

        // 定义温度集合
        List<TemHumDTO> temList = new ArrayList<>();
        // 定义湿度集合
        List<TemHumDTO> humList = new ArrayList<>();


        // 查询温度结果
        temperatureMapper.getTemList().forEach(item ->{
            TemHumDTO temHumDTO = new TemHumDTO();
            temHumDTO.setQuotaValue(item.getMessage());
            temHumDTO.setCreateTime(item.getCreatetime());
            temList.add(temHumDTO);
        });

        // 查询湿度结果
        humidityMapper.getHumList().forEach(item ->{
            TemHumDTO temHumDTO = new TemHumDTO();
            temHumDTO.setQuotaValue(item.getMessage());
            temHumDTO.setCreateTime(item.getCreatetime());
            humList.add(temHumDTO);
        });

        resultMap.put("temperature", temList);
        resultMap.put("humidity", humList);

        return resultMap;
    }
}
