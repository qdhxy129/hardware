package com.edu.rest;

import com.edu.biz.PhotosensitiveBiz;
import com.edu.entity.Photosensitive;
import com.edu.mapper.PhotosensitiveMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 光敏
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("photosensitive")
public class PhotosensitiveController {

    @Resource
    private PhotosensitiveMapper photosensitiveMapper;

    @Resource
    private PhotosensitiveBiz photosensitiveBiz;

    @GetMapping("page")
    public Message<PageInfo<Photosensitive>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<Photosensitive>> message = new Message<>();
        List<Photosensitive> humidityList = photosensitiveBiz.page();
        PageInfo<Photosensitive> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<Photosensitive>> selectHumidityAll() {
        Message<List<Photosensitive>> message = new Message<>();
        message.success().setData(photosensitiveMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Photosensitive>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<Photosensitive>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<Photosensitive>> message = new Message<>();
        message.success().setData(photosensitiveBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
