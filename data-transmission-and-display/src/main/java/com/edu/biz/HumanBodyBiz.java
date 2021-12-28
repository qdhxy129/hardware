package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.HumanBody;
import com.edu.mapper.HumanBodyMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

/**
 * 人体biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class HumanBodyBiz {

    @Resource
    private HumanBodyMapper humanBodyMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.HumanBody>
     */
    public List<HumanBody> page() {
        Example example = Example.builder(HumanBody.class)
                .orderByDesc("createtime").build();
        return humanBodyMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.HumanBody>
     */
    public List<HumanBody> queryDataByTimeType(String timeQueryType) {
        List<HumanBody> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        HumanBody humanBody = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(humanBody.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(humanBody.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(humanBody.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(humanBody.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(humanBody.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(HumanBody.class)
                .where(WeekendSqls.<HumanBody>custom()
                        .andGreaterThanOrEqualTo(HumanBody::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(HumanBody::getCreatetime, humanBody.getCreatetime()))
                .build();
        return humanBodyMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public HumanBody getNewData(){
        return humanBodyMapper.getNewData();
    }
}
