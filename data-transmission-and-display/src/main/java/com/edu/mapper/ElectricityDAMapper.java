package com.edu.mapper;

import com.edu.entity.ElectricityDA;
import tk.mybatis.mapper.common.Mapper;

/** 电量DA表Mapper
 * @author yixiaowei
 * @date 2021/9/11 9:46
 */
public interface ElectricityDAMapper extends Mapper<ElectricityDA> {
    ElectricityDA getNewData();
}
