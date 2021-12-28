package com.edu.entity;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Data;

import javax.persistence.Column;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Date;

/**
 * 温度实体类
 *
 * @author 姜振炜
 * @date 2021/8/3 23:57
 */
@Data
@Table(name = "temperature")
public class Temperature {

    @Id
    @GeneratedValue(generator = "JDBC")
    private Integer id;

    @Column(name = "message")
    private String message;

    @JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone="GMT+8")
    @Column(name = "createtime")
    private Date createtime;
}
