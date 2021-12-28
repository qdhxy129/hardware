package com.edu.rest;

import com.edu.biz.SmokeBiz;
import com.edu.entity.Smoke;
import com.edu.mapper.SmokeMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 烟雾，视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("smoke")
public class SmokeController {

    @Resource
    private SmokeBiz smokeBiz;

    @Resource
    private SmokeMapper smokeMapper;

    @GetMapping("page")
    public Message<PageInfo<Smoke>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<Smoke>> message = new Message<>();
        List<Smoke> humidityList = smokeBiz.page();
        PageInfo<Smoke> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<Smoke>> selectHumidityAll() {
        Message<List<Smoke>> message = new Message<>();
        message.success().setData(smokeMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Smoke>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<Smoke>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<Smoke>> message = new Message<>();
        message.success().setData(smokeBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
