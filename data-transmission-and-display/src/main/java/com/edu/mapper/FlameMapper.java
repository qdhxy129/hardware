package com.edu.mapper;

import com.edu.entity.Flame;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

/**
 * 火焰mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface FlameMapper extends Mapper<Flame> {
    Flame getNewData();

    List<Flame> getFlameList();
}
