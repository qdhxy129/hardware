package com.edu.biz;

import com.edu.unit.Message;
import com.edu.unit.SocketSendUtil;
import org.springframework.stereotype.Service;

/**
 * socket发送数据业务类
 * @author yixiaowei
 * @date 2021/8/22 13:43
 */
@Service
public class SocketSendInfoBiz {

    /**
     * 向指定端口 ip 发送数据
     * @author yixiaowei
     * @date 13:46 2021/8/22
     * @return void
     */
    public Message socketSendInfo(String ipAddress, Integer port, String msg){
        return SocketSendUtil.sendInfo(ipAddress, port, msg);
    }
}
