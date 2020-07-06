#include "ros/ros.h"
#include "robot_navigation/make_task.h"
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/GetPlan.h>
#include <vector>
#include "util.h"
#include "sql_client.h"
#include <tuple>
#include <string>
#include <queue>

typedef struct cost_st{
    double _distance = 0;
    double _sec_diff = 0;
    int _priority = 0;
    double    _open_pos_st = 0;
    double _battery_level = 0;
    double _cost;
    cost_st(double distance,double sec_diff,double priority,double open_pos_st,double battery_level):
        _distance(distance),_sec_diff(sec_diff),_priority(priority),_open_pos_st(open_pos_st),_battery_level(battery_level){
            _cost = 1.0 * _distance + 0.2 * _sec_diff + (-100) * _open_pos_st +(-10) * _priority  + (-1.0) * _battery_level;
        }
}CostFunction;

class CentralizedPool{

public:
    CentralizedPool():sql_client("centralized_pool","pass"){
        init();
        create_gather_info_tasks(10);
    }

    void init(){
	    ros::Duration(1).sleep();
        ROS_INFO_STREAM("Current office time: "<<Util::time_str(ros::Time::now()));
        task_server = nh.advertiseService("make_task",&CentralizedPool::process_robot_request,this);
        plan_client = nh.serviceClient<nav_msgs::GetPlan>("move_base/NavfnROS/make_plan");      
    }

    void create_gather_info_tasks(int num){
        sql_client.print_table("targets");
        sql_client.truncate_costs_tasks(); // clear task table and cost table
        sql_client.insert_gather_info_tasks(num,ros::Time::now(),ros::Duration(30));
        sql_client.print_table("tasks");
    }

    void create_go_to_point_task(){
        geometry_msgs::PoseStamped goal;
        goal.pose.position.x = -2;
        goal.pose.position.y = -0.5;
        goal.pose.position.z = -0;
        goal.header.stamp = ros::Time::now();
        goal.header.frame_id = "map";
        sql_client.insert_new_go_to_point_task(goal);
    }
    
    double calculate_distance(geometry_msgs::Pose target_pose, geometry_msgs::Pose robot_pose){
            double distance = 0;
        // for each task, request distance from move base plan server
            nav_msgs::GetPlan make_plan_srv;
            make_plan_srv.request.start.pose= robot_pose;
            make_plan_srv.request.start.header.frame_id = "map";
            make_plan_srv.request.goal.pose = target_pose;
            make_plan_srv.request.goal.header.frame_id = "map";
            make_plan_srv.request.tolerance = 1;
	        
            // request plan with start point and end point
            if(!plan_client.call(make_plan_srv)){
                ROS_INFO_STREAM("Failed to send request to make plan server");
                return -1;
            }
            // calculate distance
            std::vector<geometry_msgs::PoseStamped> &dists = make_plan_srv.response.plan.poses;

            if(dists.size()==0){
                ROS_DEBUG("Receive empty plan");
                return -1;
            };   
     
            for(size_t i = 1; i < dists.size();i++){
                distance += sqrt(pow((dists[i].pose.position.x - dists[i-1].pose.position.x),2) + 
                                    pow((dists[i].pose.position.y - dists[i-1].pose.position.y),2));
            }   
            return distance;    
    }
    
    bool process_robot_request(robot_navigation::make_task::Request &req,robot_navigation::make_task::Response &res){
        ros::Time cur_time = ros::Time::now();
        int task_id = req.last_task.task_id;
        ROS_INFO_STREAM("Receive request from robot\n"<<req);
       
        if(task_id == 0){
            ROS_INFO_STREAM("Get initial request from robot");
        }else{
            auto p = sql_client.query_target_id_type_from_task(task_id);
            
            // Process enter room task
            if(p.second == "GatherEnviromentInfo"){
                if(!req.last_task.is_completed){
                    ROS_INFO_STREAM("Task failed");
                    sql_client.update_task_status(task_id,"Error");
                }else{
                    ROS_INFO_STREAM("Task Succeed");
                    use_task_result(task_id,p.first,req.last_task.m_time, req.last_task.door_status); 
                    if(req.last_task.door_status == false){
                        reuse_task(task_id);
                    }                
                }
            }       
            // Process charging task
            else if(p.second == "Charging"){
                if(!req.last_task.is_completed){
                    ROS_INFO_STREAM("Robot charging failed");
                }else{
                    ROS_INFO_STREAM("Robot charging succedd");
                }
            }
        }

        // Give robot new task 
        Task bt = get_best_task(req.pose,cur_time,req.battery_level);
        sql_client.print_table("tasks"); 
        ROS_INFO_STREAM("Best task id = "<<bt.task_id<<" ,cost = "<< bt.cost);
        res.best_task.task_id = bt.task_id;
        res.best_task.goal = bt.goal; 
        res.best_task.room_id = bt.target_id;
        res.best_task.task_type = bt.task_type;
        
        return true;             
    }

    void reuse_task(int task_id){
        // change task status from Running to WaitingToRun, increase 3 priority and increase 200s start time 
        sql_client.update_returned_task(task_id,ros::Duration(200),3);
    }

    void use_task_result(int task_id,int door_id, ros::Time m_time, bool door_status){
        sql_client.update_task_list_completed(task_id); // mark task as RanToCompletion
        sql_client.insert_record_door_status_list(door_id,m_time,door_status); 
        sql_client.update_open_pos_table(door_id,m_time);
    }

    Task 
    get_best_task(geometry_msgs::Pose robot_pose,ros::Time t,double battery){
        vector<Task> v;

        sql_client.update_expired_tasks_canceled(t); // set exired task to canceled
        sql_client.print_table("tasks"); 

        if(battery < 20){ // charging
            // create_charging_task(t,robot_pose);
        }else{
            // find if there are execute task
            
            v = sql_client.query_runable_tasks("ExecuteTask");
            
            if(v.size() !=0){
                v = sql_client.query_runable_tasks("GatherEnviromentInfo");
            }
        }

        for(auto i :v){
               CostFunction cf(
                   calculate_distance(i.goal.pose,robot_pose),
                   (i.goal.header.stamp- t).sec,
                   i.priority,
                   i.open_pos,
                   battery
                );
                i.cost = cf._cost;
        }

        std::sort(v.begin(),v.end(),
        [](const Task& a, const Task&b)->bool
        {
                return a.cost <b.cost;
        });
        return v.back();
    }

    std::pair<int,geometry_msgs::PoseStamped> 
    create_charging_task(ros::Time t,geometry_msgs::Pose rp){
        auto v = sql_client.query_charging_station();
        if(v.size()==0){
            ROS_INFO_STREAM("Failed to load charging station");
            exit(1);
        }

        std::pair<int,double> best; // best charging station
        for(auto i : v){
            double dist = calculate_distance(i.second,rp);
            if(dist>best.second){
                best.first = i.first;
                best.second = dist;
            }
        }
        int id = sql_client.insert_new_charging_task(best.first,t);

        auto p = find_if( begin(v), end(v),
                             [=](decltype(*begin(v)) item )->int
                             { return get< 0 >( item ) == best.first;} );
        if(p == end(v)) {
            ROS_INFO_STREAM("Failed to find best charging station");
            exit(0);
        }
        geometry_msgs::PoseStamped pst;
        pst.pose = (*p).second;
        pst.header.frame_id = "map";
        pst.header.stamp = t;
        return make_pair(id,pst);
    }

private:
    ros::ServiceServer task_server;
    ros::ServiceClient plan_client;
    SQLClient sql_client;
    ros::NodeHandle nh;
};

int main(int argc, char** argv){
    ros::init(argc,argv,"centralized_poor");
    CentralizedPool pool;
    ros::spin(); // block program
}