package com.edu.rest;

import com.edu.biz.VoiceBiz;
import com.edu.entity.Voice;
import com.edu.mapper.VoiceMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 声音
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("voice")
public class VoiceController {

    @Resource
    private VoiceMapper voiceMapper;

    @Resource
    private VoiceBiz voiceBiz;

    @GetMapping("page")
    public Message<PageInfo<Voice>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<Voice>> message = new Message<>();
        List<Voice> humidityList = voiceBiz.page();
        PageInfo<Voice> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<Voice>> selectHumidityAll() {
        Message<List<Voice>> message = new Message<>();
        message.success().setData(voiceMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Voice>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<Voice>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<Voice>> message = new Message<>();
        message.success().setData(voiceBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
