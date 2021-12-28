package com.edu.mapper;

import com.edu.entity.Temperature;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;


/**
 * 温度mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:57
 */
public interface TemperatureMapper extends Mapper<Temperature> {

    Temperature getNewData();

    List<Temperature> getTemList();
}
