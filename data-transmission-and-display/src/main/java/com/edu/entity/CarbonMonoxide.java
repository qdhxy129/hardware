package com.edu.entity;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Date;

/**
 * CO实体类
 * @author yixiaowei
 * @date 9:18 2021/9/11
 * @param
 * @return
 */
@Data
@Table(name = "carbonMonoxide")
public class CarbonMonoxide {

    @Id
    private Integer id;

    @Column(name = "message")
    private String message;

    @Column(name = "createtime")
    @JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone="GMT+8")
    private Date createtime;
}
