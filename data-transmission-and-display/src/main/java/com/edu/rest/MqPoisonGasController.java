package com.edu.rest;

import com.edu.biz.MqPoisonGasBiz;
import com.edu.entity.MqPoisonGas;
import com.edu.mapper.MqPoisonGasMapper;
import com.edu.unit.Message;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;

/**
 * Mq有毒气体
 * @author yixiaowei
 */
@RestController
@RequestMapping("mqPoisonGas")
public class MqPoisonGasController {

    @Resource
    private MqPoisonGasMapper mqPoisonGasMapper;

    @Resource
    private MqPoisonGasBiz mqPoisonGasBiz;

    @GetMapping("page")
    public Message<PageInfo<MqPoisonGas>> page(@RequestParam(value = "pn", defaultValue = "1") Integer pn) {
        PageHelper.startPage(pn, 10);
        Message<PageInfo<MqPoisonGas>> message = new Message<>();
        List<MqPoisonGas> humidityList = mqPoisonGasBiz.page();
        PageInfo<MqPoisonGas> page = new PageInfo<>(humidityList, 5);
        message.success().setData(page);
        return message;
    }

    @GetMapping("selectAll")
    public Message<List<MqPoisonGas>> selectHumidityAll() {
        Message<List<MqPoisonGas>> message = new Message<>();
        message.success().setData(mqPoisonGasMapper.selectAll());
        return message;
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return com.edu.unit.Message<java.util.List < com.edu.entity.MqPoisonGas>>
     */
    @GetMapping("queryDataWithinASpecifiedTime/{timeQueryType}")
    public Message<List<MqPoisonGas>> queryDataByTimeType(@PathVariable String timeQueryType) {
        Message<List<MqPoisonGas>> message = new Message<>();
        message.success().setData(mqPoisonGasBiz.queryDataByTimeType(timeQueryType));
        return message;
    }

}
