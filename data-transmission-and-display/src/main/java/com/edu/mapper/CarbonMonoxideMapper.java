package com.edu.mapper;

import com.edu.entity.CarbonMonoxide;
import tk.mybatis.mapper.common.Mapper;

/** CO Mapper
 * @author yixiaowei
 * @date 2021/9/11 9:45
 */
public interface CarbonMonoxideMapper extends Mapper<CarbonMonoxide> {
    CarbonMonoxide getNewData();
}
