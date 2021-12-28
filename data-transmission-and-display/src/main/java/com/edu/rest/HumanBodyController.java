package com.edu.rest;

import com.edu.biz.HumanBodyBiz;
import com.edu.entity.HumanBody;
import com.edu.mapper.HumanBodyMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 人体，视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("humanBody")
public class HumanBodyController {

    @Resource
    private HumanBodyMapper humanBodyMapper;

    @Resource
    private HumanBodyBiz humanBodyBiz;

    @GetMapping("page")
    public Message<PageInfo<HumanBody>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<HumanBody>> message = new Message<>();
        List<HumanBody> humidityList = humanBodyBiz.page();
        PageInfo<HumanBody> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<HumanBody>> selectHumidityAll() {
        Message<List<HumanBody>> message = new Message<>();
        message.success().setData(humanBodyMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.HumanBody>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<HumanBody>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<HumanBody>> message = new Message<>();
        message.success().setData(humanBodyBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
