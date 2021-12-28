package com.edu.rest;

import com.edu.biz.FlameBiz;
import com.edu.entity.Flame;
import com.edu.mapper.FlameMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 火焰，视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("flame")
public class FlameController {

    @Resource
    private FlameMapper flameMapper;

    @Resource
    private FlameBiz flameBiz;

    @GetMapping("page")
    public Message<PageInfo<Flame>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<Flame>> message = new Message<>();
        List<Flame> humidityList = flameBiz.page();
        PageInfo<Flame> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<Flame>> selectHumidityAll() {
        Message<List<Flame>> message = new Message<>();
        message.success().setData(flameMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Flame>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<Flame>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<Flame>> message = new Message<>();
        message.success().setData(flameBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
