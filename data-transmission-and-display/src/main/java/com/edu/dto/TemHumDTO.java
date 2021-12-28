package com.edu.dto;

import lombok.Data;

import java.util.Date;

/**
 * @author yixiaowei
 * @date 2021/12/26 21:28
 */
@Data
public class TemHumDTO {

    private String id;
    private String equipId;
    private String equipTypeId;
    private String quotaCode;
    private String quotaValue;
    private Date createTime;
}
