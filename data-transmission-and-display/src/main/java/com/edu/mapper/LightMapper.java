package com.edu.mapper;

import com.edu.entity.Light;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

/**
 * 光照mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface LightMapper extends Mapper<Light> {
    Light getNewData();
}
