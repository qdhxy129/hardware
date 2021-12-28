package com.edu.rest;

import com.edu.biz.ElectricityDB1Biz;
import com.edu.entity.ElectricityDB;
import com.edu.mapper.ElectricityDB1Mapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * ElectricityDB
 *
 * @author yixiaowei
 */
@RestController
@RequestMapping("electricityDB1")
public class ElectricityDB1Controller {

    @Resource
    private ElectricityDB1Mapper electricityDB1Mapper;

    @Resource
    private ElectricityDB1Biz electricityDB1Biz;

    @GetMapping("page")
    public Message<PageInfo<ElectricityDB>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<ElectricityDB>> message = new Message<>();
        List<ElectricityDB> humidityList = electricityDB1Biz.page();
        PageInfo<ElectricityDB> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<ElectricityDB>> selectHumidityAll() {
        Message<List<ElectricityDB>> message = new Message<>();
        message.success().setData(electricityDB1Mapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.ElectricityDB>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<ElectricityDB>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<ElectricityDB>> message = new Message<>();
        message.success().setData(electricityDB1Biz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
