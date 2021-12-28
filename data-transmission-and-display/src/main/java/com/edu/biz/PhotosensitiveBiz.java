package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.Photosensitive;
import com.edu.mapper.PhotosensitiveMapper;
import com.edu.unit.RecentTime;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;
import tk.mybatis.mapper.entity.Example;
import tk.mybatis.mapper.weekend.WeekendSqls;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * 光敏biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class PhotosensitiveBiz {

    @Resource
    private PhotosensitiveMapper photosensitiveMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.photosensitive>
     */
    public List<Photosensitive> page() {
        Example example = Example.builder(Photosensitive.class)
                .orderByDesc("createtime").build();
        return photosensitiveMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.photosensitive>
     */
    public List<Photosensitive> queryDataByTimeType(String timeQueryType) {
        List<Photosensitive> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Photosensitive photosensitive = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(photosensitive.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(photosensitive.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(photosensitive.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(photosensitive.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(photosensitive.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Photosensitive.class)
                .where(WeekendSqls.<Photosensitive>custom()
                        .andGreaterThanOrEqualTo(Photosensitive::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Photosensitive::getCreatetime, photosensitive.getCreatetime()))
                .build();
        return photosensitiveMapper.selectByExample(example);
    }

    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public Photosensitive getNewData(){
        return photosensitiveMapper.getNewData();
    }
}
