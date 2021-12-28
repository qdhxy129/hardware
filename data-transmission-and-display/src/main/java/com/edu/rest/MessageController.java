package com.edu.rest;

import com.edu.biz.MessageBiz;
import com.edu.mapper.MessageMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * 废数据，视图
 *
 * @author 姜振炜
 * @date 2021/8/3 23:53
 */
@RestController
@RequestMapping("message")
public class MessageController {

    @Resource
    private MessageMapper messageMapper;

    @Resource
    private MessageBiz messageBiz;

    @GetMapping("page")
    public Message<PageInfo<com.edu.entity.Message>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<com.edu.entity.Message>> message = new Message<>();
        List<com.edu.entity.Message> humidityList = messageBiz.page();
        PageInfo<com.edu.entity.Message> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<com.edu.entity.Message>> selectHumidityAll() {
        Message<List<com.edu.entity.Message>> message = new Message<>();
        message.success().setData(messageMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.Message>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<com.edu.entity.Message>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<com.edu.entity.Message>> message = new Message<>();
        message.success().setData(messageBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
