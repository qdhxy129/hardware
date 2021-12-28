package com.edu.entity;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Data;

import javax.persistence.Column;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Date;

/**
 * 烟雾实体类
 *
 * @author 姜振炜
 * @date 2021/8/8 22:23
 */
@Data
@Table
public class Smoke {

    @Id
    @GeneratedValue(generator = "JDBC")
    private Integer id;

    @Column(name = "message")
    private String message;

    @JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone="GMT+8")
    @Column(name = "createtime")
    private Date createtime;
}
