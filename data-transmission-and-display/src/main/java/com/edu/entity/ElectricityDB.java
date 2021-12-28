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
@Table(name = "electricityDB")
public class ElectricityDB {

    @Id
    private Integer id;

    /**
     * epc
     */
    @Column(name = "epc")
    private String epc;

    /**
     * 二氧化碳排放量
     */
    @Column(name = "emissions")
    private String emissions;

    /**
     * 温度
     */
    @Column(name = "temperature")
    private String temperature;

    /**
     * 总电量
     */
    @Column(name = "totalBattery")
    private String totalBattery;


    @Column(name = "createtime")
    @JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone="GMT+8")
    private Date createtime;
}
