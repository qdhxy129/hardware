package com.edu.mapper;

import com.edu.entity.Photosensitive;
import com.edu.entity.Voice;
import tk.mybatis.mapper.common.Mapper;

/**
 * 光敏mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface PhotosensitiveMapper extends Mapper<Photosensitive> {
    Photosensitive getNewData();
}
