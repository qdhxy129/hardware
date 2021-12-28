package com.edu.biz;

import com.edu.constant.TimeQueryType;
import com.edu.entity.Message;
import com.edu.mapper.MessageMapper;
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
 * 废数据biz
 *
 * @author 姜振炜
 * @date 2021/8/3 23:52
 */
@Service
public class MessageBiz {

    @Resource
    private MessageMapper messageMapper;

    /**
     * 查询所有倒序返回
     *
     * @return java.util.List<com.edu.entity.Message>
     */
    public List<Message> page() {
        Example example = Example.builder(Message.class)
                .orderByDesc("createtime").build();
        return messageMapper.selectByExample(example);
    }

    /**
     * 获取指定时间段的数据
     *
     * @param timeQueryType 时间查询类型
     * @return java.util.List<com.edu.entity.Message>
     */
    public List<Message> queryDataByTimeType(String timeQueryType) {
        List<Message> page = page();
        if (CollectionUtils.isEmpty(page)) {
            return new ArrayList<>();
        }
        Message message = page.get(0);
        Date recentTime;
        switch (timeQueryType) {
            case TimeQueryType.ONE_HOUR:
                recentTime = RecentTime.getTheTimeOneHourAgo(message.getCreatetime());
                break;
            case TimeQueryType.ONE_DAY:
                recentTime = RecentTime.getTheTimeOneDayAgo(message.getCreatetime());
                break;
            case TimeQueryType.ONE_WEEK:
                recentTime = RecentTime.getTheTimeOneWeekAgo(message.getCreatetime());
                break;
            case TimeQueryType.ONE_MONTH:
                recentTime = RecentTime.getTheTimeOneMonthAgo(message.getCreatetime());
                break;
            case TimeQueryType.ONE_YEAR:
                recentTime = RecentTime.getTheTimeOneYearAgo(message.getCreatetime());
                break;
            default:
                return new ArrayList<>();
        }
        Example example = Example.builder(Message.class)
                .where(WeekendSqls.<Message>custom()
                        .andGreaterThanOrEqualTo(Message::getCreatetime, recentTime)
                        .andLessThanOrEqualTo(Message::getCreatetime, message.getCreatetime()))
                .build();
        return messageMapper.selectByExample(example);
    }
}
