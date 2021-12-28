package com.saier.socket.domain;

import lombok.Data;

import java.util.Date;

/**
 * 电量监管实体类
 *
 * @author 姜振炜
 * @date 2021/8/18 15:41
 */
@Data
public class Electricity {
    private Integer id;
    private String voltage;
    private String current;
    private String power;
    private String emissions;
    private String powerFactor;
    private String frequency;
    private String temperature;
    private String totalBattery;
    private String  epc;
    private Date createtime;
    private String ip;
}
