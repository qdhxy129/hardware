package com.edu.rest;

import com.edu.biz.GlobalBiz;
import com.edu.dto.RealTimeDataDTO;
import com.edu.unit.Message;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;

/**
 * @author yixiaowei
 * @date 2021/10/24 16:13
 */
@RestController
@RequestMapping("global")
@CrossOrigin
public class GlobalController {

    @Resource
    private GlobalBiz globalBiz;

    @GetMapping("getRealTimeData")
    public Message getRealTimeData() {
        RealTimeDataDTO realTimeData = globalBiz.getRealTimeData();
        Message message = new Message().success();
        message.setData(realTimeData);
        return message;
    }

}
