package com.edu.rest;

import com.edu.biz.OrganicMatterBiz;
import com.edu.entity.OrganicMatter;
import com.edu.mapper.OrganicMatterMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 有机物，视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("organicMatter")
public class OrganicMatterController {

    @Resource
    private OrganicMatterMapper organicMatterMapper;

    @Resource
    private OrganicMatterBiz organicMatterBiz;

    @GetMapping("page")
    public Message<PageInfo<OrganicMatter>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<OrganicMatter>> message = new Message<>();
        List<OrganicMatter> humidityList = organicMatterBiz.page();
        PageInfo<OrganicMatter> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<OrganicMatter>> selectHumidityAll() {
        Message<List<OrganicMatter>> message = new Message<>();
        message.success().setData(organicMatterMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.OrganicMatter>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<OrganicMatter>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<OrganicMatter>> message = new Message<>();
        message.success().setData(organicMatterBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
