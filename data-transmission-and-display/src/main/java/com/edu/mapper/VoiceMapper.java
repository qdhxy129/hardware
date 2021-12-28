package com.edu.mapper;

import com.edu.entity.CarbonDioxide;
import com.edu.entity.Voice;
import tk.mybatis.mapper.common.Mapper;

/**
 * 声音mapper
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
public interface VoiceMapper extends Mapper<Voice> {
    Voice getNewData();
}
