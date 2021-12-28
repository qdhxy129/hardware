package com.edu.rest;

import com.edu.biz.TemperatureBiz;
import com.edu.entity.Temperature;
import com.edu.mapper.TemperatureMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 温度视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:58
 */
@RestController
@RequestMapping("temperature")
public class TemperatureController {

    @Resource
    private TemperatureMapper temperatureMapper;

    @Resource
    private TemperatureBiz temperatureBiz;

    /**
     * 分页查询所有
     *
     * @param pn 页数
     * @return com.edu.unit.Message<com.github.pagehelper.PageInfo < com.edu.entity.Temperature>>
     */
    @GetMapping("page")
    public Message<PageInfo<Temperature>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<Temperature>> message = new Message<>();
        List<Temperature> humidityList = temperatureBiz.page();
        PageInfo<Temperature> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    /**
     * 查询所有
     *
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Temperature>>
     */
    @GetMapping("selectAll")
    public Message<List<Temperature>> selectTemperatureAll() {
        Message<List<Temperature>> message = new Message<>();
        message.success().setData(temperatureMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Temperature>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<Temperature>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<Temperature>> message = new Message<>();
        message.success().setData(temperatureBiz.queryDataByTimeType(timeQueryType));
        return message;
    }
}
