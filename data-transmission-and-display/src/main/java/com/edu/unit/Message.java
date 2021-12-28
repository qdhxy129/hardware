package com.edu.unit;

import lombok.Data;

/**
 * controller返回信息类
 *
 * @author 姜振炜
 * @date 2021/8/3 23:12
 */
@Data
public class Message<T> {
    /**
     * 返回的数据
     */
    private T data;
    /**
     * 返回消息类型
     */
    private Integer status;

    public Message<T> success(){
        this.status = 200;
        return this;
    }

    public Message<T> fail(){
        this.status = 500;
        return this;
    }

}
