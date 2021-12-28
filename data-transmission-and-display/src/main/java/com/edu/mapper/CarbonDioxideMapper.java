package com.edu.mapper;

import com.edu.entity.CarbonDioxide;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

/**
 * 二氧化碳mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface CarbonDioxideMapper extends Mapper<CarbonDioxide> {

    CarbonDioxide getNewData();
}
