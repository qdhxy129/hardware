package com.edu.rest;

import com.edu.biz.SocketSendInfoBiz;
import com.edu.unit.Message;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;
import java.util.Map;

/**
 * @author yixiaowei
 * @date 2021/8/22 13:41
 */
@RestController
@RequestMapping("send")
public class SocketSendInfoController {

    @Resource
    private SocketSendInfoBiz socketSendInfoBiz;

    @PostMapping("info")
    public Message socketSendInfo(@RequestBody Map<String, String> data) {
        return socketSendInfoBiz.socketSendInfo(data.get("ipAddress"), Integer.valueOf(data.get("port")), data.get("msg"));
    }
}
