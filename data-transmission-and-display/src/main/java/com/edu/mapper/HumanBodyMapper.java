package com.edu.mapper;

import com.edu.entity.HumanBody;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

/**
 * 人体mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface HumanBodyMapper extends Mapper<HumanBody> {
    HumanBody getNewData();
}
