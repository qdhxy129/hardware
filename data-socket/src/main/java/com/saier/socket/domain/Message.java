package com.saier.socket.domain;

import lombok.Data;

import java.util.Date;

/**
 * @author yxw
 */
@Data
public class Message {

    private Integer id;
    private String msg;
    private Date createTime;
    private String ip;

}
