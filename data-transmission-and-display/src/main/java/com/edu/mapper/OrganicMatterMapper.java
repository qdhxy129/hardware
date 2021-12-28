package com.edu.mapper;

import com.edu.entity.OrganicMatter;
import tk.mybatis.mapper.common.Mapper;

/**
 * 有机物mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface OrganicMatterMapper extends Mapper<OrganicMatter> {
    OrganicMatter getNewData();
}
