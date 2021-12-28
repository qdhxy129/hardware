package com.edu.rest;

import com.edu.biz.ElectricityDABiz;
import com.edu.biz.MqPoisonGasBiz;
import com.edu.entity.ElectricityDA;
import com.edu.entity.MqPoisonGas;
import com.edu.mapper.ElectricityDAMapper;
import com.edu.mapper.MqPoisonGasMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * ElectricityDA
 * @author yixiaowei
 */
@RestController
@RequestMapping("electricityDA")
public class ElectricityDAController {

    @Resource
    private ElectricityDAMapper electricityDAMapper;

    @Resource
    private ElectricityDABiz electricityDABiz;

    @GetMapping("page")
    public Message<PageInfo<ElectricityDA>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<ElectricityDA>> message = new Message<>();
        List<ElectricityDA> humidityList = electricityDABiz.page();
        PageInfo<ElectricityDA> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<ElectricityDA>> selectHumidityAll() {
        Message<List<ElectricityDA>> message = new Message<>();
        message.success().setData(electricityDAMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.ElectricityDA>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<ElectricityDA>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<ElectricityDA>> message = new Message<>();
        message.success().setData(electricityDABiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
