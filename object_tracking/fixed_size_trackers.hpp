#ifndef FIXED_SIZE_TRACKERS_HPP
#define FIXED_SIZE_TRACKERS_HPP

#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>

#include <functional>
#include <string>

/**
 *Tracker based on the tracking algorithms from
 *opencv contribute module
 */
class fixed_size_trackers
{
public:
    using search_func =
    std::function<std::vector<cv::Rect2d>(cv::Mat const&)>;

    /**
     * @param search_strategy specify the strategy to search the target, this
     * strategy will be called when occlusion or miss frame happen
     * @param search strategy of the tracking solution
     * @param max_player maximum player of the tracker tracking
     * @param miss_frame If the tracking box stuck at the
     * same location >= miss_fram_count, this algorithm
     * will start the re-track progress.If the value == 0, it will
     * never re-track even the tracking box stuck.
     * @param occlusion_thresh If the value of IoU(intersection
     * of Union, measure how many regions of the box intersect)
     * greater than occlusion_thresh, this algorithm will start
     * the re-track progress
     */
    explicit fixed_size_trackers(search_func search_strategy,
                                 size_t max_player = 1,
                                 size_t miss_frame = 150,
                                 double occlusion_thresh = 0.5);

    /**
     * add a new tracker
     * @param input input image with target want to track
     * @param roi roi of the target
     * @param strategy tracking strategy, it could be
     * "MIL", "BOOSTING", "MEDIANFLOW", "TLD", "KCF"
     */
    void add(cv::Mat const &input,
             cv::Rect const &roi,
             std::string const &strategy = "MEDIANFLOW");

    void clear();

    bool empty() const;

    size_t get_miss_frame() const;
    double get_occlusion_thresh() const;
    cv::Rect2d get_position(size_t target) const;
    std::vector<cv::Rect2d> const& get_position() const;
    size_t get_track_size() const;

    void set_miss_frame(size_t value);
    void set_occlusion_thresh(double value);
    void set_search(search_func search_strategy);

    void update(cv::Mat const &input);

private:        
    bool should_retrack(std::vector<cv::Rect2d> const &old_pos);

    size_t max_player_;
    size_t miss_frame_;
    std::vector<size_t> miss_records_;
    double occlusion_thresh_;
    search_func search_strategy_;
    cv::MultiTrackerTLD trackers_;
    std::vector<std::string> track_strategy_;
};

#endif //FIXED_SIZE_TRACKERS_HPP
