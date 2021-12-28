package com.edu.rest;

import com.edu.biz.CarbonMonoxideBiz;
import com.edu.entity.CarbonMonoxide;
import com.edu.mapper.CarbonMonoxideMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * CO
 *
 * @author yixiaowei
 */
@RestController
@RequestMapping("carbonMonoxide")
public class CarbonMonoxideController {

    @Resource
    private CarbonMonoxideMapper carbonMonoxideMapper;

    @Resource
    private CarbonMonoxideBiz carbonMonoxideBiz;

    @GetMapping("page")
    public Message<PageInfo<CarbonMonoxide>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<CarbonMonoxide>> message = new Message<>();
        List<CarbonMonoxide> humidityList = carbonMonoxideBiz.page();
        PageInfo<CarbonMonoxide> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<CarbonMonoxide>> selectHumidityAll() {
        Message<List<CarbonMonoxide>> message = new Message<>();
        message.success().setData(carbonMonoxideMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.CarbonMonoxide>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<CarbonMonoxide>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<CarbonMonoxide>> message = new Message<>();
        message.success().setData(carbonMonoxideBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
