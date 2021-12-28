package com.edu.rest;

import com.edu.biz.CarbonDioxideBiz;
import com.edu.entity.CarbonDioxide;
import com.edu.mapper.CarbonDioxideMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 二氧化碳，视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("carbonDioxide")
public class CarbonDioxideController {

    @Resource
    private CarbonDioxideMapper carbonDioxideMapper;

    @Resource
    private CarbonDioxideBiz carbonDioxideBiz;

    @GetMapping("page")
    public Message<PageInfo<CarbonDioxide>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<CarbonDioxide>> message = new Message<>();
        List<CarbonDioxide> humidityList = carbonDioxideBiz.page();
        PageInfo<CarbonDioxide> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<CarbonDioxide>> selectHumidityAll() {
        Message<List<CarbonDioxide>> message = new Message<>();
        message.success().setData(carbonDioxideMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.CarbonDioxide>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<CarbonDioxide>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<CarbonDioxide>> message = new Message<>();
        message.success().setData(carbonDioxideBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
