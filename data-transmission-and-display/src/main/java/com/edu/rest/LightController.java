package com.edu.rest;

import com.edu.biz.LightBiz;
import com.edu.entity.Light;
import com.edu.mapper.LightMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 光照controller
 *
 * @author 姜振炜
 * @date 2021/8/15 15:29
 */
@RestController
@RequestMapping("light")
public class LightController {

    @Resource
    private LightBiz lightBiz;

    @Resource
    private LightMapper lightMapper;

    @GetMapping("page")
    public Message<PageInfo<Light>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<Light>> message = new Message<>();
        List<Light> humidityList = lightBiz.page();
        PageInfo<Light> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<Light>> selectHumidityAll() {
        Message<List<Light>> message = new Message<>();
        message.success().setData(lightMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Light>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<Light>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<Light>> message = new Message<>();
        message.success().setData(lightBiz.queryDataByTimeType(timeQueryType));
        return message;
    }
}
