package com.edu.mapper;

import com.edu.entity.Smoke;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

/**
 * 烟雾mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface SmokeMapper extends Mapper<Smoke> {
    Smoke getNewData();

    List<Smoke> getSmokeList();
}
