package com.edu.entity;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Date;

/**
 * @author yixiaowei
 * @date 2021/9/11 9:34
 */
@Data
@Table(name = "electricityDA")
public class ElectricityDA {

    @Id
    private Integer id;

    /**
     * epc
     */
    @Column(name = "epc")
    private String epc;

    /**
     * 电压
     */
    @Column(name = "voltage")
    private String voltage;

    /**
     * 电流
     */
    @Column(name = "current")
    private String current;

    /**
     * 功率
     */
    @Column(name = "power")
    private String power;

    /**
     * 功率因数
     */
    @Column(name = "powerFactor")
    private String powerFactor;

    /**
     * 频率
     */
    @Column(name = "frequency")
    private String frequency;


    @Column(name = "createtime")
    @JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone="GMT+8")
    private Date createtime;
}
