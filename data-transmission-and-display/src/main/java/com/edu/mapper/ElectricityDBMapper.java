package com.edu.mapper;

import com.edu.entity.ElectricityDB;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

/** 电量DB表Mapper
 * @author yixiaowei
 * @date 2021/9/11 9:46
 */
public interface ElectricityDBMapper extends Mapper<ElectricityDB> {
    ElectricityDB getNewData();

    List<ElectricityDB> getElectrityDBList();
}
