package com.edu.mapper;

import com.edu.entity.MqPoisonGas;
import tk.mybatis.mapper.common.Mapper;

/** Mq有毒气体Mapper
 * @author yixiaowei
 * @date 2021/9/11 9:45
 */
public interface MqPoisonGasMapper extends Mapper<MqPoisonGas> {
    MqPoisonGas getNewData();
}
