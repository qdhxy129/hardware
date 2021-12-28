package com.edu.rest;

import com.edu.biz.ElectricityDA1Biz;
import com.edu.entity.ElectricityDA;
import com.edu.mapper.ElectricityDA1Mapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * ElectricityDA
 *
 * @author yixiaowei
 */
@RestController
@RequestMapping("electricityDA1")
public class ElectricityDA1Controller {

    @Resource
    private ElectricityDA1Mapper electricityDA1Mapper;

    @Resource
    private ElectricityDA1Biz electricityDA1Biz;

    @GetMapping("page")
    public Message<PageInfo<ElectricityDA>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<ElectricityDA>> message = new Message<>();
        List<ElectricityDA> humidityList = electricityDA1Biz.page();
        PageInfo<ElectricityDA> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<ElectricityDA>> selectHumidityAll() {
        Message<List<ElectricityDA>> message = new Message<>();
        message.success().setData(electricityDA1Mapper.selectAll());
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
        message.success().setData(electricityDA1Biz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
