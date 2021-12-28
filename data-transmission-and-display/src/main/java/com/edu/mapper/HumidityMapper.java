package com.edu.mapper;

import com.edu.entity.Humidity;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

/**
 * 湿度mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface HumidityMapper extends Mapper<Humidity> {
    Humidity getNewData();

    List<Humidity> getHumList();
}
