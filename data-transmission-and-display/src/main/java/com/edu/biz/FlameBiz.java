package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.Flame;
import com.edu.mapper.FlameMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.util.Sqls;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

/**
 * 火焰biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class FlameBiz {

    @Resource
    private FlameMapper flameMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.Flame>
     */
    public List<Flame> page() {
        Example example = Example.builder(Flame.class)
                .orderByDesc("createtime").build();
        return flameMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.Flame>
     */
    public List<Flame> queryDataByTimeType(String timeQueryType) {
        List<Flame> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Flame flame = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(flame.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(flame.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(flame.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(flame.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(flame.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Flame.class)
                .where(WeekendSqls.<Flame>custom()
                        .andGreaterThanOrEqualTo(Flame::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Flame::getCreatetime, flame.getCreatetime()))
                .build();
        return flameMapper.selectByExample(example);
    }

    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public Flame getNewData(){
        return flameMapper.getNewData();
    }

    public List<Flame> getFLameList() {
        return flameMapper.getFlameList();
    }
}
