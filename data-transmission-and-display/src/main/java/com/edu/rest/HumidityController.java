package com.edu.rest;

import com.edu.biz.HumidityBiz;
import com.edu.entity.Humidity;
import com.edu.mapper.HumidityMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 湿度，视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("humidity")
public class HumidityController {

    @Resource
    private HumidityMapper humidityMapper;

    @Resource
    private HumidityBiz humidityBiz;

    @GetMapping("page")
    public Message<PageInfo<Humidity>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<Humidity>> message = new Message<>();
        List<Humidity> humidityList = humidityBiz.page();
        PageInfo<Humidity> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<Humidity>> selectHumidityAll() {
        Message<List<Humidity>> message = new Message<>();
        message.success().setData(humidityMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Humidity>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<Humidity>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<Humidity>> message = new Message<>();
        message.success().setData(humidityBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
