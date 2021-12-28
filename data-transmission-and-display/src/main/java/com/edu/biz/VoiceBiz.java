package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.CarbonDioxide;
import com.edu.entity.Voice;
import com.edu.entity.Voice;
import com.edu.mapper.VoiceMapper;
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
 * 声音biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class VoiceBiz {

    @Resource
    private VoiceMapper voiceMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.Voice>
     */
    public List<Voice> page() {
        Example example = Example.builder(Voice.class)
                .orderByDesc("createtime").build();
        return voiceMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.Voice>
     */
    public List<Voice> queryDataByTimeType(String timeQueryType) {
        List<Voice> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Voice voice = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(voice.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(voice.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(voice.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(voice.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(voice.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Voice.class)
                .where(WeekendSqls.<Voice>custom()
                        .andGreaterThanOrEqualTo(Voice::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Voice::getCreatetime, voice.getCreatetime()))
                .build();
        return voiceMapper.selectByExample(example);
    }


    /**
     * 获取最新一条数据
     * @author yixiaowei
     * @date 16:45 2021/10/24
     * @return com.edu.entity.CarbonDioxide
     */
    public Voice getNewData(){
        return voiceMapper.getNewData();
    }
}
